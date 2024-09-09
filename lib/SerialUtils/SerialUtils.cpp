#include <SerialUtils.h>

void SerialHandler::AddScheduleIfValid(String data, unsigned short extra)
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
            Events->Schedule(newEvent, extra);
            return;
        }
    }

    Serial.println("Time is not valid");
}


void SerialHandler::HandleCommand(List<String> data)
{
    if (data.size() >= 3)
    {
        if (data[0] == "ADD" && data[1] == "SCHEDULE")
        {
            Serial.print("Adding to schedule: ");
            Serial.println(data[2]);

            unsigned short extra = data.size() == 4 ? data[3].toInt() : 0;

            AddScheduleIfValid(data[2], extra);
        }

        if (data[0] == "PRINT" && data[1] == "SCHEDULE" && data[2] == "ALL")
        {
            Events->PrintScheduleList();
        }
    }

    if (data.size() == 2)
    {
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

    if (c == '\n')
    {
        PushFragment();
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
        Serial.print("testse");
        Action(ReadSerialData());
    }
}

void SerialHandler::CheckSerialData()
{
    if (Serial.available())
    {
        Serial.print("testse");
        HandleCommand(ReadSerialData());
    }
}
