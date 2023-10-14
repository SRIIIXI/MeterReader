#ifndef DLMS_SPECIAL_DAY_H
#define DLMS_SPECIAL_DAY_H

#include "Date.h"

class DLMSSpecialDay
{
    int m_Index;
    Date m_Date;
    int m_DayId;

public:
    int GetIndex();
    void SetIndex(int value);

    Date& GetDate();
    void SetDate(Date& value);
    void SetDate(DateTime& value);

    int GetDayId();
    void SetDayId(int value);

    std::string ToString();
};
#endif
