#include "RobotMaze.h"
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

RobotMaze bot(5,0);

char path[100] = "";
unsigned char path_length = 0; // the length of the path

// Initializes the 3pi, displays a welcome message, calibrates, and
// plays the initial music.
void setup() {
 bot.setupRobot();
 bot.learnMaze();
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
  while (1)
  {
    // Beep to show that we solved the maze.
    OrangutanMotors::setSpeeds(0, 0);
    OrangutanBuzzer::play(">>a32");

    // Wait for the user to press a button, while displaying
    // the solution.
    while (!OrangutanPushbuttons::isPressed(BUTTON_B))
    {
        OrangutanLCD::clear();
        OrangutanLCD::print("Solved!");
        delay(30);
    }
    while (OrangutanPushbuttons::isPressed(BUTTON_B));

    delay(1000);
    bot.solveMaze();
  }
}
