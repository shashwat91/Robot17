#include "Ultrasonic.h"

#include <Arduino.h>

Ultrasonic::Ultrasonic(int trigger, int echo)
{
   trigPin=trigger;
   echoPin=echo;
   pinMode(trigger,OUTPUT);
   pinMode(echo,INPUT);
}

long Ultrasonic::time()
{
  long duration;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin,HIGH);
  return duration;
}

long Ultrasonic::distance_cm()
{
  long duration = time();
  long dist = duration / (2 * 29) ;
  return dist;
}

long Ultrasonic::distance_in()
{
  long duration = time();
  long dist = duration / (2 * 74);
  return dist;
}
