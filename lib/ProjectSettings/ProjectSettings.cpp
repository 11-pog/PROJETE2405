#include <ProjectSettings.h>

MQTTURL = "broker.emqx.io";

wifiId = "internet"; // Nome da internet
wifiPassword = "internet";  // Senha da internet

trigPin = 5;  // Pino do Trigger
echoPin = 18; // Pino do Echo

timezone = -3;
daysavetime = 0;

ClientID = "_PROJETE2405ETEFMC_ESPBOARD:2405";


WiFiClient esp_Client;
PubSubClient client(esp_Client);

Time::Clock ESPClock(timezone);
Sensor::DHT_Sensor DHTSensor(DHT_PIN, DHT_TYPE);
Sensor::Ultrasonic_Sensor USSensor(trigPin, echoPin);

EventScheduler Events;
SerialHandler SerialH(&Events);