// Referencia ao header desse arquivo
#include <Common.h>

#define SOUND_SPEED 0.034

namespace Time
{
  // Class Clock
  // All here are public
  Clock::Clock(long timezone, byte daysavetime) : timezone(timezone), daysavetime(daysavetime) {}

  void Clock::SyncTime()
  {
    configTime(timezone * 3600, daysavetime * 3600, "time.nist.gov", "0.pool.ntp.org", "1.pool.ntp.org");
  }

  void Clock::UpdateTime()
  {
    getLocalTime(&localTime);
  }

  void Clock::PrintDateTime()
  {
    UpdateTime();
    String date = String((localTime.tm_mon) + 1) + "/" + String(localTime.tm_mday);
    String time = (String(localTime.tm_hour) + ":" + String(localTime.tm_min) + ":" + String(localTime.tm_sec));

    Serial.print(date);
    Serial.print("  ");
    Serial.println(time);
  }

  DateTime Clock::GetCurrentTime()
  {
    UpdateTime();
    return localTime;
  }
}

namespace Sensor
{
  DHT_Sensor::DHT_Sensor(int pin, byte type) : dht(pin, type)
  {
    dht.begin();
  }

  float DHT_Sensor::GetHumidity()
  {
    float humidityValue = dht.readHumidity();

    return humidityValue;
  }

  float DHT_Sensor::GetTemperature()
  {
    float Temp = dht.readTemperature();

    return Temp;
  }

  Ultrasonic_Sensor::Ultrasonic_Sensor(byte trig, byte echo)
  {
    this->trigPin = trig;
    this->echoPin = echo;

    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
  }

  float Ultrasonic_Sensor::GetDistance()
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

  
}
/*
  struct Projete
  {
    static const byte day = 7;
    static const byte month = 10;
  };

  void ExplodeOnProjete(tm info)
  {
    if (info.tm_mday == Projete::day && info.tm_mon + 1 == Projete::month)
    {
      ESP.SelfDestruct();
    }
  }
  */
  // Piadas

  
void WifiStuff(WiFiEvent_t event)
{
  log_i("[WiFi-event] event: %d\n", event);
  switch (event)
  {
  case SYSTEM_EVENT_WIFI_READY:
    log_i("WiFi interface ready");
    break;
  case SYSTEM_EVENT_SCAN_DONE:
    log_i("Completed scan for access points");
    break;
  case SYSTEM_EVENT_STA_START:
    log_i("WiFi client started");
    break;
  case SYSTEM_EVENT_STA_STOP:
    log_i("WiFi clients stopped");
    break;
  case SYSTEM_EVENT_STA_CONNECTED:
    log_i("Connected to access point");
    break;
  case SYSTEM_EVENT_STA_DISCONNECTED:
    log_i("Disconnected from WiFi access point");
    break;
  case SYSTEM_EVENT_STA_AUTHMODE_CHANGE:
    log_i("Authentication mode of access point has changed");
    break;
  case SYSTEM_EVENT_STA_GOT_IP:
    log_i("Obtained IP address: %s", WiFi.localIP());
    break;
  case SYSTEM_EVENT_STA_LOST_IP:
    log_i("Lost IP address and IP address is reset to 0");
    //      vTaskDelay( 5000 );
    //      ESP.restart();
    break;
  case SYSTEM_EVENT_STA_WPS_ER_SUCCESS:
    log_i("WiFi Protected Setup (WPS): succeeded in enrollee mode");
    break;
  case SYSTEM_EVENT_STA_WPS_ER_FAILED:
    log_i("WiFi Protected Setup (WPS): failed in enrollee mode");
    //      ESP.restart();
    break;
  case SYSTEM_EVENT_STA_WPS_ER_TIMEOUT:
    log_i("WiFi Protected Setup (WPS): timeout in enrollee mode");
    break;
  case SYSTEM_EVENT_STA_WPS_ER_PIN:
    log_i("WiFi Protected Setup (WPS): pin code in enrollee mode");
    break;
  case SYSTEM_EVENT_AP_START:
    log_i("WiFi access point started");
    break;
  case SYSTEM_EVENT_AP_STOP:
    log_i("WiFi access point  stopped");
    //      WiFi.mode( WIFI_OFF);
    //      esp_sleep_enable_timer_wakeup( 1000000 * 2 ); // 1 second times how many seconds wanted
    //      esp_deep_sleep_start();
    break;
  case SYSTEM_EVENT_AP_STACONNECTED:
    log_i("Client connected");
    break;
  case SYSTEM_EVENT_AP_STADISCONNECTED:
    log_i("WiFi client disconnected");
    break;
  case SYSTEM_EVENT_AP_STAIPASSIGNED:
    log_i("Assigned IP address to client");
    break;
  case SYSTEM_EVENT_AP_PROBEREQRECVED:
    log_i("Received probe request");
    break;
  case SYSTEM_EVENT_GOT_IP6:
    log_i("IPv6 is preferred");
    break;
  case SYSTEM_EVENT_ETH_GOT_IP:
    log_i("Obtained IP address");
    break;
  default:
    break;
  }
}
