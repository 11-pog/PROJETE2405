// Inclue todas constantes, 'defines' e todas as
#include <ProjectSettings.h>

/**/

void setup()
{
  Serial.begin(115200);
  Events.begin();

  pinMode(MOTOR_PIN, OUTPUT);

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

  Serial.print("Distancia: ");
  Serial.println(USSensor.GetDistance());

  Serial.print("Temperatura: ");
  Serial.println(DHTSensor.GetTemperature());
  
  Serial.print('\n');
}