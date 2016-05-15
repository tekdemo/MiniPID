#include "miniPID.h"
#include "Arduino.h"

#define SETPOINTPIN 0
#define FEEDBACKPIN 1
#define OUTPUTPIN 10

pid=miniPID(1,.1,.01);
unsigned long int t;

void setup(){
	t=0;
}

void loop(){
	double output=pid.getOutput(analogRead(FEEDBACKPIN),analogRead(SETPOINTPIN););
	analogWrite(OUTPUTPIN,output);

	//Wait until the next 50ms interval
	while(millis()-t<50) delay(1);
	t=millis();
}
