// Inclue todas constantes, 'defines' e todas as
#include <ProjectSettings.h>

/**/

void setup()
{
  Serial.begin(9600);

  pinMode(MOTOR_PIN, OUTPUT);

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
  Serial.println("Conectado!");

  ESPClock.SyncTime();
}

TimerActions SerialChecker(SerialUtils::CheckSerialData);

void loop()
{
  SerialChecker.WaitWhileExecuting(5000);
  Serial.println(".");
}