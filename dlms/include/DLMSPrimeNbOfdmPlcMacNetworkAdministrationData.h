#ifndef DLMS_PRIME_NBOFDMPLC_MACNETWORK_ADMINISTRATION_DATA_H
#define DLMS_PRIME_NBOFDMPLC_MACNETWORK_ADMINISTRATION_DATA_H

#include "Ignore.h"
#ifndef DLMS_IGNORE_PRIME_NB_OFDM_PLC_MAC_NETWORK_ADMINISTRATION_DATA
#include "DLMSObject.h"
#include "MacMulticastEntry.h"
#include "MacDirectTable.h"
#include "MacAvailableSwitch.h"
#include "MacPhyCommunication.h"

class DLMSPrimeNbOfdmPlcMacNetworkAdministrationData : public DLMSObject
{
    /**
     * List of entries in multicast switching table.
     */
    std::vector<MacMulticastEntry*> m_MulticastEntries;
    /**
     * Switch table.
     */
    std::vector<short> m_SwitchTable;

    /**
     * List of entries in multicast switching table.
     */
    std::vector<MacDirectTable*> m_DirectTable;
    /**
     * List of available switches.
     */
    std::vector<MacAvailableSwitch*> m_AvailableSwitches;
    /**
     * List of PHY communication parameters.
     */
    std::vector<MacPhyCommunication*> m_Communications;

    int Invoke(DLMSSettings& settings, DLMSValueEventArg& e);

    int GetMulticastEntries(DLMSSettings& settings, DLMSValueEventArg& e);
    int GetSwitchTable(DLMSSettings& settings, DLMSValueEventArg& e);
    int GetDirectTable(DLMSSettings& settings, DLMSValueEventArg& e);
    int GetAvailableSwitches(DLMSSettings& settings, DLMSValueEventArg& e);
    int GetCommunications(DLMSSettings& settings, DLMSValueEventArg& e);


    int SetMulticastEntry(std::vector<DLMSVariant>& list);
    int SetSwitchTable(std::vector<DLMSVariant>& list);
    int SetDirectTable(std::vector<DLMSVariant>& list);
    int SetAvailableSwitches(std::vector<DLMSVariant>& list);
    int SetCommunications(std::vector<DLMSVariant>& list);
public:
    //Constructor.
    DLMSPrimeNbOfdmPlcMacNetworkAdministrationData();

    //SN Constructor.
    DLMSPrimeNbOfdmPlcMacNetworkAdministrationData(std::string ln, unsigned short sn);

    //LN Constructor.
    DLMSPrimeNbOfdmPlcMacNetworkAdministrationData(std::string ln);

    //Destructor.
    ~DLMSPrimeNbOfdmPlcMacNetworkAdministrationData();

    /**
    * Returns List of entries in multicast switching table.
    */
    std::vector<MacMulticastEntry*>& GetMulticastEntries();

    /**
     * Returns Switch table.
     */
    std::vector<short>& GetSwitchTable();

    /**
     * Returns List of entries in multicast switching table.
     */
    std::vector<MacDirectTable*>& GetDirectTable();


    /**
     * Returns List of available switches.
     */
    std::vector<MacAvailableSwitch*>& GetAvailableSwitches();

    /**
     * Returns List of PHY communication parameters.
     */
    std::vector<MacPhyCommunication*>& GetCommunications();

    int Reset(DLMSClient* client, std::vector<ByteBuffer>& reply);

    void ClearMulticastEntry();

    void ClearDirectTable();

    void ClearAvailableSwitches();

    void ClearCommunications();

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
