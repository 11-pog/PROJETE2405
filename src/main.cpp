// Bibliotecas do ESP32
#include <Arduino.h>
#include <DHT.h>
#include <DHT_U.h>
#include <Preferences.h>
#include <time.h>
#include <WiFi.h>
#include <esp_timer.h>

// Bibliotecas privadas do projeto
#include <CppTypeDefs.h>
#include <Common.h>

// Definições de constantes
#define DHT_PIN 12
#define DHT_TYPE DHT22

const char *wifiId = "internet";       // Nome da internet
const char *wifiPassword = "internet"; // Senha da internet

const byte trigPin = 5;  // Pino do Trigger
const byte echoPin = 18; // Pino do Echo

const byte timezone = -4;
const byte daysavetime = 1;

// #define FLASH_SIZE 20 [UNUSED]

/**/

namespace Flash // Namespace pra guardar o objeto preferences (Pra ficar bonitinho xd)
{
  Preferences Schedule;
}


void setup()
{
  Serial.begin(9600);

  Flash::Schedule.begin("Schedule", false);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  WiFi.begin(wifiId, wifiPassword);

  Serial.print("Conectando");

  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print('.');
    delay(1000);

    if (WiFi.status() == WL_CONNECT_FAILED)
    {
      ESP.restart();
    }
  }

  Serial.println("");

  // Common::SyncTime();

  for (byte i = 10; i > 0; i--)
  {
    Serial.println(i);
    delay(500);
  }
}

void loop()
{
  // Common::PrintDateTime();
  delay(1000);
}