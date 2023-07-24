#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#define DHTPIN 2     // Digital pin connected to the DHT sensor 
#define MOVEPIN 3
#define SOUNDPIN 4


#define DHTTYPE DHT11

#include <ArduinoJson.h> // Library for create JSON objects

DHT_Unified dht(DHTPIN, DHTTYPE);

int isMoved;
int isSounded;


void setup() {
  Serial.begin(9600);

  pinMode(MOVEPIN, INPUT);
  pinMode(SOUNDPIN, INPUT);

  // Initialize device.
  dht.begin();

  sensor_t sensor;

}

void loop() {
  // Delay between measurements.
  delay(1000);
  
  DynamicJsonDocument doc(1024);
  JsonArray array = doc.to<JsonArray>();

  isMoved = digitalRead(MOVEPIN);
  isSounded = digitalRead(SOUNDPIN);

  // Get temperature event and print its value.
  sensors_event_t event;
  dht.temperature().getEvent(&event);
  if (isnan(event.temperature)) {
    Serial.println(F("Error reading temperature!"));
  }
  else {
    JsonObject temperature = doc.createNestedObject();
    
    temperature["name"] = "temperature";
    temperature["value"] = event.temperature;
    temperature["measurement"] = "celcius";

    // array.add(temperature);`
    // Serial.println(F("°C"));
  }

  // Get humidity event and print its value.
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) {
    Serial.println(F("Error reading humidity!"));
  }
  else {
    JsonObject humidity = doc.createNestedObject();

    humidity["name"] = "humidity";
    humidity["value"] = event.relative_humidity;
    humidity["measurement"] = "percent";

    // array.add(humidity);
    // Serial.print(event.relative_humidity);

  }

  if (isMoved == 1) {
    JsonObject moved = doc.createNestedObject();

    moved["name"] = "moved";
    moved["value"] = 1;
    moved["measurement"] = "bool";
  } else {
    JsonObject moved = doc.createNestedObject();

    moved["name"] = "moved";
    moved["value"] = 0;
    moved["measurement"] = "bool";
  }


  if (isSounded == 0) {
    JsonObject sounded = doc.createNestedObject();

    sounded["name"] = "sound";
    sounded["value"] = 1;
    sounded["measurement"] = "bool";
  } else {
    JsonObject sounded = doc.createNestedObject();

    sounded["name"] = "sound";
    sounded["value"] = 0;
    sounded["measurement"] = "bool";
  }

  serializeJson(array, Serial);
  Serial.println();
}
