/**
* Group number : 17
* Student 1:
* Shashwat Verma, 4507746
* Student 2:
* Ishu Goel, 4632699
*/

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
	if(linear == 0 && turn == 0)		//Directly stopping motors
	{
		stop();
		return;
	}

	float vel = fabs(linear);	//Calculating velocity for motors
	if(fabs(turn) > vel)
		vel = fabs(turn);

	if(vel > MAX_ROS_VALUE)
		vel = MAX_ROS_VALUE;
	else if(vel < -MAX_ROS_VALUE)
		vel = -MAX_ROS_VALUE;
		
	if(linear > 0)
	{//run forward
		setSpeed(MAX_PWM_VALUE * vel, MAX_PWM_VALUE * vel);   
	}
	else if(linear < 0)
	{//run backward
		setSpeed(MAX_PWM_VALUE * vel *(-1), MAX_PWM_VALUE * vel *(-1)); 
	}

	if( turn > 0)
	{//go right
		setSpeed(MAX_PWM_TURN * vel, MAX_PWM_TURN * vel * (-0.5));
	}
	else if(turn< 0)
	{//go left
		setSpeed(MAX_PWM_TURN * vel * (-0.5), MAX_PWM_TURN * vel);
	}
}

void Motor::setSpeed(int pwm_l, int pwm_r)
{
	digitalWrite(L_EN,1);
	digitalWrite(R_EN,1);

	if(pwm_r > 0) //right motor forward
	{
	analogWrite(R_FWD, pwm_r);
	analogWrite(R_REV, 0);
	}
	else //right motor backwards
	{
	analogWrite(R_FWD, 0); 
	analogWrite(R_REV, fabs(pwm_r)); 
	}
	
	if(pwm_l > 0) //left motor forward
	{
	analogWrite(L_FWD, pwm_l);
	analogWrite(L_REV, 0);
	}
	else //left motor backwards
	{
	analogWrite(L_FWD, 0);
	analogWrite(L_REV, fabs(pwm_l));      
	}
	delay(100);
	stop();
}

void Motor::stop() //Stoping the motor by setting enable as LOW
{
	digitalWrite(L_EN,0);
	analogWrite(L_FWD,0);
	analogWrite(L_REV,0);
	digitalWrite(R_EN,0);
	analogWrite(R_FWD,0);
	analogWrite(R_REV,0);
}
