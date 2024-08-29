#include <DateTime.h>
#include <time.h>

void DateTime::updateDateTime()
{
    time_t now = time(NULL);
    struct tm* localtm = localtime(&now);

    dt->date.Year = localtm->tm_year + 1900;
    dt->date.Month = localtm->tm_mon + 1;
    dt->date.Day = localtm->tm_mday;

    dt->time.hours = localtm->tm_hour;
    dt->time.minutes = localtm->tm_min;
    dt->time.seconds = localtm->tm_sec;
    dt->time.milliseconds = 0; // assuming no millisecond precision
}