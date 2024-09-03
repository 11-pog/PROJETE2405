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

void PrintALotOfDots()
{
  Serial.print('.');
  delay(100);
}

Common::Time::DelayHandler PrintStuff(PrintALotOfDots);

void loop()
{
  PrintStuff.DelayInParallel(5000);
  delay(1000);
  Serial.println("TEST");
  delay(1000);
}