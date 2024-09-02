// Inclue todas constantes, 'defines' e todas as
#include <ProjectSettings.h>

/**/

void setup()
{
  Serial.begin(9600);

  Flash::Schedule.begin("Schedule", false);

  WiFi.begin(wifiId, wifiPassword);

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

  ESPClock.SyncTime();
}

void loop()
{
  ESPClock.PrintDateTime();
  delay(1000);
}