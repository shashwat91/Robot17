#include <ros.h>
#include <geometry_msgs/Twist.h>
#include <ArduinoHardware.h>

#include "Ultrasonic.h"
#include "Motor.h"

#define M1REV 7
#define M1EN 24
#define M1FWD 6
#define M2REV 3
#define M2EN 25
#define M2FWD 2

#define trigPin 23
#define echoPin 22

class NewHardware : public ArduinoHardware
{
  public : NewHardware():ArduinoHardware(&Serial1,57600){};
};

ros::NodeHandle_<NewHardware> nh; //--When using bluetooth for communication
//ros::NodeHandle  nh; //--When using wired communication

Ultrasonic range(trigPin, echoPin);
Motor motor(M2EN, M2FWD, M2REV, M1EN, M1FWD, M1REV);
bool stop = false;

void messageCb( const geometry_msgs::Twist& serial_msg)
{
 adjustMotors(serial_msg.linear.x,serial_msg.angular.z);
 // digitalWrite(13, HIGH-digitalRead(13));   // blink the led
}

ros::Subscriber<geometry_msgs::Twist> sub("cmd_vel", &messageCb );

void initTimer1()
{
  // Using timer1 for ultrasonic sensor
  cli();
  
  TCCR1A = 0;
  TCCR1B = 0;
  OCR1A = 6260; 
  TCCR1B |= (1 << WGM12); 
  TCCR1B |= (1 << CS12);   
  TIMSK1 |= (1 << OCIE1A); 
  
  sei();
}

void initTimer5()
{}

void setup()
{ 
  pinMode(13, OUTPUT);
  initTimer1(); //Interrupt timer for every 100ms
  initTimer5();
  nh.initNode();
  nh.subscribe(sub);
  motor.setMotor(0,0);
}

void loop()
{  
  nh.spinOnce();
  delay(10);
}

ISR(TIMER1_COMPA_vect)        // interrupt service routine 
{
  ping();
}

void adjustMotors(float x, float z)
{
  if(stop && x>=0) //Cannot go forward if distance is less than 20
    return;

  if(stop && x<0)  //Can go backwards if distance is less than 20
  {
    motor.setMotor(x,z);
    return;
  }

  motor.setMotor(x,z);
}

void ping()
{
  stop = false;
  digitalWrite(13, HIGH-digitalRead(13));   // blink the led
  long distance = range.distance_cm();  
  if(distance < 20)
    {
      stop = true;
      motor.setMotor(0,0);
    }
}
