#include "Enums.h"
#include "DLMSClient.h"
#include "DLMS.h"
#include "Helpers.h"
#include "APDU.h"
#include "DLMSObjectFactory.h"
#include "Secure.h"
#include "SerialNumberCounter.h"
#include "DLMSLNParameters.h"
#include "DLMSSNParameters.h"
#include "GCM.h"    // define the various AES-GCM library functions
#include "DetectPlatform.h"
#include "SHA256.h"
#include "AES.h"

DLMSClient::DLMSClient(bool UseLogicalNameReferencing,
    int clientAddress,
    int serverAddress,
    //Authentication type.
    DLMS_AUTHENTICATION authentication,
    //Password if authentication is used.
    const char* password,
    DLMS_INTERFACE_TYPE intefaceType,
    const char* systemtitle,
    const char* customerpin) : m_Settings(false)
{
    m_UseProtectedRelease = false;
    m_IsAuthenticationRequired = true;
    m_ChallengeSize = 16;
    m_Settings.SetUseLogicalNameReferencing(UseLogicalNameReferencing);
    m_Settings.SetClientAddress(clientAddress);
    m_Settings.SetServerAddress(serverAddress);
    m_Settings.SetInterfaceType(intefaceType);
    m_Settings.SetAuthentication(authentication);
    m_Settings.GetPassword().AddString(password);
    if (UseLogicalNameReferencing)
    {
        SetProposedConformance((DLMS_CONFORMANCE)(DLMS_CONFORMANCE_BLOCK_TRANSFER_WITH_ACTION |
            DLMS_CONFORMANCE_BLOCK_TRANSFER_WITH_SET_OR_WRITE |
            DLMS_CONFORMANCE_BLOCK_TRANSFER_WITH_GET_OR_READ |
            DLMS_CONFORMANCE_SET | DLMS_CONFORMANCE_SELECTIVE_ACCESS |
            DLMS_CONFORMANCE_ACTION | DLMS_CONFORMANCE_MULTIPLE_REFERENCES |
            DLMS_CONFORMANCE_GET | DLMS_CONFORMANCE_ACCESS));
    }
    else
    {
        SetProposedConformance((DLMS_CONFORMANCE)(DLMS_CONFORMANCE_INFORMATION_REPORT |
            DLMS_CONFORMANCE_READ | DLMS_CONFORMANCE_UN_CONFIRMED_WRITE |
            DLMS_CONFORMANCE_WRITE | DLMS_CONFORMANCE_PARAMETERIZED_ACCESS |
            DLMS_CONFORMANCE_MULTIPLE_REFERENCES));
    }
    m_Settings.GetPlcSettings().Reset();
    SetManufacturerId(NULL);
}

DLMSClient::~DLMSClient()
{
}

unsigned short DLMSClient::GetMaxPduSize()
{
    return m_Settings.GetMaxPduSize();
}


bool DLMSClient::GetUseUtc2NormalTime()
{
    return m_Settings.GetUseUtc2NormalTime();
}

void DLMSClient::SetUseUtc2NormalTime(bool value)
{
    m_Settings.SetUseUtc2NormalTime(value);
}

uint64_t DLMSClient::GetExpectedInvocationCounter()
{
    return m_Settings.GetExpectedInvocationCounter();
}

void DLMSClient::SetExpectedInvocationCounter(uint64_t value)
{
    m_Settings.SetExpectedInvocationCounter(value);
}

DATETIME_SKIPS DLMSClient::GetDateTimeSkips()
{
    return m_Settings.GetDateTimeSkips();
}

void DLMSClient::SetDateTimeSkips(DATETIME_SKIPS value)
{
    m_Settings.SetDateTimeSkips(value);
}

unsigned char DLMSClient::GetUserID()
{
    return m_Settings.GetUserID();
}

void DLMSClient::SetUserID(unsigned char value)
{
    m_Settings.SetUserID(value);
}

unsigned char DLMSClient::GetQualityOfService()
{
    return m_Settings.GetQualityOfService();
}

void DLMSClient::SetQualityOfService(unsigned char value)
{
    m_Settings.SetQualityOfService(value);
}

ByteBuffer& DLMSClient::GetSourceSystemTitle()
{
    return m_Settings.GetSourceSystemTitle();
}

int DLMSClient::SetMaxReceivePDUSize(unsigned short value)
{
    return m_Settings.SetMaxReceivePDUSize(value);
}

unsigned short DLMSClient::GetMaxReceivePDUSize()
{
    return m_Settings.GetMaxPduSize();
}

int DLMSClient::SetGbtWindowSize(unsigned char value)
{
    m_Settings.SetGbtWindowSize(value);
    return 0;
}

unsigned char DLMSClient::GetGbtWindowSize()
{
    return m_Settings.GetGbtWindowSize();
}

DLMS_CONFORMANCE DLMSClient::GetNegotiatedConformance()
{
    return (DLMS_CONFORMANCE)m_Settings.GetNegotiatedConformance();
}

void DLMSClient::SetNegotiatedConformance(DLMS_CONFORMANCE value)
{
    m_Settings.SetNegotiatedConformance(value);
}

DLMS_CONFORMANCE DLMSClient::GetProposedConformance()
{
    return (DLMS_CONFORMANCE)m_Settings.GetProposedConformance();
}

void DLMSClient::SetProposedConformance(DLMS_CONFORMANCE value)
{
    m_Settings.SetProposedConformance(value);
}

bool DLMSClient::GetUseLogicalNameReferencing()
{
    return m_Settings.GetUseLogicalNameReferencing();
}

DLMS_INTERFACE_TYPE DLMSClient::GetInterfaceType()
{
    return m_Settings.GetInterfaceType();
}

DLMS_PRIORITY DLMSClient::GetPriority()
{
    return m_Settings.GetPriority();
}

void DLMSClient::SetPriority(DLMS_PRIORITY value)
{
    m_Settings.SetPriority(value);
}

DLMS_SERVICE_CLASS DLMSClient::GetServiceClass()
{
    return m_Settings.GetServiceClass();
}

DLMS_AUTHENTICATION DLMSClient::GetAuthentication()
{
    return m_Settings.GetAuthentication();
}

void DLMSClient::SetAuthentication(DLMS_AUTHENTICATION value)
{
    m_Settings.SetAuthentication(value);
}


unsigned long DLMSClient::GetClientAddress()
{
    return m_Settings.GetClientAddress();
}

void DLMSClient::SetClientAddress(unsigned long value)
{
    m_Settings.SetClientAddress(value);
}

unsigned long DLMSClient::GetServerAddress()
{
    return m_Settings.GetServerAddress();
}

// Server address.
void DLMSClient::SetServerAddress(unsigned long value)
{
    m_Settings.SetServerAddress(value);
}

void DLMSClient::SetServiceClass(DLMS_SERVICE_CLASS value)
{
    m_Settings.SetServiceClass(value);
}

bool DLMSClient::GetUseProtectedRelease()
{
    return m_UseProtectedRelease;
}

void DLMSClient::SetUseProtectedRelease(bool value)
{
    m_UseProtectedRelease = value;
}

/**
   * @return Invoke ID.
   */
unsigned char DLMSClient::GetInvokeID()
{
    return m_Settings.GetInvokeID();
}

/**
 * @param value
 *            Invoke ID.
 */
void DLMSClient::SetInvokeID(unsigned char value) {
    m_Settings.SetInvokeID(value);
}

/**
 * @return Auto increase Invoke ID.
 */
bool DLMSClient::GetAutoIncreaseInvokeID() {
    return m_Settings.GetAutoIncreaseInvokeID();
}

/**
 * @param value
 *            Auto increase Invoke ID.
 */
void DLMSClient::SetAutoIncreaseInvokeID(bool value) {
    m_Settings.SetAutoIncreaseInvokeID(value);
}

DLMSLimits& DLMSClient::GetLimits()
{
    return m_Settings.GetHdlcSettings();
}

HdlcSettings& DLMSClient::GetHdlcSettings()
{
    return m_Settings.GetHdlcSettings();
}


PlcSettings& DLMSClient::GetPlcSettings()
{
    return m_Settings.GetPlcSettings();
}

// Collection of the objects.
DLMSObjectCollection& DLMSClient::GetObjects()
{
    return m_Settings.GetObjects();
}

int DLMSClient::SNRMRequest(std::vector<ByteBuffer>& packets)
{
    int ret;
    m_Settings.SetConnected(DLMS_CONNECTION_STATE_NONE);
    packets.clear();
    //m_IsAuthenticationRequired = false;
    m_IsAuthenticationRequired = true;
    // SNRM request is not used for all communication channels.
    if (m_Settings.GetInterfaceType() == DLMS_INTERFACE_TYPE_PLC_HDLC)
    {
        ByteBuffer tmp;
        ret = DLMS::GetMacHdlcFrame(m_Settings, DLMS_COMMAND_SNRM, 0, NULL, tmp);
        if (ret == 0)
        {
            packets.push_back(tmp);
        }
        return ret;
    }
    if (m_Settings.GetInterfaceType() != DLMS_INTERFACE_TYPE_HDLC && m_Settings.GetInterfaceType() != DLMS_INTERFACE_TYPE_HDLC_WITH_MODE_E)
    {
        return 0;
    }
    ByteBuffer data(25);
    // FromatID
    data.SetUInt8(0x81);
    // GroupID
    data.SetUInt8(0x80);
    // Length is updated later.
    data.SetUInt8(0);

    data.SetUInt8(HDLC_INFO_MAX_INFO_TX);
    DLMS::AppendHdlcParameter(data, GetLimits().GetMaxInfoTX());

    data.SetUInt8(HDLC_INFO_MAX_INFO_RX);
    DLMS::AppendHdlcParameter(data, GetLimits().GetMaxInfoRX());

    data.SetUInt8(HDLC_INFO_WINDOW_SIZE_TX);
    data.SetUInt8(4);
    data.SetUInt32(GetLimits().GetWindowSizeTX());

    data.SetUInt8(HDLC_INFO_WINDOW_SIZE_RX);
    data.SetUInt8(4);
    data.SetUInt32(GetLimits().GetWindowSizeRX());

    if (data.GetSize() != 3)
    {
        // Length.
        data.SetUInt8(2, (unsigned char)(data.GetSize() - 3));
    }
    else
    {
        data.Clear();
    }
    m_Settings.ResetFrameSequence();
    ByteBuffer reply;
    ret = DLMS::GetHdlcFrame(m_Settings, DLMS_COMMAND_SNRM, &data, reply);
    packets.push_back(reply);
    return ret;
}

int DLMSClient::ParseSNObjectItem(
    DLMSVariant& value,
    bool ignoreInactiveObjects)
{
    if (value.vt != DLMS_DATA_TYPE_STRUCTURE || value.Arr.size() != 4)
    {
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    if (value.Arr[0].vt != DLMS_DATA_TYPE_INT16 ||
        value.Arr[1].vt != DLMS_DATA_TYPE_UINT16 ||
        value.Arr[2].vt != DLMS_DATA_TYPE_UINT8 ||
        value.Arr[3].vt != DLMS_DATA_TYPE_OCTET_STRING)
    {
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    short sn = value.Arr[0].ToInteger() & 0xFFFF;
    unsigned short class_id = (unsigned short)value.Arr[1].ToInteger();
    unsigned char version = (unsigned char)value.Arr[2].ToInteger();
    DLMSVariant ln = value.Arr[3];
    DLMSObject* pObj = DLMSObjectFactory::CreateObject((DLMS_OBJECT_TYPE)class_id);
    if (pObj != NULL)
    {
        pObj->SetShortName(sn);
        pObj->SetVersion(version);
        int cnt = ln.GetSize();
        assert(cnt == 6);
        DLMSObject::SetLogicalName(pObj, ln);
        std::string ln2;
        pObj->GetLogicalName(ln2);
        if (ignoreInactiveObjects && ln2.compare("0.0.127.0.0.0") == 0)
        {
            delete pObj;
            return 0;
        }
        m_Settings.GetObjects().push_back(pObj);
    }
    return 0;
}

// SN referencing
int DLMSClient::ParseSNObjects(
    ByteBuffer& buff,
    bool onlyKnownObjects,
    bool ignoreInactiveObjects)
{
    int ret;
    DataInfo info;
    //Get array tag.
    unsigned char ch;
    unsigned long cnt;
    //Check that data is in the array
    // Get array tag.
    if ((ret = buff.GetUInt8(&ch)) != 0)
    {
        return ret;
    }
    if (ch != 1)
    {
        return DLMS_ERROR_CODE_INVALID_RESPONSE;
    }
    //get object count
    DLMSVariant value;
    if ((ret = Helpers::GetObjectCount(buff, cnt)) != 0)
    {
        return ret;
    }
    for (unsigned long objPos = 0; objPos != cnt; ++objPos)
    {
        info.Clear();
        if ((ret = Helpers::GetData(&m_Settings, buff, info, value)) != 0 ||
            (ret = ParseSNObjectItem(value, ignoreInactiveObjects)) != 0)
        {
            return ret;
        }
    }
    return 0;
}

int DLMSClient::ParseLNObjectItem(
    DLMSVariant& value,
    bool ignoreInactiveObjects)
{
    int ret;
    if (value.Arr.size() != 4)
    {
        //Invalid structure format.
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    int classID = value.Arr[0].ToInteger() & 0xFFFF;
    if (classID > 0)
    {
        DLMSObject* pObj = DLMSObjectFactory::CreateObject((DLMS_OBJECT_TYPE)classID);
        if (pObj != NULL)
        {
            if (value.vt != DLMS_DATA_TYPE_STRUCTURE || value.Arr.size() != 4)
            {
                return DLMS_ERROR_CODE_INVALID_PARAMETER;
            }
            if (value.Arr[0].vt != DLMS_DATA_TYPE_UINT16)
            {
                return DLMS_ERROR_CODE_INVALID_PARAMETER;
            }
            if (value.Arr[1].vt != DLMS_DATA_TYPE_UINT8)
            {
                return DLMS_ERROR_CODE_INVALID_PARAMETER;
            }
            unsigned char version = value.Arr[1].ToInteger();
            if (value.Arr[2].vt != DLMS_DATA_TYPE_OCTET_STRING)
            {
                return DLMS_ERROR_CODE_INVALID_PARAMETER;
            }
            DLMSVariant ln = value.Arr[2];
            if ((ret = DLMSObject::SetLogicalName(pObj, ln)) != 0)
            {
                return ret;
            }
            std::string ln2;
            pObj->GetLogicalName(ln2);
            if (ignoreInactiveObjects && ln2.compare("0.0.127.0.0.0") == 0)
            {
                delete pObj;
                return 0;
            }
            //Get Access rights...
            if (value.Arr[3].vt != DLMS_DATA_TYPE_STRUCTURE || value.Arr[3].Arr.size() != 2)
            {
                return DLMS_ERROR_CODE_INVALID_PARAMETER;
            }
            pObj->SetVersion(version);
            int cnt;
            // attribute_access_descriptor Start
            if (value.Arr[3].Arr[0].vt != DLMS_DATA_TYPE_ARRAY)
            {
                delete pObj;
                return DLMS_ERROR_CODE_INVALID_PARAMETER;
            }
            if (value.Arr[3].Arr.size() == 2)
            {
                for (unsigned int pos = 0; pos != value.Arr[3].Arr[0].Arr.size(); ++pos)
                {
                    if (value.Arr[3].Arr[0].Arr[pos].vt != DLMS_DATA_TYPE_STRUCTURE ||
                        value.Arr[3].Arr[0].Arr[pos].Arr.size() != 3)
                    {
                        delete pObj;
                        return DLMS_ERROR_CODE_INVALID_PARAMETER;
                    }
                    int id = value.Arr[3].Arr[0].Arr[pos].Arr[0].ToInteger();
                    //Get access_mode
                    DLMS_DATA_TYPE tp = value.Arr[3].Arr[0].Arr[pos].Arr[1].vt;
                    if (tp != DLMS_DATA_TYPE_ENUM)
                    {
                        delete pObj;
                        return DLMS_ERROR_CODE_INVALID_PARAMETER;
                    }
                    pObj->SetAccess(id, (DLMS_ACCESS_MODE)value.Arr[3].Arr[0].Arr[pos].Arr[1].ToInteger());
                    //Get access_selectors
                    if (value.Arr[3].Arr[0].Arr[pos].Arr[2].vt == DLMS_DATA_TYPE_ARRAY)
                    {
                        int cnt2 = (unsigned long)value.Arr[3].Arr[0].Arr[pos].Arr[2].Arr.size();
                        for (int pos2 = 0; pos2 != cnt2; ++pos2)
                        {
                            //Get access_mode
                        }
                    }
                    else if (value.Arr[3].Arr[0].Arr[pos].Arr[2].vt != DLMS_DATA_TYPE_NONE)
                    {
                        delete pObj;
                        return DLMS_ERROR_CODE_INVALID_PARAMETER;
                    }
                }
                // attribute_access_descriptor End
                // method_access_item Start
                if (value.Arr[3].Arr[1].vt != DLMS_DATA_TYPE_ARRAY)
                {
                    delete pObj;
                    return DLMS_ERROR_CODE_INVALID_PARAMETER;
                }
                for (unsigned int pos = 0; pos != value.Arr[3].Arr[1].Arr.size(); ++pos)
                {
                    DLMSVariant tmp = value.Arr[3].Arr[1].Arr[pos];
                    if (tmp.vt != DLMS_DATA_TYPE_STRUCTURE ||
                        tmp.Arr.size() != 2)
                    {
                        delete pObj;
                        return DLMS_ERROR_CODE_INVALID_PARAMETER;
                    }
                    int id = tmp.Arr[0].ToInteger();
                    //Get access_mode
                    //In version 0 data type is boolean.
                    if (tmp.Arr[1].vt != DLMS_DATA_TYPE_ENUM && tmp.Arr[1].vt != DLMS_DATA_TYPE_BOOLEAN)
                    {
                        delete pObj;
                        return DLMS_ERROR_CODE_INVALID_PARAMETER;
                    }
                    pObj->SetMethodAccess(id, (DLMS_METHOD_ACCESS_MODE)tmp.Arr[1].ToInteger());
                }
            }
            // method_access_item End
            cnt = ln.GetSize();
            assert(cnt == 6);
            m_Settings.GetObjects().push_back(pObj);
        }
    }
    return 0;
}

int DLMSClient::ParseLNObjects(ByteBuffer& buff, bool onlyKnownObjects, bool ignoreInactiveObjects)
{
    int ret;
    unsigned long cnt;
    unsigned char ch;
    DataInfo info;
    // Get array tag.
    if ((ret = buff.GetUInt8(&ch)) != 0)
    {
        return ret;
    }
    // Check that data is in the array
    if (ch != 0x01)
    {
        //Invalid response.
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    DLMSVariant value;
    if ((ret = Helpers::GetObjectCount(buff, cnt)) != 0)
    {
        return ret;
    }
    for (unsigned long pos = 0; pos != cnt; ++pos)
    {
        // Some meters give wrong item count.
        // This fix Iskraemeco (MT-880) bug.
        if (buff.GetPosition() == buff.GetSize())
        {
            break;
        }
        info.SetType(DLMS_DATA_TYPE_NONE);
        info.SetIndex(0);
        info.SetCount(0);
        if ((ret = Helpers::GetData(&m_Settings, buff, info, value)) != 0 ||
            (ret = ParseLNObjectItem(value, ignoreInactiveObjects)) != 0)
        {
            return ret;
        }
    }
    return 0;
}

int DLMSClient::ParseObjects(ByteBuffer& data, bool onlyKnownObjects)
{
    return ParseObjects(data, onlyKnownObjects, true);
}

int DLMSClient::ParseObjects(ByteBuffer& data, bool onlyKnownObjects, bool ignoreInactiveObjects)
{
    int ret;
    m_Settings.GetObjects().Free();
    if (GetUseLogicalNameReferencing())
    {
        if ((ret = ParseLNObjects(data, onlyKnownObjects, ignoreInactiveObjects)) != 0)
        {
            return ret;
        }
    }
    else
    {
        if ((ret = ParseSNObjects(data, onlyKnownObjects, ignoreInactiveObjects)) != 0)
        {
            return ret;
        }
    }
    return 0;
}

int DLMSClient::ParseObjects(std::vector<DLMSVariant>& objects, bool onlyKnownObjects)
{
    return ParseObjects(objects, onlyKnownObjects, true);
}

int DLMSClient::ParseObjects(std::vector<DLMSVariant>& objects, bool onlyKnownObjects, bool ignoreInactiveObjects)
{
    int ret;
    m_Settings.GetObjects().Free();
    if (GetUseLogicalNameReferencing())
    {
        for (std::vector< DLMSVariant >::iterator it = objects.begin(); it != objects.end(); ++it)
        {
            if ((ret = ParseLNObjectItem(*it, ignoreInactiveObjects)) != 0)
            {
                return ret;
            }
        }
    }
    else
    {
        for (std::vector< DLMSVariant >::iterator it = objects.begin(); it != objects.end(); ++it)
        {
            if ((ret = ParseSNObjectItem(*it, ignoreInactiveObjects)) != 0)
            {
                return ret;
            }
        }
    }
    return 0;
}

int DLMSClient::UpdateValue(DLMSObject& target, int attributeIndex, DLMSVariant& value)
{
    int ret;
    if (value.vt == DLMS_DATA_TYPE_OCTET_STRING)
    {
        DLMS_DATA_TYPE type;
        if ((ret = target.GetDataType(attributeIndex, type)) == 0 && type == DLMS_DATA_TYPE_NONE)
        {
            if ((ret = target.SetDataType(attributeIndex, value.vt)) != 0)
            {
                return ret;
            }
        }
        if ((ret = target.GetUIDataType(attributeIndex, type)) != 0)
        {
            return ret;
        }
        if (type == DLMS_DATA_TYPE_DATETIME && value.GetSize() == 5)
        {
            type = DLMS_DATA_TYPE_DATE;
            target.SetUIDataType(attributeIndex, type);
        }
        if (type != DLMS_DATA_TYPE_NONE)
        {
            if ((ret = ChangeType(value, type, value)) != 0)
            {
                return ret;
            }
        }
    }
    DLMSValueEventArg e(NULL, &target, attributeIndex);
    e.SetValue(value);
    return target.SetValue(m_Settings, e);
}

int DLMSClient::GetValue(ByteBuffer& data, DLMSVariant& value)
{
    DataInfo info;
    return Helpers::GetData(&m_Settings, data, info, value);
}


int DLMSClient::ChangeType(DLMSVariant& value, DLMS_DATA_TYPE type, DLMSVariant& newValue)
{
    if (value.vt == DLMS_DATA_TYPE_OCTET_STRING)
    {
        ByteBuffer tmp;
        tmp.Set(value.byteArr, value.GetSize());
        return ChangeType(tmp, type, newValue);
    }
    else
    {
        newValue = value;
    }
    return 0;
}

int DLMSClient::ChangeType(ByteBuffer& value, DLMS_DATA_TYPE type, DLMSVariant& newValue)
{
    return ChangeType(value, type, false, newValue);
}

int DLMSClient::ChangeType(ByteBuffer& value, DLMS_DATA_TYPE type, bool useUtc, DLMSVariant& newValue)
{
    int ret;
    DataInfo info;
    newValue.Clear();
    if (value.GetSize() == 0)
    {
        return 0;
    }
    if (type == DLMS_DATA_TYPE_NONE)
    {
        newValue = value.ToHexString();
    }
    if (value.GetSize() == 0 && (type == DLMS_DATA_TYPE_STRING || type == DLMS_DATA_TYPE_OCTET_STRING))
    {
        newValue = "";
        return 0;
    }
    info.SetType(type);
    DLMSSettings settings(false);
    settings.SetUseUtc2NormalTime(useUtc);
    if ((ret = Helpers::GetData(&settings, value, info, newValue)) != 0)
    {
        return ret;
    }
    if (!info.IsComplete())
    {
        return DLMS_ERROR_CODE_OUTOFMEMORY;
    }
    if (type == DLMS_DATA_TYPE_OCTET_STRING && newValue.vt == DLMS_DATA_TYPE_OCTET_STRING)
    {
        int size = newValue.size;
        std::string str;
        if (size == 0)
        {
            str = "";
        }
        else
        {
            ByteBuffer bcd(size * 4);
            for (int pos = 0; pos != size; ++pos)
            {
                bcd.AddIntAsString(newValue.byteArr[pos]);
                bcd.SetUInt8('.');
            }
            //Remove last dot.
            if (bcd.GetSize() != 0)
            {
                bcd.SetSize(bcd.GetSize() - 1);
            }
            str = bcd.ToString();
        }
        newValue = str;
    }
    return ret;
}

int DLMSClient::ParseUAResponse(ByteBuffer& data)
{
    int ret = DLMS::ParseSnrmUaResponse(data, &m_Settings.GetHdlcSettings());
    if (ret == 0)
    {
        m_Settings.SetConnected(DLMS_CONNECTION_STATE_HDLC);
    }
    return ret;
}

int DLMSClient::ParseAAREResponse(ByteBuffer& reply)
{
    int ret;
    DLMS_SOURCE_DIAGNOSTIC sd;
    DLMS_ASSOCIATION_RESULT result;
#ifndef DLMS_IGNORE_XML_TRANSLATOR
    ret = APDU::ParsePDU(m_Settings, m_Settings.GetCipher(), reply, result, sd, NULL);
    if (ret != 0)
    {
        return ret;
    }
#else
    if ((ret = CGXAPDU::ParsePDU(m_Settings, m_Settings.GetCipher(),
        reply, result, sd)) != 0)
    {
        return ret;
    }
#endif //DLMS_IGNORE_XML_TRANSLATOR
    if (result != DLMS_ASSOCIATION_RESULT_ACCEPTED)
    {
        if (result == DLMS_ASSOCIATION_RESULT_TRANSIENT_REJECTED)
        {
            return DLMS_ERROR_CODE_REJECTED_TRANSIENT;
        }
        return DLMS_ERROR_CODE_REJECTED_PERMAMENT;
    }
    m_IsAuthenticationRequired = (DLMS_SOURCE_DIAGNOSTIC_AUTHENTICATION_REQUIRED == sd);
    if (m_Settings.GetDLMSVersion() != 6)
    {
        //Invalid DLMS version number.
        return DLMS_ERROR_CODE_INVALID_VERSION_NUMBER;
    }
    if (m_Settings.GetAuthentication() < DLMS_AUTHENTICATION_HIGH)
    {
        m_Settings.SetConnected((DLMS_CONNECTION_STATE)(m_Settings.GetConnected() | DLMS_CONNECTION_STATE_DLMS));
    }
    return 0;
}

bool DLMSClient::IsAuthenticationRequired()
{
    return m_IsAuthenticationRequired;
}

void DLMSClient::EnableCustomChallenge(bool fl)
{
    m_Settings.SetUseCustomChallenge(fl);
}

int DLMSClient::GetApplicationAssociationRequest(std::vector<ByteBuffer>& packets)
{
    int ret;
    DLMSVariant name;
    packets.clear();
    if (m_Settings.GetAuthentication() != DLMS_AUTHENTICATION_HIGH_ECDSA &&
        m_Settings.GetAuthentication() != DLMS_AUTHENTICATION_HIGH_GMAC &&
        m_Settings.GetPassword().GetSize() == 0)
    {
        //Password is invalid.
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    m_Settings.ResetBlockIndex();
    ByteBuffer pw, challenge;

    //Count challenge for Landis+Gyr. L+G is using custom way to count the challenge.
    if (memcmp(m_ManufacturerId, "LGZ", 3) == 0 && m_Settings.GetAuthentication() == DLMS_AUTHENTICATION_HIGH)
    {
        if ((ret = EncryptLandisGyrHighLevelAuthentication(m_Settings.GetPassword(), m_Settings.GetStoCChallenge(), challenge)) != 0)
        {
            return ret;
        }
        DLMSVariant data = challenge;
        if (GetUseLogicalNameReferencing())
        {
            name = "0.0.40.0.0.255";
            return Method(name, DLMS_OBJECT_TYPE_ASSOCIATION_LOGICAL_NAME, 1, data, packets);
        }
        name = 0xFA00;
        return Method(name, DLMS_OBJECT_TYPE_ASSOCIATION_SHORT_NAME, 8, data, packets);
    }

    if (m_Settings.GetAuthentication() == DLMS_AUTHENTICATION_HIGH_GMAC)
    {
        pw = m_Settings.GetCipher()->GetSystemTitle();
    }
    else if (m_Settings.GetAuthentication() == DLMS_AUTHENTICATION_HIGH_SHA256)
    {
        pw.Set(&m_Settings.GetPassword());
        pw.Set(&m_Settings.GetCipher()->GetSystemTitle());
        pw.Set(&m_Settings.GetSourceSystemTitle());
        pw.Set(&m_Settings.GetStoCChallenge());
        pw.Set(&m_Settings.GetCtoSChallenge());
    }
    else
    {
        pw = m_Settings.GetPassword();
    }
    long ic = 0;
    if (m_Settings.GetCipher() != NULL)
    {
        ic = m_Settings.GetCipher()->GetFrameCounter();
    }
    if ((ret = Secure::SecureEncrypt(m_Settings, m_Settings.GetCipher(), ic,
        m_Settings.GetStoCChallenge(), pw, challenge)) != 0)
    {
        return ret;
    }
    DLMSVariant data = challenge;
    if (GetUseLogicalNameReferencing())
    {
        name = "0.0.40.0.0.255";
        return Method(name, DLMS_OBJECT_TYPE_ASSOCIATION_LOGICAL_NAME,
            1, data, packets);
    }
    name = 0xFA00;
    return Method(name, DLMS_OBJECT_TYPE_ASSOCIATION_SHORT_NAME, 8, data,
        packets);
}

int DLMSClient::ParseApplicationAssociationResponse(
    ByteBuffer& reply)
{
    //Landis+Gyr is not returning StoC.
    if (memcmp(m_ManufacturerId, "LGZ", 3) == 0 && m_Settings.GetAuthentication() == DLMS_AUTHENTICATION_HIGH)
    {
        m_Settings.SetConnected((DLMS_CONNECTION_STATE) (m_Settings.GetConnected() | DLMS_CONNECTION_STATE_DLMS));
    }
    else
    {
        DataInfo info;
        unsigned char ch;
        bool equals = false;
        ByteBuffer secret;
        int ret;
        unsigned long ic = 0;
        DLMSVariant value;
        if ((ret = Helpers::GetData(&m_Settings, reply, info, value)) != 0)
        {
            return ret;
        }
        if (value.vt != DLMS_DATA_TYPE_NONE)
        {
            if (m_Settings.GetAuthentication() == DLMS_AUTHENTICATION_HIGH_GMAC)
            {
                secret = m_Settings.GetSourceSystemTitle();
                ByteBuffer bb;
                bb.Set(value.byteArr, value.GetSize());
                if ((ret = bb.GetUInt8(&ch)) != 0)
                {
                    return ret;
                }
                if ((ret = bb.GetUInt32(&ic)) != 0)
                {
                    return ret;
                }
            }
            else if (m_Settings.GetAuthentication() == DLMS_AUTHENTICATION_HIGH_SHA256)
            {
                secret.Set(&m_Settings.GetPassword());
                secret.Set(&m_Settings.GetSourceSystemTitle());
                secret.Set(&m_Settings.GetCipher()->GetSystemTitle());
                secret.Set(&m_Settings.GetCtoSChallenge());
                secret.Set(&m_Settings.GetStoCChallenge());
            }
            else
            {
                secret = m_Settings.GetPassword();
            }
            ByteBuffer challenge, cToS = m_Settings.GetCtoSChallenge();
            if ((ret = Secure::SecureEncrypt(
                m_Settings,
                m_Settings.GetCipher(),
                ic,
                cToS,
                secret,
                challenge)) != 0)
            {
                return ret;
            }
            equals = challenge.Compare(value.byteArr, value.GetSize());
        }
        else
        {
            // Server did not accept CtoS.
        }

        if (!equals)
        {
            //ParseApplicationAssociationResponse failed. Server to Client do not match.
            return DLMS_ERROR_CODE_AUTHENTICATION_FAILURE;
        }
    }
    return 0;
}

int DLMSClient::ReceiverReady(
    ReplyData& reply,
    ByteBuffer& packets)
{
    return DLMS::ReceiverReady(m_Settings, reply, NULL, packets);
}

int DLMSClient::ReceiverReady(DLMS_DATA_REQUEST_TYPES type, ByteBuffer& packets)
{
    return DLMS::ReceiverReady(m_Settings, type, NULL, packets);
}

int DLMSClient::ReleaseRequest(std::vector<ByteBuffer>& packets)
{
    int ret = 0;
    ByteBuffer buff;
    packets.clear();
    // If connection is not established, there is no need to send DisconnectRequest.
    //if ((m_Settings.GetConnected() & DLMS_CONNECTION_STATE_DLMS) == 0)
    //{
    //    return 0;
    //}
    if (!m_UseProtectedRelease)
    {
        buff.SetUInt8(3);
        buff.SetUInt8(0x80);
        buff.SetUInt8(1);
        buff.SetUInt8(0);
    }
    else
    {
        //Length.
        buff.SetUInt8(0);
        buff.SetUInt8(0x80);
        buff.SetUInt8(01);
        buff.SetUInt8(00);
        //Increase IC.
        if (m_Settings.GetCipher() != NULL && m_Settings.GetCipher()->IsCiphered())
        {
            m_Settings.GetCipher()->SetInvocationCounter(1 + m_Settings.GetCipher()->GetInvocationCounter());
        }
        APDU::GenerateUserInformation(m_Settings, m_Settings.GetCipher(), NULL, buff);
        buff.SetUInt8(0, (unsigned char)(buff.GetSize() - 1));
    }
    if (GetUseLogicalNameReferencing())
    {
        DLMSLNParameters p(&m_Settings, 0, DLMS_COMMAND_RELEASE_REQUEST, 0, NULL, &buff, 0xff, DLMS_COMMAND_NONE);
        ret = DLMS::GetLnMessages(p, packets);
    }
    else
    {
        DLMSSNParameters p(&m_Settings, DLMS_COMMAND_RELEASE_REQUEST, 0xFF, 0xFF, NULL, &buff);
        ret = DLMS::GetSnMessages(p, packets);
    }
    m_Settings.SetConnected((DLMS_CONNECTION_STATE)(m_Settings.GetConnected() & ~DLMS_CONNECTION_STATE_DLMS));
    return ret;
}

int DLMSClient::DisconnectRequest(std::vector<ByteBuffer>& packets)
{
    int ret;
    ByteBuffer reply;
    packets.clear();
    if (DLMS::UseHdlc(GetInterfaceType()) && (m_Settings.GetConnected() & DLMS_CONNECTION_STATE_HDLC) != 0)
    {
        if (GetInterfaceType() == DLMS_INTERFACE_TYPE_PLC_HDLC)
        {
            ret = DLMS::GetMacHdlcFrame(m_Settings, DLMS_COMMAND_DISCONNECT_REQUEST, 0, NULL, reply);
        }
        else
        {
            ret = DLMS::GetHdlcFrame(m_Settings, DLMS_COMMAND_DISCONNECT_REQUEST, NULL, reply);
        }
        packets.push_back(reply);
        m_Settings.SetConnected(DLMS_CONNECTION_STATE_NONE);
        return ret;
    }
    else
    {
        ret = ReleaseRequest(packets);
    }
    m_Settings.SetMaxReceivePDUSize(0xFFFF);
    return ret;
}

int DLMSClient::GetData(ByteBuffer& reply, ReplyData& data)
{
    return DLMS::GetData(m_Settings, reply, data, NULL);
}

int DLMSClient::GetData(ByteBuffer& reply, ReplyData& data, ReplyData& notify)
{
    return DLMS::GetData(m_Settings, reply, data, &notify);
}

int DLMSClient::GetObjectsRequest(std::vector<ByteBuffer>& reply)
{
    DLMSVariant name;
    if (GetUseLogicalNameReferencing())
    {
        name = "0.0.40.0.0.255";
        return Read(name, DLMS_OBJECT_TYPE_ASSOCIATION_LOGICAL_NAME, 2, reply);
    }
    name = (short)0xFA00;
    return Read(name, DLMS_OBJECT_TYPE_ASSOCIATION_SHORT_NAME, 2, reply);
}

int DLMSClient::GetKeepAlive(std::vector<ByteBuffer>& reply)
{
    DLMSVariant name;
    if (GetUseLogicalNameReferencing())
    {
        name = "0.0.40.0.0.255";
        return Read(name, DLMS_OBJECT_TYPE_ASSOCIATION_LOGICAL_NAME, 1, reply);
    }
    name = (short)0xFA00;
    return Read(name, DLMS_OBJECT_TYPE_ASSOCIATION_SHORT_NAME, 1, reply);
}

int DLMSClient::Read(DLMSObject* pObject, int attributeOrdinal, std::vector<ByteBuffer>& reply)
{
    DLMSVariant name = pObject->GetName();
    return Read(name, pObject->GetObjectType(), attributeOrdinal, NULL, reply);
}

int DLMSClient::Read(DLMSVariant& name, DLMS_OBJECT_TYPE objectType, int attributeOrdinal, std::vector<ByteBuffer>& reply)
{
    return Read(name, objectType, attributeOrdinal, NULL, reply);
}

int DLMSClient::Read(DLMSVariant& name, DLMS_OBJECT_TYPE objectType, int attributeOrdinal, ByteBuffer* data,
    std::vector<ByteBuffer>& reply)
{
    int ret;
    if ((attributeOrdinal < 1))
    {
        //Invalid parameter
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    m_Settings.ResetBlockIndex();
    ByteBuffer attributeDescriptor;
    if (GetUseLogicalNameReferencing())
    {
        // CI
        attributeDescriptor.SetUInt16(objectType);
        // Add LN
        unsigned char ln[6];
        Helpers::SetLogicalName(name.strVal.c_str(), ln);
        attributeDescriptor.Set(ln, 6);
        // Attribute ID.
        attributeDescriptor.SetUInt8(attributeOrdinal);
        if (data == NULL || data->GetSize() == 0)
        {
            // Access selection is not used.
            attributeDescriptor.SetUInt8(0);
        }
        else
        {
            // Access selection is used.
            attributeDescriptor.SetUInt8(1);
            // Add data.
            attributeDescriptor.Set(data, 0, data->GetSize());
        }
        DLMSLNParameters p(&m_Settings, 0,
            DLMS_COMMAND_GET_REQUEST, DLMS_GET_COMMAND_TYPE_NORMAL,
            &attributeDescriptor, data, 0xFF, DLMS_COMMAND_NONE);
        ret = DLMS::GetLnMessages(p, reply);
    }
    else
    {
        DLMS_VARIABLE_ACCESS_SPECIFICATION requestType;
        int sn = name.ToInteger();
        sn += (attributeOrdinal - 1) * 8;
        attributeDescriptor.SetUInt16(sn);
        // Add Selector.
        if (data != NULL && data->GetSize() != 0)
        {
            requestType = DLMS_VARIABLE_ACCESS_SPECIFICATION_PARAMETERISED_ACCESS;
        }
        else
        {
            requestType = DLMS_VARIABLE_ACCESS_SPECIFICATION_VARIABLE_NAME;
        }
        DLMSSNParameters p(&m_Settings, DLMS_COMMAND_READ_REQUEST, 1,
            requestType, &attributeDescriptor, data);
        ret = DLMS::GetSnMessages(p, reply);
    }
    return ret;
}

int DLMSClient::ReadList(
    std::vector<std::pair<DLMSObject*, unsigned char> >& list,
    std::vector<ByteBuffer>& reply)
{
    if (list.size() == 0)
    {
        //Invalid parameter
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    if ((GetNegotiatedConformance() & DLMS_CONFORMANCE_MULTIPLE_REFERENCES) == 0) {
        //Meter doesn't support multiple objects reading with one request.
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }

    int ret;
    m_Settings.ResetBlockIndex();
    ByteBuffer bb;
    if (GetUseLogicalNameReferencing())
    {
        DLMSLNParameters p(&m_Settings, 0,
            DLMS_COMMAND_GET_REQUEST, DLMS_GET_COMMAND_TYPE_WITH_LIST,
            &bb, NULL, 0xff, DLMS_COMMAND_NONE);
        //Request service primitive shall always fit in a single APDU.
        unsigned short pos = 0, count = (m_Settings.GetMaxPduSize() - 12) / 10;
        if (list.size() < count)
        {
            count = (unsigned short)list.size();
        }
        //All meters can handle 10 items.
        if (count > 10)
        {
            count = 10;
        }
        // Add length.
        Helpers::SetObjectCount(count, bb);
        for (std::vector<std::pair<DLMSObject*, unsigned char> >::iterator it = list.begin(); it != list.end(); ++it)
        {
            // CI.
            bb.SetUInt16(it->first->GetObjectType());
            bb.Set(it->first->m_LN, 6);
            // Attribute ID.
            bb.SetUInt8(it->second);
            // Attribute selector is not used.
            bb.SetUInt8(0);
            ++pos;
            if (pos % count == 0 && list.size() != pos)
            {
                if ((ret = DLMS::GetLnMessages(p, reply)) != 0)
                {
                    return ret;
                }
                bb.Clear();
                if (list.size() - pos < count)
                {
                    Helpers::SetObjectCount((unsigned long)(list.size() - pos), bb);
                }
                else
                {
                    Helpers::SetObjectCount(count, bb);
                }
            }
        }
        if ((ret = DLMS::GetLnMessages(p, reply)) != 0)
        {
            return ret;
        }
    }
    else
    {
        int sn;
        for (std::vector<std::pair<DLMSObject*, unsigned char> >::iterator it = list.begin(); it != list.end(); ++it)
        {
            // Add variable type.
            bb.SetUInt8(DLMS_VARIABLE_ACCESS_SPECIFICATION_VARIABLE_NAME);
            sn = it->first->GetShortName();
            sn += (it->second - 1) * 8;
            bb.SetUInt16(sn);
        }
        DLMSSNParameters p(&m_Settings, DLMS_COMMAND_READ_REQUEST,
            (unsigned long)list.size(), 0xFF, &bb, NULL);
        ret = DLMS::GetSnMessages(p, reply);
    }
    return ret;
}

int DLMSClient::WriteList(
    std::vector<std::pair<DLMSObject*, unsigned char> >& list,
    std::vector<ByteBuffer>& reply)
{
    if ((GetNegotiatedConformance() & DLMS_CONFORMANCE_MULTIPLE_REFERENCES) == 0) {
        //Meter doesn't support multiple objects reading with one request.
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }

    if (list.size() == 0)
    {
        //Invalid parameter
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }

    int ret = 0;
    m_Settings.ResetBlockIndex();
    ByteBuffer bb;
    if (GetUseLogicalNameReferencing())
    {
        DLMSLNParameters p(&m_Settings, 0,
            DLMS_COMMAND_SET_REQUEST, DLMS_SET_COMMAND_TYPE_WITH_LIST,
            &bb, NULL, 0xff, DLMS_COMMAND_NONE);
        // Add length.
        Helpers::SetObjectCount((unsigned long)list.size(), bb);
        for (std::vector<std::pair<DLMSObject*, unsigned char> >::iterator it = list.begin(); it != list.end(); ++it)
        {
            // CI.
            bb.SetUInt16(it->first->GetObjectType());
            bb.Set(it->first->m_LN, 6);
            // Attribute ID.
            bb.SetUInt8(it->second);
            // Attribute selector is not used.
            bb.SetUInt8(0);
        }
        // Add length.
        Helpers::SetObjectCount((unsigned long)list.size(), bb);
        for (std::vector<std::pair<DLMSObject*, unsigned char> >::iterator it = list.begin(); it != list.end(); ++it)
        {
            DLMSValueEventArg e(it->first, it->second);
            int ret = it->first->GetValue(m_Settings, e);
            if (ret != 0)
            {
                break;
            }
            DLMSVariant value = e.GetValue();
            if (e.IsByteArray())
            {
                bb.Set(value.byteArr, value.GetSize());
            }
            else
            {
                DLMS_DATA_TYPE type = DLMS_DATA_TYPE_NONE;
                if ((ret = it->first->GetDataType(it->second, type)) != 0)
                {
                    break;
                }
                if ((ret = Helpers::SetData(&m_Settings, bb, type, value)) != 0)
                {
                    break;
                }
            }
        }
        if (ret == 0)
        {
            if ((ret = DLMS::GetLnMessages(p, reply)) != 0)
            {
                return ret;
            }
        }
    }
    else
    {
        int sn;
        for (std::vector<std::pair<DLMSObject*, unsigned char> >::iterator it = list.begin(); it != list.end(); ++it)
        {
            // Add variable type.
            bb.SetUInt8(DLMS_VARIABLE_ACCESS_SPECIFICATION_VARIABLE_NAME);
            sn = it->first->GetShortName();
            sn += (it->second - 1) * 8;
            bb.SetUInt16(sn);
        }
        // Add length.
        Helpers::SetObjectCount((unsigned long)list.size(), bb);
        for (std::vector<std::pair<DLMSObject*, unsigned char> >::iterator it = list.begin(); it != list.end(); ++it)
        {
            DLMSValueEventArg e(it->first, it->second);
            int ret = it->first->GetValue(m_Settings, e);
            if (ret != 0)
            {
                break;
            }
            DLMSVariant value = e.GetValue();
            if (e.IsByteArray())
            {
                bb.Set(value.byteArr, value.GetSize());
            }
            else
            {
                DLMS_DATA_TYPE type = DLMS_DATA_TYPE_NONE;
                if ((ret = it->first->GetDataType(it->second, type)) != 0)
                {
                    break;
                }
                if ((ret = Helpers::SetData(&m_Settings, bb, type, value)) != 0)
                {
                    break;
                }
            }
        }
        if (ret == 0)
        {
            DLMSSNParameters p(&m_Settings, DLMS_COMMAND_WRITE_REQUEST,
                (unsigned long)list.size(), 0xFF, &bb, NULL);
            ret = DLMS::GetSnMessages(p, reply);
        }
    }
    return ret;
}

/**
     * Update list of values.
     *
     * @param list
     *            read objects.
     * @param values
     *            Received values.
     */
int DLMSClient::UpdateValues(
    std::vector<std::pair<DLMSObject*, unsigned char> >& list,
    std::vector<DLMSVariant>& values)
{
    int ret, pos = 0;
    for (std::vector<std::pair<DLMSObject*, unsigned char> >::iterator it = list.begin(); it != list.end(); ++it)
    {
        DLMSValueEventArg e(NULL, it->first, it->second);
        e.SetValue(values.at(pos));
        if ((ret = it->first->SetValue(m_Settings, e)) != 0)
        {
            return ret;
        }
        ++pos;
    }
    return 0;
}

int DLMSClient::Write(
    DLMSObject* pObject,
    int index,
    std::vector<ByteBuffer>& reply)
{
    if (index < 1)
    {
        //Invalid parameter
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    DLMSValueEventArg e(pObject, index);
    int ret = pObject->GetValue(m_Settings, e);
    if (ret == 0)
    {
        DLMSVariant name = pObject->GetName();
        DLMSVariant value = e.GetValue();
        int ret;
        m_Settings.ResetBlockIndex();
        ByteBuffer bb, data;
        if (e.IsByteArray())
        {
            data.Set(value.byteArr, value.GetSize());
        }
        else
        {
            DLMS_DATA_TYPE type = DLMS_DATA_TYPE_NONE;
            if ((ret = pObject->GetDataType(index, type)) != 0)
            {
                return ret;
            }
            if ((ret = Helpers::SetData(&m_Settings, data, type, value)) != 0)
            {
                return ret;
            }
        }
        if (GetUseLogicalNameReferencing())
        {
            // Add CI.
            bb.SetUInt16(pObject->GetObjectType());
            // Add LN.
            unsigned char ln[6];
            Helpers::SetLogicalName(name.strVal.c_str(), ln);
            bb.Set(ln, 6);
            // Attribute ID.
            bb.SetUInt8(index);
            // Access selection is not used.
            bb.SetUInt8(0);
            DLMSLNParameters p(&m_Settings, 0,
                DLMS_COMMAND_SET_REQUEST, DLMS_SET_COMMAND_TYPE_NORMAL,
                &bb, &data, 0xff, DLMS_COMMAND_NONE);
            ret = DLMS::GetLnMessages(p, reply);
        }
        else
        {
            // Add name.
            int sn = name.ToInteger();
            sn += (index - 1) * 8;
            bb.SetUInt16(sn);
            // Add data count.
            bb.SetUInt8(1);
            DLMSSNParameters p(&m_Settings,
                DLMS_COMMAND_WRITE_REQUEST, 1,
                DLMS_VARIABLE_ACCESS_SPECIFICATION_VARIABLE_NAME,
                &bb, &data);
            ret = DLMS::GetSnMessages(p, reply);
        }
        return ret;
    }
    return ret;
}


int DLMSClient::Write(DLMSObject* pObject,
    int index,
    DLMSVariant& data,
    std::vector<ByteBuffer>& reply)
{
    DLMSVariant name = pObject->GetName();
    return Write(name, pObject->GetObjectType(), index, data, reply);
}

int DLMSClient::Write(DLMSVariant& name, DLMS_OBJECT_TYPE objectType,
    int index, DLMSVariant& value, std::vector<ByteBuffer>& reply)
{
    if (index < 1)
    {
        //Invalid parameter
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    int ret;
    m_Settings.ResetBlockIndex();
    ByteBuffer bb, data;
    if ((ret = Helpers::SetData(&m_Settings, data, value.vt, value)) != 0)
    {
        return ret;
    }

    if (GetUseLogicalNameReferencing())
    {
        // Add CI.
        bb.SetUInt16(objectType);
        // Add LN.
        unsigned char ln[6];
        Helpers::SetLogicalName(name.strVal.c_str(), ln);
        bb.Set(ln, 6);
        // Attribute ID.
        bb.SetUInt8(index);
        // Access selection is not used.
        bb.SetUInt8(0);
        DLMSLNParameters p(&m_Settings, 0,
            DLMS_COMMAND_SET_REQUEST, DLMS_SET_COMMAND_TYPE_NORMAL,
            &bb, &data, 0xff, DLMS_COMMAND_NONE);
        ret = DLMS::GetLnMessages(p, reply);
    }
    else
    {
        // Add name.
        int sn = name.ToInteger();
        sn += (index - 1) * 8;
        bb.SetUInt16(sn);
        // Add data count.
        bb.SetUInt8(1);
        DLMSSNParameters p(&m_Settings,
            DLMS_COMMAND_WRITE_REQUEST, 1,
            DLMS_VARIABLE_ACCESS_SPECIFICATION_VARIABLE_NAME,
            &bb, &data);
        ret = DLMS::GetSnMessages(p, reply);
    }
    return ret;
}

/**
    * Generate Method (Action) request.
    *
    * @param item
    *            Method object short name or Logical Name.
    * @param index
    *            Method index.
    * @param data
    *            Method data.
    * @param type
    *            Data type.
    * @return DLMS action message.
    */
int DLMSClient::Method(DLMSObject* item, int index,
    DLMSVariant& data, std::vector<ByteBuffer>& reply)
{
    return Method(item, index, data, data.vt, reply);
}

/**
    * Generate Method (Action) request.
    *
    * @param item
    *            Method object short name or Logical Name.
    * @param index
    *            Method index.
    * @param data
    *            Method data.
    * @param type
    *            Data type.
    * @return DLMS action message.
    */
int DLMSClient::Method(DLMSObject* item, int index,
    DLMSVariant& data, DLMS_DATA_TYPE dataType, std::vector<ByteBuffer>& reply)
{
    DLMSVariant name = item->GetName();
    return Method(name, item->GetObjectType(), index, data, dataType, reply);
}

int DLMSClient::Method(
    DLMSVariant name,
    DLMS_OBJECT_TYPE objectType,
    int index,
    DLMSVariant& value,
    std::vector<ByteBuffer>& reply)
{
    return Method(name, objectType,
        index, value, value.vt, reply);
}

int DLMSClient::Method(
    DLMSVariant name,
    DLMS_OBJECT_TYPE objectType,
    int index,
    DLMSVariant& value,
    DLMS_DATA_TYPE dataType,
    std::vector<ByteBuffer>& reply)
{
    int ret;
    if (index < 1)
    {
        //Invalid parameter
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }

    ByteBuffer bb, data;
    m_Settings.ResetBlockIndex();
    if ((ret = Helpers::SetData(&m_Settings, data, dataType, value)) != 0)
    {
        return ret;
    }
    if (GetUseLogicalNameReferencing())
    {
        // CI
        bb.SetUInt16(objectType);
        // Add LN.
        unsigned char ln[6];
        Helpers::SetLogicalName(name.strVal.c_str(), ln);
        bb.Set(ln, 6);
        // Attribute ID.
        bb.SetUInt8(index);
        // Method Invocation Parameters is not used.
        if (value.vt == DLMS_DATA_TYPE_NONE)
        {
            bb.SetUInt8(0);
        }
        else
        {
            bb.SetUInt8(1);
        }
        DLMSLNParameters p(&m_Settings, 0,
            DLMS_COMMAND_METHOD_REQUEST, DLMS_ACTION_COMMAND_TYPE_NORMAL,
            &bb, &data, 0xff, DLMS_COMMAND_NONE);
        ret = DLMS::GetLnMessages(p, reply);
    }
    else
    {
        int requestType;
        if (value.vt == DLMS_DATA_TYPE_NONE)
        {
            requestType = DLMS_VARIABLE_ACCESS_SPECIFICATION_VARIABLE_NAME;
        }
        else
        {
            requestType = DLMS_VARIABLE_ACCESS_SPECIFICATION_PARAMETERISED_ACCESS;
        }
        unsigned char ind, count;
        if ((ret = DLMS::GetActionInfo(objectType, ind, count)) != 0)
        {
            return ret;
        }

        if (index > count)
        {
            //Invalid parameter
            return DLMS_ERROR_CODE_INVALID_PARAMETER;
        }
        int sn = name.ToInteger();
        index = (ind + (index - 1) * 0x8);
        sn += index;
        // Add SN count.
        bb.SetUInt8(1);
        // Add name length.
        bb.SetUInt8(4);
        // Add name.
        bb.SetUInt16(sn);
        // Method Invocation Parameters is not used.
        if (value.vt == DLMS_DATA_TYPE_NONE)
        {
            bb.SetUInt8(0);
        }
        else
        {
            bb.SetUInt8(1);
        }
        DLMSSNParameters p(&m_Settings, DLMS_COMMAND_READ_REQUEST, 1,
            requestType, &bb, &data);
        ret = DLMS::GetSnMessages(p, reply);
    }

    return ret;
}

int DLMSClient::ReadRowsByEntry(
    DLMSProfileGeneric* pg,
    int index,
    int count,
    std::vector<ByteBuffer>& reply)
{
    std::vector<std::pair<DLMSObject*, DLMSCaptureObject*> > cols;
    return ReadRowsByEntry(pg, index, count, cols, reply);
}

int DLMSClient::ReadRowsByEntry(
    DLMSProfileGeneric* pg,
    int index,
    int count,
    std::vector<std::pair<DLMSObject*, DLMSCaptureObject*> >& columns,
    std::vector<ByteBuffer>& reply)
{
    ByteBuffer buff(19);
    if (pg == NULL)
    {
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    pg->Reset();
    // Add AccessSelector value
    buff.SetUInt8(0x02);
    // Add enum tag.
    buff.SetUInt8(DLMS_DATA_TYPE_STRUCTURE);
    // Add item count
    buff.SetUInt8(0x04);
    // Add start index
    DLMSVariant tmp = index;
    Helpers::SetData(&m_Settings, buff, DLMS_DATA_TYPE_UINT32, tmp);
    // Add Count
    if (count == 0)
    {
        tmp = count;
    }
    else
    {
        tmp = index + count - 1;
    }
    Helpers::SetData(&m_Settings, buff, DLMS_DATA_TYPE_UINT32, tmp);
    tmp = 1;
    // Read all columns.
    Helpers::SetData(&m_Settings, buff, DLMS_DATA_TYPE_UINT16, tmp);
    tmp = 0;
    Helpers::SetData(&m_Settings, buff, DLMS_DATA_TYPE_UINT16, tmp);
    DLMSVariant name = pg->GetName();
    return Read(name, DLMS_OBJECT_TYPE_PROFILE_GENERIC, 2, &buff, reply);
}

int DLMSClient::ReadRowsByRange(
    DLMSProfileGeneric* pg,
    DateTime& start,
    DateTime& end,
    std::vector<ByteBuffer>& reply)
{
    if (pg == NULL)
    {
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    int ret;
    bool unixTime = false;
    unsigned char LN[] = { 0, 0, 1, 0, 0, 255 };
    DLMS_OBJECT_TYPE type = DLMS_OBJECT_TYPE_CLOCK;
    unsigned char* pLn = LN;
    ByteBuffer buff(51);
    DLMSVariant name = pg->GetName();
    if (pg->GetCaptureObjects().size() != 0)
    {
        std::pair<DLMSObject*, DLMSCaptureObject*> kv = pg->GetCaptureObjects()[0];
        type = kv.first->GetObjectType();
        pLn = kv.first->m_LN;
        unsigned char UNIX_LN[] = { 0, 0, 1, 1, 0, 255 };
        unixTime = type == DLMS_OBJECT_TYPE_DATA && memcmp(pLn, UNIX_LN, 6) == 0;
    }
    pg->Reset();
    m_Settings.ResetBlockIndex();
    // Add AccessSelector value.
    buff.SetUInt8(0x01);
    // Add enum tag.
    buff.SetUInt8(DLMS_DATA_TYPE_STRUCTURE);
    // Add item count
    buff.SetUInt8(0x04);
    // Add enum tag.
    buff.SetUInt8(0x02);
    // Add item count
    buff.SetUInt8(0x04);
    // CI
    DLMSVariant tmp = DLMS_OBJECT_TYPE_CLOCK;
    Helpers::SetData(&m_Settings, buff, DLMS_DATA_TYPE_UINT16, tmp);
    // LN
    DLMSVariant ln(pLn, 6, DLMS_DATA_TYPE_OCTET_STRING);
    Helpers::SetData(&m_Settings, buff, DLMS_DATA_TYPE_OCTET_STRING, ln);
    // Add attribute index.
    tmp = 2;
    Helpers::SetData(&m_Settings, buff, DLMS_DATA_TYPE_INT8, tmp);
    // Add version
    tmp = 0;
    if ((ret = Helpers::SetData(&m_Settings, buff, DLMS_DATA_TYPE_UINT16, tmp)) != 0)
    {
        return ret;
    }
    // Add start time
    if (unixTime)
    {
        tmp = start.ToUnixTime();
        if ((ret = Helpers::SetData(&m_Settings, buff, DLMS_DATA_TYPE_UINT32, tmp)) != 0)
        {
            return ret;
        }
    }
    else
    {
        tmp = start;
        if ((ret = Helpers::SetData(&m_Settings, buff, DLMS_DATA_TYPE_OCTET_STRING, tmp)) != 0)
        {
            return ret;
        }
    }
    // Add end time
    if (unixTime)
    {
        tmp = end.ToUnixTime();
        if ((ret = Helpers::SetData(&m_Settings, buff, DLMS_DATA_TYPE_UINT32, tmp)) != 0)
        {
            return ret;
        }
    }
    else
    {
        tmp = end;
        if ((ret = Helpers::SetData(&m_Settings, buff, DLMS_DATA_TYPE_OCTET_STRING, tmp)) != 0)
        {
            return ret;
        }
    }
    // Add array of read columns. Read All...
    buff.SetUInt8(0x01);
    // Add item count
    buff.SetUInt8(0x00);
    return Read(name, DLMS_OBJECT_TYPE_PROFILE_GENERIC, 2, &buff, reply);
}


int DLMSClient::ReadRowsByRange(
    DLMSProfileGeneric* pg,
    struct tm* start,
    struct tm* end,
    std::vector<ByteBuffer>& reply)
{
    if (pg == NULL || start == NULL || end == NULL)
    {
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    DateTime s(start), e(end);
    s.SetSkip(DATETIME_SKIPS_MS);
    e.SetSkip(DATETIME_SKIPS_MS);
    return ReadRowsByRange(pg, s, e, reply);
}

int DLMSClient::ReadRowsByRange(
    DLMSProfileGeneric* pg,
    struct tm* start,
    struct tm* end,
    std::vector<std::pair<DLMSObject*, DLMSCaptureObject*> >& columns,
    std::vector<ByteBuffer>& reply)
{
    return ReadRowsByRange(pg, start, end, reply);
}

int DLMSClient::GetServerAddressFromSerialNumber(
    unsigned long serialNumber,
    unsigned short logicalAddress,
    const char* formula)
{
    int value;
    // If formula is not given use default formula.
    // This formula is defined in DLMS specification.
    if (formula == NULL || strlen(formula) == 0)
    {
        value = SerialNumberCounter::Count(serialNumber, "SN % 10000 + 1000");
    }
    else
    {
        value = SerialNumberCounter::Count(serialNumber, formula);
    }
    if (logicalAddress != 0)
    {
        value |= logicalAddress << 14;
    }
    return value;
}

int  DLMSClient::GetServerAddress(unsigned long logicalAddress,
    unsigned long physicalAddress, unsigned char addressSize)
{
    if (addressSize < 4 && physicalAddress < 0x80
        && logicalAddress < 0x80)
    {
        return logicalAddress << 7 | physicalAddress;
    }
    if (physicalAddress < 0x4000 && logicalAddress < 0x4000)
    {
        return logicalAddress << 14 | physicalAddress;
    }
    return DLMS_ERROR_CODE_INVALID_PARAMETER;
}

char* DLMSClient::GetProtocolVersion()
{
    return m_Settings.GetProtocolVersion();
}

void DLMSClient::SetProtocolVersion(char* value)
{
    m_Settings.SetProtocolVersion(value);
}

int DLMSClient::ParsePushObjects(std::vector<DLMSVariant>& data, std::vector<std::pair<DLMSObject*, unsigned char> >& items)
{
    DLMSObject* obj;
    int ret;
    DLMSVariant tmp, value;
    DLMSVariant ln;
    for (std::vector<DLMSVariant>::iterator it = data.begin(); it != data.end(); ++it)
    {
        int classID = it->Arr[0].ToInteger() & 0xFFFF;
        if (classID > 0)
        {
            ln.Clear();
            if ((ret = DLMSClient::ChangeType(it->Arr[1], DLMS_DATA_TYPE_OCTET_STRING, ln)) != 0)
            {
                return ret;
            }
            obj = GetObjects().FindByLN((DLMS_OBJECT_TYPE)classID, ln.strVal);
            if (obj == NULL)
            {
                obj = DLMSObjectFactory::CreateObject((DLMS_OBJECT_TYPE)classID, ln.strVal);
                GetObjects().push_back(obj);
            }
            items.push_back(std::pair<DLMSObject*, unsigned char>(obj, it->Arr[2].ToInteger()));
        }
    }
    return 0;
}

void DLMSClient::SetCtoSChallenge(ByteBuffer& value)
{
    m_Settings.SetUseCustomChallenge(value.GetSize() != 0);
    m_Settings.SetCtoSChallenge(value);
}

ByteBuffer& DLMSClient::GetCtoSChallenge()
{
    return m_Settings.GetCtoSChallenge();
}

int DLMSClient::AccessRequest(struct tm* time, std::vector<DLMSAccessItem>& list, std::vector<ByteBuffer>& packets)
{
    int ret;
    if (list.size() == 0)
    {
        //Invalid parameter
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    if ((GetNegotiatedConformance() & DLMS_CONFORMANCE_ACCESS) == 0) {
        //Meter doesn't support access.
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    unsigned char ln[6];
    std::string tmp;
    ByteBuffer bb;
    if ((ret = Helpers::SetObjectCount((unsigned long)list.size(), bb)) == 0)
    {
        for (std::vector< DLMSAccessItem >::iterator it = list.begin(); it != list.end(); ++it)
        {
            if ((ret = bb.SetUInt8(it->GetCommand())) != 0 ||
                //Object type.
                (ret = bb.SetUInt16(it->GetTarget()->GetObjectType())) != 0)
            {
                break;
            }
            //LN
            Helpers::GetLogicalName(ln, tmp);
            bb.Set(it->GetTarget()->m_LN, 6);
            // Attribute ID.
            bb.SetUInt8(it->GetIndex());
        }
        //Data
        DLMS_DATA_TYPE type;
        Helpers::SetObjectCount((unsigned long)list.size(), bb);
        for (std::vector< DLMSAccessItem >::iterator it = list.begin(); it != list.end(); ++it)
        {
            if (it->GetCommand() == DLMS_ACCESS_SERVICE_COMMAND_TYPE_GET)
            {
                bb.SetUInt8(0);
            }
            else if (it->GetCommand() == DLMS_ACCESS_SERVICE_COMMAND_TYPE_SET ||
                it->GetCommand() == DLMS_ACCESS_SERVICE_COMMAND_TYPE_ACTION)
            {
                DLMSValueEventArg e(it->GetTarget(), it->GetIndex());
                if ((ret = it->GetTarget()->GetValue(m_Settings, e)) != 0)
                {
                    break;
                }
                DLMSVariant& value = e.GetValue();
                it->GetTarget()->GetDataType(it->GetIndex(), type);
                if (type == DLMS_DATA_TYPE_NONE)
                {
                    type = value.vt;
                }
                if ((ret = Helpers::SetData(&m_Settings, bb, type, value)) != 0)
                {
                    break;
                }
            }
            else
            {
                ret = DLMS_ERROR_CODE_INVALID_PARAMETER;
                break;
            }
        }
        if (ret == 0)
        {
            DLMSLNParameters p(&m_Settings, 0, DLMS_COMMAND_ACCESS_REQUEST, 0xFF, NULL, &bb, 0XFF, DLMS_COMMAND_NONE);
            p.SetTime(time);
            ret = DLMS::GetLnMessages(p, packets);
        }
    }
    return ret;
}

/// <summary>
/// Parse access response.
/// </summary>
/// <param name="list">Collection of access items.</param>
/// <param name="data">Received data from the meter.</param>
/// <returns>Collection of received data and status codes.</returns>
/// <seealso cref="AccessRequest"/>
int DLMSClient::ParseAccessResponse(std::vector<DLMSAccessItem>& list, ByteBuffer& data)
{
    unsigned char ch;
    unsigned long count;
    int ret;
    //Get count
    DataInfo info;
    DLMSVariant value;
    if ((ret = Helpers::GetObjectCount(data, count)) != 0)
    {
        return ret;
    }
    if (list.size() != count)
    {
        //List size and values size do not match.
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    for (std::vector< DLMSAccessItem >::iterator it = list.begin(); it != list.end(); ++it)
    {
        info.Clear();
        if ((ret = Helpers::GetData(&m_Settings, data, info, value)) != 0)
        {
            return ret;
        }
        it->SetValue(value);
    }
    //Get status codes.
    if ((ret = Helpers::GetObjectCount(data, count)) != 0)
    {
        return ret;
    }
    if (list.size() != count)
    {
        //List size and values size do not match.
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    for (std::vector< DLMSAccessItem >::iterator it = list.begin(); it != list.end(); ++it)
    {
        //Get access type.
        if ((ret = data.GetUInt8(&ch)) != 0)
        {
            break;
        }
        //Get status.
        if ((ret = data.GetUInt8(&ch)) != 0)
        {
            break;
        }
        it->SetError((DLMS_ERROR_CODE)ch);
        if (it->GetCommand() == DLMS_ACCESS_SERVICE_COMMAND_TYPE_GET && it->GetError() == DLMS_ERROR_CODE_OK)
        {
            if ((ret = UpdateValue(*it->GetTarget(), it->GetIndex(), it->GetValue())) != 0)
            {
                break;
            }
        }
    }
    return ret;
}

char* DLMSClient::GetManufacturerId()
{
    if (m_ManufacturerId[0] == 0)
    {
        return NULL;
    }
    return m_ManufacturerId;
}

void DLMSClient::SetManufacturerId(char value[3])
{
    if (value == NULL)
    {
        memset(m_ManufacturerId, 0, sizeof(m_ManufacturerId));
    }
    else
    {
        memcpy(m_ManufacturerId, value, sizeof(m_ManufacturerId));
    }
}

int DLMSClient::EncryptLandisGyrHighLevelAuthentication(ByteBuffer& password, ByteBuffer& seed, ByteBuffer& crypted)
{
    int pos, ret;
    if ((ret = crypted.Set(seed.GetData(), seed.GetSize())) != 0)
    {
        return ret;
    }
    unsigned char ch, ch2;
    for (pos = 0; pos != password.GetSize(); ++pos)
    {
        if ((ret = password.GetUInt8(pos, &ch)) != 0)
        {
            break;
        }
        if (ch != 0x30)
        {
            if ((ret = crypted.GetUInt8(pos, &ch2)) != 0)
            {
                break;
            }
            ch2 += ch - 0x30;
            //Convert to upper case.
            if (ch2 > 0x39)
            {
                ch2 += 7;
            }
            if ((ret = crypted.SetUInt8(pos, ch2)) != 0)
            {
                break;
            }
        }
    }
    return ret;
}

void DLMSClient::SetCustomerPIN(unsigned char* pin, int len)
{
    if (pin)
    {
        m_CustomerPIN.Clear();
        m_CustomerPIN.Set(pin, len);
    }
}

ByteBuffer DLMSClient::GetCustomerPIN()
{
    return m_CustomerPIN;
}

int DLMSClient::AARQRequest(std::vector<ByteBuffer>& packets)
{
    ByteBuffer buff(32);
    m_Settings.ResetBlockIndex();
    m_Settings.SetNegotiatedConformance((DLMS_CONFORMANCE)DLMS_CONFORMANCE_GENERAL_PROTECTION);
    packets.clear();
    int ret = DLMS::CheckInit(m_Settings);

    if (ret != 0)
    {
        return ret;
    }

    m_Settings.GetStoCChallenge().Clear();
    if (GetAutoIncreaseInvokeID())
    {
        m_Settings.SetInvokeID(0);
    }
    else
    {
        m_Settings.SetInvokeID(1);
    }

    // If authentication or ciphering is used.
    if (m_Settings.GetAuthentication() > DLMS_AUTHENTICATION_LOW)
    {
        if (!m_Settings.GetUseCustomChallenge())
        {
            ByteBuffer challenge;
            if ((ret = Secure::GenerateChallenge(m_Settings.GetAuthentication(), challenge)) != 0)
            {
                return ret;
            }
            m_Settings.SetCtoSChallenge(challenge);
        }
        else
        {
            //Custom handling for EDMI
            if (m_Settings.GetAuthentication() == DLMS_AUTHENTICATION_HIGH_GMAC && (memcmp(m_ManufacturerId, "EDM", 3) == 0))
            {
                DLMS_CONFORMANCE cnf = m_Settings.GetNegotiatedConformance();
                m_Settings.SetNegotiatedConformance((DLMS_CONFORMANCE)DLMS_CONFORMANCE_GENERAL_PROTECTION);

                const int challengesize = 32;
                unsigned char buffer[challengesize] = { 0 };

                if (m_Settings.GetClientAddress() == 80)
                {
                    memcpy(buffer, m_CustomerPIN.GetData(), 8);
                    for (int x = 8; x < challengesize; x++)
                    {
                        buffer[x] = rand();
                    }
                }
                else
                {
                    for (int x = 0; x < challengesize; x++)
                    {
                        buffer[x] = rand();
                    }
                }

                ByteBuffer challenge;
                challenge.Set(buffer, challengesize);

                m_Settings.SetCtoSChallenge(challenge);
            }
        }
    }
    else
    {
        m_Settings.GetCtoSChallenge().Clear();
    }

    if ((ret = APDU::GenerateAarq(m_Settings, m_Settings.GetCipher(), NULL, buff)) != 0)
    {
        return ret;
    }

    if (GetUseLogicalNameReferencing())
    {
        DLMSLNParameters p(&m_Settings, 0, DLMS_COMMAND_AARQ, 0, &buff, NULL, 0xff, DLMS_COMMAND_NONE);
        ret = DLMS::GetLnMessages(p, packets);
    }
    else
    {
        DLMSSNParameters p(&m_Settings, DLMS_COMMAND_AARQ, 0, 0, NULL, &buff);
        ret = DLMS::GetSnMessages(p, packets);
    }

    return ret;
}

int DLMSClient::GetReplytoEDMIStoC(std::vector<ByteBuffer>& packets, std::map<std::string, ByteBuffer> &debuginfo, bool debug)
{
    int ret;
    DLMSVariant name;
    ByteBuffer challenge;
    packets.clear();

    m_Settings.ResetBlockIndex();

    unsigned short fc = m_Settings.GetCipher()->GetFrameCounter();
    fc++;

    // IV
    char fc_str[9] = { 0 };
    unsigned char iv[12] = { 0 };
    memcpy(&iv[0], m_Settings.GetCipher()->GetSystemTitle().GetData(), 8);
    iv[8] = (fc >> 24) & 0xFF;
    iv[9] = (fc >> 16) & 0xFF;
    iv[10] = (fc >> 8) & 0xFF;
    iv[11] = fc & 0xFF;

    if(debug)
    {
        ByteBuffer val1;
        val1.Set(&iv[8], 4);
        debuginfo["IC"] = val1;

        ByteBuffer val2;
        val2.Set(iv, 12);
        debuginfo["IV"] = val2;
    }
    // IV

    //AAD
    size_t aad_len = 0;
    unsigned char aad[2 + 16 + 8 + 32] = { 0 };
    if (m_Settings.GetClientAddress() == 80)
    {
        aad[0] = DLMS_SECURITY_AUTHENTICATION;
        memcpy(&aad[1], m_Settings.GetCipher()->GetAuthenticationKey().GetData(), 16);
        memcpy(&aad[17], m_CustomerPIN.GetData(), 8);
        memcpy(&aad[25], m_Settings.GetStoCChallenge().GetData(), 32);
        aad_len = 1 + 16 + 8 + 32;
    }
    else
    {
        aad[0] = DLMS_SECURITY_AUTHENTICATION;
        memcpy(&aad[1], m_Settings.GetCipher()->GetAuthenticationKey().GetData(), 16);
        memcpy(&aad[17], m_Settings.GetStoCChallenge().GetData(), m_Settings.GetStoCChallenge().GetSize());
        aad_len = 1 + 16 + m_Settings.GetStoCChallenge().GetSize();
    }
    //AAD

    if(debug)
    {
        ByteBuffer val3;
        val3.Set(aad, aad_len);
        debuginfo["AAD"] = val3;
    }

    gcm_initialize();

    gcm_context* ctx = (gcm_context*)calloc(1, sizeof(gcm_context));

    gcm_setkey(ctx, m_Settings.GetCipher()->GetBlockCipherKey().GetData(), m_Settings.GetCipher()->GetBlockCipherKey().GetSize());

    unsigned char* outbuffer = (unsigned char*)calloc(1, m_Settings.GetStoCChallenge().GetSize());
    size_t olen = 0;
    unsigned char tagbuffer[12];
    size_t tlen = 12;

    gcm_start(ctx, 1, iv, sizeof(iv), aad, aad_len);

    gcm_finish(ctx, tagbuffer, tlen);

    if(debug)
    {
        ByteBuffer val4;
        val4.Set(tagbuffer, tlen);
        debuginfo["Tag"] = val4;
    }

    unsigned char* fStoCbuffer = (unsigned char*)calloc(1, (1 + 4 + 12));
    fStoCbuffer[0] = DLMS_SECURITY_AUTHENTICATION;
    fStoCbuffer[1] = (fc >> 24) & 0xFF;
    fStoCbuffer[2] = (fc >> 16) & 0xFF;
    fStoCbuffer[3] = (fc >> 8) & 0xFF;
    fStoCbuffer[4] = fc & 0xFF;
    memcpy(&fStoCbuffer[5], tagbuffer, tlen);

    if(debug)
    {
        ByteBuffer val5;
        val5.Set(fStoCbuffer, (1 + 4 + 12));
        debuginfo["f(StoC)"] = val5;

        ByteBuffer val6 = m_Settings.GetCipher()->GetSystemTitle();
        debuginfo["SysT"] = val6;

        ByteBuffer val7 = m_Settings.GetCipher()->GetAuthenticationKey();
        debuginfo["AuthKey"] = val7;

        ByteBuffer val8 = m_Settings.GetCipher()->GetBlockCipherKey();
        debuginfo["CipherKey"] = val8;

        ByteBuffer val9;
        val9.Set(&fStoCbuffer[0], 1);
        debuginfo["SC"] = val9;

        ByteBuffer val10 = m_Settings.GetStoCChallenge();
        debuginfo["StoC"] = val10;
    }

    challenge.Set(fStoCbuffer, (1 + 4 + 12));

    DLMSVariant data = challenge;
    name = "0.0.40.0.0.255";

    m_Settings.GetCipher()->SetFrameCounter(fc);

    ret = Method(name, DLMS_OBJECT_TYPE_ASSOCIATION_LOGICAL_NAME, 1, data, packets);

    free(outbuffer);
    free(ctx);

    return ret;
}

int DLMSClient::ValidateEDMIAutheticationResponse(ByteBuffer& reply)
{
    DataInfo info;
    unsigned char ch;
    bool equals = false;
    ByteBuffer secret;
    int ret;
    unsigned long ic = 0;
    DLMSVariant value;
    ret = Helpers::GetData(&m_Settings, reply, info, value);

    if (ret != 0)
    {
        return ret;
    }

    if (value.vt != DLMS_DATA_TYPE_NONE)
    {
        if (m_Settings.GetAuthentication() == DLMS_AUTHENTICATION_HIGH_GMAC)
        {
            secret = m_Settings.GetSourceSystemTitle();
            ByteBuffer bb;
            bb.Set(value.byteArr, value.GetSize());
            if ((ret = bb.GetUInt8(&ch)) != 0)
            {
                return ret;
            }
            if ((ret = bb.GetUInt32(&ic)) != 0)
            {
                return ret;
            }
        }

        ByteBuffer challenge;
        ByteBuffer cToS = m_Settings.GetCtoSChallenge();

        ret = Secure::SecureEncrypt(m_Settings, m_Settings.GetCipher(), ic, cToS, secret, challenge);

        if (ret != 0)
        {
            return ret;
        }

        equals = challenge.Compare(value.byteArr, value.GetSize());
    }
    else
    {
        // Server did not accept CtoS.
    }

    if (!equals)
    {
        //ParseApplicationAssociationResponse failed. Server to Client do not match.
        //return DLMS_ERROR_CODE_AUTHENTICATION_FAILURE;
    }

    return 0;
}

int DLMSClient::DecryptAesKeyWrapping(ByteBuffer& data, ByteBuffer& kek, ByteBuffer& reply)
{
    const unsigned char WRAP_IV[] = { 0xA6, 0xA6, 0xA6, 0xA6, 0xA6, 0xA6, 0xA6, 0xA6 };
    unsigned char a[8];
    ByteBuffer buf, buf2;
    signed char j, i;
    unsigned char k, v, n;
    unsigned short t;
    // Amount of 64-bit blocks.
    n = (unsigned char)(data.GetSize() >> 3);
    short x = (unsigned char)(data.GetSize() >> 3);

    if (kek.GetSize() != 16 || data.GetSize() != n * 8)
    {
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }

    reply.Capacity(32);
    reply.SetSize(32);
    reply.SetPosition(0);

    memcpy(a, data.GetData(), sizeof(WRAP_IV));
    memcpy(reply.GetData(), data.GetData() + sizeof(WRAP_IV), data.GetSize() - sizeof(WRAP_IV));

    if (--n == 0)
    {
        n = 1;
    }
    for (j = 5; j >= 0; j--)
    {
        for (i = n; i >= 1; i--)
        {
            buf2.SetSize(0);
            buf2.Set(&kek);
            buf.SetSize(0);
            buf.Set(a, sizeof(WRAP_IV));
            buf.Set(reply.GetData() + 8 * (i - 1), 8);
            t = n * j + i;
            for (k = 1; t != 0; k++)
            {
                v = (unsigned char)t;
                buf.GetData()[sizeof(WRAP_IV) - k] ^= v;
                t = (unsigned short)(t >> 8);
            }
            Cipher::Aes1Decrypt(buf, buf2);
            memcpy(a, buf.GetData(), 8);
            memcpy(reply.GetData() + 8 * (i - 1), buf.GetData() + 8, 8);
        }
    }

    if (memcmp(a, WRAP_IV, sizeof(WRAP_IV)) != 0)
    {
        reply.SetSize(32);
        return DLMS_ERROR_CODE_FALSE;
    }

    return 0;
}

void DLMSClient::UnwrapKeys(const unsigned char* random_octet, const int random_octet_len, const unsigned char* wrapped_key, const int wrapped_key_len, ByteBuffer& reply)
{
    unsigned char aes_key_raw[16] = { 0 };
    unsigned char aes_key[SHA256_BLOCK_SIZE] = { 0 };
    unsigned char aes_key_trimmed[16] = { 0 };

    memcpy(&aes_key_raw[0], m_CustomerPIN.GetData(), 8);
    memcpy(&aes_key_raw[8], random_octet, random_octet_len);

    SHA256_CTX ctx;
    sha256_init(&ctx);
    sha256_update(&ctx, aes_key_raw, sizeof(aes_key_raw));
    sha256_final(&ctx, aes_key);
    memcpy(&aes_key_trimmed[0], &aes_key[0], 16);

     ByteBuffer data;
    ByteBuffer kek;

    data.Set(wrapped_key, wrapped_key_len);
    kek.Set(aes_key_trimmed, sizeof(aes_key_trimmed));

    DecryptAesKeyWrapping(data, kek, reply);
}
