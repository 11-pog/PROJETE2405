#ifndef Common_H
#define Common_H

#include <Arduino.h>
#include <CppTypeDefs.h>
#include <DHT_U.h>
#include <DHT.h>

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

        class DelayHandler
        {
        public:
            DelayHandler(std::function<void()> Task);
            void DelayInParallel(unsigned long timeInMillis);
            void SwitchTask(std::function<void()> Task);

        private:
            std::function<void()> Task;
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
            float GetDistance();

        private:
            byte trigPin;
            byte echoPin;
        };
    };
}

#endif