#ifndef DLMS_DAY_PROFILE_ACTION_H
#define DLMS_DAY_PROFILE_ACTION_H

#include "DLMSTime.h"

//Activity Calendar's Day Profile Action is defined on the standard.
class DLMSDayProfileAction
{
    Time m_StartTime;
    std::string m_ScriptLogicalName;
    int m_ScriptSelector;
public:
    /**
     Constructor.
    */
    DLMSDayProfileAction();

    /**
     Constructor.
    */
    DLMSDayProfileAction(Time& startTime, std::string scriptLogicalName, int scriptSelector);

    /**
     Defines the time when the script is to be executed.
    */
    Time& GetStartTime();
    void SetStartTime(Time& value);

    /**
     Defines the logical name of the "Script table" object;
    */
    std::string& GetScriptLogicalName();
    void SetScriptLogicalName(std::string& value);

    /**
     Defines the script_identifier of the script to be executed.
    */
    int GetScriptSelector();
    void SetScriptSelector(int value);

    std::string ToString();
};
#endif
