#include <Arduino.h>
#include <MiniPID.h>

// Create a PID system. 
// It's not very well tuned, but some sample values will get things started. 
MiniPID pid=MiniPID(.1,.01,0);

void setup(){
	pid.setOutputLimits(-100,100);
	pid.setOutputRampRate(10);
	// ... add any other configuration options for the PID here. 

	// Plug a potentiometer into these Analog pins
	// Rotate it to adjust the "target" your system will try to go to.
	pinMode(A5,OUTPUT);
	pinMode(A4,INPUT);
	pinMode(A3,OUTPUT);
}

double setpoint=0;
double sensor=0;

void loop(){
	// This is simply an analogRead, but weighted against the last few readings.
	// It helps prevent the potentiometer readings from jumping around
	// due to "noise" on the circuit. 
	setpoint=setpoint*0.99+analogRead(A4)*.01;

	//Run through the PID calculations, and get the desired output values
	double out=pid.getOutput(sensor,setpoint);

	// "Do something" with the output. 
	// A real world system would do something more interesting, such as turn a motor or servo.
	// The sensor would then report back the "effect" of our output. 
	// For this example, we don't have a physical system, so we'll instead just add them together.
	sensor=sensor+out;

	//Send some information about what's happening to our serial print.
	Serial.print("Setpoint: ");
	Serial.print((int)setpoint);
	Serial.print("\tInput? ");
	Serial.print((int)sensor);
	Serial.print("\tOutput: ");
	Serial.print((int)out);
	Serial.print("\tError: ");
	Serial.print(setpoint-sensor);
	Serial.println("");

	// PIDs like a nice, consistent loop time. In general, the Blink Without Delay sketch 
	// is a much better way to keep time. Another good approached is ElapsedMillis or Metro.
	delay(50);
}