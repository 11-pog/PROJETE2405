#ifndef DateTime
#define DateTime

class DateTime
{
public:
    struct date
    {
        uint Year;
        byte Month;
        byte Day;
    };

    struct time
    {
        byte hours;
        byte minutes;
        byte seconds;
        uint milliseconds;
    };

    void updateDateTime();
};

#endif