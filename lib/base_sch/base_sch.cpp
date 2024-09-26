#include <base_sch.h>

template <typename T>
void base_sch<T>::begin()
{
    Serial.println("This is, indeed, initializing.");

    Flash.begin("Schedule"); // This is preferences.h btw
    this->ScheduleList = GetDataFromFlash();
    PrintScheduleList();
}

template <typename T>
void base_sch<T>::TestPacker()
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

template <typename T>
void base_sch<T>::SaveToFlash()
{
    SortEvents();

    MsgPack::Packer packer;
    packer.serialize(ScheduleList);

    unsigned int checksum = CRC32::calculate(packer.data(), packer.size());

    Flash.putBytes("schedule", packer.data(), packer.size());
    Flash.putInt("checksum", checksum);
}

template <typename T>
base_sch<T>::EventList base_sch<T>::GetDataFromFlash()
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

template <typename T>
short base_sch<T>::CheckForExistingID(unsigned int itemId)
{
    for (short i = 0; i < ScheduleList.size(); i++)
    {
        if (ScheduleList[i].ID == itemId)
        {
            return i;
        }
    }

    return -1;
}

template <typename T>
void base_sch<T>::PrintScheduleList()
{
    SortEvents();

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

        Serial.print("Unique ID: ");
        Serial.println(Data.ID);

        Serial.println("\n\n");
    }
}

template <typename T>
void base_sch<T>::ResetFlash()
{
    Flash.clear();
    ScheduleList = EventList();
}

template <typename T>
void base_sch<T>::Schedule(EventTime ToSchedule, unsigned short extra)
{
    EventData eventData(ToSchedule, extra);

    if (CheckForExistingID(eventData.ID) != -1)
    {
        Serial.println("Event Already Scheduled");
        return;
    }

    ScheduleList.push_back(eventData);
    SortEvents();
    SaveToFlash();
}

template <typename T>
void base_sch<T>::Schedule(unsigned int ID)
{
    if (CheckForExistingID(ID) != -1)
    {
        Serial.println("Event Already Scheduled");
        return;
    }

    ScheduleList.push_back(EventData(ID));
    SortEvents();
    SaveToFlash();
}

template <typename T>
void base_sch<T>::UnSchedule(unsigned int ID)
{
    short index = CheckForExistingID(ID);

    if (index == -1)
    {
        Serial.println("ERROR: Event not Scheduled");
        return;
    }

    ScheduleList.erase(ScheduleList.begin() + index);
    SaveToFlash();
}

template <typename T>
unsigned int base_sch<T>::ReSchedule(unsigned int ID, EventTime newTime)
{
    short index = CheckForExistingID(ID);

    if (index == -1)
    {
        Serial.println("ERROR: Event not Scheduled");
        return 0;
    }

    ScheduleList[index].Event = newTime;
    ScheduleList[index].EncodeID();

    SaveToFlash();

    return ScheduleList[index].ID;
}

template <typename T>
bool base_sch<T>::IsEventDue(DateTime now_tm)
{
    EventTime now(now_tm.tm_hour, now_tm.tm_min);

    GetNextScheduledEvent(now);
    EventData eventData = CurrentEvent;
    EventTime eventTime = eventData.Event;

    return now == eventTime;
}

template <typename T>
void base_sch<T>::SortEvents()
{
    std::sort(ScheduleList.begin(), ScheduleList.end());
}

template <typename T>
void base_sch<T>::GetNextScheduledEvent(EventTime now)
{
    SortEvents();

    for (EventData data : ScheduleList)
    {
        if (data.Event >= now)
        {
            CurrentEvent = data;
            return;
        }
    }

    CurrentEvent = ScheduleList[0];
}

template <typename T>
void base_sch<T>::Evaluate(DateTime now, Action<void(unsigned short)> action)
{
    if (!ScheduleList.empty())
    {
        bool isDue = IsEventDue(now);

        if (isDue && LastExecutedEventID != CurrentEvent.ID)
        {
            action(CurrentEvent.Extra);
            LastExecutedEventID = CurrentEvent.ID;
        }
        else if (!isDue && LastExecutedEventID == CurrentEvent.ID)
        {
            LastExecutedEventID = 0; // If the event time has passed, and the events are still equal (meaning there is only one scheduled event), Invalidate the last schedule
        }
    }
}

template <typename T>
base_sch<T>::EventData::EventData(EventTime event, unsigned short extra) : Event(event), Extra(extra)
{
    EncodeID();
}

template <typename T>
base_sch<T>::EventData::EventData(unsigned int ID) : ID(ID)
{
    const byte hours = (ID >> 23) & 0x1F;   // Extrai as horas
    const byte minutes = (ID >> 17) & 0x3F; // Extrai os minutos
    const unsigned short extra = ID & 0x1FFFF;

    this->Event = EventTime(hours, minutes);
    this->Extra = extra;
}

template <typename T>
void base_sch<T>::EventData::EncodeID()
{
    this->ID = (Event.Hours << 23) | (Event.Minutes << 17) | Extra;
}
