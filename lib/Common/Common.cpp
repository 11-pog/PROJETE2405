// Bibliotecas do ESP32
#include <Arduino.h>
#include <CppTypeDefs.h>
#include <time.h>
#include <DHT_U.h>
#include <DHT.h>

// Referencia ao header desse arquivo
#include <Common.h>

#define SOUND_SPEED 0.034

namespace Common
{
  class Time
  {
    LocalTime localTime;

    long timezone;
    byte daysavetime;

    Time(byte timezone = -4, byte daysavetime = 1)
    {
      this->daysavetime = daysavetime;
      this->timezone = timezone;
    }

    void SyncTime()
    {
      configTime(3600 * timezone, daysavetime * 3600, "time.nist.gov", "0.pool.ntp.org", "1.pool.ntp.org");
    }

    void UpdateTime()
    {
      getLocalTime(&localTime);
    }

    void PrintDateTime()
    {
      UpdateTime();
      String date = String((localTime.tm_mon) + 1) + "/" + String(localTime.tm_mday);
      String time = (String(localTime.tm_hour) + ":" + String(localTime.tm_min) + ":" + String(localTime.tm_sec));

      Serial.print(date);
      Serial.print("  ");
      Serial.println(time);
    }

    DateTime GetCurrentTime()
    {
      UpdateTime();
      return localTime;
    }

    void WhileWaitPerform(std::function<void()> Action, long delay)
    {
    }
  };

  namespace Sensor
  {
    class DHT_Sensor
    {
      DHT dht;

      DHT_Sensor(int pin, byte type) : dht(pin, type)
      {
        dht.begin();
      }

      float GetHumidity()
      {
        float humidityValue = dht.readHumidity();

        return humidityValue;
      }

      float GetTemperature()
      {
        float Temp = dht.readTemperature();

        return Temp;
      }
    };

    class Ultrasonic_Sensor
    {
      byte trigPin;
      byte echoPin;

      Ultrasonic_Sensor(byte trig, byte echo)
      {
        this->trigPin = trig;
        this->echoPin = echo;
      }

      float GetUltraSonic()
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
    };
  };

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
}