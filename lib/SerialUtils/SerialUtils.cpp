#include <SerialUtils.h>

void ActUponData(std::vector<String> data)
{
    if (data.size() > 2 && (data[0] == "ADD" && data[1] == "SCHEDULE"))
    {
        Serial.print("Adding to schedule: ");
        Serial.println(data[2]);
    }

    if (data.size() >= 1)
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

void PushFragment(std::vector<String> &data, String &dataFragment)
{
    dataFragment.trim();

    if (!dataFragment.isEmpty())
    {
        data.push_back(dataFragment);
        dataFragment = "";
    }
}

bool ProcessSerialFragment(std::vector<String> &data, String &dataFragment, TimerActions &timeOut)
{
    char c = Serial.read();

    if (c == '\n')
    {
        PushFragment(data, dataFragment);
        return true;
    }

    if (dataFragment == "__EOT__")
    {
        return true;
    }

    if (c == ' ' && !dataFragment.isEmpty())
    {
        PushFragment(data, dataFragment);
    }
    else if (c != ' ')
    {
        dataFragment += c;
    }

    timeOut.ResetTimer();
    return false;
}

std::vector<String> ReadSerialData(unsigned int timeOutValue)
{
    std::vector<String> data;
    String dataFragment = "";
    TimerActions timeOut;

    while (true)
    {
        if (timeOut.IsTimerUp(timeOutValue) || (Serial.available() && ProcessSerialFragment(data, dataFragment, timeOut)))
        {
            break;
        }
    }

    return data;
}

// Testes
void CheckSerialData()
{
    if (Serial.available())
    {
        ActUponData(ReadSerialData());
    }
}
