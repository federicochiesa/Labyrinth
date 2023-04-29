#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <chrono>
#include <random>
#include <thread>

#define FILE_HEADER 0
#define IMAGE_WIDTH_INDEX 1
#define IMAGE_HEIGHT_INDEX 2
#define FILE_START_OFFSET 4

struct Ball
{
    std::vector<int> x;
    std::vector<int> y;
};

void mazeSolver()
{
    // Parameters
    std::string filename = "maze1"; // Set this to the filename of the maze to solve.
    int numberOfBalls = 10;
    int startX = 213;
    int startY = 3;

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
            return;
        }
        
    }
    else{
        std::cout << "File not found. Exiting...";
        return;
    }

    int imageWidth = std::stoi(mazeVector[IMAGE_WIDTH_INDEX]);
    std::vector<Ball> ballArray;

    for(int i = 0; i < numberOfBalls; i++){
        Ball b;
        ballArray.push_back(b);
    }

    for(int i = 0; i < ballArray.size(); i++){
        ballArray[i].x.push_back(startX);
        ballArray[i].y.push_back(startY);
    }

    Ball p;
    Ball *finisher = &p;
    finisher = NULL;
    while(finisher == NULL){
        for(int i = 0; i < ballArray.size(); i++){
            int nextX = ballArray[i].x.back() + uni(rng);
            int nextY = ballArray[i].y.back() + uni(rng);
            while(nextX < 0 || nextY < 0 || mazeVector[nextY * (imageWidth - 1) + nextX] == "0"){
                nextX = ballArray[i].x.back() + uni(rng);
                nextY = ballArray[i].y.back() + uni(rng);
            }
            ballArray[i].x.push_back(nextX);
            ballArray[i].y.push_back(nextY);
            if(mazeVector[nextY * (imageWidth - 1) + nextX] != "0" && mazeVector[nextY * (imageWidth - 1) + nextX] != "255"){
                finisher = &ballArray[i];
                std::cout << "vinto";
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
                return;
            }
        }
    }
}

int main(){
    std::thread myThreads[10];

    for(int i = 0; i < 10; i++){
        myThreads[i] = std::thread(mazeSolver);
        myThreads[i].detach();
    }
    return 0;
}