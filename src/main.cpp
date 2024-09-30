#include <ProjectFunctions.h>

/**/

void setup()
{
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

  WiFi.onEvent(WifiEventHandler);

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

  client.publish("ESP_DATA", "ESP_STARTUP");
  ESPClock.SyncTime();
}


void loop()
{
  if (!client.connected())
  {
    ConnectMQTT();
  }

  TaskLoop.ExecuteWhileWaiting(1000);
  ReSyncTime.RunEvery(600000);

  TestPrint();
}