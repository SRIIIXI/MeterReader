#ifndef DLMS_SCRIPT_ACTION_H
#define DLMS_SCRIPT_ACTION_H

#include "Enums.h"
#include "DLMSVariant.h"

enum SCRIPT_ACTION_TYPE
{
    /*
     * Write attribute.
     */
    SCRIPT_ACTION_TYPE_WRITE = 1,
    /*
     * Execute specific method
     */
    SCRIPT_ACTION_TYPE_EXECUTE = 2
};

class DLMSScriptAction
{
    SCRIPT_ACTION_TYPE m_Type;
    DLMS_OBJECT_TYPE m_ObjectType;
    std::string m_LogicalName;
    int m_Index;
    DLMSVariant m_Parameter;
public:
    /**
     Defines which action to be applied to the referenced object.
    */
    SCRIPT_ACTION_TYPE GetType();
    void SetType(SCRIPT_ACTION_TYPE value);

    /**
     Executed object type.
    */
    DLMS_OBJECT_TYPE GetObjectType();
    void SetObjectType(DLMS_OBJECT_TYPE value);

    /**
     Logical name of executed object.
    */
    std::string GetLogicalName();
    void SetLogicalName(std::string value);

    /**
     defines which attribute of the selected object is affected; or
     * which specific method is to be executed.
    */
    int GetIndex();
    void SetIndex(int value);

    /**
     Parameter is service spesific.
    */
    DLMSVariant GetParameter();
    void SetParameter(DLMSVariant value);

    std::string ToString();
};
#endif
