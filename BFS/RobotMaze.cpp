#include "RobotMaze.h"
#include "Arduino.h"

#include <MazeRunner.h>
#include "Queue.h"
#include <stdlib.h>

#include <avr/pgmspace.h>

static MazeRunner bot;
Queue<int> queue(100);

RobotMaze::RobotMaze(int start_x, int start_y) {
  x = start_x;
  y = start_y;
  startCoordinates[0] = x;
  startCoordinates[1] = y;
  endOfMazeCoordinates[0] = x;
  endOfMazeCoordinates[1] = y;

  
  maze[y][x][0] = 1;
}

void RobotMaze::setupRobot() {
  bot.setupRobot();
}


void RobotMaze::straightUntilIntersection() {
  unsigned int noOfSquaresMoved = bot.straightUntilIntersection();
  for(int i = 1; i < noOfSquaresMoved; i++){
    switch(directionFacing) {
      case 'N':
        y++;
        maze[y][x][2] = 1;
        maze[y][x][4] = 1;
        break;
      case 'S':
        y--;
        maze[y][x][2] = 1;
        maze[y][x][4] = 1;
        break;
      case 'E':
        x++;
        maze[y][x][1] = 1;
        maze[y][x][3] = 1;
        break;
      case 'W':
        y++;
        maze[y][x][1] = 1;
        maze[y][x][3] = 1;
        break;
    }
  } 
}

void RobotMaze::turn(char dir) {
  switch(dir) {
    case 'L':
    case 'l':
      bot.turn('L');
      switch(directionFacing) {
      case 'N':
        directionFacing = 'W';
        break;
      case 'S':
        directionFacing = 'E';
        break;
      case 'E':
        directionFacing = 'N';
        break;
      case 'W':
        directionFacing = 'S';
        break;
    }

      break;
    case 'R':
    case 'r':
      bot.turn('R');
      switch(directionFacing) {
      case 'N':
        directionFacing = 'E';
        break;
      case 'S':
        directionFacing = 'W';
        break;
      case 'E':
        directionFacing = 'S';
        break;
      case 'W':
        directionFacing = 'N';
        break;
    }
      break;
    case 'B':
    case 'b':
      bot.turn('B');
      switch(directionFacing) {
      case 'N':
        directionFacing = 'S';
        break;
      case 'S':
        directionFacing = 'N';
        break;
      case 'E':
        directionFacing = 'W';
        break;
      case 'W':
        directionFacing = 'E';
        break;
    }
      break;
  }
}


void RobotMaze::getEndOfMazeCoordinates(int *coordinates) {
  if(endOfMazeCoordinates[0] == startCoordinates[0] && endOfMazeCoordinates[1] == startCoordinates[1]) {
      coordinates[0] = -1;
      coordinates[1] = -1;
  } else {
    coordinates[0] = endOfMazeCoordinates[0];
    coordinates[1] = endOfMazeCoordinates[1];
  }
}

void RobotMaze::getCurrentLocation(unsigned int *direction_array) {
  direction_array[0] = x;
  direction_array[1] = y;
}

char RobotMaze::getCurrentDirection() {
  return directionFacing;
}

void RobotMaze::evaluateCurrentNode() {
  unsigned int directionsAvailable[3];
  bot.directionsAvailable(directionsAvailable);
  switch(directionFacing) {
    case('N'):
      if(directionsAvailable[0] == 1) {
        maze[y][x][1] = 1;
      }
      if(directionsAvailable[1] == 1) {
        maze[y][x][2] = 1;
      }
      if(directionsAvailable[2] == 1) {
        maze[y][x][3] = 1;
      }
      maze[y][x][4] = 1;
      if(bot.isEndOfMaze()) {
        maze[y][x][5] = 1;
        endOfMazeCoordinates[0] = x;
        endOfMazeCoordinates[1] = y;
      }
      break;
    case('S'):
      if(directionsAvailable[0] == 1) {
        maze[y][x][3] = 1;
      }
      if(directionsAvailable[1] == 1) {
        maze[y][x][4] = 1;
      }
      if(directionsAvailable[2] == 1) {
        maze[y][x][1] = 1;
      }
      maze[y][x][2] = 1;
      if(bot.isEndOfMaze()) {
        maze[y][x][5] = 1;
        endOfMazeCoordinates[0] = x;
        endOfMazeCoordinates[1] = y;
      }
      break;
    case('E'):
      if(directionsAvailable[0] == 1) {
        maze[y][x][2] = 1;
      }
      if(directionsAvailable[1] == 1) {
        maze[y][x][3] = 1;
      }
      if(directionsAvailable[2] == 1) {
        maze[y][x][4] = 1;
      }
      maze[y][x][1] = 1;
      if(bot.isEndOfMaze()) {
        maze[y][x][5] = 1;
        endOfMazeCoordinates[0] = x;
        endOfMazeCoordinates[1] = y;
      }
      break;
    case('W'):
      if(directionsAvailable[0] == 1) {
        maze[y][x][4] = 1;
      }
      if(directionsAvailable[1] == 1) {
        maze[y][x][1] = 1;
      }
      if(directionsAvailable[2] == 1) {
        maze[y][x][2] = 1;
      }
      maze[y][x][3] = 1;
      if(bot.isEndOfMaze()) {
        maze[y][x][5] = 1;
        endOfMazeCoordinates[0] = x;
        endOfMazeCoordinates[1] = y;
      }
      break;
  }
}

void RobotMaze::gotoCoordinates(int x, int y) {
  char path[100];
  int length = dijkstras(x, y, path);
  if(length == 0) {
    return;
  }
  for(int i = 0; i < length; i++){
    turn(path[i]);
    straightUntilIntersection();
  }
}

int RobotMaze::queueNumber(int x, int y, char dir){
  int queueNumbers = 0;
  switch(dir){
    //L
    case '1':
      queueNumbers = 1;
      //S
    case '2':
      queueNumbers = 2;
      //R
    case '3':
      queueNumbers = 3;
      //B
    case '4':
      queueNumbers = 4;
  }
  return ((queueNumbers*10) + (y))*100 + x;
}

int RobotMaze::dijkstras(int x, int y, char *path) {
  //TODO
  return 1;
}

void RobotMaze::learnMaze() {
  evaluateCurrentNode();
  for(int dir = 1;dir < 5; dir ++) {
    if(maze[y][x][dir] == 1) {
        queue.push(queueNumber(x,y,dir));
    }
  }
  while(queue.count() > 0){
    int newNode = queue.pop();
    char buffer [4];
    itoa (newNode,buffer,10); 
    int newDirection = atoi(buffer[0]);
    int newY = atoi(buffer[1]);
    buffer[0] = 0;
    buffer[1] = 0;
    int newX = atoi(buffer);
    gotoCoordinates(newX, newY);
    turnTo(newDirection);
    straightUntilIntersection();
    //queue.remove(calculateOpositeMove(newDirection))
    evaluateCurrentNode();
    if(maze[y][x][1] == 0 && maze[y][x][2] == 0 && maze[y][x][3] == 0 && maze[y][x][4] == 0) {
      for(int dir = 1;dir < 5; dir ++) {
        if(maze[y][x][dir] == 1) {
            queue.push(queueNumber(x,y,dir));
        }
      } 
    } 
  }
}

int RobotMaze::calculateOpositeMove(int direction) {
  int queueNumbers = 0;
  switch(direction){
    //L
    case '1':
      queueNumbers = 3;
      //S
    case '2':
      queueNumbers = 4;
      //R
    case '3':
      queueNumbers = 1;
      //B
    case '4':
      queueNumbers = 2;
  }
  return ((queueNumbers*10) + (y))*100 + x;
}

void RobotMaze::turnTo(int direction){
  char dir;
  switch(direction){
    case 1:
      dir = 'E';
      break;
    case 2:
      dir = 'N';
      break;
    case 3:
      dir = 'W';
      break;
    case 4:
      dir = 'S';
      break;
  }
  while(directionFacing != dir){
    turn('L');
  }
}

void RobotMaze::solveMaze() {
  x = startCoordinates[0];
  y = startCoordinates[1];
  directionFacing = 'N';
  gotoCoordinates(endOfMazeCoordinates[0], endOfMazeCoordinates[1]);
}





