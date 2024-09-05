#ifndef EventScheduler_H
#define EventScheduler_H

#include <Arduino.h>
#include <time.h>
#include <CppTypeDefs.h>
#include <Preferences.h>

class EventScheduler
{
public:
    EventScheduler();
    void Evaluate(DateTime now, std::function<void(unsigned short)> Action);
    void Schedule(DateTime ScheduledTime, unsigned short extra = 0);

private:
    Preferences Flash;
    struct EventTime;
};

#endif