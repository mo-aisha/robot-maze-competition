#include "RobotMaze.h"
#include "Arduino.h"

#include <MazeRunner.h>
#include "Queue.h"
#include <stdlib.h>
#include <string.h>

#include <avr/pgmspace.h>

static MazeRunner bot;
Queue<double> queue(100);

RobotMaze::RobotMaze(int start_x, int start_y) {
  x = start_x;
  y = start_y;
  startCoordinates[0] = x;
  startCoordinates[1] = y;
  endOfMazeCoordinates[0] = x;
  endOfMazeCoordinates[1] = y;
  maze[y][x][1] = 1;
}

void RobotMaze::setupRobot() {
  bot.setupRobot();
}

void RobotMaze::directionsAvailable(unsigned int *directions) {
  bot.directionsAvailable(directions);
}


void RobotMaze::straightUntilIntersection() {
  unsigned int noOfSquaresMoved = bot.straightUntilIntersection();
  for(int i = 1; i < noOfSquaresMoved; i++){
    switch(directionFacing) {
      case 'N':
        y++;
        maze[y][x][1] = 1;
        maze[y][x][3] = 1;
        break;
      case 'S':
        y--;
        maze[y][x][1] = 1;
        maze[y][x][3] = 1;
        break;
      case 'E':
        x++;
        maze[y][x][0] = 1;
        maze[y][x][2] = 1;
        break;
      case 'W':
        x--;
        maze[y][x][0] = 1;
        maze[y][x][2] = 1;
        break;
    }
  }
  switch(directionFacing) {
      case 'N':
        y++;
        break;
      case 'S':
        y--;
        break;
      case 'E':
        x++;
        break;
      case 'W':
        x--;
        break;
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
  delay(100);
  switch(directionFacing) {
    case('N'):
      if(directionsAvailable[0] == 1) {
        maze[y][x][0] = 1;
      }
      if(directionsAvailable[1] == 1) {
        maze[y][x][1] = 1;
      }
      if(directionsAvailable[2] == 1) {
        maze[y][x][2] = 1;
      }
      maze[y][x][3] = 1;
      if(bot.isEndOfMaze()) {
                 maze[y][x][0] = 0;
         maze[y][x][1] = 0;
         maze[y][x][2] = 0;
         maze[y][x][3] = 1;

        endOfMazeCoordinates[0] = x;
        endOfMazeCoordinates[1] = y;
      }
      break;
    case('S'):
      if(directionsAvailable[0] == 1) {
        maze[y][x][2] = 1;
      }
      if(directionsAvailable[1] == 1) {
        maze[y][x][3] = 1;
      }
      if(directionsAvailable[2] == 1) {
        maze[y][x][0] = 1;
      }
      maze[y][x][1] = 1;
      if(bot.isEndOfMaze()) {
                 maze[y][x][0] = 0;
         maze[y][x][1] = 0;
         maze[y][x][2] = 0;
         maze[y][x][3] = 1;

        endOfMazeCoordinates[0] = x;
        endOfMazeCoordinates[1] = y;
      }
      break;
    case('E'):
      if(directionsAvailable[0] == 1) {
        maze[y][x][1] = 1;
      }
      if(directionsAvailable[1] == 1) {
        maze[y][x][2] = 1;
      }
      if(directionsAvailable[2] == 1) {
        maze[y][x][3] = 1;
      }
      maze[y][x][0] = 1;
      if(bot.isEndOfMaze()) {
         maze[y][x][0] = 0;
         maze[y][x][1] = 0;
         maze[y][x][2] = 0;
         maze[y][x][3] = 1;

        endOfMazeCoordinates[0] = x;
        endOfMazeCoordinates[1] = y;
      }
      break;
    case('W'):
      if(directionsAvailable[0] == 1) {
        maze[y][x][3] = 1;
      }
      if(directionsAvailable[1] == 1) {
        maze[y][x][0] = 1;
      }
      if(directionsAvailable[2] == 1) {
        maze[y][x][1] = 1;
      }
      maze[y][x][2] = 1;
      if(bot.isEndOfMaze()) {
                 maze[y][x][0] = 0;
         maze[y][x][1] = 0;
         maze[y][x][2] = 0;
         maze[y][x][3] = 1;

        endOfMazeCoordinates[0] = x;
        endOfMazeCoordinates[1] = y;
      }
      break;
  }
}

void RobotMaze::gotoCoordinates(int go_x, int go_y) {
  char path[20];
  int length = dijkstras(go_x, go_y, path);
  for(int i = 0; i < length; i++){
    turnNorth();
    turn(path[i]);
    straightUntilIntersection();
  }
}

double RobotMaze::queueNumber(int x, int y, int dir){
  int newX = x;
  int newY = y;
  switch(dir){
    //L
    case 0:
      newX --;
      break;
      //S
    case 1:
      newY ++;
      break;
      //R
    case 2:
      newX ++;
      break;
      //B
    case 3:
      newY --;
      break;
  }
  return newY + (newX/10.0);
}

int RobotMaze::dijkstras(int go_x, int go_y, char *path) {
  //TODO
  if(go_x == x && go_y == y){
    return 0;
  }
  int dijkstasNodes[14][10][2];
  char paths[14][10][20];

  int selected_x = x;
  int selected_y = y;
  while(dijkstasNodes[go_y][go_x][0] != 1){
    int selected_dist = dijkstasNodes[selected_y][selected_x][1];
    char selected_path[20];
    for (int i = 0; i < 20; i++) // ...initialize it
{
    selected_path[i] = paths[selected_y][selected_x][i];
}


    //Only add to path if it is intersection and 
    // Only change path/nodes if node not final
    if(maze[selected_y][selected_x][0] == 1 && dijkstasNodes[selected_y][selected_x - 1][0] == 0 && ((dijkstasNodes[selected_y][selected_x - 1][1] > selected_dist + 1) || (dijkstasNodes[selected_y][selected_x - 1][1] == 0))) {
        dijkstasNodes[selected_y][selected_x - 1][1] = selected_dist + 1;
        if(maze[selected_y][selected_x][1] == 1 || maze[selected_y][selected_x][3] == 1) {
            strcpy (paths[selected_y][selected_x - 1], selected_path);
           strcat (paths[selected_y][selected_x - 1] ,"L");
        }
    } 
    if(maze[selected_y][selected_x][1] == 1 && dijkstasNodes[selected_y + 1][selected_x][0] == 0 && ((dijkstasNodes[selected_y + 1][selected_x][1] > selected_dist + 1) || (dijkstasNodes[selected_y + 1][selected_x][1] == 0))) {
        dijkstasNodes[selected_y + 1][selected_x][1] = selected_dist + 1;
        if(maze[selected_y][selected_x][0] == 1 || maze[selected_y][selected_x][2] == 1) {
                     strcpy (paths[selected_y + 1][selected_x], selected_path);
           strcat (paths[selected_y + 1][selected_x] ,"S");
        }
    } 
    if(maze[selected_y][selected_x][2] == 1 && dijkstasNodes[selected_y][selected_x + 1][0] == 0 && ((dijkstasNodes[selected_y][selected_x + 1][1] > selected_dist + 1) || (dijkstasNodes[selected_y][selected_x + 1][1] == 0))) {
        dijkstasNodes[selected_y][selected_x + 1][1] = selected_dist + 1;
        if(maze[selected_y][selected_x][1] == 1 || maze[selected_y][selected_x][3] == 1) {
           strcpy (paths[selected_y][selected_x + 1], selected_path);
           strcat (paths[selected_y][selected_x + 1] ,"R");
        }
    } 
    if(maze[selected_y][selected_x][3] == 1 && dijkstasNodes[selected_y - 1][selected_x][0] == 0 && ((dijkstasNodes[selected_y - 1][selected_x][1] > selected_dist + 1) || (dijkstasNodes[selected_y - 1][selected_x][1] == 0))) {
        dijkstasNodes[selected_y - 1][selected_x][1] = selected_dist + 1;
        if(maze[selected_y][selected_x][0] == 1 || maze[selected_y][selected_x][2] == 1) {
                     strcpy (paths[selected_y - 1][selected_x], selected_path);
           strcat (paths[selected_y - 1][selected_x] ,"B");
        }
    }
    dijkstasNodes[selected_y][selected_x][0] = 1;
    int new_current_min = 100;
    int new_x = 0;
    int new_y = 0;
    for(int i = 0; i < 14; i ++){
      for(int j = 0; j < 10; j ++){
        if(dijkstasNodes[i][j][0] == 0){
          if(dijkstasNodes[i][j][1] < new_current_min){
            new_x = j;
            new_y = i;
            new_current_min = dijkstasNodes[i][j][1];
          }
        }
      }
    }
  selected_x = new_x;
  selected_y = new_y;
  }
   path = paths[go_y][go_x];
   return dijkstasNodes[go_y][go_x][1];
}

void RobotMaze::learnMaze() {
  straightUntilIntersection();
  delay(100);
  evaluateCurrentNode();
  delay(100);
  for(int dir = 0;dir < 4; dir ++) {
    if(maze[y][x][dir] == 1) {
      OrangutanLCD::clear();
    OrangutanLCD::print(x);
    OrangutanLCD::print(y);
    OrangutanLCD::print(dir);
    delay(1000);
        queue.push(queueNumber(x,y,dir));
    }
  }
  while(queue.count() > 0){
    double newNode = queue.pop();
    double newDoubleX, newDoubleY;
    newDoubleX = modf (newNode , &newDoubleY);
    int newX = (int) (newDoubleX * 10);
    int newY = (int) newDoubleY;

    if(maze[newY][newX][0] == 0 && maze[newY][newX][1] == 0 && maze[newY][newX][2] == 0 && maze[newY][newX][3] == 0) {

      if((x != newX) || (y != newY)){
        gotoCoordinates(newX, newY);
        evaluateCurrentNode();
        for(int dir = 0;dir < 4; dir ++) {
          if(maze[y][x][dir] == 1) {
              queue.push(queueNumber(x,y,dir));
          }  
        }
      } 
    }
  }
}

void RobotMaze::turnNorth() {
  while(directionFacing != 'N'){
    turn('L');
  }
}

void RobotMaze::solveMaze() {
  x = startCoordinates[0];
  y = startCoordinates[1];
  directionFacing = 'N';
  gotoCoordinates(endOfMazeCoordinates[0], endOfMazeCoordinates[1]);
}





