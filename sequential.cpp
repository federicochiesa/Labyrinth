#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <time.h>
#include <random>

#define FILE_HEADER 0
#define IMAGE_WIDTH_INDEX 1
#define IMAGE_HEIGHT_INDEX 2
#define FILE_START_OFFSET 4

struct Ball
{
    std::vector<int> x;
    std::vector<int> y;
};

int main()
{
    // Parameters
    std::string filename = "maze"; // Set this to the filename of the maze to solve.
    int numberOfBalls = 100;
    int startX = 213;
    int startY = 3;

    std::string maze;
    std::string textLine;
    std::ifstream fileReader((filename + ".pgm"));
    std::random_device rd;     
    std::mt19937 rng(rd());    
    std::uniform_int_distribution<int> uni(-4,4); 
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
    int imageHeight = std::stoi(mazeVector[IMAGE_HEIGHT_INDEX]);
    Ball ballArray[numberOfBalls];

    for(Ball b : ballArray){
        b.x.push_back(startX);
        b.y.push_back(startY);
    }

    Ball p;
    Ball *finisher = &p;
    finisher = NULL;
    while(finisher == NULL){
        for(Ball b : ballArray){
            int nextX = b.x.back() + uni(rng);
            int nextY = b.y.back() + uni(rng);
            while(mazeVector[nextY * (imageWidth - 1) + nextX] == "0"){
                nextX = b.x.back() + uni(rng);
                nextY = b.y.back() + uni(rng);
            }
            b.x.push_back(nextX);
            b.y.push_back(nextY);
            if(mazeVector[nextY * (imageWidth - 1) + nextX] != "0" && mazeVector[nextY * (imageWidth - 1) + nextX] != "255"){
                finisher = &b;
                for(int i = 0; i < b.x.size(); i++){
                    mazeVector[b.x[i] * (imageWidth - 1) + b.y[i]] = "100";
                }
                std::ofstream file("result.pgm");
                if (file.is_open()) {
                    file << mazeVector[0] + "\n" + mazeVector[1] + " " + mazeVector[2] + "\n" + mazeVector[3] + "\n";
                    std::string result;
                    for(int i = 4; i < mazeVector.size(); i++){
                        result += word + " ";
                    }
                    file << result;
                    file.close();
                }
            }
        }
    }
    return 0;
}