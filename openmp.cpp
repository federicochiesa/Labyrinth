#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <chrono>
#include <random>
#include <omp.h>

#define FILE_HEADER 0
#define IMAGE_WIDTH_INDEX 1
#define IMAGE_HEIGHT_INDEX 2
#define FILE_START_OFFSET 4

struct Ball
{
    std::vector<int> x;
    std::vector<int> y;
    bool arrived = false;
};

int main()
{
    // Parameters
    std::string filename = "maze1"; // Set this to the filename of the maze to solve.
    int numberOfBalls = 100;
    int startX = 213; // Starting X pixel counting from the left
    int startY = 3; // Starting X pixel counting from the top
    int tid;

    std::string maze;
    std::string textLine;
    std::ifstream fileReader((filename + ".pgm"));
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<int> uni(-3,3);
    std::string word;
    std::vector<std::string> mazeVector;

    // Load and prepare image
    while (getline(fileReader, textLine))
        maze = maze + textLine + " ";
    fileReader.close();
    if (maze.length() != 0)
    {
        std::stringstream ss(maze);
        while (ss >> word)
            mazeVector.push_back(word);

        if (mazeVector[FILE_HEADER] != "P2"){
            std::cout << "File is in an invalid format. Please convert your file to PGM P2 format";
            return -1;
        }
        
    }
    else{
        std::cout << "File not found. Exiting...";
        return -1;
    }

    int imageWidth = std::stoi(mazeVector[IMAGE_WIDTH_INDEX]);
    std::vector<Ball> ballArray;
    std::vector<bool> arrivedBalls;

    for(int i = 0; i < numberOfBalls; i++){
        Ball b;
        ballArray.push_back(b);
        arrivedBalls.push_back(false);
    }

    for(int i = 0; i < ballArray.size(); i++){
        ballArray[i].x.push_back(startX);
        ballArray[i].y.push_back(startY);
    }
    
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    int num_finished;
    while(num_finished < numberOfBalls){

        #pragma omp parallel for
        for(int i = 0; i < ballArray.size(); i++){

            if(arrivedBalls[i] == false){
                // calculate next point
                int nextX = ballArray[i].x.back() + uni(rng);
                int nextY = ballArray[i].y.back() + uni(rng);
                while(nextX < 0 || nextY < 0 || mazeVector[nextY * (imageWidth - 1) + nextX] == "0"){
                    nextX = ballArray[i].x.back() + uni(rng);
                    nextY = ballArray[i].y.back() + uni(rng);
                }
                ballArray[i].x.push_back(nextX);
                ballArray[i].y.push_back(nextY);

                // check if reached finish line
                if(mazeVector[nextY * (imageWidth - 1) + nextX] != "0" && mazeVector[nextY * (imageWidth - 1) + nextX] != "255"){
                    num_finished++;
                    arrivedBalls[i] = true;
                    // draw path
                    for(int j = 0; j < ballArray[i].x.size(); j++)
                        mazeVector[ballArray[i].y[j] * (imageWidth - 1) + ballArray[i].x[j]] = "100";
                    std::ofstream file("result.pgm");
                    if (file.is_open()) {
                        file << mazeVector[0] + "\n" + mazeVector[1] + " " + mazeVector[2] + "\n" + mazeVector[3] + "\n";
                        std::string result;
                        for(int k = 4; k < mazeVector.size(); k++){
                            result += mazeVector[k] + " ";
                        }
                        file << result;
                        file.close();
                    }
                    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
                    auto time = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
                    std::cout << "Serialized Time = " << time << "[ms] " << i << std::endl;
                }
            }
        }
    }
    return 0;
}
