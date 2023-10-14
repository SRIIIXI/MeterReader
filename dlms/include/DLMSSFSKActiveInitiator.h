#ifndef DLMS_SFSK_ACTIVE_INITIATOR_H
#define DLMS_SFSK_ACTIVE_INITIATOR_H

#include "Ignore.h"
#ifndef DLMS_IGNORE_SFSK_ACTIVE_INITIATOR
#include "DLMSObject.h"

class DLMSSFSKActiveInitiator : public DLMSObject
{
    /**
     * System title of active initiator.
     */
    ByteBuffer m_SystemTitle;
    /**
     * MAC address of active initiator.
     */
    uint16_t m_MacAddress;
    /**
     * L SAP selector of active initiator.
     */
    unsigned char m_LSapSelector;

public:
    //Constructor.
    DLMSSFSKActiveInitiator();

    //SN Constructor.
    DLMSSFSKActiveInitiator(std::string ln, unsigned short sn);

    //LN Constructor.
    DLMSSFSKActiveInitiator(std::string ln);

    /**
    * Returns System title of active initiator.
    */
    ByteBuffer& GetSystemTitle();

    /**
     * @param value
     *            System title of active initiator.
     */
    void SetSystemTitle(ByteBuffer& value);

    /**
     * Returns MAC address of active initiator.
     */
    uint16_t GetMacAddress();

    /**
     * @param value
     *            MAC address of active initiator.
     */
    void SetMacAddress(uint16_t value);

    /**
     * Returns L SAP selector of active initiator.
     */
    unsigned char GetLSapSelector();

    /**
     * @param value
     *            L SAP selector of active initiator.
     */
    void SetLSapSelector(unsigned char value);

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
