#include <ProjectFunctions.h>

List<String> LastLevelUpdates;
List<String> LastHumidityUpdates;

String BuildAllData(List<String> data)
{
    String final = "";

    for (String frag : data)
    {
        final += (" " + frag);
    }

    return final;
}

void SendLevelToHost()
{
    String query = "ACK_LVL" + BuildAllData(LastLevelUpdates);
    client.publish("ESP_DATA", query.c_str());
}

void SendHumiToHost()
{
    String query = "ACK_HUMI" + BuildAllData(LastHumidityUpdates);
    client.publish("ESP_DATA", query.c_str());
}

void MQTT_Act(byte *Data, unsigned int size)
{
    String DataSTR = String((char *)Data, size);
    if (DataSTR == "ON")
    {
        Serial.println("MotorON");
        digitalWrite(MOTOR_PIN, 1);
    }
    else if (DataSTR == "OFF")
    {
        Serial.println("MotorOFF");
        digitalWrite(MOTOR_PIN, 0);
    }
    else if (DataSTR == "GETGRAPHINFO")
    {
        SendLevelToHost();
        SendHumiToHost();
    }
}

void MQTT_Callback(char *Topic, byte *payload, unsigned int loadSize)
{
    string topic(Topic);
    if (topic != "ESP_COMMAND")
    {
        Serial.print("Recieved: ");
        Serial.print(Topic);
        Serial.print(" => ");
        String load = String((char *)payload, loadSize);
        Serial.println(load);
    }
    else
    {
        MQTT_Act(payload, loadSize);
    }
}

// "Inspirado" no do Nicholas
void ConnectMQTT()
{
    byte i = 0;
    while (!client.connected())
    {

        if (!WiFi.isConnected())
        {
            Serial.println("Wifi Not Connected!");
            ESP.restart();
        }

        Serial.println("Connecting MQTT...");
        client.connect(ClientID);

        client.subscribe("Testes");
        client.subscribe("ESP_COMMAND");

        client.publish("RXTeste", "CONNECTADO");

        if (i > 0)
        {
            ESP.restart();
        }

        i++;
    }
    Serial.println("Connected");
}

void Place(unsigned short Holder)
{
    Serial.println("In fact, this event has been triggered: ");
    Serial.println(Holder);

    unsigned short timeToHoldON = Holder ? Holder : 5000;

    Serial.println("Motor ON");
    digitalWrite(MOTOR_PIN, 1);

    delay(timeToHoldON);

    Serial.println("Motor OFF");
    digitalWrite(MOTOR_PIN, 0);
}

String buildQuery(float value)
{
    DateTime now = ESPClock.GetCurrentTime();

    float Value = std::min((float)100, value);

    String hour(now.tm_hour);
    String min(now.tm_min);
    String sec(now.tm_sec);

    String query = String(Value) + "/" + hour + ':' + min + ':' + sec;
    return query;
}

void AddTo(List<String> &data, String payload)
{
    data.push_back(payload);

    if (data.size() > 10)
    {
        data.erase(data.begin());
    }
}

void updtSiteLvl()
{
    unsigned short distance = USSensor.GetDistance();
    byte value = 100*((MAX_FOOD_LVL - distance) / MAX_FOOD_LVL);

    String query = buildQuery(value);
    client.publish("DHT_DATA:LEVEL", query.c_str());
    AddTo(LastLevelUpdates, query);
}
PeriodicExecutor UpdateSiteLevel(updtSiteLvl);

void updtSiteHumidity()
{
    String query = buildQuery(DHTSensor.GetHumidity());
    client.publish("DHT_DATA:HUMI", query.c_str());
    AddTo(LastHumidityUpdates, query);
}
PeriodicExecutor UpdateSiteHumidity(updtSiteHumidity);

// Teste
void Tasks()
{
    SerialH.CheckSerialData();

    Events.Evaluate(ESPClock.GetCurrentTime(), Place);

    client.loop();

    UpdateSiteHumidity.RunEvery(5000);
    UpdateSiteLevel.RunEvery(2500);

    delay(1);
}
SimultaneousExecutor TaskLoop(Tasks);

void TestPrint()
{
    ESPClock.PrintDateTime();

    Serial.print('\n');

    Serial.print("Distancia: ");
    Serial.println(USSensor.GetDistance());

    Serial.print("Humildade: ");
    Serial.println(DHTSensor.GetHumidity());

    Serial.println('\n');
}
