
#include <SoftwareSerial.h>
SoftwareSerial esp8266(9,10);
#include<DHT.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
#include <LiquidCrystal_PCF8574.h>
#include <stdlib.h>
LiquidCrystal_PCF8574 lcd(0x27);

#define SSID "ABC"
#define PASS "12345678"
#define IP "184.106.153.149"// thingspeak.com ip
String msg = "GET /update?key=XRTKLTPTXSAEGLNG";

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
int switchState2 = 0;
int switchState6 = 0;
int switchState7 = 0;

void setup()
{
Serial.begin(9600);
pinMode(A1,INPUT);
dht.begin();
lcd.setBacklight(255);
lcd.begin(16,2);
esp8266.begin(115200);
if(esp8266.find("OK")){
    connectWiFi();
  }
}

void loop()
{
light=analogRead(A1);
delay(500);
updateLight();
updateTemp();
updateHumidity();
updateTempBarometer();
updatePressure();
updateAltitude();
updateUV();
updateDust();
}

void updateLight(){
  String cmd = "AT+CIPSTART=\"TCP\",\"";
  cmd += IP;
  cmd += "\",80";
  Serial.println(cmd);
  esp8266.println(cmd);
  delay(2000);
  if(esp8266.find("Error")){
    return;
  }
  cmd = msg ;
  cmd += "&field1=";   
  cmd += light;
  cmd += "\r\n";
  Serial.print("AT+CIPSEND=");
  esp8266.print("AT+CIPSEND=");
  Serial.println(cmd.length());
  esp8266.println(cmd.length());
  Serial.print(cmd);
  esp8266.print(cmd);
}

void updateTemp(){
    hum = dht.readHumidity();
    temp= dht.readTemperature();
  String tcmd = "AT+CIPSTART=\"TCP\",\"";
  tcmd += IP;
  tcmd += "\",80";
  Serial.println(tcmd);
  esp8266.println(tcmd);
  delay(2000);
  if(esp8266.find("Error")){
    return;
  }
  tcmd = msg ;
  tcmd += "&field2=";   
  tcmd += temp;
  tcmd += "\r\n";
  Serial.print("AT+CIPSEND=");
  esp8266.print("AT+CIPSEND=");
  Serial.println(tcmd.length());
  esp8266.println(tcmd.length());
  Serial.print(tcmd);
  esp8266.print(tcmd);
}

void updateHumidity(){
  String hcmd = "AT+CIPSTART=\"TCP\",\"";
  hcmd += IP;
  hcmd += "\",80";
  Serial.println(hcmd);
  esp8266.println(hcmd);
  delay(2000);
  if(esp8266.find("Error")){
    return;
  }
  hcmd = msg ;
  hcmd += "&field3=";   
  hcmd += temp;
  hcmd += "\r\n";
  Serial.print("AT+CIPSEND=");
  esp8266.print("AT+CIPSEND=");
  Serial.println(hcmd.length());
  esp8266.println(hcmd.length());
  Serial.print(hcmd);
  esp8266.print(hcmd);
}

void updateTempBarometer(){
  String Barometercmd = "AT+CIPSTART=\"TCP\",\"";
  Barometercmd += IP;
  Barometercmd += "\",80";
  Serial.println(Barometercmd);
  esp8266.println(Barometercmd);
  delay(2000);
  if(esp8266.find("Error")){
    return;
  }
  Barometercmd = msg ;
  Barometercmd += "&field4=";   
  Barometercmd += bmp.readTemperature();
  Barometercmd += "\r\n";
  Serial.print("AT+CIPSEND=");
  esp8266.print("AT+CIPSEND=");
  Serial.println(Barometercmd.length());
  esp8266.println(Barometercmd.length());
  Serial.print(Barometercmd);
  esp8266.print(Barometercmd);
}

void updatePressure(){
  String Barometercmd = "AT+CIPSTART=\"TCP\",\"";
  Barometercmd += IP;
  Barometercmd += "\",80";
  Serial.println(Barometercmd);
  esp8266.println(Barometercmd);
  delay(2000);
  if(esp8266.find("Error")){
    return;
  }
  Barometercmd = msg ;
  Barometercmd += "&field5=";   
  Barometercmd += bmp.readPressure();
  Barometercmd += "\r\n";
  Serial.print("AT+CIPSEND=");
  esp8266.print("AT+CIPSEND=");
  Serial.println(Barometercmd.length());
  esp8266.println(Barometercmd.length());
  Serial.print(Barometercmd);
  esp8266.print(Barometercmd);
}

void updateAltitude(){
  String Barometercmd = "AT+CIPSTART=\"TCP\",\"";
  Barometercmd += IP;
  Barometercmd += "\",80";
  Serial.println(Barometercmd);
  esp8266.println(Barometercmd);
  delay(2000);
  if(esp8266.find("Error")){
    return;
  }
  Barometercmd = msg ;
  Barometercmd += "&field6=";   
  Barometercmd += bmp.readAltitude(1013.25);
  Barometercmd += "\r\n";
  Serial.print("AT+CIPSEND=");
  esp8266.print("AT+CIPSEND=");
  Serial.println(Barometercmd.length());
  esp8266.println(Barometercmd.length());
  Serial.print(Barometercmd);
  esp8266.print(Barometercmd);
}

void updateDust(){
  duration = pulseIn(pin, LOW);
  lowpulseoccupancy = lowpulseoccupancy+duration;
  if ((millis()-starttime) >= sampletime_ms) //if the sampel time = = 30s
  {
    ratio = lowpulseoccupancy/(sampletime_ms*10.0);  
    concentration = 1.1*pow(ratio,3)-3.8*pow(ratio,2)+520*ratio+0.62;
  }
  String cmd = "AT+CIPSTART=\"TCP\",\"";
  cmd += IP;
  cmd += "\",80";
  Serial.println(cmd);
  esp8266.println(cmd);
  delay(2000);
  if(esp8266.find("Error")){
    return;
  }
  cmd = msg ;
  cmd += "&field7=";   
  cmd += concentration;
  cmd += "\r\n";
  Serial.print("AT+CIPSEND=");
  esp8266.print("AT+CIPSEND=");
  Serial.println(cmd.length());
  esp8266.println(cmd.length());
  Serial.print(cmd);
  esp8266.print(cmd);
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

 String cmd = "AT+CIPSTART=\"TCP\",\"";
  cmd += IP;
  cmd += "\",80";
  Serial.println(cmd);
  esp8266.println(cmd);
  delay(2000);
  if(esp8266.find("Error")){
    return;
  }
  cmd = msg ;
  cmd += "&field8=";   
  cmd += Vsig;
  cmd += "\r\n";
  Serial.print("AT+CIPSEND=");
  esp8266.print("AT+CIPSEND=");
  Serial.println(cmd.length());
  esp8266.println(cmd.length());
  Serial.print(cmd);
  esp8266.print(cmd);
}

boolean connectWiFi(){
  Serial.println("AT+CWMODE=1");
  esp8266.println("AT+CWMODE=1");
  delay(2000);
  String cmd="AT+CWJAP=\"";
  cmd+=SSID;
  cmd+="\",\"";
  cmd+=PASS;
  cmd+="\"";
  Serial.println(cmd);
  esp8266.println(cmd);
  delay(5000);
  if(esp8266.find("OK")){
    Serial.println("OK");
    return true;    
  }else{
    return false;
  }
}
