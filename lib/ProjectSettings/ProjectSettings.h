#ifndef ProjectSettings_H
#define ProjectSettings_H

// Bibliotecas do ESP32
#include <WiFi.h>
#include <time.h>
#include <PubSubClient.h>
#include <random>

// Bibliotecas privadas do projeto
#include <Common.h>
#include <TimerActions.h>
#include <SerialUtils.h>

// Definições de constantes
#define DHT_PIN 27
#define DHT_TYPE DHT22

#define MQTTUSER "2405PROJ"
#define MQTTPASS "123Grupo2405Incrivel"

const IPAddress MQTTURL(35, 172, 255, 228);

const char *wifiId = "alunos"; // Nome da internet
const char *wifiPassword = "etefmc123";  // Senha da internet

const byte trigPin = 5;  // Pino do Trigger
const byte echoPin = 18; // Pino do Echo

const long timezone = -3;
const byte daysavetime = 0;

const IPAddress staticIP(192, 168, 1, 2);
const IPAddress gateway(192, 168, 1, 1);
const IPAddress subnet(255, 255, 255, 0);
const IPAddress dnsServer(8, 8, 8, 8);

const char* ClientID = "_PROJETE2405ETEFMC_ESPBOARD:2405";

// Object instantiation

WiFiClient esp_Client;
PubSubClient mqtt_Client(esp_Client);

Time::Clock ESPClock(timezone);
Sensor::DHT_Sensor DHTSensor(DHT_PIN, DHT_TYPE);
Sensor::Ultrasonic_Sensor USSensor(trigPin, echoPin);

EventScheduler Events;
SerialHandler SerialH(&Events);

#endif