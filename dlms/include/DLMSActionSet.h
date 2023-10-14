#ifndef DLMS_ACTION_SET_H
#define DLMS_ACTION_SET_H

#include "DLMSActionItem.h"

class DLMSActionSet
{
    DLMSActionItem m_ActionUp;
    DLMSActionItem m_ActionDown;
public:
    /**
     Constructor.
    */
    DLMSActionSet();

    /**
     Destructor.
    */
    ~DLMSActionSet()
    {
    }

    DLMSActionItem& GetActionUp();

    DLMSActionItem& GetActionDown();

    std::string ToString();
};

#endif
