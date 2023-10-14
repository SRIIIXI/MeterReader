#ifndef DLMSDEMANDREGISTER_H
#define DLMSDEMANDREGISTER_H

#include "DLMSObject.h"
#ifndef DLMS_IGNORE_DEMAND_REGISTER

class DLMSDemandRegister : public DLMSObject
{
    DLMSVariant m_CurrentAverageValue;
    DLMSVariant m_LastAverageValue;
    signed char m_Scaler;
    unsigned char m_Unit;
    DLMSVariant m_Status;
    DateTime m_CaptureTime;
    DateTime m_StartTimeCurrent;
    int m_NumberOfPeriods;
    unsigned long m_Period;

protected:
    bool IsRead(int index);

public:
    /**
     Constructor.
    */
    DLMSDemandRegister();

    /**
     Constructor.

     @param ln Logical Name of the object.
    */
    DLMSDemandRegister(std::string ln);

    /**
     Constructor.

     @param ln Logical Name of the object.
     @param sn Short Name of the object.
    */
    DLMSDemandRegister(std::string ln, unsigned short sn);

    /**
     Current Average value of COSEM Data object.
    */
    DLMSVariant& GetCurrentAverageValue();
    void SetCurrentAverageValue(DLMSVariant& value);
    void SetCurrentAverageValue(DLMSVariant value);

    /**
     Last Average value of COSEM Data object.
    */
    DLMSVariant& GetLastAverageValue();
    void SetLastAverageValue(DLMSVariant& value);
    void SetLastAverageValue(DLMSVariant value);

    /**
     Scaler of COSEM Register object.
    */
    double GetScaler();
    void SetScaler(double value);

    // Unit of COSEM Register object.
    int GetUnit();
    void SetUnit(unsigned char value);

    /**
     Scaler of COSEM Register object.
    */
    DLMSVariant& GetStatus();
    void SetStatus(DLMSVariant& value);
    void SetStatus(DLMSVariant value);

    /**
     Capture time of COSEM Register object.
    */
    DateTime& GetCaptureTime();
    void SetCaptureTime(DateTime value);

    /**
     Current start time of COSEM Register object.
    */
    DateTime& GetStartTimeCurrent();
    void SetStartTimeCurrent(DateTime& value);

    unsigned long GetPeriod();
    void SetPeriod(unsigned long value);

    int GetNumberOfPeriods();
    void SetNumberOfPeriods(int value);

    /*
     * Reset value.
     */
    int Reset(DLMSClient* client,
        std::vector<ByteBuffer>& reply);

    /*
     * Next period.
     */
    int NextPeriod(DLMSClient* client,
        std::vector<ByteBuffer>& reply);

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

    // Returns amount of attributes.
    int GetAttributeCount();

    // Returns amount of methods.
    int GetMethodCount();

    int Invoke(DLMSSettings& settings, DLMSValueEventArg& e);

    int GetDataType(int index, DLMS_DATA_TYPE& type);

    int GetValue(DLMSSettings& settings, DLMSValueEventArg& e);

    int SetValue(DLMSSettings& settings, DLMSValueEventArg& e);
};
#endif
#endif
