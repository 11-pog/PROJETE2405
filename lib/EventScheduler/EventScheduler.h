#ifndef EventScheduler_H
#define EventScheduler_H

#include <Arduino.h>
#include <time.h>
#include <CppTypeDefs.h>
#include <Preferences.h>
#include <msgpack.h>
#include <CRC32.h>
#include <algorithm>

struct EventTime
{
    byte Hours;
    byte Minutes;

    EventTime(byte hours = 12, byte minutes = 0) : Hours(hours), Minutes(minutes) {}

    bool operator==(const EventTime &other) const
    {
        return Hours == other.Hours && Minutes == other.Minutes;
    }

    bool operator<=(const EventTime &other) const
    {
        return Hours <= other.Hours && Minutes <= other.Minutes;
    }

    bool operator>=(const EventTime &other) const
    {
        return Hours >= other.Hours && Minutes >= other.Minutes;
    }

    MSGPACK_DEFINE(Hours, Minutes);
};

class EventScheduler
{
private: // Structs
    struct EventData
    {
        EventTime Event;
        unsigned short Extra;

        EventData(EventTime event = EventTime(), unsigned short extra = 0) : Event(event), Extra(extra) {}

        bool operator==(const EventData &other) const
        {
            return Event == other.Event && Extra == other.Extra;
        }

        bool operator!=(const EventData &other) const
        {
            return !(Event == other.Event && Extra == other.Extra);
        }

        bool operator<=(const EventData &other) const
        {
            return Event <= other.Event;
        }

        bool operator>(const EventData &other) const
        {
            return !(Event <= other.Event);
        }

        bool operator>=(const EventData &other) const
        {
            return Event >= other.Event;
        }

        bool operator<(const EventData &other) const
        {
            return !(Event >= other.Event);
        }

        MSGPACK_DEFINE(Event, Extra);
    };

public:
    void begin();

    void Evaluate(DateTime now, std::function<void(unsigned short)> Action);
    void Schedule(EventTime ScheduledTime, unsigned short extra = 0);

    void TestPacker();
    void ResetFlash();
    void PrintScheduleList();

    // Yet-To-Define Functions
    void UnSchedule();
    void ReSchedule();

    using EventList = List<EventData>;

private:
    std::pair<bool, unsigned short> IsEventDue(DateTime now);
    bool done;
    void SaveToFlash();
    EventData GetNextScheduledEvent(DateTime now);
    Preferences Flash;
    EventList GetDataFromFlash();
    EventList ScheduleList;
};

using EventList = typename EventScheduler::EventList;

#endif