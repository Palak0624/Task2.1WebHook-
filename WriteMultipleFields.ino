#include <WiFiNINA.h>
#include "secrets.h"
#include "ThingSpeak.h" // always include thingspeak header file after other header files and custom macros
#include "DHT.h"
#define DHTPIN 2
#define DHTTYPE DHT22 

char ssid[] = SECRET_SSID;   // your network SSID (name) 
char pass[] = SECRET_PASS;   // your network password
int keyIndex = 0;            // your network key Index number (needed only for WEP)
WiFiClient  client;
DHT dht(DHTPIN, DHTTYPE);

unsigned long myChannelNumber = SECRET_CH_ID;
const char * myWriteAPIKey = SECRET_WRITE_APIKEY;

float temperatureC = 0;
float humidity = 0;
String myStatus = "";

void setup() {
  Serial.begin(115200);      // Initialize serial 
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo native USB port only
  }
  
  ThingSpeak.begin(client);  // Initialize ThingSpeak 
  Serial.println(F("DHTxx test!"));
  dht.begin();
}

void loop() {

  // Connect or reconnect to WiFi
  if(WiFi.status() != WL_CONNECTED){
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(SECRET_SSID);
    while(WiFi.status() != WL_CONNECTED){
      WiFi.begin(ssid, pass);  // Connect to WPA/WPA2 network. Change this line if using open or WEP network
      Serial.print(".");
      delay(5000);     
    } 
    Serial.println("\nConnected.");
  }
    
  float humadity = dht.readHumidity();
  float temperature = dht.readTemperature();
  if (isnan(humadity) || isnan(temperature)) 
  {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  myStatus = "Temperature: " + String(temperature) + " C, Humidity: " + String(humidity) + "%";

  ThingSpeak.setField(1, temperature);
  ThingSpeak.setField(2, humidity);

  ThingSpeak.setStatus(myStatus);


  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  if(x == 200){
    Serial.println("Channel update successful.");
  }
  else{
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }

  delay(20000); 
}
