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
  else if(linear < 0)
  {
    //run backward
    setSpeed(MAX_PWM_VALUE * vel, MAX_PWM_VALUE * vel); 
  }

  if( turn < 0)
  {
    //go right
    setSpeed(MAX_PWM_VALUE * vel, MAX_PWM_VALUE * vel * (-0.5));
  }
  else if(turn> 0)
  {
  //go left
    setSpeed(MAX_PWM_VALUE * vel * (-0.5), MAX_PWM_VALUE * vel);
  }
}

void Motor::setSpeed(int pwm_l, int pwm_r)
{
  digitalWrite(L_EN,1);
  digitalWrite(R_EN,1);

  if(pwm_r > 0) //right motor forward
  {
   OCR3B = (pwm_r / MAX_PWM_VALUE) * MAX_PWM_REG;//analogWrite(R_FWD, pwm_r);
   OCR3C = 0;//analogWrite(R_REV, 0);
  }
  else //right motor backwards
  {
   OCR3B = 0;//analogWrite(R_FWD, 0); 
   OCR3C = (fabs(pwm_r) / MAX_PWM_VALUE) * MAX_PWM_REG;//analogWrite(R_REV, fabs(pwm_r)); 
  }
    
  if(pwm_l > 0) //left motor forward
  {
   OCR4A = (pwm_r / MAX_PWM_VALUE) * MAX_PWM_REG;//analogWrite(L_FWD, pwm_l);
   OCR4B = 0;//analogWrite(L_REV, 0);
  }
  else //left motor backwards
  {
   OCR4A = 0;//analogWrite(L_FWD, 0);
   OCR4B = (fabs(pwm_r) / MAX_PWM_VALUE) * MAX_PWM_REG;//analogWrite(L_REV, fabs(pwm_l));      
  }
  delay(100);
  stop();
}

void Motor::stop()
{
  digitalWrite(L_EN,0);
  OCR4A = 0;//analogWrite(L_FWD,0);
  OCR4B = 0;//analogWrite(L_REV,0);
  digitalWrite(R_EN,0);
  OCR3B = 0;//analogWrite(R_FWD,0);
  OCR3C = 0;//analogWrite(R_REV,0);
}
