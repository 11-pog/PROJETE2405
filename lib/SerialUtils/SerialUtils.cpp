#include <SerialUtils.h>

EventTime SerialHandler::ValidadeHourCode(String data)
{
    int colonIndex = data.indexOf(':');
    if (colonIndex != -1)
    {
        String hours = data.substring(0, colonIndex);
        String minutes = data.substring(colonIndex + 1);

        int hourInt = hours.toInt();
        int minuteInt = minutes.toInt();

        bool isHourValid = (hourInt != 0 || hours == "0" || hours == "00") && hourInt >= 0 && hourInt < 24;
        bool isMinuteValid = (minuteInt != 0 || minutes == "0" || minutes == "00") && minuteInt >= 0 && minuteInt < 60;

        if (isHourValid && isMinuteValid)
        {
            EventTime newEvent(hourInt, minuteInt);
            return newEvent;
        }
    }

    return EventTime(255);
}

void SerialHandler::AddScheduleIfValid(String data, unsigned short extra)
{
    EventTime validatedTime = ValidadeHourCode(data);

    if (validatedTime.Hours != 255) // 255, AKA byte equivalent of -1, literally the best representation of "this code isnt valid" i could find
    {
        Events->Schedule(validatedTime, extra);
        return;
    }

    Serial.println("Time is not valid");
}

void SerialHandler::ReScheduleIfValid(String data, unsigned int ID)
{
    EventTime validatedTime = ValidadeHourCode(data);

    if (validatedTime.Hours != 255 && ID != 0) // 255, AKA byte equivalent of -1, literally the best representation of "this code isnt valid" i could find
    {
        Serial.print("Rescheduling: ");
        Serial.print(ID);
        Serial.print(" to ");
        Serial.println(data);
        Serial.print('\n');

        Serial.print("New ID: ");
        Serial.println(Events->ReSchedule(ID, validatedTime));

        return;
    }

    Serial.println("Time or ID not valid");
}

void SerialHandler::UnScheduleIfValid(unsigned int ID)
{
    if (ID == 0)
    {
        Serial.println("Please provide a valid ID");
        return;
    }

    Serial.print("Unscheduling: ");
    Serial.println(ID);

    Events->UnSchedule(ID);
}

void SerialHandler::HandleCommand(List<String> data)
{
    if (data.size() >= 3)
    {
        if (data[0] == "PRINT" && data[1] == "SCHEDULE" && data[2] == "ALL")
        {
            Events->PrintScheduleList();
        }

        if (data[0] == "RESCHEDULE")
        {
            ReScheduleIfValid(data[2], data[1].toInt());
        }
    }

    if (data.size() >= 2)
    {
        if (data[0] == "SCHEDULE")
        {
            Serial.print("Adding to schedule: ");
            Serial.println(data[1]);

            unsigned short extra = data.size() == 3 ? data[2].toInt() : 0;

            AddScheduleIfValid(data[1], extra);
        }

        if (data[0] == "TEST" && data[1] == "PACKER")
        {
            Serial.println("Testing...");
            Events->TestPacker();
        }

        if (data[0] == "CLEAR" && data[1] == "FLASH")
        {
            Serial.println("Cleaning all data in flash.");
            Events->ResetFlash();
        }

        if (data[0] == "ESP" && data[1] == "RESTART")
        {
            Serial.println("Esp should restart ");
            delay(1000);
            Serial.println("NOW");
            delay(500);
            ESP.restart();
        }

        if (data[0] == "UNSCHEDULE")
        {
            UnScheduleIfValid(data[1].toInt());
        }
    }

    if (data.size() == 1)
    {
        if (data[0] == "ON")
        {
            Serial.println("Motor is now ON");
            digitalWrite(MOTOR_PIN, 1);
        }

        else if (data[0] == "OFF")
        {
            Serial.println("Motor is now OFF");
            digitalWrite(MOTOR_PIN, 0);
        }
    }
}

void SerialHandler::PushFragment()
{
    dataFragment.trim();

    if (!dataFragment.isEmpty())
    {
        data.push_back(dataFragment);
        dataFragment = "";
    }
}

bool SerialHandler::ProcessFragment(List<String> &data, String &dataFragment)
{
    char c = Serial.read();
    timeOut.ResetTimer();

    if (c == '\n' || c == ';')
    {
        PushFragment();
        return true;
    }

    if ((dataFragment.isEmpty() && c == '<') || c == '>')
    {
        if (c == '<')
        {
            spaceIgnoreMode = true;
        }
        else
        {
            spaceIgnoreMode = false;
        }

        return false;
    }

    if (c == ' ' && !dataFragment.isEmpty() && !spaceIgnoreMode)
    {
        PushFragment();
    }
    else if (c != ' ' || spaceIgnoreMode)
    {
        dataFragment += c;
    }

    return false;
}

List<String> SerialHandler::ReadSerialData(unsigned int timeOutValue)
{
    data = List<String>();
    dataFragment = "";
    spaceIgnoreMode = false;

    while (!(timeOut.IsTimerUp(timeOutValue) || (Serial.available() && ProcessFragment(data, dataFragment))))
    {
        delay(1);
    }

    return data;
}

// Testes
void SerialHandler::CheckSerialData(Action<void(List<String>)> Action)
{
    if (Serial.available())
    {
        Action(ReadSerialData());
    }
}

void SerialHandler::CheckSerialData()
{
    if (Serial.available())
    {
        HandleCommand(ReadSerialData());
    }
}
