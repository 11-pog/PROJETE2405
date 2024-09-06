#include <EventScheduler.h>

EventScheduler::EventScheduler()
{
    Flash.begin("Schedule", false); // This is preferences.h btw
    this->ScheduleList = GetDataFromFlash();
}

void EventScheduler::SaveToFlash()
{
    unsigned long size = ScheduleList.size() * sizeof(EventData);

    byte serializedScheduler[size];
    memcpy(serializedScheduler, &ScheduleList, size);

    Flash.putBytes("schedule", serializedScheduler, size);
}

EventList EventScheduler::GetDataFromFlash()
{
    if (!Flash.isKey("schedule"))
    {
        return EventList();
    }

    unsigned long size = Flash.getBytes("schedule", nullptr, 0);
    byte *unserializedScheduler = new byte[size];
    Flash.getBytes("schedule", unserializedScheduler, size);

    EventList eventData;

    for (unsigned long i = 0; i < size; i += sizeof(EventData))
    {
        EventData temp;
        memcpy(&temp, unserializedScheduler + i, sizeof(EventData));
        eventData.push_back(temp);
    }

    delete[] unserializedScheduler;
    return eventData;
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

std::pair<bool, unsigned short> EventScheduler::IsEventDue(DateTime now)
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
    std::pair<bool, unsigned short> result = IsEventDue(now);

    if (result.first && !done)
    {
        action(result.second);
        done = true;
    }
    else if (!result.first)
    {
        done = false;
    }
}
