#include <dht.h>

#define DHT11_PIN A0
#define FC28_PIN A1
#define PHOTORESISTOR_PIN A2
#define WATER_PUMP_CONTROL_PIN 8

dht DHT;
int SOIL_MOISTURE_THRESHOLD = -1;
int SOIL_MOISTURE = -1;
int AIR_HUMIDITY = -1;
int AIR_TEMPERATURE = -1;
int PHOTORESISTANCE = -1;

String USER_ID = "bfd8e2c7-ed21-459a-af8e-a5a3128fb390";
String GARDENER_ID = "9d9a01d3-92a8-4148-a7d3-92ef1e9fd29e";
String SOIL_MOISTURE_TAG = "\"soilMoisture\":";
String CONFIGS_SEPARATOR = ",";
String CONFIGS_END_CHAR = "}";

void setup() {
  Serial.begin(115200); //initialize serial monitor
  pinMode(DHT11_PIN, INPUT);
  pinMode(FC28_PIN, INPUT);
  pinMode(PHOTORESISTOR_PIN, INPUT);
  pinMode(WATER_PUMP_CONTROL_PIN, OUTPUT);
}

void loop() {
  updateReadings();
  printReadings();
  modifyEnvironment();
  delay(10000);
  readConfigs();
}

void updateReadings() {
  DHT.read11(DHT11_PIN);
  AIR_TEMPERATURE = DHT.temperature;
  AIR_HUMIDITY = DHT.humidity;
  SOIL_MOISTURE = analogRead(FC28_PIN);
  PHOTORESISTANCE = analogRead(PHOTORESISTOR_PIN);
}

void printReadings() {
  String airTemperature = String(AIR_TEMPERATURE);
  String airHumidity = String(AIR_HUMIDITY);
  String soilMoisture = String(SOIL_MOISTURE);
  String photoresistance = String(PHOTORESISTANCE);

  String readingsJSON = "{\"userId\": \"" + USER_ID +"\", \"gardenerId\": \"" + GARDENER_ID + "\" , \"airTemperature\": " + airTemperature + ", \"airHumidity\": " + airHumidity + ", \"soilMoisture\": " + soilMoisture + ", \"lightIntensity\": " + photoresistance + "}";
  Serial.println(readingsJSON);
}

void readConfigs() {
  String input = Serial.readString();
  if (input == "") {
    return;
  }
  int startIndexSoilMoisture = input.indexOf(SOIL_MOISTURE_TAG);
  if (startIndexSoilMoisture != -1) {
    startIndexSoilMoisture = startIndexSoilMoisture + SOIL_MOISTURE_TAG.length();
    int endIndexSoilMoisture = input.indexOf(CONFIGS_SEPARATOR, startIndexSoilMoisture);
    if (endIndexSoilMoisture == -1) {
      endIndexSoilMoisture = input.indexOf(CONFIGS_END_CHAR, startIndexSoilMoisture);
    }
    if (endIndexSoilMoisture != -1) {
      String soilMoistureThresholdString = input.substring(startIndexSoilMoisture, endIndexSoilMoisture);
      SOIL_MOISTURE_THRESHOLD = soilMoistureThresholdString.toInt();
    }
  }
}

void modifyEnvironment() {
  if (SOIL_MOISTURE_THRESHOLD != -1 && SOIL_MOISTURE > SOIL_MOISTURE_THRESHOLD) {
    digitalWrite(WATER_PUMP_CONTROL_PIN, HIGH);
  } else {
    digitalWrite(WATER_PUMP_CONTROL_PIN, LOW);
  }
}
