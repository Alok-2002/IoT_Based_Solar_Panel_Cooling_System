#define BLYNK_TEMPLATE_ID "YourTemplateID"
#define BLYNK_TEMPLATE_NAME "SolarPanelCooling"
#define BLYNK_AUTH_TOKEN "YourBlynkAuthToken"

#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <BlynkSimpleEsp8266.h>
#include <PubSubClient.h>
#include <DHT.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// WiFi and AWS
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";
const char* aws_endpoint = "YOUR_AWS_ENDPOINT";
const int port = 8883;
WiFiClientSecure net;
PubSubClient client(net);

// Blynk
char auth[] = BLYNK_AUTH_TOKEN;

// Sensors and Pins
#define DHTPIN D1
#define DHTTYPE DHT11
#define ONE_WIRE_BUS D2
#define RELAY_PIN D5
#define VOLTAGE_PIN A0

DHT dht(DHTPIN, DHTTYPE);
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature ds18b20(&oneWire);

// Variables
bool pumpOn = false;
float panelTemp = 0.0;
float voltage = 0.0;
float current = 1.2;  // Placeholder
float power = 0.0;

void connectAWS() {
  net.setCACert(AWS_ROOT_CA);
  net.setCertificate(DEVICE_CERT);
  net.setPrivateKey(DEVICE_PRIVATE_KEY);
  client.setServer(aws_endpoint, port);
  while (!client.connected()) {
    Serial.println("Connecting to AWS IoT...");
    if (client.connect("ESP8266Client")) {
      Serial.println("Connected to AWS IoT");
    } else {
      Serial.print("AWS connect failed. State: ");
      Serial.println(client.state());
      delay(2000);
    }
  }
}

void publishToAWS(float panelTemp, float envTemp, float humidity, bool pumpStatus, float voltage, float current, float power) {
  String payload = "{";
  payload += "\"panel_temp\":" + String(panelTemp, 2) + ",";
  payload += "\"env_temp\":" + String(envTemp, 2) + ",";
  payload += "\"humidity\":" + String(humidity, 2) + ",";
  payload += "\"pump_status\":\"" + String(pumpStatus ? "ON" : "OFF") + "\",";
  payload += "\"voltage\":" + String(voltage, 2) + ",";
  payload += "\"current\":" + String(current, 2) + ",";
  payload += "\"power\":" + String(power, 2);
  payload += "}";
  client.publish("solar/panel/data", payload.c_str());
  Serial.println("Published to AWS: " + payload);
}

void sendToBlynk(float panelTemp, float envTemp, float humidity, bool pumpStatus, float voltage, float current, float power) {
  Blynk.virtualWrite(V0, panelTemp);
  Blynk.virtualWrite(V1, envTemp);
  Blynk.virtualWrite(V2, humidity);
  Blynk.virtualWrite(V3, voltage);
  Blynk.virtualWrite(V4, current);
  Blynk.virtualWrite(V5, power);
  Blynk.virtualWrite(V6, pumpStatus ? 1 : 0);
}

void setup() {
  Serial.begin(115200);
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);
  dht.begin();
  ds18b20.begin();

  WiFi.begin(ssid, password);
  Blynk.begin(auth, ssid, password);  // Initializes Blynk
  connectAWS();
}

void loop() {
  if (!client.connected()) {
    connectAWS();
  }
  client.loop();
  Blynk.run();

  // Sensor Readings
  ds18b20.requestTemperatures();
  panelTemp = ds18b20.getTempCByIndex(0);
  float envTemp = dht.readTemperature();
  float humidity = dht.readHumidity();
  int rawVoltage = analogRead(VOLTAGE_PIN);
  voltage = (rawVoltage / 1023.0) * 25.0;
  power = voltage * current;

  if (isnan(envTemp) || isnan(humidity) || panelTemp == DEVICE_DISCONNECTED_C) {
    Serial.println("Sensor Error");
    delay(5000);
    return;
  }

  // Pump Control Logic
  if (panelTemp > 25.0) {
    digitalWrite(RELAY_PIN, HIGH);
    pumpOn = true;
  } else {
    digitalWrite(RELAY_PIN, LOW);
    pumpOn = false;
  }

  // Data Send
  publishToAWS(panelTemp, envTemp, humidity, pumpOn, voltage, current, power);
  sendToBlynk(panelTemp, envTemp, humidity, pumpOn, voltage, current, power);

  delay(10000);
}

// ============== AWS Certificates ==============
static const char AWS_ROOT_CA[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
...Your Root CA...
-----END CERTIFICATE-----
)EOF";

static const char DEVICE_CERT[] PROGMEM = R"KEY(
-----BEGIN CERTIFICATE-----
...Your Device Certificate...
-----END CERTIFICATE-----
)KEY";

static const char DEVICE_PRIVATE_KEY[] PROGMEM = R"KEY(
-----BEGIN RSA PRIVATE KEY-----
...Your Device Private Key...
-----END RSA PRIVATE KEY-----
)KEY";
