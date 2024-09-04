// Referencia ao header desse arquivo
#include <Common.h>

#define SOUND_SPEED 0.034

namespace Common::Time
{
  //Class Clock
  //All here are public
  Clock::Clock(long timezone, byte daysavetime) : timezone(timezone), daysavetime(daysavetime) {}

  void Clock::SyncTime()
  {
    configTime(timezone * 3600, daysavetime * 3600, "time.nist.gov", "0.pool.ntp.org", "1.pool.ntp.org");
  }

  void Clock::UpdateTime()
  {
    getLocalTime(&localTime);
  }

  void Clock::PrintDateTime()
  {
    UpdateTime();
    String date = String((localTime.tm_mon) + 1) + "/" + String(localTime.tm_mday);
    String time = (String(localTime.tm_hour) + ":" + String(localTime.tm_min) + ":" + String(localTime.tm_sec));

    Serial.print(date);
    Serial.print("  ");
    Serial.println(time);
  }

  DateTime Clock::GetCurrentTime()
  {
    UpdateTime();
    return localTime;
  }
}

namespace Common::Sensor
{
  DHT_Sensor::DHT_Sensor(int pin, byte type) : dht(pin, type)
  {
    dht.begin();
  }

  float DHT_Sensor::GetHumidity()
  {
    float humidityValue = dht.readHumidity();

    return humidityValue;
  }

  float DHT_Sensor::GetTemperature()
  {
    float Temp = dht.readTemperature();

    return Temp;
  }

  Ultrasonic_Sensor::Ultrasonic_Sensor(byte trig, byte echo)
  {
    this->trigPin = trig;
    this->echoPin = echo;

    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
  }

  float Ultrasonic_Sensor::GetDistance()
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