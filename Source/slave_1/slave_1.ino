//COM9

//I2C
#include <Wire.h>
#define DEBUG 1
char dataReceived[3];
int newData = 0;
char workDone = '1';

// Stepper Motor X
const int stepPinX = 2; //X.STEP
const int dirPinX = 5; // X.DIR
const int endStopX = 9;
int endStopXStatus;
#define DELAYX  600
char moveX = '-';
char column = '-';
//steps
#define XA 0
#define XB 19500
#define XC 39750
#define XS 5500
const int X1 = XA - XS;
const int X2 = XB - XS;
const unsigned int X3 = XC - XS;

// Stepper Motor G
const int stepPinG = 3; //G.STEP
const int dirPinG = 6; // G.DIR
const int endStopG = 10;
int endStopGStatus;
#define DELAYG  3000
char moveG = '-';
//steps
#define GG 42
#define GH 20

// Stepper Motor Y
const int stepPinY = 4; //Y.STEP
const int dirPinY = 7; // Y.DIR
const int endStopY = 11;
int endStopYStatus;
#define DELAYY  900
char moveY = '-';


 
 void setup() {
  delay(3000);
  Serial.begin(9600);
  
  pinMode(stepPinX,OUTPUT); 
  pinMode(dirPinX,OUTPUT);
  pinMode(endStopX , INPUT_PULLUP);
  endStopXStatus = digitalRead(endStopX);
  endStopXStatus = calibrateStepper(dirPinX, stepPinX, endStopX, DELAYX, endStopXStatus, 1);
  
  pinMode(stepPinG,OUTPUT); 
  pinMode(dirPinG,OUTPUT);
  pinMode(endStopG , INPUT_PULLUP);
  endStopGStatus = digitalRead(endStopG);
  endStopGStatus = calibrateStepper(dirPinG, stepPinG, endStopG, DELAYG, endStopGStatus, 0);
  pinMode(stepPinY,OUTPUT); 
  pinMode(dirPinY,OUTPUT);
  pinMode(endStopY , INPUT_PULLUP);
  endStopYStatus = digitalRead(endStopY);
  endStopYStatus = calibrateStepper(dirPinY, stepPinY, endStopY, DELAYY, endStopYStatus, 1);
 
  //calibrateSteppers();
  //I2C
  Wire.begin(10);
  Wire.onReceive(receiveEvent);
 }
 void loop() {
  if (moveX != '-'){
    moveStepper('x', moveX);
  }
  if (moveG != '-'){
    moveStepper('g', moveG);
  }
  if (newData == 1)
  {
    newDataProcess();
  }
 }
 int calibrateStepper(int dirPin, int stepPin, int endStop, int del, int endStopStatus, int inverse){
  //A function that ensures that the motor are at the starting possition
  //dirPin is the pin that's controlling the direction
  //stepPin is the pin that's controlling the steps
  //endStop is the pin the endstop is connected to
  //del is the delay between steps in milliseconds
  //endStopStatus is the last known status of the endstop
//  //inverse Set this to 1 if you need to inverse the motor rotation. 0 otherwise.
//
//  //if the endstop is activated
//  //Move until it's not pushed anymore
//  
//  if (DEBUG){
//    Serial.println(endStopStatus);
//  }
//  if (inverse){
//    digitalWrite(dirPin,HIGH);
//  }
//  else{
//    digitalWrite(dirPin,LOW);
//  }
//  while (endStopStatus == 0){
//    digitalWrite(stepPin,HIGH); 
//    delayMicroseconds(del); 
//    digitalWrite(stepPin,LOW); 
//    delayMicroseconds(del); 
//    endStopStatus = digitalRead(endStop);
//  }
//  //then move 5 steps more to be sure it's correct
//  for(int x = 0; x < 5; x++){
//    digitalWrite(stepPin,HIGH); 
//    delayMicroseconds(del); 
//    digitalWrite(stepPin,LOW); 
//    delayMicroseconds(del); 
//  }
//  
//  endStopStatus = digitalRead(endStop);
//  if (DEBUG){
//    Serial.println(endStopStatus);
//  }
//  
  if (inverse){
    digitalWrite(dirPin,LOW);
  }
  else{
    digitalWrite(dirPin,HIGH);
  }
  //move until the endstop is pushed
  while (endStopStatus == 1)
  {
    digitalWrite(stepPin,HIGH); 
    delayMicroseconds(del); 
    digitalWrite(stepPin,LOW); 
    delayMicroseconds(del); 
    endStopStatus = digitalRead(endStop);
  }
  if (DEBUG){
    Serial.println(endStopStatus);
  }
  return endStopStatus;
}

void calibrateSteppers()
 {
  Serial.println(endStopXStatus);
  while (endStopXStatus == 0)
  {
    digitalWrite(dirPinX,HIGH); // Enables the motor to move in a particular direction
 // Makes 200 pulses for making one full cycle rotation
    for(int x = 0; x < 2000; x++)
    {
      digitalWrite(stepPinX,HIGH); 
      delayMicroseconds(DELAYX); 
      digitalWrite(stepPinX,LOW); 
      delayMicroseconds(DELAYX); 
    }
    endStopXStatus = digitalRead(endStopX);
  }
  
  Serial.println(endStopXStatus);
  while (endStopXStatus == 1)
  {
    digitalWrite(dirPinX,LOW);
    digitalWrite(stepPinX,HIGH); 
    delayMicroseconds(DELAYX); 
    digitalWrite(stepPinX,LOW); 
    delayMicroseconds(DELAYX); 
    endStopXStatus = digitalRead(endStopX);
  }
  Serial.println(endStopXStatus);

  endStopGStatus = digitalRead(endStopG);
  if (endStopGStatus == 0)
  {
    digitalWrite(dirPinG,LOW);
    for(int x = 0; x < 45; x++)
    {
      digitalWrite(stepPinG,HIGH); 
      delayMicroseconds(DELAYG); 
      digitalWrite(stepPinG,LOW); 
      delayMicroseconds(DELAYG);
    }
  }
  endStopGStatus = digitalRead(endStopG);
  Serial.println(endStopGStatus);
  while (endStopGStatus == 1)
  {
    digitalWrite(dirPinG,HIGH);
    digitalWrite(stepPinG,HIGH); 
    delayMicroseconds(DELAYG); 
    digitalWrite(stepPinG,LOW); 
    delayMicroseconds(DELAYG); 
    endStopGStatus = digitalRead(endStopG);
  }
  
  endStopYStatus = digitalRead(endStopY);
  if (endStopYStatus == 0)
  {
    digitalWrite(dirPinY,HIGH);
    for(int x = 0; x < 4000; x++)
    {
      digitalWrite(stepPinY,HIGH); 
      delayMicroseconds(DELAYY); 
      digitalWrite(stepPinY,LOW); 
      delayMicroseconds(DELAYY);
    }
  }
  endStopYStatus = digitalRead(endStopY);
  Serial.println(endStopYStatus);
  while (endStopYStatus == 1)
  {
    digitalWrite(dirPinY,LOW);
    digitalWrite(stepPinY,HIGH); 
    delayMicroseconds(DELAYY); 
    digitalWrite(stepPinY,LOW); 
    delayMicroseconds(DELAYY); 
    endStopYStatus = digitalRead(endStopY);
  }
}
void moveStepper(char axle, char value){
  long int steps;
  if (axle == 'x')
  {
    if (value == 'a')
    {
      steps = XA;
    }
    if (value == 'b')
    {
      steps = XB;
    }
    if (value == 'c')
    {
      steps = XC;
    }
    if (value == '1')
    {
      steps = X1;
    }
    if (value == '2')
    {
      steps = X2;
    }
    if (value == '3')
    {
      steps = X3;
    }
    if (value == 's')
    {
      if (column == 'a'){
        steps = XA - XS;
      }
      if (column == 'b'){
        steps = XB - XS;
      }
      if (column == 'c'){
        steps = XC - XS;
      }
    }
    moveX = '-';
    Serial.println(steps);
  }
  if (axle == 'g'){
    if (value == 'g'){
      steps = GG;
    }
    if (value == 'h'){
      steps = GH;
    }
    moveG = '-';
  }
  if (value != 'r')
  {
    if (axle == 'g'){
      digitalWrite(dirPinG,LOW);
      for(int x = 0; x < steps; x++)
      {
        digitalWrite(stepPinG,HIGH); 
        delayMicroseconds(DELAYG); 
        digitalWrite(stepPinG,LOW); 
        delayMicroseconds(DELAYG); 
      }
    }
    if (axle == 'x'){
      if (value == 's'){
        digitalWrite(dirPinX,LOW);
      }
      else{
        digitalWrite(dirPinX,HIGH);
      }
      for(int x = 0; x < steps; x++)
      {
        digitalWrite(stepPinX,HIGH); 
        delayMicroseconds(DELAYX); 
        digitalWrite(stepPinX,LOW); 
        delayMicroseconds(DELAYX); 
      }
    }
  }
  else
  {
    if (axle == 'x'){
      endStopXStatus = digitalRead(endStopX);
      while (endStopXStatus == 1)
      {
        digitalWrite(dirPinX,LOW); // Enables the motor to move in a particular direction
     // Makes 200 pulses for making one full cycle rotation
        for(int x = 0; x < 1; x++)
        {
          digitalWrite(stepPinX,HIGH); 
          delayMicroseconds(DELAYX); 
          digitalWrite(stepPinX,LOW); 
          delayMicroseconds(DELAYX); 
        }
        endStopXStatus = digitalRead(endStopX);
      }
    }
    if (axle == 'g'){
      endStopGStatus = digitalRead(endStopG);
      while (endStopGStatus == 1)
      {
        digitalWrite(dirPinG,HIGH); // Enables the motor to move in a particular direction
     // Makes 200 pulses for making one full cycle rotation
        for(int x = 0; x < 1; x++)
        {
          digitalWrite(stepPinG,HIGH); 
          delayMicroseconds(DELAYG); 
          digitalWrite(stepPinG,LOW); 
          delayMicroseconds(DELAYG); 
        }
        endStopGStatus = digitalRead(endStopG);
      }
    }
  }
  workDone = '1';
  
  Wire.end();
  Wire.begin();
  Wire.beginTransmission(9);
  Wire.write('1');
  Wire.endTransmission();
  Wire.end();
  Wire.begin(10);
  Wire.onReceive(receiveEvent);
}
void receiveEvent(){
  workDone = '0';
  while (0 < Wire.available())
  {
    dataReceived[0] = Wire.read();
    dataReceived[1] = Wire.read();
    dataReceived[2] = '\0';
  }
  newData = 1;
  Serial.write(dataReceived);
}
void newDataProcess(){
  if (dataReceived[0] == 'X')
  {
    if (dataReceived[1] == 'A')
    {
      moveX = 'a';
      column = 'a';
    }
    if (dataReceived[1] == 'B')
    {
      moveX = 'b';
      column = 'b';
    }
    if (dataReceived[1] == 'C')
    {
      moveX = 'c';
      column = 'c';
    }
    if (dataReceived[1] == 'S')
    {
      moveX = 's';
    }
    if (dataReceived[1] == 'R')
    {
      moveX = 'r';
    }
    if (dataReceived[1] == '1')
    {
      moveX = '1';
    }
    if (dataReceived[1] == '2')
    {
      moveX = '2';
    }
    if (dataReceived[1] == '3')
    {
      moveX = '3';
    }
  }
  if (dataReceived[0] == 'G'){
    if(dataReceived[1] == 'G'){
      moveG = 'g';
    }
    if(dataReceived[1] == 'R'){
      moveG = 'r';
    }
    if(dataReceived[1] == 'H'){
      moveG = 'h';
    }
  }
  newData = 0;
}
