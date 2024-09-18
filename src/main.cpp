#include <ProjectSettings.h>

/**/

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
}

void MQTT_Callback(char *Topic, byte *payload, unsigned int loadSize)
{
  if (Topic != "ESP_COMMAND")
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

    if (WiFi.status() == WL_CONNECT_FAILED)
    {
      ESP.restart();
    }
  }

  WiFi.onEvent(WifiStuff);

  Serial.println("");
  Serial.println("Conectado!");

  mqtt_Client.setServer(MQTTURL, MQTTPORT);

  int rdn = random(1, 99999);
  String thing = "ESP_BOARD" + String(rdn);
  Serial.println(thing);
  delay(1000);

  while (!mqtt_Client.connected())
  {
    Serial.println("Connecting MQTT...");
    if(!mqtt_Client.connect(thing.c_str()))
    {
      Serial.println("Failed...");
      delay(4000);
    }
  }

  Serial.println("MQTT Connected");

  mqtt_Client.subscribe("Testes");
  mqtt_Client.subscribe("ESP_COMMAND");

  mqtt_Client.publish("RXTeste", "CONNECTADO");
  mqtt_Client.setCallback(MQTT_Callback);

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
  SerialChecker.ExecuteWhileWaiting(1000);
  ESPClock.PrintDateTime();

  Serial.print('\n');

  Serial.print("Distancia: ");
  Serial.println(USSensor.GetDistance());

  Serial.print("Humildade: ");
  Serial.println(DHTSensor.GetHumidity());

  Serial.println('\n');
}