#include <EventScheduler.h>

EventScheduler::EventScheduler()
{
    Flash.begin("Schedule", false);
}

void EventScheduler::UpdateFlash()
{
    //Flash.putBytes("SystemSchedule", ScheduleList);
}

void EventScheduler::Schedule(DateTime ScheduledTime, unsigned short extra)
{
    EventTime eventTime;

    eventTime.Hours = ScheduledTime.tm_hour;
    eventTime.Minutes = ScheduledTime.tm_min;

    EventData eventData(eventTime, extra);
    ScheduleList.push_back(eventData);
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
