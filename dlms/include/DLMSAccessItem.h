#ifndef DLMS_ACCESS_ITEM_H
#define DLMS_ACCESS_ITEM_H

#include "DLMSObject.h"

/**
Access item is used to generate Access Service message.
*/
class DLMSAccessItem
{
    /**
     * COSEM target object.
     */
    DLMSObject* m_Target;
    /**
     * Executed command type.
     */
    DLMS_ACCESS_SERVICE_COMMAND_TYPE m_Command;

    /**
     * Attribute index.
     */
    unsigned char m_Index;

    //Reply error code.
    DLMS_ERROR_CODE m_Error;

    // Reply value.
    DLMSVariant m_Value;
public:
    /**
     * Returns COSEM target object.
     */
    DLMSObject* GetTarget();

    /**
     * value: COSEM target object.
     */
    void SetTarget(DLMSObject* value);

    /**
     * Returns Executed command type.
     */
    DLMS_ACCESS_SERVICE_COMMAND_TYPE GetCommand();

    /**
     * value: Executed command type.
     */
    void SetCommand(DLMS_ACCESS_SERVICE_COMMAND_TYPE value);

    /**
     * Returns Attribute index.
     */
    unsigned char GetIndex();

    /**
     * value: Attribute index.
     */
    void SetIndex(unsigned char value);

    /**
     * value: Reply error code.
     */
    void SetError(DLMS_ERROR_CODE value);

    /**
     * Returns reply error code.
     */
    DLMS_ERROR_CODE GetError();

    /**
     * value:  Reply value.
     */
    void SetValue(DLMSVariant& value);
    /**
     *  Returns reply value.
     */
    DLMSVariant& GetValue();

    /**
     * Constructor.
     */
    DLMSAccessItem();

    /**
     * Constructor.
     *
     * command: Command to execute.
     * target: COSEM target object.
     * attributeIndex: Attribute index.
     */
    DLMSAccessItem(DLMS_ACCESS_SERVICE_COMMAND_TYPE command,
        DLMSObject* target,
        unsigned char attributeIndex);
};
#endif
