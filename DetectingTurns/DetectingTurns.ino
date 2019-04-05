/*
 * Simple3piMazeSolver - demo code for the Pololu 3pi Robot
 * 
 * This code will solve a line maze constructed with a black line on a
 * white background, as long as there are no loops.  It has two
 * phases: first, it learns the maze, with a "left hand on the wall"
 * strategy, and computes the most efficient path to the finish.
 * Second, it follows its most efficient solution.
 *
 * http://www.pololu.com/docs/0J21
 * http://www.pololu.com
 * http://forum.pololu.com
 *
 */

// The following libraries will be needed by this demo
#include <Pololu3pi.h>
#include <PololuQTRSensors.h>
#include <OrangutanMotors.h>
#include <OrangutanAnalog.h>
#include <OrangutanLEDs.h>
#include <OrangutanLCD.h>
#include <OrangutanPushbuttons.h>
#include <OrangutanBuzzer.h>

Pololu3pi robot;
unsigned int sensors[5]; // an array to hold sensor values


// This include file allows data to be stored in program space.  The
// ATmega168 has 16k of program space compared to 1k of RAM, so large
// pieces of static data should be stored in program space.
#include <avr/pgmspace.h>

// Introductory messages.  The "PROGMEM" identifier causes the data to
// go into program space.
const char leftTurn[] PROGMEM = " Left";
const char rightTurn[] PROGMEM = "Right";
const char straight[] PROGMEM = "Straight";

// A couple of simple tunes, stored in program space.
const char welcome[] PROGMEM = "!T240 L8 MS >e<eb>c>d>e16>d16>cba<aa>c>e<a>d>cb<g+b>c>d<e>e<e>c<aarae<bc d>d<d>f>ad16d16>g>f>e<cr>c>e<g>d>cb<g+b>c>d<e>e<e>c<aa<aa";
const char charge[] PROGMEM = "v12 L16 o4 cfa>cra>c4";
const char oops[] PROGMEM = "v12 L16 o4 frc32<b32c32c#8cr8.erf";
const char doh[] PROGMEM = "v12 L16 o4 c8.e8f#8ag8.e8c8<a<f#<f#<f#<g";


// Initializes the 3pi, displays a welcome message, calibrates, and
// plays the initial music.  This function is automatically called
// by the Arduino framework at the start of program execution.
void setup()
{
  unsigned int counter; // used as a simple timer

    // This must be called at the beginning of 3pi code, to set up the
  // sensors.  We use a value of 2000 for the timeout, which
  // corresponds to 2000*0.4 us = 0.8 ms on our 20 MHz processor.
  robot.init(2000);

  // Always wait for the button to be released so that 3pi doesn't
  // start moving until your hand is away from it.
  OrangutanLCD::print("Press B");   
  while (!OrangutanPushbuttons::isPressed(BUTTON_B))
  {

    delay(100);
  } 
  OrangutanPushbuttons::waitForRelease(BUTTON_B);
  delay(1000);

  // Auto-calibration: turn right and left while calibrating the
  // sensors.
  for (counter=0; counter<80; counter++)
  {
    if (counter < 20 || counter >= 60)
      OrangutanMotors::setSpeeds(40, -40);
    else
      OrangutanMotors::setSpeeds(-40, 40);

    // This function records a set of sensor readings and keeps
    // track of the minimum and maximum values encountered.  The
    // IR_EMITTERS_ON argument means that the IR LEDs will be
    // turned on during the reading, which is usually what you
    // want.
    robot.calibrateLineSensors(IR_EMITTERS_ON);

    // Since our counter runs to 80, the total delay will be
    // 80*20 = 1600 ms.
    delay(20);
  }
  OrangutanMotors::setSpeeds(0, 0);

  while (!OrangutanPushbuttons::isPressed(BUTTON_B))
  {

    delay(100);
  }
  OrangutanPushbuttons::waitForRelease(BUTTON_B);
  delay(1000);
  OrangutanLCD::clear();

  OrangutanLCD::print("Go!");		

  // Play music and wait for it to finish before we start driving.
  OrangutanBuzzer::playFromProgramSpace(welcome);
  while(OrangutanBuzzer::isPlaying());
  OrangutanLCD::clear();
}


// This function, causes the 3pi to follow a segment of the maze until
// it detects an intersection, a dead end, or the finish.
void follow_segment()
{
  while(1)
  {
    robot.readLine(sensors, IR_EMITTERS_ON);

 
    OrangutanMotors::setSpeeds(20, 20);



    if (sensors[1] < 100 && sensors[2] < 100 && sensors[3] < 100) {
      return;
    }
    else if (sensors[0] > 200 || sensors[4] > 200)
    {
      return;
    }

  }
}

void showIntersection(unsigned char found_left, unsigned char found_straight, unsigned char found_right) {
  if(found_left == 1){
      OrangutanLCD::printFromProgramSpace(leftTurn);
      OrangutanBuzzer::playFromProgramSpace(oops);
      while(OrangutanBuzzer::isPlaying());
      delay(1000);
      OrangutanLCD::clear();
  }  
  if(found_right == 1){
      OrangutanLCD::printFromProgramSpace(rightTurn);
      OrangutanBuzzer::playFromProgramSpace(doh);
      while(OrangutanBuzzer::isPlaying());
      delay(1000);
      OrangutanLCD::clear();
  }  
  if(found_straight == 1){
      OrangutanLCD::printFromProgramSpace(straight);
      OrangutanBuzzer::playFromProgramSpace(charge);
      while(OrangutanBuzzer::isPlaying());
      delay(1000);
      OrangutanLCD::clear();
  }  
    
}

void loop()
{
  
  unsigned char found_left = 1;
  unsigned char found_straight = 1;
  unsigned char found_right = 1;
  robot.readLine(sensors, IR_EMITTERS_ON);
  while (found_straight == 1)
  {
    follow_segment();
    OrangutanMotors::setSpeeds(0, 0);

    found_left = 0;
    found_straight = 0;
    found_right = 0;

    if (sensors[0] > 100)
      found_left = 1;
    if (sensors[4] > 100)
      found_right = 1;

    if (sensors[1] > 200 || sensors[2] > 200 || sensors[3] > 200)
      found_straight = 1;
    showIntersection(found_left, found_straight, found_right);
    if(found_straight == 0){
      return;
    }
    OrangutanMotors::setSpeeds(20, 20);
    delay(500);
    OrangutanMotors::setSpeeds(0, 0);
  }
}


