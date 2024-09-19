#include <ProjectSettings.h>

/**/

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
  while (!mqtt_Client.connected())
  {

    if (!WiFi.isConnected())
    {
      Serial.println("Wifi Not Connected!");
      ESP.restart();
    }

    Serial.println("Connecting MQTT...");
    mqtt_Client.connect(ClientID);

    mqtt_Client.subscribe("Testes");
    mqtt_Client.subscribe("ESP_COMMAND");

    mqtt_Client.publish("RXTeste", "CONNECTADO");

    if (i > 2)
    {
      ESP.restart();
    }

    i++;
  }
  Serial.println("Connected?");
}

void setup()
{
  randomSeed(analogRead(34));

  Serial.begin(9600);

  Events.begin();

  pinMode(MOTOR_PIN, OUTPUT);

  WiFi.begin(wifiId, wifiPassword);
  WiFi.config(staticIP, gateway, subnet, dnsServer);

  Serial.print("Conectando");

  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print('.');
    delay(1000);
  }

  WiFi.onEvent(WifiStuff);

  Serial.println("");
  Serial.println("Conectado!");

  mqtt_Client.setServer(MQTTURL, 1883);
  mqtt_Client.setCallback(MQTT_Callback);

  ConnectMQTT();

  digitalWrite(MOTOR_PIN, 1);
  delay(200);
  digitalWrite(MOTOR_PIN, 0);

  ESPClock.SyncTime();
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

void UpdateSiteData()
{
  mqtt_Client.publish("DHT_DATA:HUMI", String(DHTSensor.GetHumidity()).c_str());
  mqtt_Client.publish("DHT_DATA:TEMP", String(USSensor.GetDistance()).c_str());
}

TimerActions PeriodicRunner();

void AsyncTasks()
{
  SerialH.CheckSerialData();

  Events.Evaluate(ESPClock.GetCurrentTime(), Place);

  mqtt_Client.loop();

  delay(2);
}

TimerActions SerialChecker(AsyncTasks);

void loop()
{
  if (!mqtt_Client.connected())
  {
    ConnectMQTT();
  }

  SerialChecker.ExecuteWhileWaiting(1000);
  ESPClock.PrintDateTime();

  Serial.print('\n');

  Serial.print("Distancia: ");
  Serial.println(USSensor.GetDistance());

  Serial.print("Humildade: ");
  Serial.println(DHTSensor.GetHumidity());

  Serial.println('\n');
}