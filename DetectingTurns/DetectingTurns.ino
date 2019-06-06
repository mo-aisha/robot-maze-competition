#include <Pololu3pi.h>
#include <PololuQTRSensors.h>
#include <OrangutanMotors.h>
#include <OrangutanAnalog.h>
#include <OrangutanLCD.h>
#include <OrangutanPushbuttons.h>
#include <OrangutanBuzzer.h>
#include "Queue.h"

Pololu3pi bot;
OrangutanLCD lcd;
OrangutanPushbuttons button;
OrangutanMotors motors;
OrangutanBuzzer buzzer;
OrangutanAnalog analog;

// This include file allows data to be stored in program space.  The
// ATmega168 has 16k of program space compared to 1k of RAM, so large
// pieces of static data should be stored in program space.
#include <avr/pgmspace.h>

// Introductory messages.  The "PROGMEM" identifier causes the data to
// go into program space.
const char welcome_line1[] PROGMEM = "HELLO";
const char welcome_line2[] PROGMEM = "3\xf7 Robot";
const char demo_name_line1[] PROGMEM = "Maze";
const char demo_name_line2[] PROGMEM = "solver";

char path[100] = "";
unsigned char path_length = 0; // the length of the path

// A couple of simple tunes, stored in program space.
const char welcome[] PROGMEM = ">g32>>c32";
const char go[] PROGMEM = "L16 cdegreg4";
const char done[] PROGMEM = ">>a32";

// Data for generating the characters used in load_custom_characters
// and display_readings. This is also stored in program space.
const char levels[] PROGMEM = {
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111
};

// This function loads custom characters into the LCD.  Up to 8
// characters can be loaded; we use them for 7 levels of a bar graph.
void load_custom_characters()
{
  lcd.loadCustomCharacter(levels+0,0); // no offset, e.g. one bar
  lcd.loadCustomCharacter(levels+1,1); // two bars
  lcd.loadCustomCharacter(levels+2,2); // etc...
  lcd.loadCustomCharacter(levels+3,3);
  lcd.loadCustomCharacter(levels+4,4);
  lcd.loadCustomCharacter(levels+5,5);
  lcd.loadCustomCharacter(levels+6,6);
  lcd.clear(); 
}

// This function displays the sensor readings using a bar graph.
void display_readings(const unsigned int *calibrated_values)
{
  unsigned char i;

  for(i=0;i<5;i++) {
    // Initialize the array of characters that we will use for the
    // graph.  Using the space, an extra copy of the one-bar
    // character, and character 255 (a full black box), we get 10
    // characters in the array.
    const int display_characters[10] = {' ',0,0,1,2,3,4,5,6,255};

    // The variable c will have values from 0 to 9, since
    // calibrated values are in the range of 0 to 1000, and
    // 1000/101 is 9 with integer math.
    char c = display_characters[calibrated_values[i]/101];

    // Display the bar graph character.
    lcd.print(c);
  }
}

// Initializes the 3pi, displays a welcome message, calibrates, and
// plays the initial music.
void setup()
{
  unsigned int counter; // used as a simple timer
  unsigned int sensors[5]; // an array to hold sensor values

  // This must be called at the beginning of 3pi code, to set up the
  // sensors.  We use a value of 2000 for the timeout, which
  // corresponds to 2000*0.4 us = 0.8 ms on our 20 MHz processor.
  bot.init(2000);
  load_custom_characters(); // load the custom characters
  
  // Play welcome music and display a message
  lcd.printFromProgramSpace(welcome_line1);
  lcd.gotoXY(0,1);
  lcd.printFromProgramSpace(welcome_line2);
  buzzer.playFromProgramSpace(welcome);
  delay(1000);

  lcd.clear();
  lcd.printFromProgramSpace(demo_name_line1);
  lcd.gotoXY(0,1);
  lcd.printFromProgramSpace(demo_name_line2);
  delay(1000);

  // Display battery voltage and wait for button press
  while(!button.isPressed(BUTTON_B))
  {
    int bat = analog.readBatteryMillivolts();

    lcd.clear();
    lcd.print(bat);
    lcd.print("mV");
    lcd.gotoXY(0,1);
    lcd.print("Press B");

    delay(100);
  }

  // Always wait for the button to be released so that 3pi doesn't
  // start moving until your hand is away from it.
  button.waitForRelease(BUTTON_B);
  delay(1000);

  // Auto-calibration: turn right and left while calibrating the
  // sensors.
  for(counter=0;counter<80;counter++)
  {
    if(counter < 20 || counter >= 60)
      motors.setSpeeds(40,-40);
    else
      motors.setSpeeds(-40,40);

    // This function records a set of sensor readings and keeps
    // track of the minimum and maximum values encountered.  The
    // IR_EMITTERS_ON argument means that the IR LEDs will be
    // turned on during the reading, which is usually what you
    // want.
    bot.calibrateLineSensors(IR_EMITTERS_ON);

    // Since our counter runs to 80, the total delay will be
    // 80*20 = 1600 ms.
    delay(20);
  }
  motors.setSpeeds(0,0);

  // Display calibrated values as a bar graph.
  while(!button.isPressed(BUTTON_B))
  {
    // Read the sensor values and get the position measurement.
    unsigned int position = bot.readLine(sensors,IR_EMITTERS_ON);

    // Display the position measurement, which will go from 0
    // (when the leftmost sensor is over the line) to 4000 (when
    // the rightmost sensor is over the line) on the 3pi, along
    // with a bar graph of the sensor readings.  This allows you
    // to make sure the robot is ready to go.
    lcd.clear();
    print_long(position);
    lcd.gotoXY(0,1);
    display_readings(sensors);

    delay(100);
  }
  button.waitForRelease(BUTTON_B);

  lcd.clear();

  lcd.print("Go!");   

  // Play music and wait for it to finish before we start driving.
  buzzer.playFromProgramSpace(go);
  while(buzzer.isPlaying());
}

void follow_line()
{
  int last_proportional = 0;
  long integral=0;

  while(1)
  {

    // Get the position of the line.
    unsigned int sensors[5];
    unsigned int position = bot.readLine(sensors,IR_EMITTERS_ON);

    // The "proportional" term should be 0 when we are on the line.
    int proportional = ((int)position) - 2000;

    // Compute the derivative (change) and integral (sum) of the
    // position.
    int derivative = proportional - last_proportional;
    integral += proportional;

    // Remember the last position.
    last_proportional = proportional;

    // Compute the difference between the two motor power settings,
    // m1 - m2.  If this is a positive number the robot will turn
    // to the left.  If it is a negative number, the robot will
    // turn to the right, and the magnitude of the number determines
    // the sharpness of the turn.
    int power_difference = proportional/20 + integral/10000 + derivative*3/2;

    // Compute the actual motor settings.  We never set either motor
    // to a negative value.
    const int max = 60; // the maximum speed
    if(power_difference > max)
    power_difference = max;
    if(power_difference < -max)
    power_difference = -max;
    
    if(power_difference < 0)
    motors.setSpeeds(max+power_difference,max);
    else
    motors.setSpeeds(max,max-power_difference);
    
    // We use the inner three sensors (1, 2, and 3) for
    // determining whether there is a line straight ahead, and the
    // sensors 0 and 4 for detecting lines going to the left and
    // right.

    if(sensors[1] < 100 && sensors[2] < 100 && sensors[3] < 100)
    {
      // There is no line visible ahead, and we didn't see any
      // intersection.  Must be a dead end.
      return;
    }
    else if(sensors[0] > 200 || sensors[4] > 200)
    {
      // Found an intersection.
      return;
    }
  }
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

// Turns according to the parameter dir, which should be 'L', 'R', 'S'
// (straight), or 'B' (back).
void turn(char dir)
{
  switch(dir)
  {
    case 'L':
    // Turn left.
    motors.setSpeeds(-80,80);
    delay(175);
    break;
    case 'R':
    // Turn right.
    motors.setSpeeds(80,-80);
    delay(175);
    break;
    case 'B':
    // Turn around.
    motors.setSpeeds(80,-80);
    delay(350);
    break;
    case 'S':
    // Don't do anything!
    break;
  }
}

// This function is called once.
void loop()
{
  // Loop until we have solved the maze.
  while(1)
  {
    // FIRST MAIN LOOP BODY
    follow_line();
    
    // Drive straight a bit.  This helps us in case we entered the
    // intersection at an angle.
    // Note that we are slowing down - this prevents the robot
    // from tipping forward too much.
    motors.setSpeeds(50,50);
    delay(50);

    // These variables record whether the robot has seen a line to the
    // left, straight ahead, and right, while examining the current
    // intersection.
    unsigned char found_left=0;
    unsigned char found_straight=0;
    unsigned char found_right=0;

    unsigned int sensors[5];
    lcd.clear();
    read_line(sensors,IR_EMITTERS_ON);

    // Check for left and right exits.
    if(sensors[0] > 100) {
      found_left = 1;
      lcd.print("L ");
    }
        
    if(sensors[4] > 100) {
      found_right = 1;
      lcd.print("R ");
    }
      
    // Drive straight a bit more - this is enough to line up our
    // wheels with the intersection.
    motors.setSpeeds(40,40);
    delay(200);
      

    // Check for a straight exit.
    unsigned int position = read_line(sensors,IR_EMITTERS_ON);
    if(position >= 1000 && position <= 3000)
    {
      found_straight = 1;
      print("S ");
    }
  

    // Check for the ending spot.
    // If all three middle sensors are on dark black, we have
    // solved the maze.
        if (sensors[1] > 600 && sensors[2] > 600 && sensors[3] > 600 && sensors[4] > 600 && sensors[0] > 600) {
      break;
    }
    

    // Intersection identification is complete.
    // If the maze has been solved, we can follow the existing
    // path.  Otherwise, we need to learn the solution.
    unsigned char dir = select_turn(found_left, found_straight, found_right);
    
    motors.setSpeeds(0,0);
    delay(800);
    motors.setSpeeds(40,40);
    // Make the turn indicated by the path.
    turn(dir);

    // Store the intersection in the path variable.
    path[path_length] = dir;
    path_length ++;

    // Display the path on the LCD.
    // display_path();
  }
  // Solved the maze!

  while (1) {
    motors.setSpeeds(0, 0);
  }
}
