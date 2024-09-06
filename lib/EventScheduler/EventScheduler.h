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
typedef std::vector<EventData> EventList;

class EventScheduler
{
public:
    EventScheduler();
    void Evaluate(DateTime now, std::function<void(unsigned short)> Action);
    void Schedule(DateTime ScheduledTime, unsigned short extra = 0);

    //Yet-To-Define Functions
    void UnSchedule();
    void ReSchedule();
    void ResetFlash();

private:
    std::pair<bool, unsigned short> IsEventDue(DateTime now);
    bool done;
    void SaveToFlash();
    EventData GetNextScheduledEvent();
    Preferences Flash;
    EventList GetDataFromFlash();
    EventList ScheduleList;
};

#endif