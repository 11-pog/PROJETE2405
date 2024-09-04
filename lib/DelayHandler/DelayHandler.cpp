#include <DelayHandler.h>

DelayHandler::DelayHandler(std::function<void()> executableTask) : executableTask(executableTask) {}

void DelayHandler::DelayWhileExecuting(unsigned long delay)
{
    UpdateLastTime();

    while (!HasDelayElapsed(delay))
    {
        executableTask();
    }
}

void DelayHandler::SwitchTask(std::function<void()> executableTask)
{
    this->executableTask = executableTask;
}

// Privates
bool DelayHandler::HasDelayElapsed(unsigned long delay)
{
    return millis() - lastTime > delay;
}

void DelayHandler::UpdateLastTime()
{
    lastTime = millis();
}