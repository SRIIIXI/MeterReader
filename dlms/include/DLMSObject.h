#ifndef DLMS_OBJECT_H
#define DLMS_OBJECT_H

#include <string.h>
#include <vector>
#include <map>
#include <assert.h>
#include "AttributeCollection.h"
#include "DLMSVariant.h"
#include "IDLMSBase.h"
#include "Helpers.h"

class DLMSObjectCollection;

class DLMSObject : public IDLMSBase
{
    friend class DLMSClient;
    friend class DLMSObjectCollection;
    friend class DLMSObjectFactory;
    friend class DLMSAssociationLogicalName;
    friend class DLMSAssociationShortName;
    friend class DLMSProfileGeneric;

    AttributeCollection m_Attributes;
    AttributeCollection m_MethodAttributes;
    void Initialize(short sn, unsigned short class_id, unsigned char version, ByteBuffer* pLogicalName);
    std::string m_Description;
    DLMS_OBJECT_TYPE m_ObjectType;
protected:
    unsigned short m_Version;
    std::map<int, time_t> m_ReadTimes;
    unsigned short m_SN;
    unsigned char m_LN[6];

    /*
     * Is attribute read. This can be used with static attributes to make
     * meter reading faster.
     */
    bool IsRead(int index);
    bool CanRead(int index);
    static int GetLogicalName(DLMSObject * target, DLMSVariant& value);
    static int SetLogicalName(DLMSObject * target, DLMSVariant& value);
    static int SetLogicalName(DLMSObject* target, std::string& value);
public:

    static bool IsLogicalNameEmpty(unsigned char* pLN)
    {
        const unsigned char EmptyLN[] = { 0, 0, 0, 0, 0, 0 };
        return memcmp(pLN, EmptyLN, 6) == 0;
    }

    DLMSObject(void);
    DLMSObject(DLMS_OBJECT_TYPE type);

    //SN Constructor.
    DLMSObject(DLMS_OBJECT_TYPE type, std::string& ln, unsigned short sn);

    //LN Constructor.
    DLMSObject(DLMS_OBJECT_TYPE type, std::string& ln);
    DLMSObject(short sn, unsigned short class_id, unsigned char version, ByteBuffer& ln);

    virtual ~DLMSObject(void);

    //Get Object's Logical or Short Name as a std::string.
    DLMSVariant GetName();

    int SetName(DLMSVariant& value);

    //Get Object's Interface class type.
    DLMS_OBJECT_TYPE GetObjectType();

    //Get Object's Short Name.
    unsigned short GetShortName();

    //Set Object's Short Name.
    void SetShortName(unsigned short value);

    //Get Object's Logical Name.
    void GetLogicalName(std::string& ln);

    void SetVersion(unsigned short value);
    unsigned short GetVersion();

    AttributeCollection& GetAttributes();
    AttributeCollection& GetMethodAttributes();
    virtual int SetDataType(int index, DLMS_DATA_TYPE type);
    virtual int GetDataType(int index, DLMS_DATA_TYPE& type);

    virtual int GetUIDataType(int index, DLMS_DATA_TYPE& type);
    void SetUIDataType(int index, DLMS_DATA_TYPE type);

    DLMS_ACCESS_MODE GetAccess(int index);
    void SetAccess(int index, DLMS_ACCESS_MODE access);
    DLMS_METHOD_ACCESS_MODE GetMethodAccess(int index);
    void SetMethodAccess(int index, DLMS_METHOD_ACCESS_MODE access);


    //Get description of the object.
    std::string& GetDescription();

    //Set description of the object.
    void SetDescription(std::string& value);

    //Get values as std::string.
    virtual void GetValues(std::vector<std::string>& values)
    {
        assert(0);
    }

    /////////////////////////////////////////////////////////////////////////
    // Returns collection of attributes to read.
    //
    // If attribute is static and already read or device is returned
    // HW error it is not returned.
    //
    // all: All items are returned even if they are read already.
    // attributes: Collection of attributes to read.
    virtual void GetAttributeIndexToRead(bool all, std::vector<int>& attributes)
    {
        assert(0);
    }

    /////////////////////////////////////////////////////////////////////////
    // Returns collection of attributes to read.
    //
    // If attribute is static and already read or device is returned
    // HW error it is not returned.
    //
    // all: All items are returned even if they are read already.
    // attributes: Collection of attributes to read.
    virtual void GetAttributeIndexToRead(std::vector<int>& attributes)
    {
        GetAttributeIndexToRead(false, attributes);
    }

    // Returns amount of attributes.
    virtual int GetAttributeCount()
    {
        assert(0);
        return 1;
    }

    // Returns amount of methods.
    virtual int GetMethodCount()
    {
        assert(0);
        return 0;
    }

    // Returns value of given attribute.
    virtual int GetValue(DLMSSettings& settings, DLMSValueEventArg& e)
    {
        e.SetError(DLMS_ERROR_CODE_READ_WRITE_DENIED);
        return DLMS_ERROR_CODE_OK;
    }

    // Set value of given attribute.
    virtual int SetValue(DLMSSettings& settings, DLMSValueEventArg& e)
    {
        e.SetError(DLMS_ERROR_CODE_READ_WRITE_DENIED);
        return DLMS_ERROR_CODE_OK;
    }

    // Set value of given attribute.
    virtual int Invoke(DLMSSettings& settings, DLMSValueEventArg& e)
    {
        e.SetError(DLMS_ERROR_CODE_READ_WRITE_DENIED);
        return DLMS_ERROR_CODE_OK;
    }
};

class CGXDLMSCustomObject : public DLMSObject
{
public:
    CGXDLMSCustomObject(DLMS_OBJECT_TYPE type) : DLMSObject(type)
    {
    }
};

#endif
