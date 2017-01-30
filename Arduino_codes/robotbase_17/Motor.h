/**
* Group number : 17
* Student 1:
* Shashwat Verma, 4507746
* Student 2:
* Ishu Goel, 4632699
*/

#ifndef Motor_h
#define Motor_h

#define MAX_ROS_VALUE 1.0f 		//Maximum linear value, above this constant speed
#define MAX_PWM_VALUE 200		//PWM limited at 80% duty cycle for forward and reverse
#define MAX_PWM_TURN 127		//Slower speed on turns

class Motor
{
	public:
		Motor(int,int,int,int,int,int);		//Set motor variables
		void setMotor(float,float);
	private:
		int L_EN;
		int L_FWD;
		int L_REV;
		int R_EN;
		int R_FWD;
		int R_REV; 
		void setSpeed(int,int);
		void stop();
};

#endif //Motor_h
