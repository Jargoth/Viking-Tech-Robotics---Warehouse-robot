//COM5

//I2C
#include <Wire.h>

char dataReceived[3];
int newData = 0;
char workDone = '1';
#define DEBUG 1

// Stepper Motor Z
const int stepPinZ = A1;
const int dirPinZ = A0;
const int endStopZ = A2;
int endStopZStatus;
#define DELAYZ  2000
char moveZ = '-';
char shelf = '-';
//steps
const int ZN = 35;
const int ZA = 50 + ZN;
const int ZB = 50 + ZN;
const int ZC = 50 + ZN;
const int ZD = 425 + ZN;
const int ZL = 460;
const int ZS = 25;
const int Za = ZA + ZL - ZS;
const int Zb = ZB + ZL - ZS;
const int Zc = ZC + ZL - ZS;
const int Zd = ZD + ZL - ZS;

// Stepper Motor A
//steps
#define A1 50
#define A2 50
#define A3 50
#define A4 50
#define A5 50

// Stepper Motor B
//steps
#define B1 50
#define B2 50
#define B3 50
#define B4 50
#define B5 50

// Stepper Motor C
//steps
#define C1 50
#define C2 50
#define C3 50
#define C4 50
#define C5 50

// Stepper Motor D
const int stepPinD = 12;
const int dirPinD = 11;
const int endStopD = 13;
int endStopDStatus;
#define DELAYD  2000
char moveD = '-';
//steps
#define D1 1260
#define D2 1250
#define D3 1250
#define D4 1250
#define D5 1250
 
 void setup() {
  delay(3000);
  Serial.begin(9600);
  
  //Motor Z initiate
  if (DEBUG){
    Serial.println("Z");
  }
  pinMode(stepPinZ,OUTPUT); 
  pinMode(dirPinZ,OUTPUT);
  pinMode(endStopZ , INPUT_PULLUP);
  endStopZStatus = digitalRead(endStopZ);
  endStopZStatus = calibrateStepper(dirPinZ, stepPinZ, endStopZ, DELAYZ, endStopZStatus, 0);

  //Motor D initiate
  
  if (DEBUG){
    Serial.println("D");
  }
  pinMode(stepPinD,OUTPUT); 
  pinMode(dirPinD,OUTPUT);
  pinMode(endStopD , INPUT_PULLUP);
  endStopDStatus = digitalRead(endStopD);
  endStopDStatus = calibrateStepper(dirPinD, stepPinD, endStopD, DELAYD, endStopDStatus, 0);
  
  //I2C
  Wire.begin(11);
  Wire.onReceive(receiveEvent);
 }
 void loop() {
  if (moveZ != '-'){
    moveStepper('z', moveZ);
  }
  if (moveD != '-'){
    moveStepper('d', moveD);
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
void moveStepper(char axle, char value){
  long int steps;
  if (axle == 'z'){
    if (value == 'A')
    {
      steps = ZA;
    }
    if (value == 'B')
    {
      steps = ZB;
    }
    if (value == 'C')
    {
      steps = ZC;
    }
    if (value == 'D')
    {
      steps = ZD;
    }
    if (value == 'l')
    {
      steps = ZL;
    }
    if (value == 'm')
    {
      steps = ZL;
    }
    if (value == 'n')
    {
      steps = ZN;
    }
    if (value == 'o')
    {
      steps = ZN;
    }
    if (value == 'a')
    {
      steps = Za;
    }
    if (value == 'b')
    {
      steps = Zb;
    }
    if (value == 'c')
    {
      steps = Zc;
    }
    if (value == 'd')
    {
      steps = Zd;
    }
    if (value == 's')
    {
      if (shelf = 'A'){
        steps = Za - ZS;
      }
      if (shelf = 'B'){
        steps = Zb - ZS;
      }
      if (shelf = 'C'){
        steps = Zc - ZS;
      }
      if (shelf = 'D'){
        steps = Zd - ZS;
      }
    }
    moveZ = '-';
  }
  if (axle == 'd'){
    if (value == '1')
    {
      steps = D1;
    }
    if (value == '2')
    {
      steps = D2;
    }
    if (value == '3')
    {
      steps = D3;
    }
    if (value == '4')
    {
      steps = D4;
    }
    if (value == '5')
    {
      steps = D5;
    }
    moveD = '-';
  }
  Serial.println(steps);
  if (axle == 'z'){
    if (value != 'r' && value != 'R'){
      if (value == 'm' || value == 's' || value == 'n'){
        digitalWrite(dirPinZ,HIGH);
        for(int x = 0; x < steps; x++){
          digitalWrite(stepPinZ,HIGH); 
          delayMicroseconds(DELAYZ); 
          digitalWrite(stepPinZ,LOW); 
          delayMicroseconds(DELAYZ); 
        }
      }
      else{
        digitalWrite(dirPinZ,LOW);
        for(int x = 0; x < steps; x++){
          digitalWrite(stepPinZ,HIGH); 
          delayMicroseconds(DELAYZ); 
          digitalWrite(stepPinZ,LOW); 
          delayMicroseconds(DELAYZ); 
        }
      }
    }
    else{
      endStopZStatus = digitalRead(endStopZ);
      digitalWrite(dirPinZ,HIGH);
      while (endStopZStatus == 1)
      {
        digitalWrite(stepPinZ,HIGH); 
        delayMicroseconds(DELAYZ); 
        digitalWrite(stepPinZ,LOW); 
        delayMicroseconds(DELAYZ);
        endStopZStatus = digitalRead(endStopZ);
      }
    }
  }
  if (axle == 'd'){
    if (value != 'r' && value != 'R'){
      digitalWrite(dirPinD,LOW);
      for(int x = 0; x < steps; x++){
        digitalWrite(stepPinD,HIGH); 
        delayMicroseconds(DELAYD); 
        digitalWrite(stepPinD,LOW); 
        delayMicroseconds(DELAYD); 
      }
    }
    else{
      endStopDStatus = digitalRead(endStopD);
      digitalWrite(dirPinD,HIGH);
      for(int x = 0; x < D1; x++){
        digitalWrite(stepPinD,HIGH); 
        delayMicroseconds(DELAYD); 
        digitalWrite(stepPinD,LOW); 
        delayMicroseconds(DELAYD); 
      }
      while (endStopDStatus == 1)
      {
        digitalWrite(stepPinD,HIGH); 
        delayMicroseconds(DELAYD); 
        digitalWrite(stepPinD,LOW); 
        delayMicroseconds(DELAYD);
        endStopDStatus = digitalRead(endStopD);
      }
    }
  }
  Wire.end();
  Wire.begin();
  Wire.beginTransmission(9);
  Wire.write('1');
  Wire.endTransmission();
  Wire.end();
  Wire.begin(11);
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
  if (dataReceived[0] == 'Z')
  {
    if (dataReceived[1] == '1')
    {
      moveZ = 'A';
      shelf = 'A';
    }
    if (dataReceived[1] == '2')
    {
      moveZ = 'B';
      shelf = 'B';
    }
    if (dataReceived[1] == '3')
    {
      moveZ = 'C';
      shelf = 'C';
    }
    if (dataReceived[1] == '4')
    {
      moveZ = 'D';
      shelf = 'D';
    }
    if (dataReceived[1] == 'L')
    {
      moveZ = 'l';
    }
    if (dataReceived[1] == 'M')
    {
      moveZ = 'm';
    }
    if (dataReceived[1] == 'N')
    {
      moveZ = 'n';
    }
    if (dataReceived[1] == 'O')
    {
      moveZ = 'o';
    }
    if (dataReceived[1] == 'R')
    {
      moveZ = 'r';
    }
    if (dataReceived[1] == 'a')
    {
      moveZ = 'a';
    }
    if (dataReceived[1] == 'b')
    {
      moveZ = 'b';
    }
    if (dataReceived[1] == 'c')
    {
      moveZ = 'c';
    }
    if (dataReceived[1] == 'd')
    {
      moveZ = 'd';
    }
    if (dataReceived[1] == 'S')
    {
      moveZ = 's';
    }
  }
  if (dataReceived[0] == 'D')
  {
    moveD = dataReceived[1];
  }
  newData = 0;
}
