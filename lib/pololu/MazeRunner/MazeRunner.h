/*
  MazeRunner.h - Library for the Maze Runner Competition.
  Created by Manav Gupta, Jun 3, 2019.
  Released into the public domain.
*/
#ifndef MazeRunner_h
#define MazeRunner_h

#ifndef LIB_POLOLU
#ifndef ARDUINO

#include "../OrangutanAnalog/OrangutanAnalog.h"
#include "../OrangutanBuzzer/OrangutanBuzzer.h"
#include "../OrangutanMotors/OrangutanMotors.h"
#include "../OrangutanLCD/OrangutanLCD.h"
#include "../OrangutanLEDs/OrangutanLEDs.h"
#include "../OrangutanPushbuttons/OrangutanPushbuttons.h"
#include "../OrangutanTime/OrangutanTime.h"
#include "../OrangutanSerial/OrangutanSerial.h"
#include "../OrangutanServos/OrangutanServos.h"
#include "../PololuWheelEncoders/PololuWheelEncoders.h"
#include "../OrangutanResources/OrangutanResources.h"
#include "../OrangutanDigital/OrangutanDigital.h"
#include "../OrangutanPulseIn/OrangutanPulseIn.h"
#include "../OrangutanSPIMaster/OrangutanSPIMaster.h"

#endif
#endif

class MazeRunner
{
  public:
    MazeRunner(){};
    static void setupRobot();
    void turn(char dir);
    unsigned int straightUntilIntersection();
   	void directionsAvailable(unsigned int *direction_array);
   	unsigned char isEndOfMaze();
  private:
  	void load_custom_characters();
  	void display_readings(const unsigned int *calibrated_values);
};

#endif
