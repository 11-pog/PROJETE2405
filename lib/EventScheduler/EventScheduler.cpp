#include <EventScheduler.h>

void EventScheduler::begin()
{
    Serial.println("This is, indeed, initializing.");

    Flash.begin("Schedule"); // This is preferences.h btw
    this->ScheduleList = GetDataFromFlash();
    PrintScheduleList();
}

void EventScheduler::TestPacker()
{
    MsgPack::Packer packer;
    packer.serialize(ScheduleList);

    unsigned int firstChecksum = CRC32::calculate(packer.data(), packer.size());

    MsgPack::Unpacker unpacker;
    unpacker.feed(packer.data(), packer.size());

    EventList dd;

    unpacker.deserialize(dd);

    MsgPack::Packer packer2;
    packer2.serialize(dd);

    unsigned int secondChecksum = CRC32::calculate(packer2.data(), packer2.size());

    if (firstChecksum == secondChecksum)
    {
        Serial.println("Checksum Matches: ");
        Serial.println(firstChecksum);
    }

    if (ScheduleList == dd)
    {
        Serial.println("Data Matches");
        return;
    }

    Serial.println("Failed Serialization");
    return;
}

void EventScheduler::SaveToFlash()
{
    MsgPack::Packer packer;
    packer.serialize(ScheduleList);

    unsigned int checksum = CRC32::calculate(packer.data(), packer.size());

    Flash.putBytes("schedule", packer.data(), packer.size());
    Flash.putInt("checksum", checksum);
}

EventList EventScheduler::GetDataFromFlash()
{
    if (!Flash.isKey("schedule"))
    {
        Serial.println("No key 'schedule' found.");
        return EventList();
    }

    unsigned long size = Flash.getBytesLength("schedule");

    byte *data = new byte[size];
    Flash.getBytes("schedule", data, size);

    unsigned int checksum = CRC32::calculate(data, size);
    unsigned int storedChecksum = Flash.getInt("checksum");

    MsgPack::Unpacker unpacker;
    EventList eventList;

    unpacker.feed(data, size);
    unpacker.deserialize(eventList);

    delete[] data;
    if (checksum == storedChecksum)
    {
        return eventList;
    }

    Serial.println("ERROR: Possible data Corruption");
    Flash.clear();
    return EventList();
}

void EventScheduler::PrintScheduleList()
{
    Serial.println("Printing Current Itens:");
    Serial.print("\n");
    for (EventData Data : ScheduleList)
    {
        Serial.print("Hours: ");
        Serial.println(Data.Event.Hours);

        Serial.print("Minutes: ");
        Serial.println(Data.Event.Minutes);

        Serial.print("Extra: ");
        Serial.println(Data.Extra);

        Serial.println("\n\n");
    }
}

void EventScheduler::ResetFlash()
{
    Flash.clear();
    ScheduleList = EventList();
}

void EventScheduler::Schedule(EventTime ToSchedule, unsigned short extra)
{
    EventData eventData(ToSchedule, extra);
    ScheduleList.push_back(eventData);

    SaveToFlash();

    Done = true;
}

std::pair<bool, unsigned short> EventScheduler::IsEventDue(DateTime now_tm)
{
    EventTime now(now_tm.tm_hour, now_tm.tm_min);

    GetNextScheduledEvent(now);
    EventData eventData = NextEvent;
    EventTime eventTime = eventData.Event;

    return std::make_pair(now == eventTime, eventData.Extra);
}

void EventScheduler::GetNextScheduledEvent(EventTime now)
{
    if (Done)
    {
        std::sort(ScheduleList.begin(), ScheduleList.end());

        for (EventData data : ScheduleList)
        {
            if (data.Event > now)
            {
                NextEvent = data;
                Done = false;
                return;
            }
        }

        NextEvent = ScheduleList[0];
    }
}

void EventScheduler::Evaluate(DateTime now, std::function<void(unsigned short)> action)
{
    std::pair<bool, unsigned short> result = IsEventDue(now);

    if (result.first && !Done)
    {
        action(result.second);
        Done = true;
    }
    else if (!result.first)
    {
        Done = false;
    }
}
