#include <Servo.h>          //Servo motor library. This is standard library
#include <NewPing.h>        //Ultrasonic sensor function library. You must install this library
#include <SoftwareSerial.h>
SoftwareSerial BTSerial (0,1);
char val;

//sensor pins
#define trig_pin A1 //analog input 1
#define echo_pin A2 //analog input 2

#define maximum_distance 200
boolean goesForward = false;
int distance = 100;

NewPing sonar(trig_pin, echo_pin, maximum_distance); //sensor function
 //our servo name
Servo lServo; //Higher value = forward
Servo rServo; //Lower value = forward


void setup(){
  Serial.begin(9600);
  //our servo pin
  lServo.attach(9);
  rServo.attach(8);
  delay(2000);
  distance = readPing();
  delay(100);
  distance = readPing();
  delay(100);
  distance = readPing();
  delay(100);
  distance = readPing();
  delay(100);
 
}

void loop(){
  while(Serial.available() > 0){
    val = Serial.read();
    Serial.println(val);
    }

   if( val == 'F'){
      forwardBtn();
    }
    if( val == 'R'){
     turnRight(); 
    }
    if( val == 'L'){
      turnLeft();
    }
    if( val == 'B'){
      backwardBtn();
    }
    if( val == 'S'){
      moveStop();
    }
    
  int distanceRight = 0;
  int distanceLeft = 0;
  delay(50);

  if (distance <= 20){
    moveStop();
    delay(300);
    moveBackward();
    delay(400);
    moveStop();
    delay(300);

    if (distance >= distanceLeft){
      turnRight();
      moveStop();
    }
    else{
      turnLeft();
      moveStop();
    }
  }
  else{
    moveForward(); 
  }
    distance = readPing();
}


int readPing(){
  delay(70);
  int cm = sonar.ping_cm();
  if (cm==0){
    cm=250;
  }
  return cm;
}

void moveStop(){
  lServo.write(90);
  rServo.write(90);
}

void forwardBtn(){
 lServo.write(110);
 rServo.write(70); 
 
 distance = readPing();
  if (distance <= 20){
    moveStop();
    moveBackward();
    delay(400);
    moveStop();
    delay(300);
    }
}

void moveForward(){

  if(!goesForward){
    goesForward=true;
    lServo.write(110);
    rServo.write(70); 
  }
}

void backwardBtn(){
  lServo.write(70);
  rServo.write(110);
}

void moveBackward(){

  goesForward=false;

  lServo.write(70);
  rServo.write(110);
  
}

void turnRight(){
  lServo.write(110);
  rServo.write(90);  
}

void turnLeft(){
  lServo.write(90);
  rServo.write(70);
}
