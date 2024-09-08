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
}

std::pair<bool, unsigned short> EventScheduler::IsEventDue(DateTime now)
{
    EventData eventData = GetNextScheduledEvent(now);
    EventTime eventTime = eventData.Event;

    bool isHour = eventTime.Hours == now.tm_hour;
    bool isMinute = eventTime.Minutes == now.tm_min;

    return std::make_pair(isHour && isMinute, eventData.Extra);
}

EventScheduler::EventData EventScheduler::GetNextScheduledEvent(DateTime now_tm)
{
    std::sort(ScheduleList.begin(), ScheduleList.end());

    EventTime now(now_tm.tm_hour, now_tm.tm_min);

    for(EventData data : ScheduleList)
    {
        if(data.Event >= now)
        {
            return data;
        }
    }

    return ScheduleList[0];
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
