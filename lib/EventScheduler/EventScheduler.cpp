#include <EventScheduler.h>

EventScheduler::EventScheduler()
{
    Flash.begin("Schedule", false);
}

void EventScheduler::Schedule(DateTime ScheduledTime, unsigned short extra)
{
    EventTime eventTime;

    eventTime.Hours = ScheduledTime.tm_hour;
    eventTime.Minutes = ScheduledTime.tm_min;

    eventTime.Extra = extra;
}

void EventScheduler::Evaluate(DateTime now, std::function<void(unsigned short)> Action)
{

}

struct EventScheduler::EventTime
{
    unsigned char Hours;
    unsigned char Minutes;
    unsigned short Extra;
};