#include <ProjectFunctions.h>

/**/

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

void loop()
{
  if (!client.connected())
  {
    ConnectMQTT();
  }

  SerialChecker.ExecuteWhileWaiting(1000);

  TestPrint();
}