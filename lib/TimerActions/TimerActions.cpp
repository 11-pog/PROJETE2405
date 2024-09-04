#include <TimerActions.h>

TimerActions::TimerActions(std::function<void()> executableTask) : executableTask(executableTask) {}

void TimerActions::WaitWhileExecuting(unsigned long delay)
{
    while (!IsTimerUp(delay))
    {
        executableTask();
    }
}

void TimerActions::SwitchTask(std::function<void()> executableTask)
{
    this->executableTask = executableTask;
}

// Privates
bool TimerActions::IsTimerUp(unsigned long delay)
{
    SetLastTimeIfZero();

    if (millis() - lastTime > delay)
    {
        lastTime = 0;
        return true;
    }

    return false;
}

void TimerActions::SetLastTimeIfZero()
{
    lastTime = lastTime ? lastTime : millis();
}

void TimerActions::ResetTimer()
{
    lastTime = millis();
}