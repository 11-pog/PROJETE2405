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

const char* MQTTURL;

const char *wifiId; // Nome da internet
const char *wifiPassword;  // Senha da internet

const byte trigPin;  // Pino do Trigger
const byte echoPin; // Pino do Echo

const long timezone;
const byte daysavetime;

const char* ClientID;

// Object instantiation (done in the .cpp file)

extern PubSubClient client;

extern Time::Clock ESPClock;
extern Sensor::DHT_Sensor DHTSensor;
extern Sensor::Ultrasonic_Sensor USSensor;

extern EventScheduler Events;
extern SerialHandler SerialH;

#endif