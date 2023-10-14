#ifndef DLMS_PROFILE_GENERIC_H
#define DLMS_PROFILE_GENERIC_H

#include "Ignore.h"

#ifndef DLMS_IGNORE_PROFILE_GENERIC
#include "DLMSCaptureObject.h"
#include "DLMSRegister.h"

enum GX_SORT_METHOD
{
    /**
     First in first out

     When circle buffer is full first item is removed.
    */
    DLMS_SORT_METHOD_FIFO = 1,
    /**
     Last in first out.

     When circle buffer is full last item is removed.
    */
    DLMS_SORT_METHOD_LIFO = 2,
    /**
     Largest is first.
    */
    DLMS_SORT_METHOD_LARGEST = 3,
    /**
     Smallest is first.
    */
    DLMS_SORT_METHOD_SMALLEST = 4,
    /**
     Nearst to zero is first.
    */
    DLMS_SORT_METHOD_NEAREST_TO_ZERO = 5,
    /**
     Farest from zero is first.
    */
    DLMS_SORT_METHOD_FAREST_FROM_ZERO = 6
};

class DLMSServer;

class DLMSProfileGeneric : public DLMSObject
{
private:
    std::vector< std::vector<DLMSVariant> > m_Buffer;
    std::vector<std::pair<DLMSObject*, DLMSCaptureObject*> > m_CaptureObjects;
    int m_CapturePeriod;
    GX_SORT_METHOD m_SortMethod;
    DLMSObject* m_SortObject;
    unsigned long m_ProfileEntries;
    unsigned long m_EntriesInUse;

    int m_SortObjectAttributeIndex;
    int m_SortObjectDataIndex;

    int GetColumns(ByteBuffer& data);
    int GetData(
        DLMSSettings& settings,
        DLMSValueEventArg& e,
        std::vector< std::vector<DLMSVariant> >& table,
        std::vector<std::pair<DLMSObject*, DLMSCaptureObject*> >& columns,
        ByteBuffer& data);

    int GetProfileGenericData(
        DLMSSettings& settings,
        DLMSValueEventArg& e,
        ByteBuffer& reply);

    /**
     * Get selected columns.
     *
     * @param parameters
     *            Received data.
     * @param cols
     *            Selected columns.
     */
    int GetSelectedColumns(
        std::vector<DLMSVariant>& cols,
        std::vector<std::pair<DLMSObject*, DLMSCaptureObject*> >& columns);
public:
    /*
     * Add new capture object (column) to the profile generic.
     */
    int AddCaptureObject(
        DLMSObject* pObj,
        int attributeIndex,
        int dataIndex);

    /**
    * Get selected columns from parameters.
    *
    * @param selector
    *            Is read by entry or range.
    * @param parameters
    *            Received parameters where columns information is found.
    * @return Selected columns.
    */
    int GetSelectedColumns(
        int selector,
        DLMSVariant& parameters,
        std::vector<std::pair<DLMSObject*, DLMSCaptureObject*> >& columns);

    /**
     Constructor.
    */
    DLMSProfileGeneric();

    //SN Constructor.
    DLMSProfileGeneric(std::string ln, unsigned short sn);

    /**
     Constructor.

     @param ln Logical Name of the object.
    */
    DLMSProfileGeneric(std::string ln);

    /**
     Destructor.
    */
    virtual ~DLMSProfileGeneric();

    /**
     Data of profile generic.
    */
    std::vector< std::vector<DLMSVariant> >& GetBuffer();

    /**
     Captured Objects.
    */
    std::vector<std::pair<DLMSObject*, DLMSCaptureObject*> >& GetCaptureObjects();

    /**
     How often values are captured.
    */
    int GetCapturePeriod();
    void SetCapturePeriod(int value);

    /**
     How columns are sorted.
    */
    GX_SORT_METHOD GetSortMethod();
    void SetSortMethod(GX_SORT_METHOD value);

    /**
     Column that is used for sorting.
    */
    DLMSObject* GetSortObject();
    void SetSortObject(DLMSObject* value);

    /**
     Entries (rows) in Use.
    */
    unsigned long GetEntriesInUse();
    void SetEntriesInUse(unsigned long value);

    /**
     Maximum Entries (rows) count.
    */
    unsigned long GetProfileEntries();
    void SetProfileEntries(unsigned long value);
    /**
    Attribute index of sort object.
    */
    int GetSortObjectAttributeIndex();
    void SetSortObjectAttributeIndex(int value);

    /**
     Data index of sort object.
    */
    int GetSortObjectDataIndex();
    void SetSortObjectDataIndex(int value);

    /**
     Clears the buffer.
    */
    void Reset();

    /*
    * Copies the values of the objects to capture into the buffer by reading
    * capture objects.
    */
    int Capture(DLMSServer* server);

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

    //Returns amount of attributes.
    int GetAttributeCount();

    //Returns amount of methods.
    int GetMethodCount();

    int GetDataType(int index, DLMS_DATA_TYPE& type);

    int Invoke(DLMSSettings& settings, DLMSValueEventArg& e);

    /*
    * Returns value of given attribute.
    */
    int GetValue(DLMSSettings& settings, DLMSValueEventArg& e);

    /*
     * Set value of given attribute.
     */
    int SetValue(DLMSSettings& settings, DLMSValueEventArg& e);

    /**
    * Clears the buffer.
    *
    * @param client
    *            DLMS client.
    * @param reply
    *            Action bytes.
    * @return Result code.
    */
    int Reset(DLMSClient& client, std::vector<ByteBuffer>& reply);

    /**
    * Copies the values of the objects to capture into the buffer by reading
    * each capture object.
    *
    * @param client
    *            DLMS client.
    * @param reply
    *            Action bytes.
    * @return Result code.
    */
    int Capture(DLMSClient& client, std::vector<ByteBuffer>& reply);
};
#endif
#endif
