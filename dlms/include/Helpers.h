#ifndef HELPERS_H
#define HELPERS_H

#include "ErrorCodes.h"
#include "DLMSVariant.h"
#include "DataInfo.h"

class DLMSSettings;

class Helpers
{
    /*
    * Convert char hex value to byte value.
    * @param c Character to convert hex.
    * @return Byte value of hex char value.
    */
    static unsigned char GetValue(char c);

    static int GetCompactArrayItem(
        DLMSSettings* settings,
        ByteBuffer& buff,
        std::vector< DLMSVariant>& dt,
        std::vector< DLMSVariant>& list,
        int len);

    static int GetCompactArrayItem(
        DLMSSettings* settings,
        ByteBuffer& buff,
        DLMS_DATA_TYPE dt,
        std::vector< DLMSVariant>& list,
        int len);

    static int GetDataTypes(
        ByteBuffer& buff,
        std::vector<DLMSVariant>& cols,
        int len);
    static int AppendDataTypeAsXml(
        std::vector<DLMSVariant>& cols,
        DataInfo info);


    /**
     * Get compact array value from DLMS data.
     *
     * @param buff
     *            Received DLMS data.
     * @param info
     *            Data info.
     * @return parsed UInt16 value.
     */
    static int GetCompactArray(
        DLMSSettings* settings,
        ByteBuffer& buff,
        DataInfo& info,
        DLMSVariant& value);
public:

    /**
    * Convert object to DLMS bytes.
    *
    * buff : Byte buffer where data is write.
    * type : Data type.
    * value : Added Value.
    */
    static int SetData2(DLMSSettings* settings, ByteBuffer& buff, DLMS_DATA_TYPE type, DLMSVariant value);

    /**
    * Convert object to DLMS bytes.
    *
    * buff : Byte buffer where data is write.
    * type : Data type.
    * value : Added Value.
    */
    static int SetData(DLMSSettings* settings, ByteBuffer& buff, DLMS_DATA_TYPE type, DLMSVariant& value);

    /*
    * Get data from DLMS frame.
    *
    * data: received data.
    * info: Data info.
    * value: Received data.
    @return Error code.
    */
    static int GetData(DLMSSettings* settings, ByteBuffer& data, DataInfo& info, DLMSVariant& value);

    static void GetLogicalName(unsigned char* buff, std::string& ln);

    static void GetLogicalName(ByteBuffer& buff, std::string& ln);

    /////////////////////////////////////////////////////////////////////////////
    //Set logical name from std::string.
    /////////////////////////////////////////////////////////////////////////////
    static int SetLogicalName(const char* name, DLMSVariant& value);

    /////////////////////////////////////////////////////////////////////////////
    //Set logical name from std::string.
    /////////////////////////////////////////////////////////////////////////////
    static int SetLogicalName(const char* name, unsigned char ln[6]);

    /////////////////////////////////////////////////////////////////////////////
    // Get object count. If first byte is 0x80 or higger it will tell bytes count.
    // data ; received data.
    // Returns Object count.
    /////////////////////////////////////////////////////////////////////////////
    static int GetObjectCount(ByteBuffer& data, unsigned long& count);

    /**
        * Return how many bytes object count takes.
        *
        * @param count
        *            Value
        * @return Value size in bytes.
        */
    static unsigned char GetObjectCountSizeInBytes(unsigned long count);

    /////////////////////////////////////////////////////////////////////////////
    // Set item count.
    /////////////////////////////////////////////////////////////////////////////
    // count : Item count.
    // buff : Byte buffer.
    /////////////////////////////////////////////////////////////////////////////
    static int SetObjectCount(unsigned long count, ByteBuffer& buff);

    static std::vector< std::string > Split(std::string& s, char separator);

    static std::vector< std::string > Split(std::string& s, std::string separators, bool ignoreEmpty);

    static void Replace(std::string& str, std::string oldString, std::string newString);

    /////////////////////////////////////////////////////////////////////////////
    // Trim from start.
    /////////////////////////////////////////////////////////////////////////////
    static std::string& ltrim(std::string& s);

    /////////////////////////////////////////////////////////////////////////////
    // Trim from end.
    /////////////////////////////////////////////////////////////////////////////
    static std::string& rtrim(std::string& s);

    /////////////////////////////////////////////////////////////////////////////
    // Trim from both ends
    /////////////////////////////////////////////////////////////////////////////
    static std::string& trim(std::string& s);

    static std::string BytesToHex(unsigned char* pBytes, int count);

    static std::string BytesToHex(unsigned char* pBytes, int count, char addSpaces);

    /**
     * Convert std::string to byte array.
     *
     * @param value
     *            Hex std::string.
     * @param buffer
     *            byte array.
     * @return Occurred error.
     */
    static void HexToBytes(std::string& value, ByteBuffer& buffer);

    static void Write(char* fileName, char* pData, int len);

    static void Write(std::string fileName, std::string data);

    static bool GetBits(unsigned char& tmp, unsigned char BitMask);

    static inline bool StringCompare(const char* c1, const char* c2);

    /**
    * Get data type in bytes.
    *
    * @param type
    *            Data type.
    * @return Size of data type in bytes.
    */
    static int GetDataTypeSize(DLMS_DATA_TYPE type);

    /**
    * Convert Bit string to DLMS bytes.
    *
    * buff
    *            Byte buffer where data is write.
    * value
    *            Added value.
    */
    static int SetBitString(ByteBuffer& buff, DLMSVariant& value, bool addCount);

    //Reserved for internal use.
    static void ToBitString(ByteBuffer& sb, unsigned char value, int count);

    static std::string GeneralizedTime(struct tm* date);

    //Convert int value to string.
    static std::string IntToString(int value);
    //Join list of strings to one string.
    static void Join(std::string separator, std::vector< std::string >& list, std::string& res);

    // Reserved for internal use.
    static unsigned char SwapBits(unsigned char value);

    static bool checkLuhn(const std::string userdata);

    static std::string hexString(std::string decimalString);

    static bool isFloat(std::string realString);

};
#endif
