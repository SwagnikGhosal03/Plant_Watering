#define BLYNK_TEMPLATE_ID "TMPL3VWQJ2CIv"
#define BLYNK_TEMPLATE_NAME "Plant Watering System"
#define BLYNK_AUTH_TOKEN "03q6u9AIE7FpZ4UB8ja4_G37fpkI9EWN"
#include <OneWire.h>
#include <SPI.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS D2
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
char auth[] ="03q6u9AIE7FpZ4UB8ja4_G37fpkI9EWN";
char ssid[] = "Redmi 12 5G";
char pass[] = "12345678r";
#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
SimpleTimer timer;
#define PUMP_PIN D5 // Pump pin
#define LIGHT_PIN D3 // Light pin

void sendSensor()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  Blynk.virtualWrite(V5, h); // Humidity
  Blynk.virtualWrite(V6, t); // Temperature
}

BLYNK_WRITE(V3) // Virtual Pin for the pump
{
  int pinValue = param.asInt(); // Get the value from the Blynk app
  digitalWrite(PUMP_PIN, pinValue ? HIGH : LOW); // Control the pump
}

BLYNK_WRITE(V4) // Virtual Pin for the light
{
  int pinValue = param.asInt(); // Get the value from the Blynk app
  digitalWrite(LIGHT_PIN, pinValue ? HIGH : LOW); // Control the light
}

void setup()
{
  Serial.begin(9600);
  dht.begin();
  pinMode(PUMP_PIN, OUTPUT); // Set PUMP_PIN as output
  pinMode(LIGHT_PIN, OUTPUT); // Set LIGHT_PIN as output

  timer.setInterval(1000L, sendSensor);
  Blynk.begin(auth, ssid, pass);
  sensors.begin();
}

int sensor=0;
int output=0;

void sendTemps()
{
  sensor=analogRead(A0);
  output=(145-map(sensor,0,1023,0,100)); 
  delay(1000);
  sensors.requestTemperatures();
  float temp = sensors.getTempCByIndex(0);
  Serial.println(temp);
  Serial.print("moisture = ");
  Serial.print(output);
  Serial.println("%");
  Blynk.virtualWrite(V1, temp); // Temperature
  Blynk.virtualWrite(V2, output); // Soil moisture
  delay(1000);
}

void loop()
{
  Blynk.run();
  timer.run();
  sendTemps();
}
