#include <TimerActions.h>

void SimultaneousExecutor::ExecuteWhileWaiting(unsigned long delay)
{
    while (!IsTimerUp(delay))
    {
        executableTask();
    }
}

void TimerActions::SwitchTask(std::function<void()> newTask)
{
    this->executableTask = newTask;
}

bool TimerActions::IsTimerUp(unsigned long delay)
{
    SetLastTimeIfZero();

    if (millis() - lastTime > delay)
    {
        ResetTimer();
        return true;
    }

    return false;
}

void PeriodicExecutor::RunEvery(unsigned long delay)
{
    if (IsTimerUp(delay))
    {
        executableTask();
    }
}

void TimerActions::ResetTimer()
{
    lastTime = 0;
}


// Privates
void TimerActions::SetLastTimeIfZero()
{
    lastTime = lastTime ? lastTime : millis();
}