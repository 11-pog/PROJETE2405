#ifndef Common_H
#define Common_H

#include <Arduino.h>
#include <CppTypeDefs.h>
#include <time.h>
#include <DHT_U.h>
#include <DHT.h>
#include <WiFi.h>

namespace Common
{
    namespace Time
    {
        class Clock
        {
        public:
            Clock(long timezone, byte daysavetime);
            void SyncTime();
            void UpdateTime();
            void PrintDateTime();
            DateTime GetCurrentTime();

        private:
            LocalTime localTime;
            long timezone;
            byte daysavetime;
        };

        class DelayTask
        {
        public:
            void RunInParallel(std::function<void()> Action, unsigned long timeInMillis);

        private:
            bool HasDelayElapsed(unsigned long timeInMillis);
            void UpdateLastTime();
            unsigned long lastTime;
        };
    }

    namespace Sensor
    {

        class DHT_Sensor
        {
        public:
            DHT_Sensor(int pin, byte type);
            float GetHumidity();
            float GetTemperature();

        private:
            DHT dht;
        };

        class Ultrasonic_Sensor
        {
        public:
            Ultrasonic_Sensor(byte trig, byte echo);
            float GetUltraSonic();

        private:
            byte trigPin;
            byte echoPin;
        };
    };
}

#endif