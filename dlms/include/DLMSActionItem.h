#ifndef DLMS_ACTION_ITEM_H
#define DLMS_ACTION_ITEM_H

#include <string>

class DLMSActionItem
{
    friend class DLMSActionSet;
    std::string m_LogicalName;
    int m_ScriptSelector;
public:
    //Constructor.
    DLMSActionItem();

    //Destructor.
    ~DLMSActionItem()
    {
        m_LogicalName.clear();
    }
    std::string& GetLogicalName();

    void SetLogicalName(std::string& value);

    int GetScriptSelector();

    void SetScriptSelector(int value);

    std::string ToString();
};
#endif
