#ifndef SerialUtils_H
#define SerialUtils_H

#include <vector>
#include <TimerActions.h>

#define DEF_SERIAL_TIMEOUT 1250U
#define MOTOR_PIN 19U

void ActUponData(std::vector<String> Data);
void CheckSerialData();
std::vector<String> ReadSerialData(unsigned int timeOut = DEF_SERIAL_TIMEOUT);

#endif // !SerialUtils_H