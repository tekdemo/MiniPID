#include <MiniPID.h>

#define SETPOINTPIN 0
#define FEEDBACKPIN A1
#define GROUNDPIN A0
#define OUTPUTPIN 10

MiniPID pid(.1, 0.01, 0.00);

void setup() {
  Serial.begin(9600);
  Serial.println("Configuring");

  // put your setup code here, to run once:
  pid.setOutputLimits(0,255);
  //pid.setOutputFilter(0.01);
  pid.setOutputRampRate(20);
  
  
  pinMode(OUTPUTPIN,OUTPUT);
  digitalWrite(OUTPUTPIN,LOW);
  
  pinMode(GROUNDPIN,OUTPUT);
  digitalWrite(GROUNDPIN,LOW);

  delay(1000);
  Serial.println("All set! Starting loop");
  //Serial.print("Initial Voltage: ");
  //Serial.println(analogRead(FEEDBACKPIN);
}

double fakesensor=0;
void loop() {

  double target=768;
  fakesensor-=5;
  double out=pid.getOutput(fakesensor,target);
  fakesensor+=out;
  Serial.println((int)fakesensor);
  delay(200);
/*
  double target=768;
  double feedback=analogRead(FEEDBACKPIN);
  
  int out=pid.getOutput(feedback,target);
  
  //analogWrite(OUTPUTPIN,out);
  
 
  Serial.print("F: ");
  Serial.println((int)feedback);
  Serial.print("\t\tO: ");
  Serial.println((int)out);
  delay(50);
  //*/
}