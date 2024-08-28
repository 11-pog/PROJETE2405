#include <Arduino.h>
#include <WiFi.h>
#include <DHT.h>
#include <DHT_U.h>

#define DHT_PIN 12
#define DHT_TYPE DHT22

// define sound speed in cm/uS
#define SOUND_SPEED 0.034

DHT dht(DHT_PIN, DHT_TYPE);

const int trigPin = 5;
const int echoPin = 18;

/**/

const char *wifiid = "WarningWarningWarning";
const char *wifipass = "porraporra";

WiFiServer server(80);

namespace Funcs
{
  static float GetUltraSonic()
  {
    long duration;
    float distanceCm;

    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);

    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);

    digitalWrite(trigPin, LOW);

    duration = pulseIn(echoPin, HIGH);

    distanceCm = duration * SOUND_SPEED / 2;

    return distanceCm;
  }

  static float GetHumidity()
  {
    float humidityValue = dht.readHumidity();

    return humidityValue;
  }

  static float GetTemperature()
  {
    float Temp = dht.readTemperature();

    return Temp;
  }

  /*
  void ExplodeOnProjete(DateTime Day)
  {
    if (Day == DiaOfProjete)
    {
      Self.Explode();
    }
  }
  */
  // joke funcgion ignore pls
};

void setup()
{
  Serial.begin(9600);
  /*
    WiFi.begin(wifiid, wifipass);

    Serial.print("Connecting to ");
    Serial.println(wifiid);
    WiFi.begin(wifiid, wifipass);

    while (WiFi.status() != WL_CONNECTED)
    {
      delay(1000);
      Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected.");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
  */
  dht.begin();

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // server.begin();

  for (byte i = 20; i > 0; i--)
  {
    Serial.println(i);
    delay(500);
  }
}

void loop()
{
  /*
    WiFiClient client = server.available();

    if (client)
    {
      // wip possibly abandoned idea
    }
  */

  Serial.print("Humidity, Temperature and ultrasonic: ");
  Serial.print(Funcs::GetHumidity());
  Serial.print(", ");
  Serial.print(Funcs::GetTemperature());
  Serial.print(", ");
  Serial.println(Funcs::GetUltraSonic());

  delay(200);
}