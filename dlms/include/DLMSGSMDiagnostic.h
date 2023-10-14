#ifndef GXDLMSGSMDIAGNOSTIC_H
#define GXDLMSGSMDIAGNOSTIC_H

#ifndef DLMS_IGNORE_GSM_DIAGNOSTIC
#include "DLMSObject.h"
#include "AdjacentCell.h"
#include "DLMSGSMCellInfo.h"

class DLMSGSMDiagnostic : public DLMSObject
{
    /**
    * Name of network operator.
    */
    std::string m_Operator;

    /**
    * Registration status of the modem.
    */
    DLMS_GSM_STATUS m_Status;

    /**
    * Registration status of the modem.
    */
    DLMS_GSM_CIRCUIT_SWITCH_STATUS m_CircuitSwitchStatus;

    /**
    * Registration status of the modem.
    */
    DLMS_GSM_PACKET_SWITCH_STATUS m_PacketSwitchStatus;

    /**
    * Registration status of the modem.
    */
    DLMSGSMCellInfo m_CellInfo;

    /**
    * Adjacent cells.
    */
    std::vector<AdjacentCell*> m_AdjacentCells;

    /**
    * Date and time when the data have been last captured.
    */
    DateTime m_CaptureTime;
public:
    //Constructor.
    DLMSGSMDiagnostic();

    //SN Constructor.
    DLMSGSMDiagnostic(std::string ln, unsigned short sn);

    //LN Constructor.
    DLMSGSMDiagnostic(std::string ln);


    //Destructor.
    ~DLMSGSMDiagnostic();

    /**
    * @return Name of network operator.
    */
    std::string& GetOperator();

    /**
    * @param value
    *            Name of network operator.
    */
    void SetOperator(std::string& value);

    /**
    * @return Registration status of the modem.
    */
    DLMS_GSM_STATUS GetStatus();

    /**
    * @param value
    *            Registration status of the modem.
    */
    void SetStatus(DLMS_GSM_STATUS value);

    /**
    * @return Registration status of the modem.
    */
    DLMS_GSM_CIRCUIT_SWITCH_STATUS GetCircuitSwitchStatus();

    /**
    * @param value
    *            Registration status of the modem.
    */
    void SetCircuitSwitchStatus(DLMS_GSM_CIRCUIT_SWITCH_STATUS value);

    /**
    * @return Registration status of the modem.
    */
    DLMS_GSM_PACKET_SWITCH_STATUS GetPacketSwitchStatus();

    /**
    * @param value
    *            Registration status of the modem.
    */
    void SetPacketSwitchStatus(DLMS_GSM_PACKET_SWITCH_STATUS value);

    DLMSGSMCellInfo& GetCellInfo();

    /**
    * @return Adjacent cells.
    */
    std::vector<AdjacentCell*>& GetAdjacentCells();

    /**
    * @return Date and time when the data have been last captured.
    */
    DateTime& GetCaptureTime();

    /**
    * @param value
    *            Date and time when the data have been last captured.
    */
    void SetCaptureTime(DateTime& value);

    // Returns amount of attributes.
    int GetAttributeCount();

    // Returns amount of methods.
    int GetMethodCount();

    //Get attribute values of object.
    void GetValues(std::vector<std::string>& values);

    /////////////////////////////////////////////////////////////////////////
    // Returns collection of attributes to read.
    //
    // If attribute is static and already read or device is returned
    // HW error it is not returned.
    //
    // all: All items are returned even if they are read already.
    // attributes: Collection of attributes to read.
    void GetAttributeIndexToRead(bool all, std::vector<int>& attributes);

    int GetDataType(int index, DLMS_DATA_TYPE& type);

    // Returns value of given attribute.
    int GetValue(DLMSSettings& settings, DLMSValueEventArg& e);

    // Set value of given attribute.
    int SetValue(DLMSSettings& settings, DLMSValueEventArg& e);
};
#endif
#endif
