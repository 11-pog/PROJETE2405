#ifndef TimerActions_H
#define TimerActions_H

#include <Arduino.h>
#include <CppTypeDefs.h>

class TimerActions
{
public:
    TimerActions(Action<void()> executableTask = std::function<void()>());
    void ExecuteWhileWaiting(unsigned long timeInMillis);
    void SwitchTask(Action<void()> executableTask);
    bool IsTimerUp(unsigned long timeInMillis);
    void RunEvery(unsigned long delay);
    void ResetTimer();

private:
    void SetLastTimeIfZero();
    std::function<void()> executableTask;
    unsigned long lastTime;
};

#endif
