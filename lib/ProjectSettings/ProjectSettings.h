#ifndef ProjectSettings_H
#define ProjectSettings_H

// Bibliotecas do ESP32
#include <Preferences.h>
#include <WiFi.h>
#include <time.h>

// Bibliotecas do C++
#include <vector>

// Bibliotecas privadas do projeto
#include <Common.h>
#include <TimedEvents.h>
#include <DelayHandler.h>

// Definições de constantes
#define DHT_PIN 12
#define DHT_TYPE DHT22

const char *wifiId = "Anizio CPereira"; // Nome da internet
const char *wifiPassword = "25030917";  // Senha da internet

const byte trigPin = 5;  // Pino do Trigger
const byte echoPin = 18; // Pino do Echo

const long timezone = -3;
const byte daysavetime = 0;

namespace Flash // Namespace pra guardar o objeto preferences (Pra ficar bonitinho xd)
{
    Preferences Schedule;
}

// Object instantiation

Common::Time::Clock ESPClock(timezone);
Common::Sensor::DHT_Sensor DHTSensor(DHT_PIN, DHT_TYPE);
Common::Sensor::Ultrasonic_Sensor USSensor(trigPin, echoPin);

#endif