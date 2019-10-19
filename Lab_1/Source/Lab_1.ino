#include <SoftwareSerial.h>
SoftwareSerial BTSerial (0,1);
#include<DHT.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
#include <LiquidCrystal_PCF8574.h>
#include <stdlib.h>
LiquidCrystal_PCF8574 lcd(0x27);


//Constants
#define DHTPIN 7     // what pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302)
DHT dht(DHTPIN, DHTTYPE); //// Initialize DHT sensor for normal 16mhz Arduino
float hum;  //Stores humidity value
float temp; //Stores temperature value
Adafruit_BMP280 bmp;


int pin = 8;
unsigned long duration;
unsigned long starttime;
unsigned long sampletime_ms = 2000; 
unsigned long lowpulseoccupancy = 0;
float ratio = 0;
float concentration = 0;

float Vsig;

int light;

//Variables from heartbeat
int error;
int pulsePin = 2;                 // Pulse Sensor purple wire connected to analog pin 2
int blinkPin = 3;                // pin to blink led at each beat
int fadePin = 5;
int fadeRate = 0;

// Volatile Variables, used in the interrupt service routine!
volatile int BPM;                   // int that holds raw Analog in 0. updated every 2mS
volatile int Signal;                // holds the incoming raw data
volatile int IBI = 600;             // int that holds the time interval between beats! Must be seeded! 
volatile boolean Pulse = false;     // "True" when heartbeat is detected. "False" when not a "live beat". 
volatile boolean QS = false;        // becomes true when Arduino finds a beat.

// Regards Serial OutPut  -- Set This Up to your needs
static boolean serialVisual = true;   // Set to 'false' by Default.  Re-set to 'true' to see Arduino Serial Monitor ASCII Visual Pulse 
volatile int rate[10];                    // array to hold last ten IBI values
volatile unsigned long sampleCounter = 0;          // used to determine pulse timing
volatile unsigned long lastBeatTime = 0;           // used to find IBI
volatile int P =512;                      // used to find peak in pulse wave, seeded
volatile int T = 512;                     // used to find trough in pulse wave, seeded
volatile int thresh = 525;                // used to find instant moment of heart beat, seeded
volatile int amp = 100;                   // used to hold amplitude of pulse waveform, seeded
volatile boolean firstBeat = true;        // used to seed rate array so we startup with reasonable BPM
volatile boolean secondBeat = false;      // used to seed rate array so we startup with reasonable BPM

char val;

void setup()
{
Serial.begin(9600);
pinMode(A1,INPUT);
dht.begin();
lcd.setBacklight(255);
lcd.begin(16,2);
}

void loop()
{
while (Serial.available() > 0)
  {
  val = Serial.read();
  Serial.println(val);
  }

  delay(500);
  if( val == '1'){
    updateLight();
    }
  else if(val == '2'){
    useTempSensor();
    }
    else if(val == '3'){
      useBarometer(); 
    }
    else if(val == '4'){
      updateUV();
    }
    else if(val == '5'){
      updateDust();
    }
    else if(val == '6'){
      updatebeat();
    }
     delay(2500);
     lcd.clear();
}

void updateLight(){

  lcd.setCursor(0,0);
  light=analogRead(A1);
  lcd.print("Light= ");
  lcd.print(String(light));
}

void updateTemp(){
  temp= dht.readTemperature();
  String t = String(temp);
  lcd.print("Temp: ");
  lcd.print(t);
}

void updateHumidity(){
  hum = dht.readHumidity();
  String h = String(hum);
  lcd.print("Humidity: ");
  lcd.print(h);
  
}

void useTempSensor(){
  lcd.setCursor(0,0);
  updateTemp();
  lcd.setCursor(0,1);
  updateHumidity();
}

void useBarometer(){
  lcd.setCursor(0,0);
  updateTempBarometer();
  lcd.setCursor(9,0);
  updatePressure();
  lcd.setCursor(0,1);
  updateAltitude();
}


void updateTempBarometer(){
  lcd.print("T=");
  lcd.print(bmp.readTemperature());   
}

void updatePressure(){
  lcd.print(" P=");
  lcd.print(bmp.readPressure());
}

void updateAltitude(){
  lcd.print("A=");
  lcd.print(bmp.readAltitude(1013.25));
}

void updateDust(){
  duration = pulseIn(pin, LOW);
  lowpulseoccupancy = lowpulseoccupancy+duration;
  if ((millis()-starttime) >= sampletime_ms) //if the sampel time = = 30s
  {
    ratio = lowpulseoccupancy/(sampletime_ms*10.0);  
    concentration = 1.1*pow(ratio,3)-3.8*pow(ratio,2)+520*ratio+0.62;
  }

  lcd.setCursor(0,0);
  lcd.print("Dust levels: ");
  lcd.print(concentration);
}

void updateUV(){
  int sensorValue;
  long  sum=0;
  for(int i=0;i<1024;i++)
   {  
      sensorValue=analogRead(A0);
      sum=sensorValue+sum;
      delay(2);
   }   
  sum = sum >> 10;
  Vsig = sum*4980.0/1023.0;
  lcd.setCursor(0,0);
  lcd.print("UV: ");
  lcd.print(Vsig);
  lcd.setCursor(0,1);
  if (Vsig < 50) {lcd.print("UV Index: 0");}
  if (Vsig > 50 && Vsig < 227) {lcd.print("UV Index: 1");}
  if (Vsig > 227 && Vsig < 318) {lcd.print("UV Index: 2");}
  if (Vsig > 318 && Vsig < 408) {lcd.print("UV Index: 3");}
  if (Vsig > 408 && Vsig < 503) {lcd.print("UV Index: 4");}
  if (Vsig > 503 && Vsig < 606) {lcd.print("UV Index: 5");}
  if (Vsig > 606 && Vsig < 696) {lcd.print("UV Index: 6");}
  if (Vsig > 696 && Vsig < 795) {lcd.print("UV Index: 7");}
  if (Vsig > 795 && Vsig < 881) {lcd.print("UV Index: 8");}
  if (Vsig > 881 && Vsig < 976) {lcd.print("UV Index: 9");}
  if (Vsig > 976 && Vsig < 1079) {lcd.print("UV Index: 10");}
  if (Vsig > 1079 && Vsig < 1170) {lcd.print("UV Index: 11");}
  if (Vsig > 1170) {lcd.print("UV Index: 12");}
}

void updatebeat(){

  lcd.setCursor(0,0);
  lcd.print("BPM: ");
  lcd.print(BPM);

}

void interruptSetup(){     
  TCCR2A = 0x02;     // DISABLE PWM ON DIGITAL PINS 3 AND 11, AND GO INTO CTC MODE
  TCCR2B = 0x06;     // DON'T FORCE COMPARE, 256 PRESCALER 
  OCR2A = 0X7C;      // SET THE TOP OF THE COUNT TO 124 FOR 500Hz SAMPLE RATE
  TIMSK2 = 0x02;     // ENABLE INTERRUPT ON MATCH BETWEEN TIMER2 AND OCR2A
  sei();             // MAKE SURE GLOBAL INTERRUPTS ARE ENABLED      
} 

ISR(TIMER2_COMPA_vect){                       // triggered when Timer2 counts to 124
  cli();                                      // disable interrupts while we do this
  Signal = analogRead(pulsePin);              // read the Pulse Sensor 
  sampleCounter += 2;                         // keep track of the time in mS
  int N = sampleCounter - lastBeatTime;       // monitor the time since the last beat to avoid noise

    //  find the peak and trough of the pulse wave
  if(Signal < thresh && N > (IBI/5)*3){      // avoid dichrotic noise by waiting 3/5 of last IBI
    if (Signal < T){                         // T is the trough
      T = Signal;                            // keep track of lowest point in pulse wave 
    }
  }

  if(Signal > thresh && Signal > P){        // thresh condition helps avoid noise
    P = Signal;                             // P is the peak
  }                                         // keep track of highest point in pulse wave

  //  NOW IT'S TIME TO LOOK FOR THE HEART BEAT
  // signal surges up in value every time there is a pulse
  if (N > 250){                                   // avoid high frequency noise
    if ( (Signal > thresh) && (Pulse == false) && (N > (IBI/5)*3) ){        
      Pulse = true;                               // set the Pulse flag when there is a pulse
      digitalWrite(blinkPin,HIGH);                // turn on pin 13 LED
      IBI = sampleCounter - lastBeatTime;         // time between beats in mS
      lastBeatTime = sampleCounter;               // keep track of time for next pulse
      
      if(secondBeat){                        // if this is the second beat
        secondBeat = false;                  // clear secondBeat flag
       
        for(int i=0; i<=9; i++){             // seed the running total to get a realistic BPM at startup
          rate[i] = IBI;                      
        }
      }

      if(firstBeat){                         // if it's the first time beat is found
        firstBeat = false;                   // clear firstBeat flag
        secondBeat = true;                   // set the second beat flag
        sei();                               // enable interrupts again
        return;                              // IBI value is unreliable so discard it
      }   
      word runningTotal = 0;                  // clear the runningTotal variable    

      for(int i=0; i<=8; i++){                // shift data in the rate array
        rate[i] = rate[i+1];                  // and drop the oldest IBI value 
        runningTotal += rate[i];              // add up the 9 oldest IBI values
      }

      rate[9] = IBI;                          // add the latest IBI to the rate array
      runningTotal += rate[9];                // add the latest IBI to runningTotal
      runningTotal /= 10;                     // average the last 10 IBI values 
      BPM = 60000/runningTotal;               // how many beats can fit into a minute? that's BPM!
     
      QS = true;                              // set Quantified Self flag 
      // QS FLAG IS NOT CLEARED INSIDE THIS ISR
      
      if(BPM>170){ //Red
          digitalWrite(4, HIGH);
          digitalWrite(5, LOW);
         
      }
      else if(BPM>99){
        digitalWrite(4, LOW);
        digitalWrite(5, LOW);
      }
      else{ //green
        digitalWrite(4, LOW);
        digitalWrite(5, HIGH);
        
      }
    }                       
  }

  if (Signal < thresh && Pulse == true){   // when the values are going down, the beat is over
    digitalWrite(blinkPin,LOW);            // turn off pin 3 LED
    Pulse = false;                         // reset the Pulse flag so we can do it again
    amp = P - T;                           // get amplitude of the pulse wave
    thresh = amp/2 + T;                    // set thresh at 50% of the amplitude
    P = thresh;                            // reset these for next time
    T = thresh;
  }

  if (N > 2500){                           // if 2.5 seconds go by without a beat
    thresh = 512;                          // set thresh default
    P = 512;                               // set P default
    T = 512;                               // set T default
    lastBeatTime = sampleCounter;          // bring the lastBeatTime up to date        
    firstBeat = true;                      // set these to avoid noise
    secondBeat = false;                     // when we get the heartbeat back
  }

  sei();     
  // enable interrupts when youre done!
}// end isr
