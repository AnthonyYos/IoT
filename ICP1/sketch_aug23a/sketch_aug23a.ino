int switchState = 0;
void setup() {
  pinMode(1, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(2, INPUT);
}

void loop(){
  switchState = digitalRead(2);
  if(switchState == LOW){
  // the button is not pressed

  digitalWrite(3, HIGH); //green LED
  digitalWrite(4, LOW); //yello LED
  digitalWrite(5, LOW); //red LED
  }

  else{
  digitalWrite(3, LOW);
  digitalWrite(4, LOW);
  digitalWrite(5, HIGH);

  delay(500); // wait for a quarter seconds
  digitalWrite(3, HIGH);
  digitalWrite(5, LOW);
   digitalWrite(4, LOW);
  delay(500);
   digitalWrite(4, HIGH);
  digitalWrite(3, LOW);
  digitalWrite(5, LOW);
  delay(500);
  
  }

}
