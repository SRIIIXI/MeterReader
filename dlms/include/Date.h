#ifndef DATE_H
#define DATE_H

#include "DateTime.h"

/**
* Date object. Time part is ignored.
*/
class Date : public DateTime
{
public:
    // Constructor.
    Date() : DateTime()
    {
        SetSkip((DATETIME_SKIPS)(DATETIME_SKIPS_HOUR | DATETIME_SKIPS_MINUTE | DATETIME_SKIPS_SECOND | DATETIME_SKIPS_MS));
    };

    // Constructor.
    Date(struct tm value) : DateTime(value)
    {
        SetSkip((DATETIME_SKIPS)(DATETIME_SKIPS_HOUR | DATETIME_SKIPS_MINUTE | DATETIME_SKIPS_SECOND | DATETIME_SKIPS_MS));
    }

    // Constructor.
    Date(int year, int month, int day) :
        DateTime(year, month, day, -1, -1, -1, -1)
    {
    }

    /**
    * Constructor.
    *
    * @param forvalue
    *            Date value.
    */
    /// <summary>
    /// Constructor.
    /// </summary>
    Date(DateTime& value) : DateTime(value.GetValue())
    {
        SetSkip((DATETIME_SKIPS)(value.m_Skip | DATETIME_SKIPS_HOUR | DATETIME_SKIPS_MINUTE | DATETIME_SKIPS_SECOND | DATETIME_SKIPS_MS));
    }

    Date& operator=(const DateTime& value)
    {
        SetValue(value.m_Value);
        SetSkip((DATETIME_SKIPS)(value.m_Skip | DATETIME_SKIPS_HOUR | DATETIME_SKIPS_MINUTE | DATETIME_SKIPS_SECOND | DATETIME_SKIPS_MS));
        return *this;
    }
};
#endif
