#ifndef DLMS_TIME_H
#define DLMS_TIME_H

#include "DateTime.h"

/**
* Time object. Date part is ignored.
*/
class Time : public DateTime
{
public:
    // Constructor.
    Time() : DateTime()
    {
        SetSkip((DATETIME_SKIPS)(DATETIME_SKIPS_YEAR | DATETIME_SKIPS_MONTH | DATETIME_SKIPS_DAY | DATETIME_SKIPS_DAYOFWEEK));
    }
    // Constructor.
    Time(struct tm value) : DateTime(value)
    {
        SetSkip((DATETIME_SKIPS)(DATETIME_SKIPS_YEAR | DATETIME_SKIPS_MONTH | DATETIME_SKIPS_DAY | DATETIME_SKIPS_DAYOFWEEK));
    }

    // Constructor.
    Time(int hour, int minute, int second, int millisecond) :
        DateTime(-1, -1, -1, hour, minute, second, millisecond)
    {
        SetSkip((DATETIME_SKIPS)(m_Skip | DATETIME_SKIPS_DAYOFWEEK));
    }

    /**
    * Constructor.
    *
    * @param value
    *            Date value.
    */
    Time(DateTime& value) : DateTime(value.GetValue())
    {
        SetSkip((DATETIME_SKIPS)(value.m_Skip | DATETIME_SKIPS_YEAR | DATETIME_SKIPS_MONTH | DATETIME_SKIPS_DAY | DATETIME_SKIPS_DAYOFWEEK));
        SetExtra(value.m_Extra);
    }

    Time& operator=(const DateTime& value)
    {
        SetValue(value.m_Value);
        SetSkip((DATETIME_SKIPS)(value.m_Skip | DATETIME_SKIPS_YEAR | DATETIME_SKIPS_MONTH | DATETIME_SKIPS_DAY | DATETIME_SKIPS_DAYOFWEEK));
        SetExtra(value.m_Extra);
        return *this;
    }
};
#endif
