#ifndef Common_H
#define Common_H

#include <Arduino.h>
#include <CppTypeDefs.h>
#include <time.h>
#include <DHT_U.h>
#include <DHT.h>

namespace Common
{
    class Time
    {
        public:
        Time(byte timezone = -4, byte daysavetime = 1);
        void SyncTime();
        void UpdateTime();
        void PrintDateTime();
        DateTime GetCurrentTime();
        void WhileWaitPerform(std::function<void()> Action, long delay);
    };

    namespace Sensor
    {
        class DHT_Sensor
        {
            DHT_Sensor(int pin, byte type);
            float GetHumidity();
            float GetTemperature();
        };

        class Ultrasonic_Sensor
        {
            Ultrasonic_Sensor(byte trig, byte echo);
            float GetUltraSonic();
        };
    }
}

#endif