#include <OrangutanMotors.h>
#include <OrangutanPushbuttons.h>

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:
  unsigned long startTime = millis();
  OrangutanMotors::setSpeeds(255,255);
  while(millis()-startTime < 1000){
    delay(5);
  }
  OrangutanMotors::setSpeeds(0,0);
  while (1);
}
