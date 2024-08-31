// Bibliotecas do ESP32
#include <Arduino.h>
#include <DHT.h>
#include <DHT_U.h>
#include <Preferences.h>

// Bibliotecas privadas do projeto
#include <CppInterop.h>

// Definições de constantes
#define DHT_PIN 12
#define DHT_TYPE DHT22

#define FLASH_SIZE 20

// define sound speed in cm/uS
#define SOUND_SPEED 0.034

DHT dht(DHT_PIN, DHT_TYPE);

const int trigPin = 5;
const int echoPin = 18;

/**/

namespace Flash
{
  Preferences Schedule;
}

namespace Funcs
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

  /*
  void ExplodeOnProjete(DateTime Day)
  {
    if (Day == DiaOfProjete)
    {
      Self.Explode();
    }
  }
  */
  // joke funcgion ignore pls
};

void setup()
{
  Serial.begin(9600);
  
  Flash::Schedule.begin("Schedule", false);

  dht.begin();

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  for (byte i = 20; i > 0; i--)
  {
    Serial.println(i);
    delay(500);
  }
}

void loop()
{
  Serial.print("pedro: pedro");


  delay(200);
}