#include "RobotMaze.h"
#include "MazeNode.h"
#include <MazeRunner.h>
#include "Arduino.h"
#include <OrangutanMotors.h>
#include <OrangutanLCD.h>
#include <OrangutanBuzzer.h>
#include <OrangutanPushbuttons.h>
#include <Pololu3pi.h>
#include <PololuQTRSensors.h>
#include <OrangutanAnalog.h>
#include <OrangutanBuzzer.h>

RobotMaze bot(0,0);

char path[100] = "";
unsigned char path_length = 0; // the length of the path

// Initializes the 3pi, displays a welcome message, calibrates, and
// plays the initial music.
void setup() {
 bot.setupRobot();
}

// This function is called once.
void loop()
{
  
}
