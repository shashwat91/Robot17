#include <ros.h>
#include <geometry_msgs/Twist.h>
#include <ArduinoHardware.h>

#include "Motor.h"
#include "Ultrasonic.h"

#define M1REV 7
#define M1EN 24
#define M1FWD 6
#define M2REV 3
#define M2EN 25
#define M2FWD 2

#define trigPin 23
#define echoPin 22

void setup()
{
  Motor motorR = Motor(M1EN, M1FWD, M1REV);
  Motor motorL = Motor(M2EN, M2FWD, M2REV);
  
}

void loop()
{
  
}
