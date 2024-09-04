#include <SerialUtils.h>

void ActUponData(std::vector<String> Data)
{
    if (Data.size() > 2 && (Data[0] == "ADD" && Data[1] == "SCHEDULE"))
    {
        Serial.print("Adding to schedule: ");
        Serial.println(Data[2]);
    }

    if (Data.size() >= 2 && Data[0] == "MOTOR")
    {
        if (Data[1] == "ON")
        {
            Serial.println("Motor is now ON");
            digitalWrite(MOTOR_PIN, 1);
        }

        else if (Data[1] == "OFF")
        {
            Serial.println("Motor is now OFF");
            digitalWrite(MOTOR_PIN, 0);
        }
    }
}

std::vector<String> ReadSerialData(unsigned int timeOut)
{
    std::vector<String> Data;
    String DataFragment = "";
    TimerActions TimeOut;

    auto PushFragment = [&Data, &DataFragment]()
    {
        Data.push_back(DataFragment);
        DataFragment = "";
    };

    while (true)
    {
        char c;

        if (Serial.available())
        {
            c = Serial.read();

            if (c == '\n' || c == '\u0004')
            {
                PushFragment();
                break;
            }

            if (c == ' ' && !DataFragment.isEmpty())
            {
                PushFragment();
            }
            else if (c != ' ')
            {
                DataFragment += c;
            }

            TimeOut.ResetTimer();
        }

        if (TimeOut.IsTimerUp(timeOut))
        {
            break;
        }
    }

    return Data;
}

// Testes
void CheckSerialData()
{
    std::vector<String> DataList;

    if (Serial.available())
    {
        DataList = ReadSerialData();
        int listSize = DataList.size();

        ActUponData(DataList);

        if (listSize > 0)
        {
            for (int i = 0; i < listSize; i++)
            {
                Serial.println(DataList[i]);
            }
            Serial.println(listSize);
        }
    }
}
