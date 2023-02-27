#include "FirebaseESP8266.h"  // Install Firebase ESP8266 library
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
//#include <DHT.h>    // Install DHT11 Library and Adafruit Unified Sensor Library


#define FIREBASE_HOST "bgreen-272ee-default-rtdb.firebaseio.com/FirebaseIOT" //Without http:// or https:// schemes
#define FIREBASE_AUTH "NnMPl7uY48lttWweaRc2V2NVUj4eKkfkbEJe7Bv2"
#define WIFI_SSID "bgreen"
#define WIFI_PASSWORD "bgreen123"
const char* server = "api.openweathermap.org";
const char* url = "/data/2.5/weather?q=trivandrum&appid=c50d182fa705fb41e79a6e09c76b9cc6";

//#define DHTPIN 2    // Connect Data pin of DHT to D2
//int led = D5;     // Connect LED to D5

//#define DHTTYPE    DHT11
//DHT dht(DHTPIN, DHTTYPE);

//Define FirebaseESP8266 data object
FirebaseData firebaseData;
FirebaseData ledData;

FirebaseJson json;

int Ledpin=4;
float moisture_percent;
void setup(){
  Serial.begin(9600);
  pinMode(Ledpin,OUTPUT);
  pinMode(A0,INPUT);
   pinMode(2,OUTPUT);
   digitalWrite(2, HIGH);
  WiFi.begin(WIFI_SSID,WIFI_PASSWORD);
  Serial.println("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  
  Serial.println();

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Serial.println("firebase_begin");
  Firebase.reconnectWiFi(true);
}
void sensor_update(){
  float moisture= analogRead(A0);
  
  Serial.print("sensor read\n");
  Serial.println(moisture);
  delay(100);
  moisture_percent = 100-(moisture/1024*100);
  Serial.println(moisture_percent);
  if(isnan(moisture_percent)){
    Serial.println("null_exit");
    //return;
   }
   if (Firebase.setFloat(firebaseData, "/FirebaseIOT/moisture",moisture_percent))
  {
    Serial.println("PASSED");
    Serial.println("PATH: " + firebaseData.dataPath());
    Serial.println("TYPE: " + firebaseData.dataType());
    Serial.println("ETag: " + firebaseData.ETag());
    Serial.println("------------------------------------");
    Serial.println();
  }
  else
  {
    Serial.println("FAILED");
    Serial.println("REASON: " + firebaseData.errorReason());
    Serial.println("------------------------------------");
    Serial.println();
  }

  }
void weatherupdate(){
  if (WiFi.status() == WL_CONNECTED) {
    WiFiClient client;
    if (client.connect(server, 80)) {
      String request = "GET " + String(url) + " HTTP/1.1\r\n" +
                       "Host: " + String(server) + "\r\n" +
                       "Connection: close\r\n\r\n";
      client.print(request);
      delay(1000);
      String response = "";
      while (client.available()) {
        char c = client.read();
        response=response+c;
       //Serial.println(response);
      }
      client.stop();
      response.replace('[',' ');
      response.replace(']',' ');
       Serial.println("after removal");
       

       //Serial.println(response);
       //char jsonArray [response.length()+1];
       /*response.toCharArray(jsonArray,sizeof(jsonArray));
       jsonArray[response.length()+1]='\0';
       
     //  DynamicJsonDocument doc(1024);
       StaticJsonDocument<1024> doc;
       DeserializationError error = deserializeJson(doc, response);
       if(error){
        Serial.println("deserialization_error");
        Serial.println(error.c_str());
        return;
       }
        String location = doc["name"];
  String country = doc["sys"]["country"];
  float temperature = doc["main"]["temp"];
  int humidity = doc["main"]["humidity"];
  float pressure = doc["main"]["pressure"];
  int id = doc["id"];
  float Speed = doc["wind"]["speed"];
  int degree = doc["wind"]["deg"];
  float longitude = doc["coord"]["lon"];
  float latitude = doc["coord"]["lat"];

  Serial.println();
  Serial.print("Country: ");
  Serial.println(country);
  Serial.print("Location: ");
  Serial.println(location);
  Serial.print("Location ID: ");
  Serial.println(id);
  Serial.printf("Temperature: %.2f°C\r\n", temperature);
  Serial.printf("Humidity: %d %%\r\n", humidity);
  Serial.printf("Pressure: %.2f hpa\r\n", pressure);
  Serial.printf("Wind speed: %.1f m/s\r\n", Speed);
  Serial.printf("Wind degree: %d°\r\n", degree);
  Serial.printf("Longitude: %.2f\r\n", longitude);
  Serial.printf("Latitude: %.2f\r\n", latitude);
     // deserializeJson(doc, response);
    //  Serial.println(doc);
    
    }else {
      Serial.println("Connection to server failed");
    }*/
    }
   }} 
void turnon(){
  Serial.println("checking On from user!");
  Firebase.get(firebaseData,"/FirebaseIOT/status");
  if (firebaseData.dataType() == "string") {
  String value = firebaseData.stringData();
  Serial.println("Value: " + value);
}else if (firebaseData.dataType() == "int") {
  int value = firebaseData.intData();
  Serial.println("Value: " + String(value));
  if(value==1){
    // turn on
    motor();
  }
}
}

void motor(){
  for(int i=0;i<3;i++){
   digitalWrite(2,HIGH);
   delay(2500);
  digitalWrite(2,LOW);
  delay(500);
  i=i+1;
  }
  int value=0;
   if (Firebase.setInt(firebaseData, "/FirebaseIOT/status",value))
  {
    Serial.println("PASSED");
    Serial.println("PATH: " + firebaseData.dataPath());
    Serial.println("TYPE: " + firebaseData.dataType());
    Serial.println("ETag: " + firebaseData.ETag());
    Serial.println("------------------------------------");
    Serial.println();
  }
  else
  {
    Serial.println("FAILED");
    Serial.println("REASON: " + firebaseData.errorReason());
    Serial.println("------------------------------------");
    Serial.println();
  }

  
}
void loop(){
  Serial.println("loop start");
  turnon();
  
  sensor_update();
  if(moisture_percent<45)
  {
    digitalWrite(Ledpin,LOW);
  float moisture_percentch=moisture_percent;
    Serial.println("trying on");
    Serial.begin(9600);
     while(moisture_percentch<45){
    
    Serial.println("change in moisture");
   
 
   digitalWrite(2,HIGH);
   delay(2500);
  digitalWrite(2,LOW);
  delay(500);
 // sensor_update();
 float moisturech= analogRead(A0);
 moisture_percentch = 100-(moisturech/1024*100);
   
  Serial.print("sensor read\n");
  Serial.println(moisturech);
  if(moisture_percentch>45){
    digitalWrite(2,HIGH);
    Serial.println("close immediately");
    delay(5000);
    break;
  }
     }
  }
  if(moisture_percent>45){
   Serial.println("trying off"); 
 // digitalWrite(Ledpin,HIGH);
   digitalWrite(2,HIGH);
  delay(5000);
  }
}
