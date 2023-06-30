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
  // delay(60000);
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
    
    temperature["sensorType"] = "temperature";
    temperature["sensorValue"] = event.temperature;
    temperature["sensorMeasurement"] = "celcius";

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

    humidity["sensorType"] = "humidity";
    humidity["sensorValue"] = event.relative_humidity;
    humidity["sensorMeasurement"] = "percent";

    // array.add(humidity);
    // Serial.print(event.relative_humidity);

  }

  if (isMoved == 1) {
    JsonObject moved = doc.createNestedObject();

    moved["sensorType"] = "moved";
    moved["sensorValue"] = true;
    moved["sensorMeasurement"] = "bool";
  } else {
    JsonObject moved = doc.createNestedObject();

    moved["sensorType"] = "moved";
    moved["sensorValue"] = false;
    moved["sensorMeasurement"] = "bool";
  }


  if (isSounded == 0) {
    JsonObject sounded = doc.createNestedObject();

    sounded["sensorType"] = "sound";
    sounded["sensorValue"] = true;
    sounded["sensorMeasurement"] = "bool";
  } else {
    JsonObject sounded = doc.createNestedObject();

    sounded["sensorType"] = "sound";
    sounded["sensorValue"] = false;
    sounded["sensorMeasurement"] = "bool";
  }

  serializeJson(array, Serial);
  Serial.println();
}
