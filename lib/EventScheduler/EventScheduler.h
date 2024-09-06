#ifndef EventScheduler_H
#define EventScheduler_H

#include <Arduino.h>
#include <time.h>
#include <CppTypeDefs.h>
#include <Preferences.h>

#include <vector>

struct EventTime
{
    unsigned char Hours;
    unsigned char Minutes;
};

typedef std::pair<EventTime, unsigned short> EventData;

class EventScheduler
{
public:
    EventScheduler();
    void Evaluate(DateTime now, std::function<void(unsigned short)> Action);
    void Schedule(DateTime ScheduledTime, unsigned short extra = 0);

private:
    std::pair<bool, unsigned short> EvaluateConditions(DateTime now);
    bool done;
    void SaveToFlash();
    EventData GetNextScheduledEvent();
    Preferences Flash;
    std::vector<EventData> GetDataFromFlash();
    std::vector<EventData> ScheduleList;
};

#endif