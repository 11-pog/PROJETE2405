#ifndef SerialUtils_H
#define SerialUtils_H

#include <TimerActions.h>
#include <CppTypeDefs.h>

#include <EventScheduler.h>

#define DEF_SERIAL_TIMEOUT 1250U
#define MOTOR_PIN 22U

class SerialHandler
{
public:
    SerialHandler(EventScheduler *events) : Events(events) {}
    List<String> ReadSerialData(unsigned int timeOut = DEF_SERIAL_TIMEOUT);
    void CheckSerialData(Action<void(List<String>)> Action);
    void CheckSerialData();

private:
    void AddScheduleIfValid(String data, unsigned short extra);
    void HandleCommand(List<String> data);
    void PushFragment();
    bool ProcessFragment(List<String> &data, String &dataFragment);

    TimerActions timeOut;
    bool spaceIgnoreMode;
    String dataFragment;
    List<String> data;

    EventScheduler *Events;
};

#endif