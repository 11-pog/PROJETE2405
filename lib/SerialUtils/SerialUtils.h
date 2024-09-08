#ifndef SerialUtils_H
#define SerialUtils_H

#include <TimerActions.h>
#include <CppTypeDefs.h>

#include <EventScheduler.h>

#define DEF_SERIAL_TIMEOUT 1250U
#define MOTOR_PIN 22U

extern EventScheduler Events;

List<String> ReadSerialData(unsigned int timeOut = DEF_SERIAL_TIMEOUT);
void CheckSerialData();

#endif // !SerialUtils_H