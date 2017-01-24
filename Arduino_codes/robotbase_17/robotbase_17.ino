#include <ros.h>
#include <geometry_msgs/Twist.h>
#include <ArduinoHardware.h>

#include "Ultrasonic.h"
#include "Motor.h"

#define M1REV 7 //Motor on left
#define M1EN 24
#define M1FWD 6
#define M2REV 3 //Motor on right
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

void initPwmTimer()
{
  //Timer 3 and 4 are set at 10bit PWM mode. Max value=0x3FF
  TCCR3A = _BV(COM3A1)|_BV(COM3B1) | _BV(WGM31) | _BV(WGM30);
  TCCR3B = _BV(WGM32)| _BV(CS30); 
  //OCR3A -- pin 5, not set
  //OCR3B -- pin 2, Motor 2 FWD ,right
  //OCR3C -- pin 4, Motor 2 REV 
  TCCR4A = _BV(COM4C1)|_BV(COM4B1) | _BV(WGM41) | _BV(WGM40);
  TCCR4B = _BV(WGM42)| _BV(CS40);
  //OCR4A -- pin 6, Motor 1 FWD, left
  //OCR4B -- pin 7, Motor 1 REV
  //OCR4C -- pin 8, not set
}

void setup()
{ 
  pinMode(13, OUTPUT);
  initTimer1(); //Interrupt timer for every 100ms
  initPwmTimer();
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
