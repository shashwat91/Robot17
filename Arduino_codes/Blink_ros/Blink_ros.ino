/* 
 * rosserial Subscriber Example
 * Blinks an LED on callback
 */

#include <ros.h>
#include <geometry_msgs/Twist.h>
#include <ArduinoHardware.h>
#define M1REV 7
#define M1EN 24
#define M1FWD 6
#define M2REV 3
#define M2EN 25
#define M2FWD 2
//ros::NodeHandle  nh;

void messageCb( const geometry_msgs::Twist& serial_msg){
  func1(serial_msg.linear.x,serial_msg.angular.z);
  digitalWrite(13, HIGH-digitalRead(13));   // blink the led
}

ros::Subscriber<geometry_msgs::Twist> sub("cmd_vel", &messageCb );

class NewHardware : public ArduinoHardware{
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
  digitalWrite(M1EN,HIGH);
  digitalWrite(M2EN,HIGH);
  digitalWrite(M1FWD,LOW);
  digitalWrite(M1REV,LOW);
  digitalWrite(M2FWD,LOW);
  digitalWrite(M2REV,LOW);
  pinMode(13, OUTPUT);
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
  if(x == -1)
  { 
    //run forward
    digitalWrite(M1FWD,HIGH);
    digitalWrite(M1REV,LOW);
    digitalWrite(M2FWD,HIGH);
    digitalWrite(M2REV,LOW);
    delay(1000);
    
  }
  
  if(x == 1)
  {
    //run backward
    digitalWrite(M1FWD,LOW);
    digitalWrite(M1REV,HIGH);
    digitalWrite(M2FWD,LOW);
    digitalWrite(M2REV,HIGH);
    delay(1000);
  }

  if( y == -1)
  {
    //go right
    digitalWrite(M1FWD,LOW);
    digitalWrite(M1REV,HIGH);
    digitalWrite(M2FWD,HIGH);
    digitalWrite(M2REV,LOW);
    delay(1000);
  }
  
  if(y == 1)
  {
  //go left
    digitalWrite(M1FWD,HIGH);
    digitalWrite(M1REV,LOW);
    digitalWrite(M2FWD,LOW);
    digitalWrite(M2REV,HIGH);
    delay(1000);
  }
  
}

