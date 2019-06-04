/*
  MazeRunner.h - Library for the Maze Runner Competition.
  Created by Manav Gupta, Jun 3, 2019.
  Released into the public domain.
*/
#ifndef MazeRunner_h
#define MazeRunner_h


#include "../Pololu3pi/Pololu3pi.h"
#include "../PololuQTRSensors/PololuQTRSensors.h"
#include "../OrangutanMotors/OrangutanMotors.h"
#include "../OrangutanAnalog/OrangutanAnalog.h"
#include "../OrangutanLCD/OrangutanLCD.h"
#include "../OrangutanPushbuttons/OrangutanPushbuttons.h"
#include "../OrangutanBuzzer/OrangutanBuzzer.h"

class MazeRunner
{
  public:
    MazeRunner();
    void setupRobot();
    void turn(char dir);
    unsigned int straightUntilIntersection();
   	void directionsAvailable(unsigned int *direction_array);
    unsigned char isEndOfMaze();
    void stop();
  private:
  	void load_custom_characters();
  	void display_readings(const unsigned int *calibrated_values);
};

#endif
