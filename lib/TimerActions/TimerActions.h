#ifndef TimerActions_H
#define TimerActions_H

#include <Arduino.h>
#include <CppTypeDefs.h>

class TimerActions
{
public:
    TimerActions(std::function<void()> executableTask = Action<void()>()) : executableTask(executableTask) {}
    bool IsTimerUp(unsigned long timeInMillis);

    void ResetTimer();
    void SwitchTask(Action<void()> executableTask);

protected:
    std::function<void()> executableTask;

private:
    void SetLastTimeIfZero();
    unsigned long lastTime;
};

// Classe bem legal q roda simultaneamente
class SimultaneousExecutor : public TimerActions
{
public:
    SimultaneousExecutor(Action<void()> executableTask) : TimerActions(executableTask) {}

    void ExecuteWhileWaiting(unsigned long timeInMillis);
};

// Classe bem legal q roda de tempo em tempo
class PeriodicExecutor : public TimerActions
{
public:
    PeriodicExecutor(Action<void()> executableTask) : TimerActions(executableTask) {}

    void RunEvery(unsigned long delay);
};

#endif