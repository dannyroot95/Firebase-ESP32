#include <WiFi.h>
#include "DHT.h"
#define DHTTYPE DHT11  
#define DHTPIN 4     // Digital pin connected to the DHT sensor

#include "FirebaseESP32.h"
#define WIFI_SSID "YOUR_SSID"
#define WIFI_PASSWORD "SSID_PASSSWORD"

const char *FIREBASE_HOST = "YOUR_URL_DATABASE";
const char *FIREBASE_AUTH = "SECRET_FIREBASE_KEY";

FirebaseData firebaseData;
FirebaseJson json;

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600); 
  while (!Serial)
  {
  }
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(5000);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
  dht.begin();
}

void loop() {
  
  // Wait a few seconds between measurements.
  delay(2000);

  // Reading temperature or humidity
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.println(F("°C "));
  Firebase.setFloat(firebaseData,"Device/DHT11/humidity", h);
  delay(2000);
  Firebase.setFloat(firebaseData,"Device/DHT11/temperature", t);
  delay(2000);
  json.set("humidity",h);
  json.set("temperature",t);
  Firebase.pushJSON(firebaseData, "Device/registers", json);
  delay(10000);
}
