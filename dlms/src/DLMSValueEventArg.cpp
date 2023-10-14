#include "DLMSValueEventArg.h"
#include "DLMSSettings.h"
#include "DLMSServer.h"

DLMSObject* DLMSValueEventArg::GetTarget()
{
    return m_Target;
}

void DLMSValueEventArg::SetTarget(DLMSObject* value)
{
    m_Target = value;
}

int DLMSValueEventArg::GetIndex()
{
    return m_Index;
}

void DLMSValueEventArg::SetIndex(int value)
{
    m_Index = value;
}

DLMSVariant& DLMSValueEventArg::GetValue()
{
    return m_Value;
}

void DLMSValueEventArg::SetValue(DLMSVariant value)
{
    m_Value = value;
}

bool DLMSValueEventArg::GetHandled()
{
    return m_Handled;
}

void DLMSValueEventArg::SetHandled(bool value)
{
    m_Handled = value;
}

int DLMSValueEventArg::GetSelector()
{
    return m_Selector;
}

void DLMSValueEventArg::SetSelector(int value)
{
    m_Selector = value;
}

DLMSVariant& DLMSValueEventArg::GetParameters()
{
    return m_Parameters;
}

void DLMSValueEventArg::SetParameters(DLMSVariant& value)
{
    m_Parameters = value;
}

void DLMSValueEventArg::Init(
    DLMSServer* server,
    DLMSObject* target,
    int index,
    int selector)
{
    m_Server = server;
    m_Settings = &server->GetSettings();
    m_Handled = false;
    SetTarget(target);
    SetIndex(index);
    m_Selector = selector;
    m_Error = DLMS_ERROR_CODE_OK;
    m_ByteArray = false;
    m_SkipMaxPduSize = false;
    m_RowToPdu = 0;
    m_RowBeginIndex = 0;
    m_RowEndIndex = 0;
}

DLMSValueEventArg::DLMSValueEventArg(
    DLMSServer* server,
    DLMSObject* target,
    int index)
{
    Init(server, target, index, 0);
}

DLMSValueEventArg::DLMSValueEventArg(
    DLMSServer* server,
    DLMSObject* target,
    int index,
    int selector,
    DLMSVariant& parameters)
{
    Init(server, target, index, selector);
    m_Parameters = parameters;
}

DLMSValueEventArg::DLMSValueEventArg(
    DLMSObject* target,
    int index)
{
    Init(NULL, target, index, 0);
}

DLMSValueEventArg::DLMSValueEventArg(
    DLMSObject* target,
    int index,
    int selector,
    DLMSVariant& parameters)
{
    Init(NULL, target, index, selector);
    m_Parameters = parameters;
}

DLMS_ERROR_CODE DLMSValueEventArg::GetError()
{
    return m_Error;
}

void DLMSValueEventArg::SetError(DLMS_ERROR_CODE value)
{
    m_Error = value;
}

bool DLMSValueEventArg::IsAction()
{
    return m_Action;
}

void DLMSValueEventArg::SetAction(bool value)
{
    m_Action = value;
}

bool DLMSValueEventArg::IsByteArray()
{
    return m_ByteArray;
}

void DLMSValueEventArg::SetByteArray(bool value)
{
    m_ByteArray = value;
}

bool DLMSValueEventArg::GetSkipMaxPduSize()
{
    return m_SkipMaxPduSize;
}

void DLMSValueEventArg::SetSkipMaxPduSize(bool value)
{
    m_SkipMaxPduSize = value;
}

unsigned short DLMSValueEventArg::GetRowToPdu()
{
    return m_RowToPdu;
}

void DLMSValueEventArg::SetRowToPdu(unsigned short value) {
    m_RowToPdu = value;
}

unsigned int DLMSValueEventArg::GetRowEndIndex() {
    return m_RowEndIndex;
}

void DLMSValueEventArg::SetRowEndIndex(unsigned int value) {
    m_RowEndIndex = value;
}

unsigned int DLMSValueEventArg::GetRowBeginIndex() {
    return m_RowBeginIndex;
}

void DLMSValueEventArg::SetRowBeginIndex(unsigned int value) {
    m_RowBeginIndex = value;
}

DLMSSettings* DLMSValueEventArg::GetSettings()
{
    return m_Settings;
}


DLMSServer* DLMSValueEventArg::GetServer()
{
    return m_Server;
}

void DLMSValueEventArg::SetInvokeId(unsigned int value)
{
    m_InvokeId = value;
}

unsigned int DLMSValueEventArg::GetInvokeId()
{
    return m_InvokeId;
}
