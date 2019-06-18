/*
  MazeRunner.h - Library for the Maze Runner Competition.
  Created by Manav Gupta, Jun 3, 2019.
  Released into the public domain.
*/
#ifndef RobotMaze_h
#define RobotMaze_h


#include <MazeRunner.h>

class RobotMaze
{
  public:
    RobotMaze(int start_x, int start_y);
    void setupRobot();
    void turn(char dir);
    void straightUntilIntersection();
    void directionsAvailable(unsigned int *directions);
    void getEndOfMazeCoordinates(int *coordinates);
    void getCurrentLocation(unsigned int *direction_array);
    char getCurrentDirection();
    void evaluateCurrentNode();
    void gotoCoordinates(int x, int y);
    int dijkstras(int x, int y, char *path);
    void learnMaze();
    void solveMaze();
  private:
    int maze[14][10][4];
    char directionFacing = 'N';
    int x;
    int y;
    int endOfMazeCoordinates[2];
    void turnNorth();
    int startCoordinates[2];
    double queueNumber(int x, int y, int dir);


};

#endif
