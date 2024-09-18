#ifndef Common_H
#define Common_H

#include <Arduino.h>
#include <CppTypeDefs.h>
#include <DHT_U.h>
#include <DHT.h>
#include <TimerActions.h>

#include <WiFi.h>

// Bibliotecas do C++
#include <vector>

namespace Time
{
    class Clock
    {
    public:
        Clock(long timezone, byte daysavetime = 0);
        void SyncTime();
        void UpdateTime();
        void PrintDateTime();
        DateTime GetCurrentTime();

    private:
        LocalTime localTime;
        long timezone;
        byte daysavetime;
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
}

void WifiStuff(WiFiEvent_t event);

#endif