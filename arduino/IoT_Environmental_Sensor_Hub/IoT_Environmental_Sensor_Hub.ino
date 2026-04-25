#include <WiFi.h>
#include <ArduinoMqttClient.h>
#include <SPI.h>
#include <DHT.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>

///////please enter your sensitive data here
char ssid[] = "BAHAR";        // your network SSID (name)
char pass[] = "123456";                   // your network password

WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);

const char broker[] = "broker.emqx.io";
int        port     = 1883;
const char topic[]  = "esp32/dht22/temperature";
const char topic2[] = "esp32/dht22/humidity";
const char topic3[] = "esp32/bmp280/temperature";
const char topic4[] = "esp32/bmp280/pressure";
const char topic5[] = "esp32/bmp280/altitude";

//set interval for sending messages (milliseconds)
const long interval = 5000;
unsigned long previousMillis = 0;

// ===== Sensor pins =====
#define DHT_PIN     15
#define DHT_TYPE    DHT22
#define BMP_SCK     18
#define BMP_MOSI    19
#define BMP_MISO    23
#define BMP_CS      5

DHT dht(DHT_PIN, DHT_TYPE);
Adafruit_BMP280 bmp(BMP_CS, BMP_MOSI, BMP_MISO, BMP_SCK);

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  dht.begin();
  if (!bmp.begin()) {
    Serial.println("BMP280 not found, check wiring!");
    while (1);
  }

  // attempt to connect to Wifi network:
  Serial.print("Attempting to connect to WPA SSID: ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    // failed, retry
    Serial.print(".");
    delay(500);
  }

  Serial.println("You're connected to the network");
  Serial.println();

  Serial.print("Attempting to connect to the MQTT broker: ");
  Serial.println(broker);

  if (!mqttClient.connect(broker, port)) {
    Serial.print("MQTT connection failed! Error code = ");
    Serial.println(mqttClient.connectError());

    while (1);
  }

  Serial.println("You're connected to the MQTT broker!");
  Serial.println();
}

void loop() {
  // call poll() regularly to allow the library to send MQTT keep alive which
  // avoids being disconnected by the broker
  mqttClient.poll();

  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    // save the last time a message was sent
    previousMillis = currentMillis;

    // read sensors
    float Rvalue  = dht.readTemperature();          // DHT22 temp (°C)
    float Rvalue2 = dht.readHumidity();             // DHT22 humidity (%)
    float Rvalue3 = bmp.readTemperature();          // BMP280 temp (°C)
    float Rvalue4 = bmp.readPressure() / 100.0;     // BMP280 pressure (hPa)
    float Rvalue5 = bmp.readAltitude(1013.25);      // BMP280 altitude (m)

    Serial.print("Sending message to topic: ");
    Serial.println(topic);
    Serial.println(Rvalue);

    Serial.print("Sending message to topic: ");
    Serial.println(topic2);
    Serial.println(Rvalue2);

    Serial.print("Sending message to topic: ");
    Serial.println(topic3);
    Serial.println(Rvalue3);

    Serial.print("Sending message to topic: ");
    Serial.println(topic4);
    Serial.println(Rvalue4);

    Serial.print("Sending message to topic: ");
    Serial.println(topic5);
    Serial.println(Rvalue5);

    // send message, the Print interface can be used to set the message contents
    mqttClient.beginMessage(topic);
    mqttClient.print(Rvalue);
    mqttClient.endMessage();

    mqttClient.beginMessage(topic2);
    mqttClient.print(Rvalue2);
    mqttClient.endMessage();

    mqttClient.beginMessage(topic3);
    mqttClient.print(Rvalue3);
    mqttClient.endMessage();

    mqttClient.beginMessage(topic4);
    mqttClient.print(Rvalue4);
    mqttClient.endMessage();

    mqttClient.beginMessage(topic5);
    mqttClient.print(Rvalue5);
    mqttClient.endMessage();

    Serial.println();
  }
}
