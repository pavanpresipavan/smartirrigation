#include <ESP8266WiFi.h>                                                // esp8266 library    
#include "FirebaseESP8266.h"


#define WIFI_SSID "host-name"
#define WIFI_PASSWORD "host-password"
#define FIREBASE_HOST "firebase-host" 
#define FIREBASE_AUTH "auth-key"

#define FIREBASE_FCM_SERVER_KEY "server-key"
#define FIREBASE_FCM_DEVICE_TOKEN_1 "device-token"

FirebaseData firebaseData1;
int buz=0;
void setup() {
 
  pinMode(0,OUTPUT);
 
  Serial.begin(9600);   // initialize serial communication at 9600 BPS
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

  int sensorValue = analogRead(A0);   // read the input on analog pin 0

  float voltage = sensorValue * (10.0 / 1023.0)*10;   // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V)
 
  Serial.println(voltage);   // print out the value you read
  Firebase.setFloat(firebaseData1,"Light",voltage);

  int result=Firebase.getInt(firebaseData1,"/Mstatus");
  int status=firebaseData1.intData();
  Serial.println(status);
  if(status==1)
  {
    digitalWrite(buz,HIGH);
  }
  if(status==0)
  {
  digitalWrite(buz,LOW);
  }

}
 void sendMessage()
{
    Serial.println("------------------------------------");
    Serial.println("Send Firebase Cloud Messaging...");
    firebaseData1.fcm.setNotifyMessage("Notification", "Open your AgriSmart app");
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
