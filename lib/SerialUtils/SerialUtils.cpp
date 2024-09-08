#include <SerialUtils.h>

EventScheduler Events;

void AddScheduleIfValid(String data)
{
    int colonIndex = data.indexOf(':');
    if (colonIndex != -1)
    {
        String hours = data.substring(0, colonIndex);
        String minutes = data.substring(colonIndex + 1);

        int hourInt = hours.toInt();
        int minuteInt = minutes.toInt();

        bool isHourValid = (hourInt != 0 || hours == "0") && hourInt >= 0 && hourInt < 24;
        bool isMinuteValid = (minuteInt != 0 || minutes == "0") && minuteInt >= 0 && minuteInt < 60;

        if (isHourValid && isMinuteValid)
        {
            EventTime newEvent(hourInt, minuteInt);
            Events.Schedule(newEvent);
            return;
        }
    }

    Serial.println("Time is not valid");
}

void HandleCommand(List<String> data)
{
    if (data.size() == 3)
    {
        if (data[0] == "ADD" && data[1] == "SCHEDULE")
        {
            Serial.print("Adding to schedule: ");
            Serial.println(data[2]);

            AddScheduleIfValid(data[2]);
        }

        if (data[0] == "PRINT" && data[1] == "SCHEDULE" && data[2] == "ALL")
        {
            Serial.println("ok");
            Events.PrintScheduleList();
        }
    }

    if (data.size() == 2)
    {
        if (data[0] == "TEST" && data[1] == "PACKER")
        {   
            Serial.println("Testing.");
            Events.TestPacker();
        }

        if (data[0] == "CLEAR" && data[1] == "FLASH")
        {
            Serial.println("Cleaning all data in flash.");
            Events.ResetFlash();
        }

        if (data[0] == "RESTART" && data[1] == "ESP")
        {
            Serial.println("Esp should restart ");
            delay(1000);
            Serial.println("NOW");
            delay(500);
            ESP.restart();
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

void PushFragment(List<String> &data, String &dataFragment)
{
    dataFragment.trim();

    if (!dataFragment.isEmpty())
    {
        data.push_back(dataFragment);
        dataFragment = "";
    }
}

bool ProcessFragment(List<String> &data, String &dataFragment, TimerActions &timeOut, bool &spaceIgnoreMode)
{
    char c = Serial.read();
    timeOut.ResetTimer();

    if (c == '\n')
    {
        PushFragment(data, dataFragment);
        return true;
    }

    if (dataFragment == "__EOT__")
    {
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
        PushFragment(data, dataFragment);
    }
    else if (c != ' ' || spaceIgnoreMode)
    {
        dataFragment += c;
    }

    return false;
}

List<String> ReadSerialData(unsigned int timeOutValue)
{
    List<String> data;
    String dataFragment = "";
    TimerActions timeOut;

    bool spaceIgnoreMode;

    while (!(timeOut.IsTimerUp(timeOutValue) || (Serial.available() && ProcessFragment(data, dataFragment, timeOut, spaceIgnoreMode))))
    {
        delay(1);
    }

    return data;
}

// Testes
void CheckSerialData()
{
    if (Serial.available())
    {
        List<String> Commands = ReadSerialData();
        HandleCommand(Commands);

        for (String Command : Commands)
        {
            Serial.println(Command);
        }
    }
}
