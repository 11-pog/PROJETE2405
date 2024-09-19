#include <MQTT_Utils.h>

void MQTT_Act(byte *Data, unsigned int size)
{
    String DataSTR = String((char *)Data, size);
    if (DataSTR == "ON")
    {
        Serial.println("MotorON");
        digitalWrite(MOTOR_PIN, 1);
    }
    else if (DataSTR == "OFF")
    {
        Serial.println("MotorOFF");
        digitalWrite(MOTOR_PIN, 0);
    }
}