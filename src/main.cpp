#include <ProjectSettings.h>

/**/

void MQTT_Act(byte *Data)
{
  String DataSTR = String((char *)Data);
  if (DataSTR == "ON")
  {
    digitalWrite(MOTOR_PIN, 1);
  }
  else if(DataSTR == "OFF")
  {
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
    String load = String((char *)payload);
    Serial.println(load);
  }
  else
  {
    MQTT_Act(payload);
  }
}

const IPAddress staticIP(192, 168, 1, 2);
const IPAddress gateway(192, 168, 1, 1);
const IPAddress subnet(255, 255, 255, 0);
const IPAddress dnsServer(8, 8, 8, 8); 

void setup()
{
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

  Serial.println("");
  Serial.println("Conectado!");

  mqtt_Client.setServer(MQTTURL, MQTTPORT);
  mqtt_Client.connect("ESP_BOARD");

  Serial.print("Connecting MQTT");

  while (!mqtt_Client.connected())
  {
    Serial.print('.');
    delay(1000);
  }

  Serial.println("");
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