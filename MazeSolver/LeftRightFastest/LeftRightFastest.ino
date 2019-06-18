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

char left_path[100] = "";
char right_path[100] = "";
char path[100] = "";

unsigned char left_path_length = 0; // the length of the path
unsigned char right_path_length = 0; // the length of the path
unsigned char path_length = 0;
// Initializes the 3pi, displays a welcome message, calibrates, and
// plays the initial music.
void setup() {
 bot.setupRobot();
}

// This function decides which way to turn during the learning phase of
// maze solving.  It uses the variables found_left, found_straight, and
// found_right, which indicate whether there is an exit in each of the
// three directions, applying the "left hand on the wall" strategy.
char select_left_turn(unsigned char found_left, unsigned char found_straight, unsigned char found_right)
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

char select_right_turn(unsigned char found_left, unsigned char found_straight, unsigned char found_right)
{
  // Make a decision about how to turn.  The following code
  // implements a left-hand-on-the-wall strategy, where we always
  // turn as far to the left as possible.
  if(found_right)
  return 'R';
  else if(found_straight)
  return 'S';
  else if(found_left)
  return 'L';
  else
  return 'B';
}

// Path simplification.  The strategy is that whenever we encounter a
// sequence xBx, we can simplify it by cutting out the dead end.  For
// example, LBL -> S, because a single S bypasses the dead end
// represented by LBL.
void simplify_left_path()
{
  // only simplify the path if the second-to-last turn was a 'B'
  if (left_path_length < 3 || left_path[left_path_length-2] != 'B')
    return;

  int total_angle = 0;
  int i;
  for (i = 1; i <= 3; i++)
  {
    switch (left_path[left_path_length - i])
    {
    case 'R':
      total_angle += 90;
      break;
    case 'L':
      total_angle += 270;
      break;
    case 'B':
      total_angle += 180;
      break;
    }
  }

  // Get the angle as a number between 0 and 360 degrees.
  total_angle = total_angle % 360;

  // Replace all of those turns with a single one.
  switch (total_angle)
  {
  case 0:
    left_path[left_path_length - 3] = 'S';
    break;
  case 90:
    left_path[left_path_length - 3] = 'R';
    break;
  case 180:
    left_path[left_path_length - 3] = 'B';
    break;
  case 270:
    left_path[left_path_length - 3] = 'L';
    break;
  }

  // The path is now two steps shorter.
  left_path_length -= 2;
}

void simplify_right_path()
{
  // only simplify the path if the second-to-last turn was a 'B'
  if (right_path_length < 3 || right_path[right_path_length-2] != 'B')
    return;

  int total_angle = 0;
  int i;
  for (i = 1; i <= 3; i++)
  {
    switch (right_path[right_path_length - i])
    {
    case 'R':
      total_angle += 90;
      break;
    case 'L':
      total_angle += 270;
      break;
    case 'B':
      total_angle += 180;
      break;
    }
  }

  // Get the angle as a number between 0 and 360 degrees.
  total_angle = total_angle % 360;

  // Replace all of those turns with a single one.
  switch (total_angle)
  {
  case 0:
    right_path[right_path_length - 3] = 'S';
    break;
  case 90:
    right_path[right_path_length - 3] = 'R';
    break;
  case 180:
    right_path[right_path_length - 3] = 'B';
    break;
  case 270:
    right_path[right_path_length - 3] = 'L';
    break;
  }

  // The path is now two steps shorter.
  right_path_length -= 2;
}

void display_path()
{
  // Set the last character of the path to a 0 so that the print()
  // function can find the end of the string.  This is how strings
  // are normally terminated in C.
  path[path_length] = 0;

  OrangutanLCD::clear();
  OrangutanLCD::print(path);

  if (path_length > 8)
  {
    OrangutanLCD::gotoXY(0, 1);
    OrangutanLCD::print(path + 8);
  }
}

// This function is called once.
void loop()
{
      
  // Loop until we have solved the maze.
  while(1)
  {
    // FIRST MAINL OOP BODY
    bot.straightUntilIntersection();
    delay(500);
    // Check for the ending spot.
    // If all three middle sensors are on dark black, we have
    // solved the maze.
    unsigned int directions[3];
    bot.directionsAvailable(directions);
    delay(500);
      if (bot.isEndOfMaze()) {
      break;
    }

    // Intersection identification is complete.
    // If the maze has been solved, we can follow the existing
    // path.  Otherwise, we need to learn the solution.
    unsigned char dir = select_left_turn(directions[0], directions[1], directions[2]);

    // Make the turn indicated by the path.
    bot.turn(dir);
    delay(500);
    // Store the intersection in the path variable.
    left_path[left_path_length] = dir;
    left_path_length ++;

    // Display the path on the LCD.
    simplify_left_path();
    display_path();
  }
  // Solved the maze!
 
    delay(1000);
    bot.turn('B');
    delay(1000);
    // Re-run the maze.  It's not necessary to identify the
    // intersections, so this loop is really simple.
    for (int i = left_path_length-1; i >= 0; i--)
    {
      bot.straightUntilIntersection();
      delay(500);
    unsigned int directions[3];
    bot.directionsAvailable(directions);
    delay(500);      // Drive straight while slowing down, as before.

      // Make a turn according to the instruction stored in
      // path[i].
      if(left_path[i] == 'R'){
         bot.turn('L');
      }    else  if(left_path[i] == 'L'){
         bot.turn('R');
      } else {
      bot.turn(left_path[i]);
      }
      delay(500);
    }

    // Follow the last segment up to the finish.
    bot.straightUntilIntersection();

  delay(1000);
    bot.turn('B');
    delay(1000);

    // Loop until we have solved the maze.
  while(1)
  {
    // FIRST MAINL OOP BODY
    unsigned int x= bot.straightUntilIntersection();
    delay(500);
    // Check for the ending spot.
    // If all three middle sensors are on dark black, we have
    // solved the maze.
    unsigned int directions[3];
    bot.directionsAvailable(directions);
    delay(500);
      if (bot.isEndOfMaze()) {
      break;
    }

    // Intersection identification is complete.
    // If the maze has been solved, we can follow the existing
    // path.  Otherwise, we need to learn the solution.
    unsigned char dir = select_right_turn(directions[0], directions[1], directions[2]);

    // Make the turn indicated by the path.
    bot.turn(dir);
    delay(500);
    // Store the intersection in the path variable.
    right_path[right_path_length] = dir;
    right_path_length ++;

    // Display the path on the LCD.
    simplify_right_path();
  }

    delay(1000);
    bot.turn('B');
    delay(1000);
    // Re-run the maze.  It's not necessary to identify the
    // intersections, so this loop is really simple.
    for (int i = left_path_length-1; i >= 0; i--)
    {
      bot.straightUntilIntersection();
      delay(500);
    unsigned int directions[3];
    bot.directionsAvailable(directions);
    delay(500);      // Drive straight while slowing down, as before.

      // Make a turn according to the instruction stored in
      // path[i].
      if(right_path[i] == 'R'){
         bot.turn('L');
      }    else  if(right_path[i] == 'L'){
         bot.turn('R');
      } else {
      bot.turn(right_path[i]);
      }
      delay(500);
    }

    // Follow the last segment up to the finish.
    bot.straightUntilIntersection();
    delay(500);
    bot.turn('B');

   if(left_path_length < right_path_length){
        OrangutanLCD::clear();
  OrangutanLCD::print("LEFT");
  path_length = left_path_length;
    for(int i = 0; i < path_length;i++){
      path[i] = left_path[i];
    }
    }  
    if(left_path_length > right_path_length){
        OrangutanLCD::clear();
  OrangutanLCD::print("RIGHT");
    path_length = right_path_length;
    for(int i = 0; i < path_length;i++){
      path[i] = right_path[i];
    }
    }
// 
//  // Now enter an infinite loop - we can re-run the maze as many
//  // times as we want to.
// 
  while (1)
  {
    // Beep to show that we solved the maze.
    OrangutanMotors::setSpeeds(0, 0);
    OrangutanBuzzer::play(">>a32");

    // Wait for the user to press a button, while displaying
    // the solution.
    while (!OrangutanPushbuttons::isPressed(BUTTON_B))
    {
      if (millis() % 2000 < 1000)
      {
        OrangutanLCD::clear();
        OrangutanLCD::print("Solved!");
        if(left_path_length > right_path_length){
          OrangutanLCD::gotoXY(0, 1);
          OrangutanLCD::print("RIGHT");
        }
        if(left_path_length < right_path_length){
          OrangutanLCD::gotoXY(0, 1);
          OrangutanLCD::print("LEFT");
        }
      }
      else
        display_path();
      delay(30);
    }
    while (OrangutanPushbuttons::isPressed(BUTTON_B));

    delay(1000);

    // Re-run the maze.  It's not necessary to identify the
    // intersections, so this loop is really simple.
    int i;
    for (i = 0; i < path_length; i++)
    {
 bot.straightUntilIntersection();
      delay(500);
    unsigned int directions[3];
    bot.directionsAvailable(directions);
      // Make a turn according to the instruction stored in
      // path[i].
      bot.turn(path[i]);
    }

    // Follow the last segment up to the finish.
 bot.straightUntilIntersection();

    // Now we should be at the finish!  Restart the loop.
  }
}
