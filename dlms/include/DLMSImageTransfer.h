#ifndef DLMS_IMAGE_TRANSFER_H
#define DLMS_IMAGE_TRANSFER_H

#ifndef DLMS_IGNORE_IMAGE_TRANSFER

#include "DLMSObject.h"
#include "DLMSImageActivateInfo.h"

class DLMSImageTransfer : public DLMSObject
{
    long m_ImageBlockSize;
    std::string m_ImageTransferredBlocksStatus;
    long m_ImageFirstNotTransferredBlockNumber;
    bool m_ImageTransferEnabled;
    DLMS_IMAGE_TRANSFER_STATUS m_ImageTransferStatus;
    std::vector<DLMSImageActivateInfo*> m_ImageActivateInfo;
    int GetImageBlocks(ByteBuffer& image, std::vector<ByteBuffer>& packets);
public:
    //Constructor.
    DLMSImageTransfer();

    //SN Constructor.
    DLMSImageTransfer(std::string ln, unsigned short sn);

    //LN Constructor.
    DLMSImageTransfer(std::string ln);
    ~DLMSImageTransfer();

    /**
     Holds the ImageBlockSize, expressed in octets,
     * which can be handled by the server
    */
    long GetImageBlockSize();
    void SetImageBlockSize(long value);

    /**
     * Provides information about the transfer status of each
     * ImageBlock. Each bit in the bit-std::string provides information about
     * one individual ImageBlock.
    */
    std::string& GetImageTransferredBlocksStatus();
    void SetImageTransferredBlocksStatus(std::string value);

    /**
     Provides the ImageBlockNumber of the first ImageBlock not transferred.
     * NOTE If the Image is complete, the value returned should be above the
     * number of blocks calculated from the Image size and the ImageBlockSize
    */
    long GetImageFirstNotTransferredBlockNumber();
    void SetImageFirstNotTransferredBlockNumber(long value);

    /**
     * Controls enabling the Image transfer process. The method can
     * be invoked successfully only if the value of this attribute is true.
     */
    bool GetImageTransferEnabled();
    void SetImageTransferEnabled(bool value);

    /**
     * Holds the status of the Image transfer process.
     */
    DLMS_IMAGE_TRANSFER_STATUS GetImageTransferStatus();
    void SetImageTransferStatus(DLMS_IMAGE_TRANSFER_STATUS value);

    std::vector<DLMSImageActivateInfo*>& GetImageActivateInfo();

    int ImageTransferInitiate(DLMSClient* client, unsigned char* imageIdentifier, unsigned char imageIdentifierSize, long imageSize, std::vector<ByteBuffer>& reply);

    int ImageTransferInitiate(DLMSClient* client, std::string imageIdentifier, long imageSize, std::vector<ByteBuffer>& reply);

    // Move image to the meter.
    int ImageBlockTransfer(DLMSClient* client, ByteBuffer& image, unsigned long& imageBlockCount, std::vector<ByteBuffer>& reply);

    // Verify image.
    int ImageVerify(DLMSClient* client, std::vector<ByteBuffer>& reply);

    // Activate image.
    int ImageActivate(DLMSClient* client, std::vector<ByteBuffer>& reply);


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

    int Invoke(DLMSSettings& settings, DLMSValueEventArg& e);

    // Returns value of given attribute.
    int GetValue(DLMSSettings& settings, DLMSValueEventArg& e);

    // Set value of given attribute.
    int SetValue(DLMSSettings& settings, DLMSValueEventArg& e);
};
#endif
#endif
