#ifndef DelayHandler_H
#define DelayHandler_H

#include <Arduino.h>

class DelayHandler
{
public:
    DelayHandler(std::function<void()> executableTask);
    void DelayWhileExecuting(unsigned long timeInMillis);
    void SwitchTask(std::function<void()> executableTask);

private:
    std::function<void()> executableTask;
    bool HasDelayElapsed(unsigned long timeInMillis);
    void UpdateLastTime();
    unsigned long lastTime;
};

#endif
