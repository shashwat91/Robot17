/* 
 * rosserial Subscriber Example
 * Blinks an LED on callback
 */
#include <TimerOne.h>
#include <ros.h>
#include <geometry_msgs/Twist.h>
#include <ArduinoHardware.h>

#define M1REV 7
#define M1EN 24
#define M1FWD 6
#define M2REV 3
#define M2EN 25
#define M2FWD 2

#define trigPin 23
#define echoPin 22

bool stop = false;

//ros::NodeHandle  nh; --commented code, not a comment

void messageCb( const geometry_msgs::Twist& serial_msg)
{
  func1(serial_msg.linear.x,serial_msg.angular.z);
 // digitalWrite(13, HIGH-digitalRead(13));   // blink the led
}

ros::Subscriber<geometry_msgs::Twist> sub("cmd_vel", &messageCb );
class NewHardware : public ArduinoHardware
{
  public : NewHardware():ArduinoHardware(&Serial1,57600){};
};

ros::NodeHandle_<NewHardware> nh;

void setup()
{ 
  pinMode(M1REV, OUTPUT);
  pinMode(M1EN, OUTPUT);
  pinMode(M1FWD, OUTPUT);
  pinMode(M2REV, OUTPUT);
  pinMode(M2EN, OUTPUT);
  pinMode(M2FWD, OUTPUT);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  
  digitalWrite(M1EN,HIGH);
  digitalWrite(M2EN,HIGH);
  digitalWrite(M1FWD,LOW);
  digitalWrite(M1REV,LOW);
  digitalWrite(M2FWD,LOW);
  digitalWrite(M2REV,LOW);
  pinMode(13, OUTPUT);

  Timer1.initialize(100000);
  Timer1.attachInterrupt(ping); 

  nh.initNode();
  nh.subscribe(sub);
}

void loop()
{  
  nh.spinOnce();
  digitalWrite(M1FWD,LOW);
  digitalWrite(M1REV,LOW);
  digitalWrite(M2FWD,LOW);
  digitalWrite(M2REV,LOW);
  delay(1);
}

void func1(float x, float y)
{
  if(stop)
    return;
    
  if(x == -1)
  { 
    //run forward
    digitalWrite(M1FWD,HIGH);
    digitalWrite(M1REV,LOW);
    digitalWrite(M2FWD,HIGH);
    digitalWrite(M2REV,LOW);
    delay(100);
    
  }
  
  if(x == 1)
  {
    //run backward
    digitalWrite(M1FWD,LOW);
    digitalWrite(M1REV,HIGH);
    digitalWrite(M2FWD,LOW);
    digitalWrite(M2REV,HIGH);
    delay(100);
  }

  if( y == -1)
  {
    //go right
    digitalWrite(M1FWD,LOW);
    digitalWrite(M1REV,HIGH);
    digitalWrite(M2FWD,HIGH);
    digitalWrite(M2REV,LOW);
    delay(100);
  }
  
  if(y == 1)
  {
  //go left
    digitalWrite(M1FWD,HIGH);
    digitalWrite(M1REV,LOW);
    digitalWrite(M2FWD,LOW);
    digitalWrite(M2REV,HIGH);
    delay(100);
  }
  
}

void ping()
{
  stop = false;
  digitalWrite(13, HIGH-digitalRead(13));   // blink the led
  long duration;
  long distance;
  digitalWrite(trigPin, LOW);
  delay(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration/2) / 29.1;
  if(distance < 20)
    {
      stop = true;
      motor_stop();
    }
}

void motor_stop()
{
    digitalWrite(M1FWD,LOW);
  digitalWrite(M1REV,LOW);
  digitalWrite(M2FWD,LOW);
  digitalWrite(M2REV,LOW);
}