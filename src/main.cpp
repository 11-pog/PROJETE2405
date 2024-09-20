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

  Serial.print("Conectando");

  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print('.');
    delay(1000);
  }

  WiFi.onEvent(WifiStuff);

  Serial.println("");
  Serial.println("Conectado!");
  Serial.println("IP: ");
  Serial.println(WiFi.localIP());

  client.setServer(MQTTURL, 1883);
  client.setCallback(MQTT_Callback);

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

String buildQuery(float value)
{
  DateTime now = ESPClock.GetCurrentTime();

  float Value = std::min((float)100, value);

  String year(now.tm_year + 1900);
  String month(now.tm_mon + 1);
  String day(now.tm_mday);

  String date = year + ' ' + month + ' ' + day;

  String hour(now.tm_hour);
  String min(now.tm_min);
  String sec(now.tm_sec);

  String time = hour + ' ' + min + ' ' + sec;

  String query = String(Value) + " / " + date + ' ' + time;
  return query;
}

PeriodicExecutor UpdateSiteLevel([]()
                                  { client.publish("DHT_DATA:LEVEL", buildQuery(USSensor.GetDistance()).c_str()); });

PeriodicExecutor UpdateSiteHumidity([]()
                                    { client.publish("DHT_DATA:HUMI", buildQuery(DHTSensor.GetHumidity()).c_str()); });

SimultaneousExecutor SerialChecker([]()
                                  {
  SerialH.CheckSerialData();

  Events.Evaluate(ESPClock.GetCurrentTime(), Place);

  client.loop();

  UpdateSiteHumidity.RunEvery(5000);
  UpdateSiteLevel.RunEvery(1000);

  delay(1); });

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

void loop()
{
  if (!client.connected())
  {
    ConnectMQTT();
  }

  SerialChecker.ExecuteWhileWaiting(1000);

  TestPrint();
}