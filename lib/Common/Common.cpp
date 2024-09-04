// Referencia ao header desse arquivo
#include <Common.h>

#define SOUND_SPEED 0.034

namespace Time
{
  // Class Clock
  // All here are public
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

namespace Sensor
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

namespace SerialUtils
{
  void ActUponData(std::vector<String> Data)
  {
    if (Data.size() > 2 && (Data[0] == "ADD" && Data[1] == "SCHEDULE"))
    {
      Serial.print("Adding to schedule: ");
      Serial.println(Data[2]);
    }

    if (Data.size() >= 2 && Data[0] == "MOTOR")
    {
      if (Data[1] == "ON")
      {
        Serial.println("Motor is now ON");
        digitalWrite(MOTOR_PIN, 1);
      }

      else if (Data[1] == "OFF")
      {
        Serial.println("Motor is now OFF");
        digitalWrite(MOTOR_PIN, 0);
      }
    }
  }

  std::vector<String> ReadSerialData(unsigned int timeOut)
  {
    std::vector<String> Data;
    String DataFragment = "";
    TimerActions TimeOut;

    auto PushFragment = [&Data, &DataFragment]()
    {
      Data.push_back(DataFragment);
      DataFragment = "";
    };

    while (true)
    {
      char c;

      if (Serial.available())
      {
        c = Serial.read();

        if (c == '\n' || c == '\u0004')
        {
          PushFragment();
          break;
        }

        if (c == ' ' && !DataFragment.isEmpty())
        {
          PushFragment();
        }
        else if (c != ' ')
        {
          DataFragment += c;
        }

        TimeOut.ResetTimer();
      }

      if (TimeOut.IsTimerUp(timeOut))
      {
        break;
      }
    }

    return Data;
  }

  // Testes
  void CheckSerialData()
  {
    std::vector<String> DataList;

    if (Serial.available())
    {
      DataList = ReadSerialData();
      int listSize = DataList.size();

      ActUponData(DataList);

      if (listSize > 0)
      {
        for (int i = 0; i < listSize; i++)
        {
          Serial.println(DataList[i]);
        }
        Serial.println(listSize);
      }
    }
  }
}