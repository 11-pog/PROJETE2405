#ifndef TimerActions_H
#define TimerActions_H

#include <Arduino.h>

class TimerActions
{
public:
    TimerActions(std::function<void()> executableTask = std::function<void()>());
    void ExecuteWhileWaiting(unsigned long timeInMillis);
    void SwitchTask(std::function<void()> executableTask);
    bool IsTimerUp(unsigned long timeInMillis);
    void ResetTimer();

private:
    void SetLastTimeIfZero();
    std::function<void()> executableTask;
    unsigned long lastTime;
};

#endif
