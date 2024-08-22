#include <Arduino.h>
#include <WiFi.h>

const char *wifiid = "WarningWarningWarning";
const char *wifipass = "porraporra";

WiFiServer server(80);

void setup()
{
  Serial.begin(9600);

  WiFi.begin(wifiid, wifipass);

  Serial.print("Connecting to ");
  Serial.println(wifiid);
  WiFi.begin(wifiid, wifipass);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop()
{
  WiFiClient client = server.available();

  if (client)
  {
  }
}