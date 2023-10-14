#ifndef DLMS_SCRIPT_H
#define DLMS_SCRIPT_H

#include "DLMSScriptAction.h"

class DLMSScript
{
    /**
     * Script identifier.
     */
    int m_ID;

    /**
     * Script actions.
     */
    std::vector<DLMSScriptAction*> m_Actions;

public:
    /**
     * Constructor.
     */
    DLMSScript();

    /**
    * Destructor.
    */
    ~DLMSScript();

    /**
     * @return Script identifier.
     */
    int GetID();

    /**
     * @param value
     *            Script identifier.
     */
    void SetID(int value);

    /**
     * @return Script actions.
     */
    std::vector<DLMSScriptAction*>& GetActions();
};
#endif
