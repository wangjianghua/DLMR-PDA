#include <string.h>
#include <stdlib.h>
#include <time.h>
// convert broken time to calendar time (seconds since 2000)
U32 OS_mktime(struct RTCCounterValue *tmp)
{

#define TIME_CAL_START_YEAR   2000L
    
    int year, month, i;
    U32 seconds;
    struct RTCCounterValue timeptr;

    memcpy(&timeptr, tmp, sizeof(struct RTCCounterValue));
    Bcd2HexBuffer((unsigned char*) &timeptr, sizeof(struct RTCCounterValue));

    CheckTime(&timeptr);

    year = timeptr.Year + TIME_CAL_START_YEAR;

    month = timeptr.Month;

    // seconds from 2000 till 1 jan 00:00:00 this year
    seconds= (year - TIME_CAL_START_YEAR)*(60*60*24L*365);

    // add extra days for leap years
    for (i=TIME_CAL_START_YEAR; i<year; i++)
    {
        if (LEAP_YEAR(i))
        {
            seconds+= 60*60*24L;
        }
    }

    // add days for this year
    for (i=0; i<(month-1); i++)
    {
        if (i==1 && LEAP_YEAR(year))
        {
            seconds+= 60L*60*24L*29;
        }
        else
        {
            seconds+= 60*60*24L*monthDays[i];
        }
    }

    seconds+= (timeptr.Day-1)*60L*60*24L;
    seconds+= timeptr.Hour*60L*60;
    seconds+= timeptr.Min*60L;
    seconds+= timeptr.Sec;

    return seconds;
}

