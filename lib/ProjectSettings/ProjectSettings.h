#ifndef ProjectSettings_H
#define ProjectSettings_H

// Bibliotecas do ESP32
#include <WiFi.h>
#include <time.h>
#include <PubSubClient.h>
#include <random>

// Bibliotecas privadas do projeto
#include <ProjectUtilities.h>
#include <TimerActions.h>
#include <SerialUtils.h>

// Definições de constantes
#define DHT_PIN 27
#define DHT_TYPE DHT22

#define MAX_FOOD_LVL 5.0f
#define MIN_FOOD_LVL 30.0f

#define MOTORPULSEON 200u
#define MOTORPULSEOFF 200u

extern const char *MQTTURL;

extern const char *wifiId;       // Nome da internet
extern const char *wifiPassword; // Senha da internet

extern const byte trigPin; // Pino do Trigger
extern const byte echoPin; // Pino do Echo

extern const long timezone;
extern const byte daysavetime;

extern const char *ClientID;

// Object instantiation (done in the .cpp file)

extern PubSubClient client;

extern Time::Clock ESPClock;
extern Sensor::DHT_Sensor DHTSensor;
extern Sensor::Ultrasonic_Sensor USSensor;

extern DynamicScheduler Events;
extern SerialHandler SerialH;

#endif