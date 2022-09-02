#define BLYNK_TEMPLATE_ID "TMPLTWPf2Qr0"
#define BLYNK_DEVICE_NAME "Smart Bulb"
#define BLYNK_AUTH_TOKEN "3YV3kJp9f-q2K9fJpXgBCPbcaiRg2kYQ"

char ssid[] = "Dialog 4G 122";
char pass[] = "5B5Fe3FD";

#define MQ2_SENSOR    A0 //A0
#define GREEN_LED     4 //D2
#define RED_LED       0 //D3
#define BUZZER_PIN    2 //D4
const int ldrPin = 5 ; // D1 pin
const int pirPin = 16; // D0 pin


#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

int MQ2_SENSOR_Value = 0;
const int bulbPin = 15; // D8 pin





bool isconnected = false;
char auth[] = BLYNK_AUTH_TOKEN;


#define VPIN_BUTTON_1    V0 
#define VPIN_BUTTON_1    V2 

BlynkTimer timer;

void checkBlynkStatus() { // called every 2 seconds by SimpleTimer
  getSensorData();
  ldrSensorReading();
  isconnected = Blynk.connected();
  if (isconnected == true) {  
    sendSensorData();
  }
}

void getSensorData()
{
 
  MQ2_SENSOR_Value = map(analogRead(MQ2_SENSOR), 0, 1024, 0, 100);
  if (MQ2_SENSOR_Value < 70 && MQ2_SENSOR_Value > 50 ){
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(RED_LED, HIGH);


      digitalWrite(BUZZER_PIN, HIGH);
      delay(500);
      digitalWrite(BUZZER_PIN, LOW);
      delay(500);
  
  }else if(MQ2_SENSOR_Value > 70){
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(RED_LED, HIGH);
    digitalWrite(BUZZER_PIN, HIGH);
  }
  else{
    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(RED_LED, LOW);
    digitalWrite(BUZZER_PIN, LOW);
  }
}

void ldrSensorReading(){
  int ldrStatus = digitalRead(ldrPin);
  if (ldrStatus == 1){
    digitalWrite(bulbPin,HIGH);
    //Serial.println("Its Dark");

  }  
   else if(ldrStatus == 0){
    digitalWrite(bulbPin,LOW);
    //Serial.println("Its Bright");
    pirSensorReading();
  }
}

void pirSensorReading(){
 int pirStatus = digitalRead(pirPin);
  if (pirStatus == 0){
    digitalWrite(bulbPin,HIGH);
    //Serial.println("There is a motion");
  }  
   else if(pirStatus == 1){
    digitalWrite(bulbPin,LOW);
    //Serial.println("There isnt any motion");
   
  }
}


void sendSensorData()
{  
  Blynk.virtualWrite(VPIN_BUTTON_1, MQ2_SENSOR_Value);
  
}
BLYNK_WRITE(V0){
  //getSensorData();
  //ldrSensorReading();
  int ldrStatus = digitalRead(ldrPin);
  int pirStatus = digitalRead(pirPin);
  if(param.asInt()==1){
    //getSensorData();
    //sendSensorData();
    digitalWrite(bulbPin,HIGH);
    if(ldrStatus == 0){
       delay(60000);
       digitalWrite(bulbPin,LOW);
       
    }else if (ldrStatus == 1 && pirStatus == 0){
      
      delay(300000);
      if(ldrStatus == 1 && pirStatus == 0){
        digitalWrite(bulbPin,LOW);
        
      }
    }
    
  }
  else if(param.asInt()==0){
    int i;
   digitalWrite(bulbPin,LOW);
   Blynk.virtualWrite(VPIN_BUTTON_1, 0);
   for (i = 0; i < 5; i++) {
   digitalWrite(GREEN_LED, LOW);
   delay(300);
   digitalWrite(GREEN_LED, HIGH);
   delay(300);
   digitalWrite(GREEN_LED, LOW);
   delay(300);
  }
  exit(0);
   

  }
   
}



void setup()
{
  Serial.begin(9600);
 

  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(bulbPin,OUTPUT);
  pinMode(ldrPin, INPUT); 
  pinMode(pirPin, INPUT);
  pinMode(MQ2_SENSOR, INPUT);
  

  digitalWrite(GREEN_LED, LOW);
  digitalWrite(RED_LED, LOW);
  digitalWrite(bulbPin, LOW);


  WiFi.begin(ssid, pass);
  timer.setInterval(2000L, checkBlynkStatus); // check if Blynk server is connected every 2 seconds
  Blynk.config(auth);
  delay(1000);
}

void loop()
{
  Blynk.run();
  timer.run();

}
