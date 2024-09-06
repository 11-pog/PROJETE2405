#include <EventScheduler.h>

EventScheduler::EventScheduler()
{
    Flash.begin("Schedule", false);
}

void EventScheduler::SaveToFlash()
{
    unsigned long size = ScheduleList.size() * sizeof(std::pair<EventTime, unsigned short>);

    byte serializedScheduler[size];
    memcpy(serializedScheduler, &ScheduleList, size);

    Flash.putBytes("schedule", serializedScheduler, size);
}

std::vector<EventData> EventScheduler::GetDataFromFlash()
{
    if (Flash.isKey("schedule"))
    {
        unsigned long size = Flash.getBytes("schedule", nullptr, 0);
        byte unserializedScheduler[] = new(size);
    }
    return std::vector<EventData>();
}

void EventScheduler::Schedule(DateTime ScheduledTime, unsigned short extra)
{
    EventTime eventTime;
    
    eventTime.Hours = ScheduledTime.tm_hour;
    eventTime.Minutes = ScheduledTime.tm_min;

    EventData eventData(eventTime, extra);
    ScheduleList.push_back(eventData);

    SaveToFlash();
}

std::pair<bool, unsigned short> EventScheduler::EvaluateConditions(DateTime now)
{
    EventData eventData = GetNextScheduledEvent();
    EventTime eventTime = eventData.first;

    bool isHour = eventTime.Hours == now.tm_hour;
    bool isMinute = eventTime.Minutes == now.tm_min;

    return std::make_pair(isHour && isMinute, eventData.second);
}

EventData EventScheduler::GetNextScheduledEvent()
{
    return EventData(); // PlaceHolder
}

void EventScheduler::Evaluate(DateTime now, std::function<void(unsigned short)> action)
{
    std::pair<bool, unsigned short> result = EvaluateConditions(now);

    if (result.first)
    {
        if (!done)
        {
            action(result.second);
            done = true;
        }
    }
    else
    {
        done = false;
    }
}
