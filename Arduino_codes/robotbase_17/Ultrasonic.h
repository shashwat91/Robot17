/**
* Group number : 17
* Student 1:
* Shashwat Verma, 4507746
* Student 2:
* Ishu Goel, 4632699
*/

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
