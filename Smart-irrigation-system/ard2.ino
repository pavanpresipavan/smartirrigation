#include <ESP8266WiFi.h>                                                // esp8266 library 
#include "FirebaseESP8266.h"
#define WIFI_SSID "host-name"
#define WIFI_PASSWORD "host-password"
#define FIREBASE_HOST "firebase-host" 
#define FIREBASE_AUTH "auth-key"

#define FIREBASE_FCM_SERVER_KEY "server-key"
#define FIREBASE_FCM_DEVICE_TOKEN_1 "device-token"

const int trigPin = 2;  //d4
const int echoPin = 0; //d3
const int sensor_pin = A0;
long duration;
int distance;

FirebaseData firebaseData1;

void setup() {
 
  pinMode(trigPin, OUTPUT);
pinMode(echoPin, INPUT);
Serial.begin(9600);
   
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  firebaseData1.fcm.begin(FIREBASE_FCM_SERVER_KEY);
  firebaseData1.fcm.addDeviceToken(FIREBASE_FCM_DEVICE_TOKEN_1);
  firebaseData1.fcm.setPriority("high");
  firebaseData1.fcm.setTimeToLive(1000);
 
}


void loop() {
 
digitalWrite(trigPin, LOW);
delayMicroseconds(2);
digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);
duration = pulseIn(echoPin, HIGH);
Serial.println(duration);
distance= duration*0.034/2;
if(distance>200)
{
  distance=200;
}
int mapp=map(distance,4,200,100,0);
Serial.println(mapp);
Serial.print("Distance: ");
Serial.println(distance);
Firebase.setInt(firebaseData1,"/wlevel",mapp);
delay(1000);
float moisture_percentage;
 moisture_percentage= analogRead(sensor_pin);
   Serial.print(moisture_percentage);
moisture_percentage=( 100.00 - ( (moisture_percentage/1023.00) * 100.00 ) );
   Serial.print("Mositure : ");
   Serial.print(moisture_percentage);
   Serial.println("%");
Firebase.setInt(firebaseData1,"/Mois",moisture_percentage);
if(moisture_percentage<25 ||mapp<25){
  sendMessage();
  }
}

void sendMessage()
{
    Serial.println("------------------------------------");
    Serial.println("Send Firebase Cloud Messaging...");
    firebaseData1.fcm.setNotifyMessage("Notification", "Please open AgriSmart App");
    if (Firebase.sendMessage(firebaseData1, 0))
    {
        Serial.println("PASSED");
        Serial.println(firebaseData1.fcm.getSendResult());
        Serial.println("------------------------------------");
        Serial.println();
    }
    else
    {
        Serial.println("FAILED");
        Serial.println("REASON: " + firebaseData1.errorReason());
        Serial.println("------------------------------------");
        Serial.println();
    }
}
