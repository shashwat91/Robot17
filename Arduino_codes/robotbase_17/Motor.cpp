#include "Motor.h"
#include <Arduino.h>
#include <math.h>

Motor::Motor(int lEn, int lFwd, int lRev, int rEn, int rFwd, int rRev)
{
	//Assigning pins
	L_EN = lEn;
	L_FWD = lFwd;
	L_REV = lRev;
  R_EN = rEn;
  R_FWD = rFwd;
  R_REV = rRev;
  //Declaring as output pins
  pinMode(L_EN, OUTPUT);
  pinMode(L_FWD, OUTPUT);
  pinMode(L_REV, OUTPUT);
  pinMode(R_EN, OUTPUT);
  pinMode(R_FWD, OUTPUT);
  pinMode(R_REV, OUTPUT);
  //Making sure that initially motors are in stop state
  stop();
}

void Motor::setMotor(float linear, float turn)
{
  if(linear == 0 && turn == 0)
    {
      stop();
      return;
    }

  float vel = fabs(linear);
  if(fabs(turn) > vel)
    vel = fabs(turn);

  if(vel > MAX_ROS_VALUE)
    vel = MAX_ROS_VALUE;
  else if(vel < -MAX_ROS_VALUE)
    vel = -MAX_ROS_VALUE;
    
  if(linear > 0)
  { 
    //run forward
    setSpeed(MAX_PWM_VALUE * vel, MAX_PWM_VALUE * vel);   
  }
  
  if(linear < 0)
  {
    //run backward
    setSpeed(MAX_PWM_VALUE * vel, MAX_PWM_VALUE * vel); 
  }

  if( turn < 0)
  {
    //go right
    setSpeed(MAX_PWM_VALUE * vel, MAX_PWM_VALUE * vel * (-1));
  }
  
  if(turn> 0)
  {
  //go left
    setSpeed(MAX_PWM_VALUE * vel, MAX_PWM_VALUE * vel * (-1));
  }
}

void Motor::setSpeed(int pwm_l, int pwm_r)
{
  
}

void Motor::stop()
{
  analogWrite(L_EN,0);
  analogWrite(L_FWD,0);
  analogWrite(L_REV,0);
  analogWrite(R_EN,0);
  analogWrite(R_FWD,0);
  analogWrite(R_REV,0);
}
