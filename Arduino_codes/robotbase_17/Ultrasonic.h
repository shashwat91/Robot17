#ifndef Ultrasonic_h
#define Ultrasonic_h

class Ultrasonic
{
	public:
		Ultrasonic(int,int);
		long distance_cm();
		long distance_in();
  
	private:
		int trigPin;
		int echoPin;
		long time();
};

#endif //Ultrasonic_h
