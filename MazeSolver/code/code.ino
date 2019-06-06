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

MazeRunner bot;

char path[100] = "";
unsigned char path_length = 0; // the length of the path

// Initializes the 3pi, displays a welcome message, calibrates, and
// plays the initial music.
void setup() {
 bot.setupRobot();
}

// This function decides which way to turn during the learning phase of
// maze solving.  It uses the variables found_left, found_straight, and
// found_right, which indicate whether there is an exit in each of the
// three directions, applying the "left hand on the wall" strategy.
char select_turn(unsigned char found_left, unsigned char found_straight, unsigned char found_right)
{
  // Make a decision about how to turn.  The following code
  // implements a left-hand-on-the-wall strategy, where we always
  // turn as far to the left as possible.
  if(found_left)
  return 'L';
  else if(found_straight)
  return 'S';
  else if(found_right)
  return 'R';
  else
  return 'B';
}

// This function is called once.
void loop()
{
  // Loop until we have solved the maze.

    // FIRST MAIN LOOP BODY
    bot.straightUntilIntersection();
    delay(1000);
    
  // Solved the maze!

  while (1) {
    bot.stop();
  }
}
