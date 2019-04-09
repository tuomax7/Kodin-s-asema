#include <DHT.h>
#include <ESP8266WiFi.h>
const int analogInPin = A0;
String apiKey = "C2J6BGG7EKMUWGOY";
const char* ssid = "Verkko-3";
const char* password = "lintuvaara";

const char* server = "api.thingspeak.com";

#define DHTPIN 2 // what pin we're connected to
 
DHT dht(DHTPIN, DHT11,2);  // this is moisture temp sensor model
WiFiClient client;
//WiFiServer server1(80);
void setup() {
  Serial.begin(115200);
  delay(1000);
  dht.begin();
  
 
  WiFi.begin(ssid, password);
 
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
 
}
 
void loop() {
  //String request = client.readStringUntil('\r');
  //Serial.println(request);
  //client.flush();
  /* int value = LOW;
  if (request.indexOf("/fan=ON") != -1)  {
    digitalWrite(fanPin, HIGH);
    value = HIGH;
  }
  if (request.indexOf("/fan=OFF") != -1)  {
    digitalWrite(fanPin, LOW);
    value = LOW;
  }
  */
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  int sensorValue = analogRead(analogInPin);
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    
    return;
  }
 
  if (client.connect(server,80)) {  //   "184.106.153.149" or api.thingspeak.com
    String postStr = apiKey;
           postStr +="&field1=";
           postStr += String(t);
           postStr +="&field2=";
           postStr += String(h);
           postStr +="&field3=";
           postStr += String(sensorValue);
           postStr += "\r\n\r\n";
 
     client.print("POST /update HTTP/1.1\n");
     client.print("Host: api.thingspeak.com\n");
     client.print("Connection: close\n");
     client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
     client.print("Content-Type: application/x-www-form-urlencoded\n");
     client.print("Content-Length: ");
     client.print(postStr.length());
     client.print("\n\n");
     client.print(postStr);
 
     Serial.print("Temperature: ");
     Serial.print(t);
     Serial.print(" degrees Celcius Humidity: ");
     Serial.print(h);
     Serial.println("% send to Thingspeak");
  }
  client.stop();
  Serial.println("Waiting...");
  // thingspeak needs minimum 15 sec delay between updates
  delay(590000);
  }
