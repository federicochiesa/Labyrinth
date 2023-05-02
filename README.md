# Labyrinth
## Introduction
This is a Parallel Computing course assignment that is meant to show how parallel code runs faster than sequential code.
For this, we created a maze solver that moves a certain number of balls around a labyrinth randomly and records the time that it takes for the first ball to get to the end of the maze.
## Usage
To run the code you simply need to specify a maze file to solve, a starting point, and the number of balls that you want to be in the maze. These parameters are at the beginning of the ```main``` function.
## Custom mazes
You can provide your own custom mazes by following these steps(you'll need ```imagemagick``` installed on your system):
1. Generate a maze using [this](https://www.mazegenerator.net/) website(this website is the only one we found that doesn't do any antialiasing on the maze walls, which confuses the solver) and download it in PNG format.
2. Scale the image by 300% by using the command: ```magick <PNGFromMazegenerator>.png -scale 300% <YourMazeName>.png```
3. Convert the image in PGM P2 format using the command: ```magick <YourMazeName>.png -compress none <YourMazeName>.pgm```
