#include <ProjectSettings.h>

const char *MQTTURL = "broker.emqx.io";

const char *wifiId = "internet";       // Nome da internet
const char *wifiPassword = "internet"; // Senha da internet

const byte trigPin = 5;  // Pino do Trigger
const byte echoPin = 18; // Pino do Echo

const long timezone = -3;
const byte daysavetime = 0;

const char *ClientID = "_PROJETE2405ETEFMC_ESPBOARD:2405";

WiFiClient esp_Client;
PubSubClient client(esp_Client);

Time::Clock ESPClock(timezone);
Sensor::DHT_Sensor DHTSensor(DHT_PIN, DHT_TYPE);
Sensor::Ultrasonic_Sensor USSensor(trigPin, echoPin);

EventScheduler Events;
SerialHandler SerialH(&Events);