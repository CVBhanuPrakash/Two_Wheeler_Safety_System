#include <Firebase.h>
#include <FirebaseArduino.h>
#include <FirebaseCloudMessaging.h>
#include <FirebaseError.h>
#include <FirebaseHttpClient.h>
#include <FirebaseObject.h>
#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

// Set these to run example.
#define FIREBASE_HOST "bike-safety-bbd15-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "TZsLRV6WGQziTOmWmTpYFBvlFnDNx9TEXd0hoOoM"
#define WIFI_SSID "realmex2pro" //hotspot name
#define WIFI_PASSWORD "1234567890" //password

FirebaseData firebaseData;

int trigpin = D5;
int echopin = D6;

void setup() {
  Serial.begin(9600);

  pinMode(trigpin, OUTPUT);
  pinMode(echopin, INPUT);

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
  Serial.println(); 
    
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH); 
  Firebase.reconnectWiFi(true);
   
}  

void loop()
{
   sensorUpdate(); //main code
}

void sensorUpdate() {

  long d, r;
  digitalWrite(trigpin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigpin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigpin, LOW);
  
  d = pulseIn(echopin, HIGH);
  r = (d/2) / 29.1;

  Serial.print(r);
  Serial.println(" cm");
  
  if(r <= 5) {
    return 1;
  }
  else{
    return 0;
  }
 
  if(isnan(r)) 
  {
    Serial.print(F("Failed to read from sensor"));
    return;
  }
  if(Firebase.setFloat(firebaseData, "Ultrasonic/cm", r)) 
  {
    Serial.println("PASSED");
    Serial.println("PATH: " + firebaseData.dataPath());
    Serial.println("TYPE: " + firebaseData.dataType());
    Serial.println("Etag: " + firebaseData.ETag());
    Serial.println("----------------------------");
    Serial.println(); 
  }
  else
  {
    Serial.println("FAILED");
    Serial.println("REASON: " + firebaseData.errorReason());
    Serial.println("----------------------------");
    Serial.println(); 
  }
  
}