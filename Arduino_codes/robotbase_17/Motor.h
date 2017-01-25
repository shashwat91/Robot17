#ifndef Motor_h
#define Motor_h

#define MAX_ROS_VALUE 1.0f
#define MAX_PWM_VALUE 200
#define MAX_PWM_TURN 127


class Motor
{
	public:
		Motor(int,int,int,int,int,int);
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
