// Bibliotecas do ESP32
#include <Arduino.h>
#include <DHT.h>
#include <DHT_U.h>
#include <Preferences.h>
#include <time.h>
#include <WiFi.h>

// Bibliotecas privadas do projeto
#include <CppInterop.h>

// Definições de constantes
#define DHT_PIN 12
#define DHT_TYPE DHT22

#define SOUND_SPEED 0.034 // Velocidade do som em Cm/us

const char *wifiId = "Anizio CPereira"; // Nome da internet
const char *wifiPassword = "25030917";  // Senha da internet

const byte trigPin = 5;  // Pino do Trigger
const byte echoPin = 18; // Pino do Echo

const long timezone = -4;
const byte daysavetime = 1;

// #define FLASH_SIZE 20 [UNUSED]

DHT dht(DHT_PIN, DHT_TYPE); // Criação de uma instancia do objeto DHT

struct tm localTime;

/**/

namespace Flash // Namespace pra guardar o objeto preferences (Pra ficar bonitinho xd)
{
  Preferences Schedule;
}

namespace Funcs // Namespace pra guardar funções gerais
{
  static float GetUltraSonic()
  {
    long duration;
    float distanceCm;

    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);

    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);

    digitalWrite(trigPin, LOW);

    duration = pulseIn(echoPin, HIGH);

    distanceCm = duration * SOUND_SPEED / 2;

    return distanceCm;
  }

  static float GetHumidity()
  {
    float humidityValue = dht.readHumidity();

    return humidityValue;
  }

  static float GetTemperature()
  {
    float Temp = dht.readTemperature();

    return Temp;
  }

  static void SyncTime()
  {
    configTime(3600 * timezone, daysavetime * 3600, "time.nist.gov", "0.pool.ntp.org", "1.pool.ntp.org");
  }

  static void UpdateTime()
  {
    getLocalTime(&localTime);
  }

  static void PrintDateTime()
  {
    UpdateTime();
    String date = String((localTime.tm_mon) + 1) + "/" + String(localTime.tm_mday);
    String time = (String(localTime.tm_hour) + ":" + String(localTime.tm_min) + ":" + String(localTime.tm_sec));

    Serial.print(date);
    Serial.print("    ");
    Serial.println(time);
  }

  /*
  struct Projete
  {
    static const byte day = 7;
    static const byte month = 10;
  };

  void ExplodeOnProjete(tm info)
  {
    if (info.tm_mday == Projete::day && info.tm_mon + 1 == Projete::month)
    {
      ESP.SelfDestruct();
    }
  }
  */
  // Piadas
};

void setup()
{
  Serial.begin(9600);

  Flash::Schedule.begin("Schedule", false);

  dht.begin();

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

  Funcs::SyncTime();

  for (byte i = 10; i > 0; i--)
  {
    Serial.println(i);
    delay(500);
  }
}

void loop()
{
  Funcs::PrintDateTime();
  delay(1000);
}