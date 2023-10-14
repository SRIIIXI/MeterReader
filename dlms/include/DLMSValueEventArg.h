#ifndef DLMS_VALUEEVENT_ARGS_H
#define DLMS_VALUEEVENT_ARGS_H

#include "ErrorCodes.h"
#include "DLMSVariant.h"

class DLMSObject;
class DLMSServer;
class DLMSClient;
class DLMSNotify;
class DLMSSettings;
class DLMSAssociationLogicalName;

class DLMSValueEventArg
{
    friend class DLMSClient;
    friend class DLMSServer;
    friend class DLMSNotify;
    friend class DLMSProfileGeneric;
    friend class DLMSAssociationLogicalName;
    friend class DLMSAssociationShortName;
    friend class DLMSLNCommandHandler;
    friend class DLMSSNCommandHandler;
private:
    /**
    * CGXDLMSVariant value.
    */
    DLMSVariant m_Value;
    /**
    * Is request handled.
    */
    bool m_Handled;
    /**
    * Target DLMS object
    */
    DLMSObject* m_Target;
    /**
    * Attribute index.
    */
    int m_Index;
    /**
    * Optional selector.
    */
    int m_Selector;
    /**
    * Optional parameters.
    */
    DLMSVariant m_Parameters;

    /**
     * Occurred error.
     */
    DLMS_ERROR_CODE m_Error;
    /**
    * Is action. This is reserved for internal use.
    */
    bool m_Action;

    /**
    * Is data handled as byte array.
    */
    bool m_ByteArray;

    /**
    * Is value max PDU size skipped.
    */
    bool m_SkipMaxPduSize;

    /**
    * Row to PDU is used with Profile Generic to tell how many rows are fit to
    * one PDU.
    */
    unsigned short m_RowToPdu;
    /**
    * Rows begin index.
    */
    unsigned short m_RowBeginIndex;
    /**
    * Rows end index.
    */
    unsigned short m_RowEndIndex;

    /**
    * DLMS settings.
    */
    DLMSSettings* m_Settings;

    /**
    * DLMS server.
    */
    DLMSServer* m_Server;

    /**
    * Received invoke ID.
    */
    unsigned int m_InvokeId;

    void Init(
        DLMSServer* server,
        DLMSObject* target,
        int index,
        int selector);

    /**
    * Constructor.
    *
    * @param target
    *            Event target.
    * @param index
    *            Event index.
    */
    DLMSValueEventArg(
        DLMSServer* server,
        DLMSObject* target,
        int index);

    /**
    * Constructor.
    *
    * @param target
    *            Event target.
    * @param index
    *            Event index.
    * @param selector
    *            Optional read event selector.
    * @param parameters
    *            Optional parameters.
    */
    DLMSValueEventArg(
        DLMSServer* server,
        DLMSObject* target,
        int index,
        int selector,
        DLMSVariant& parameters);

    /**
    * DLMS server.
    */
    DLMSServer* GetServer();
public:
    /**
    * @return Target DLMS object.
    */
    DLMSObject* GetTarget();

    void SetTarget(DLMSObject* value);

    /**
    * @return Attribute index of queried object.
    */
    int GetIndex();

    void SetIndex(int value);

    /**
    * @return CGXDLMSVariant value.
    */
    DLMSVariant& GetValue();

    /**
    * @param value
    *            CGXDLMSVariant value.
    */
    void SetValue(DLMSVariant value);

    /**
    * @return Is request handled.
    */
    bool GetHandled();

    /**
    * @param value
    *            Is request handled.
    */
    void SetHandled(bool value);

    /**
    * @return Optional selector.
    */
    int GetSelector();

    /**
    * @param value
    *           Selector.
    */
    void SetSelector(int value);

    /**
    * @return Optional parameters.
    */
    DLMSVariant& GetParameters();

    /**
    * @param value
    *           Parameters.
    */
    void SetParameters(DLMSVariant& value);


    /**
    * Constructor.
    *
    * @param target
    *            Event target.
    * @param index
    *            Event index.
    */
    DLMSValueEventArg(
        DLMSObject* target,
        int index);

    /**
    * Constructor.
    *
    * @param target
    *            Event target.
    * @param index
    *            Event index.
    * @param selector
    *            Optional read event selector.
    * @param parameters
    *            Optional parameters.
    */
    DLMSValueEventArg(
        DLMSObject* target,
        int index,
        int selector,
        DLMSVariant& parameters);

    /**
    * @return Occurred error.
    */
    DLMS_ERROR_CODE GetError();

    /**
     * @param error
     *            Occurred error.
     */
    void SetError(DLMS_ERROR_CODE value);

    /**
    * @return Is action.
    */
    bool IsAction();

    /**
    * @param value
    *            Is action.
    */
    void SetAction(bool value);

    /**
       * @return Is byte array.
       */
    bool IsByteArray();

    /**
    * @param value
    *            Is byte array.
    */
    void SetByteArray(bool value);

    /**
    * @return Is value max PDU size skipped.
    */
    bool GetSkipMaxPduSize();

    /**
     * @param value
     *            Is value max PDU size skipped.
     */
    void SetSkipMaxPduSize(bool value);


    /**
    * @return How many rows are read to one PDU.
    */
    unsigned short GetRowToPdu();

    /**
    * @param value
    *            How many rows are read to one PDU.
    */
    void SetRowToPdu(unsigned short value);

    /**
    * @return Rows end index.
    */
    unsigned int GetRowEndIndex();

    /**
    * @param value
    *            Rows end index.
    */
    void SetRowEndIndex(unsigned int value);

    /**
    * @return Rows begin index.
    */
    unsigned int GetRowBeginIndex();

    /**
    * @param value
    *            Rows begin index.
    */
    void SetRowBeginIndex(unsigned int value);

    /**
    * DLMS settings.
    */
    DLMSSettings* GetSettings();

    /**
    * @param value
    *            Received invoke ID.
    */
    void SetInvokeId(unsigned int value);

    /**
    * @return Received invoke ID.
    */
    unsigned int GetInvokeId();
};
#endif
