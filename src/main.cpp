// Inclue todas constantes, 'defines' e todas as
#include <ProjectSettings.h>

/**/

void setup()
{
  Serial.begin(9600);
  Serial.setTimeout(500);

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

void ActUponData(std::vector<String> Data)
{
  if (Data[0] == "ADD" && Data[1] == "SCHEDULE")
  {
    Serial.print("Adding to schedule: ");
    Serial.println(Data[2]);
  }
}

//Testes
void CheckSerialData()
{
  std::vector<String> DataList;

  while (Serial.available() > 0)
  {
    String c = Serial.readStringUntil(' ');
    c.trim();
    if (c != "")
    {
      DataList.push_back(c);
    }
  }

  int listSize = DataList.size();

  if (listSize > 2)
  {
    ActUponData(DataList);
  }

  if (listSize > 0)
  {
    for (int i = 0; i < listSize; i++)
    {
      Serial.println(DataList[i]);
    }
    Serial.println(listSize);
  }

  delay(20);
}

DelayHandler SerialChecker(CheckSerialData);

void loop()
{
  SerialChecker.DelayWhileExecuting(2500);
  Serial.println("Testes");
}