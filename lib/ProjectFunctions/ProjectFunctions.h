#ifndef ProjectFunctions_H
#define ProjectFunctions_H

#include <ProjectSettings.h>
#include <CppTypeDefs.h>

void MQTT_Act(byte *Data, unsigned int size);
void MQTT_Callback(char *Topic, byte *payload, unsigned int loadSize);
void ConnectMQTT();
void Place(unsigned short Holder);
String buildQuery(float value);
void TestPrint();
extern SimultaneousExecutor TaskLoop;

#endif