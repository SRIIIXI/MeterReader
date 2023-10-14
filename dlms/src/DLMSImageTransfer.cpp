#include "DLMSImageTransfer.h"
#include "DLMSClient.h"
#include <sstream>

#ifndef DLMS_IGNORE_IMAGE_TRANSFER
//Constructor.
DLMSImageTransfer::DLMSImageTransfer() : DLMSImageTransfer("0.0.44.0.0.255", 0)
{
}

//SN Constructor.
DLMSImageTransfer::DLMSImageTransfer(std::string ln, unsigned short sn) :
    DLMSObject(DLMS_OBJECT_TYPE_IMAGE_TRANSFER, ln, sn)
{
    m_ImageBlockSize = 200;
    m_ImageFirstNotTransferredBlockNumber = 0;
    m_ImageTransferEnabled = true;
    m_ImageTransferStatus = DLMS_IMAGE_TRANSFER_STATUS_NOT_INITIATED;
}

//LN Constructor.
DLMSImageTransfer::DLMSImageTransfer(std::string ln) : DLMSImageTransfer(ln, 0)
{
}

DLMSImageTransfer::~DLMSImageTransfer()
{
    for (std::vector<DLMSImageActivateInfo*>::iterator it = m_ImageActivateInfo.begin();
        it != m_ImageActivateInfo.end(); ++it)
    {
        delete *it;
    }
    m_ImageActivateInfo.clear();
}
/**
 Holds the ImageBlockSize, expressed in octets,
 * which can be handled by the server
*/
long DLMSImageTransfer::GetImageBlockSize()
{
    return m_ImageBlockSize;
}
void DLMSImageTransfer::SetImageBlockSize(long value)
{
    m_ImageBlockSize = value;
}

/**
 * Provides information about the transfer status of each
 * ImageBlock. Each bit in the bit-std::string provides information about
 * one individual ImageBlock.
*/
std::string& DLMSImageTransfer::GetImageTransferredBlocksStatus()
{
    return m_ImageTransferredBlocksStatus;
}
void DLMSImageTransfer::SetImageTransferredBlocksStatus(std::string value)
{
    m_ImageTransferredBlocksStatus = value;
}

/**
 Provides the ImageBlockNumber of the first ImageBlock not transferred.
 * NOTE If the Image is complete, the value returned should be above the
 * number of blocks calculated from the Image size and the ImageBlockSize
*/
long DLMSImageTransfer::GetImageFirstNotTransferredBlockNumber()
{
    return m_ImageFirstNotTransferredBlockNumber;
}
void DLMSImageTransfer::SetImageFirstNotTransferredBlockNumber(long value)
{
    m_ImageFirstNotTransferredBlockNumber = value;
}

/**
 * Controls enabling the Image transfer process. The method can
 * be invoked successfully only if the value of this attribute is true.
 */
bool DLMSImageTransfer::GetImageTransferEnabled()
{
    return m_ImageTransferEnabled;
}
void DLMSImageTransfer::SetImageTransferEnabled(bool value)
{
    m_ImageTransferEnabled = value;
}

/**
 * Holds the status of the Image transfer process.
 */
DLMS_IMAGE_TRANSFER_STATUS DLMSImageTransfer::GetImageTransferStatus()
{
    return m_ImageTransferStatus;
}
void DLMSImageTransfer::SetImageTransferStatus(DLMS_IMAGE_TRANSFER_STATUS value)
{
    m_ImageTransferStatus = value;
}

std::vector<DLMSImageActivateInfo*>& DLMSImageTransfer::GetImageActivateInfo()
{
    return m_ImageActivateInfo;
}

int DLMSImageTransfer::ImageTransferInitiate(DLMSClient* client, unsigned char* imageIdentifier, unsigned char imageIdentifierSize, long imageSize, std::vector<ByteBuffer>& reply)
{
    if (m_ImageBlockSize == 0)
    {
        //Invalid image block size"
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    if (m_ImageBlockSize > client->GetMaxReceivePDUSize())
    {
        //Image block size is bigger than max PDU size.
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    int ret;
    ByteBuffer data;
    DLMSVariant identifier, size;
    identifier.Add(imageIdentifier, imageIdentifierSize);
    size = imageSize;
    data.SetUInt8(DLMS_DATA_TYPE_STRUCTURE);
    data.SetUInt8(2);
    if ((ret = Helpers::SetData(NULL, data, DLMS_DATA_TYPE_OCTET_STRING, identifier)) == 0 &&
        (ret = Helpers::SetData(NULL, data, DLMS_DATA_TYPE_UINT32, size)) == 0)
    {
        DLMSVariant tmp = data;
        ret = client->Method(this, 1, tmp, DLMS_DATA_TYPE_ARRAY, reply);
    }
    return ret;
}

int DLMSImageTransfer::ImageTransferInitiate(DLMSClient* client, std::string imageIdentifier, long imageSize, std::vector<ByteBuffer>& reply)
{
    return ImageTransferInitiate(client, (unsigned char*) imageIdentifier.c_str(), (unsigned char)imageIdentifier.length(), imageSize, reply);
}

// Returns image blocks to send to the meter.
int DLMSImageTransfer::GetImageBlocks(ByteBuffer& image, std::vector<ByteBuffer>& packets)
{
    int ret = 0;
    unsigned long cnt = (unsigned long)(image.GetSize() / m_ImageBlockSize);
    if (image.GetSize() % m_ImageBlockSize != 0)
    {
        ++cnt;
    }
    packets.clear();
    DLMSVariant tmp;
    for (unsigned long pos = 0; pos != cnt; ++pos)
    {
        ByteBuffer data;
        data.SetUInt8(DLMS_DATA_TYPE_STRUCTURE);
        data.SetUInt8(2);
        tmp = pos;
        if ((ret = Helpers::SetData(NULL, data, DLMS_DATA_TYPE_UINT32, tmp)) != 0)
        {
            break;
        }
        int bytes = (int)(image.GetSize() - ((pos + 1) * m_ImageBlockSize));
        //If last packet
        if (bytes < 0)
        {
            bytes = (int)(image.GetSize() - (pos * m_ImageBlockSize));
            data.Set(&image, data.GetSize(), bytes);
        }
        else
        {
            data.Set(&image, data.GetSize(), m_ImageBlockSize);
        }
        tmp = data;
        if ((ret = Helpers::SetData(NULL, data, DLMS_DATA_TYPE_OCTET_STRING, tmp)) != 0)
        {
            break;
        }
        packets.push_back(data);
    }
    return ret;
}

int DLMSImageTransfer::ImageBlockTransfer(DLMSClient* client, ByteBuffer& image, unsigned long& imageBlockCount, std::vector<ByteBuffer>& reply)
{
    int ret;
    std::vector<ByteBuffer> blocks;
    if ((ret = GetImageBlocks(image, blocks)) == 0)
    {
        imageBlockCount = (unsigned long) blocks.size();
        DLMSVariant tmp;
        for (std::vector<ByteBuffer>::iterator it = blocks.begin(); it != blocks.end(); ++it)
        {
            tmp = *it;
            if ((ret = client->Method(this, 2, tmp, DLMS_DATA_TYPE_ARRAY, reply)) != 0)
            {
                break;
            }
        }
    }
    return ret;
}

int DLMSImageTransfer::ImageVerify(DLMSClient* client, std::vector<ByteBuffer>& reply)
{
    DLMSVariant data((char)0);
    return client->Method(this, 3, data, reply);
}

int DLMSImageTransfer::ImageActivate(DLMSClient* client, std::vector<ByteBuffer>& reply)
{
    DLMSVariant data((char)0);
    return client->Method(this, 4, data, reply);
}

// Returns amount of attributes.
int DLMSImageTransfer::GetAttributeCount()
{
    return 7;
}

// Returns amount of methods.
int DLMSImageTransfer::GetMethodCount()
{
    return 4;
}

void DLMSImageTransfer::GetValues(std::vector<std::string>& values)
{
    values.clear();
    std::string ln;
    GetLogicalName(ln);
    values.push_back(ln);
    values.push_back(DLMSVariant(m_ImageBlockSize).ToString());
    values.push_back(DLMSVariant(m_ImageTransferredBlocksStatus).ToString());
    values.push_back(DLMSVariant(m_ImageFirstNotTransferredBlockNumber).ToString());
    values.push_back(DLMSVariant(m_ImageTransferEnabled).ToString());
    values.push_back(DLMSVariant(m_ImageTransferStatus).ToString());
    std::stringstream sb;
    sb << '[';
    bool empty = true;
    for (std::vector<DLMSImageActivateInfo*>::iterator it = m_ImageActivateInfo.begin();
        it != m_ImageActivateInfo.end(); ++it)
    {
        if (!empty)
        {
            sb << ", ";
        }
        empty = false;
        std::string str = (*it)->ToString();
        sb.write(str.c_str(), str.size());
    }
    sb << ']';
    values.push_back(sb.str());
}

void DLMSImageTransfer::GetAttributeIndexToRead(bool all, std::vector<int>& attributes)
{
    //LN is static and read only once.
    if (all || DLMSObject::IsLogicalNameEmpty(m_LN))
    {
        attributes.push_back(1);
    }
    //ImageBlockSize
    if (all || !IsRead(2))
    {
        attributes.push_back(2);
    }
    //ImageTransferredBlocksStatus
    if (all || !IsRead(3))
    {
        attributes.push_back(3);
    }
    //ImageFirstNotTransferredBlockNumber
    if (all || !IsRead(4))
    {
        attributes.push_back(4);
    }
    //ImageTransferEnabled
    if (all || !IsRead(5))
    {
        attributes.push_back(5);
    }
    //ImageTransferStatus
    if (all || !IsRead(6))
    {
        attributes.push_back(6);
    }
    //ImageActivateInfo
    if (all || !IsRead(7))
    {
        attributes.push_back(7);
    }
}

int DLMSImageTransfer::Invoke(DLMSSettings& settings, DLMSValueEventArg& e)
{
    //Image transfer initiate
    if (e.GetIndex() == 1)
    {
        m_ImageFirstNotTransferredBlockNumber = 0;
        m_ImageTransferredBlocksStatus = "";
        unsigned long size = (unsigned long) e.GetParameters().Arr[0].GetSize();
        unsigned char * imageIdentifier = e.GetParameters().Arr[0].byteArr;
        int ImageSize = e.GetParameters().Arr[1].ToInteger();
        m_ImageTransferStatus = DLMS_IMAGE_TRANSFER_STATUS_INITIATED;
        DLMSImageActivateInfo *item = NULL;
        for (std::vector<DLMSImageActivateInfo*>::iterator it = m_ImageActivateInfo.begin(); it != m_ImageActivateInfo.end(); ++it)
        {
            if ((*it)->GetIdentification().Compare(imageIdentifier, size) == 0)
            {
                item = *it;
                break;
            }
        }
        if (item == NULL)
        {
            item = new DLMSImageActivateInfo();
            m_ImageActivateInfo.push_back(item);
        }
        item->SetSize(ImageSize);
        ByteBuffer tmp;
        tmp.Set(imageIdentifier, size);
        item->SetIdentification(tmp);
        int cnt = ImageSize / m_ImageBlockSize;
        if (ImageSize % m_ImageBlockSize != 0)
        {
            ++cnt;
        }
        for (int pos = 0; pos < cnt; ++pos)
        {
            m_ImageTransferredBlocksStatus.push_back('0');
        }
        return 0;
    }
    //Image block transfer
    else if (e.GetIndex() == 2)
    {
        int imageIndex = e.GetParameters().Arr[0].ToInteger();
        m_ImageTransferredBlocksStatus[imageIndex] = '1';
        m_ImageFirstNotTransferredBlockNumber = imageIndex + 1;
        m_ImageTransferStatus = DLMS_IMAGE_TRANSFER_STATUS_INITIATED;
        return 0;
    }
    //Image verify
    else if (e.GetIndex() == 3)
    {
        return 0;
    }
    //Image activate.
    else if (e.GetIndex() == 4)
    {
        return 0;
    }
    else
    {
        e.SetError(DLMS_ERROR_CODE_READ_WRITE_DENIED);
        return 0;
    }
    return 0;
}

int DLMSImageTransfer::GetDataType(int index, DLMS_DATA_TYPE& type)
{
    if (index == 1)
    {
        type = DLMS_DATA_TYPE_OCTET_STRING;
        return DLMS_ERROR_CODE_OK;
    }
    if (index == 2)
    {
        type = DLMS_DATA_TYPE_UINT32;
        return DLMS_ERROR_CODE_OK;
    }
    if (index == 3)
    {
        type = DLMS_DATA_TYPE_BIT_STRING;
        return DLMS_ERROR_CODE_OK;
    }
    if (index == 4)
    {
        type = DLMS_DATA_TYPE_UINT32;
        return DLMS_ERROR_CODE_OK;
    }
    if (index == 5)
    {
        type = DLMS_DATA_TYPE_BOOLEAN;
        return DLMS_ERROR_CODE_OK;
    }
    if (index == 6)
    {
        type = DLMS_DATA_TYPE_ENUM;
        return DLMS_ERROR_CODE_OK;
    }
    if (index == 7)
    {
        type = DLMS_DATA_TYPE_ARRAY;
        return DLMS_ERROR_CODE_OK;
    }
    return DLMS_ERROR_CODE_INVALID_PARAMETER;
}

// Returns value of given attribute.
int DLMSImageTransfer::GetValue(DLMSSettings& settings, DLMSValueEventArg& e)
{
    if (e.GetIndex() == 1)
    {
        int ret;
        DLMSVariant tmp;
        if ((ret = GetLogicalName(this, tmp)) != 0)
        {
            return ret;
        }
        e.SetValue(tmp);
        return DLMS_ERROR_CODE_OK;
    }
    if (e.GetIndex() == 2)
    {
        e.SetValue(GetImageBlockSize());
        return DLMS_ERROR_CODE_OK;
    }
    if (e.GetIndex() == 3)
    {
        e.SetValue(m_ImageTransferredBlocksStatus);
        return DLMS_ERROR_CODE_OK;
    }
    if (e.GetIndex() == 4)
    {
        e.SetValue(m_ImageFirstNotTransferredBlockNumber);
        return DLMS_ERROR_CODE_OK;
    }
    if (e.GetIndex() == 5)
    {
        e.SetValue(m_ImageTransferEnabled);
        return DLMS_ERROR_CODE_OK;

    }
    if (e.GetIndex() == 6)
    {
        e.SetValue(m_ImageTransferStatus);
        return DLMS_ERROR_CODE_OK;
    }
    if (e.GetIndex() == 7)
    {
        e.SetByteArray(true);
        ByteBuffer data;
        data.SetUInt8(DLMS_DATA_TYPE_ARRAY);
        //ImageActivateInfo is returned only after verification is succeeded.
        if (m_ImageTransferStatus != DLMS_IMAGE_TRANSFER_STATUS_VERIFICATION_SUCCESSFUL)
        {
            data.SetUInt8(0);
        }
        else
        {
            Helpers::SetObjectCount((unsigned long)m_ImageActivateInfo.size(), data); //Count
            int ret;
            DLMSVariant size;
            for (std::vector<DLMSImageActivateInfo*>::iterator it = m_ImageActivateInfo.begin(); it != m_ImageActivateInfo.end(); ++it)
            {
                data.SetUInt8(DLMS_DATA_TYPE_STRUCTURE);
                data.SetUInt8(3);//Item count.
                size = (*it)->GetSize();
                DLMSVariant id((*it)->GetIdentification());
                DLMSVariant signature((*it)->GetSignature());
                if ((ret = Helpers::SetData(&settings, data, DLMS_DATA_TYPE_UINT32, size)) != 0 ||
                    (ret = Helpers::SetData(&settings, data, DLMS_DATA_TYPE_OCTET_STRING, id)) != 0 ||
                    (ret = Helpers::SetData(&settings, data, DLMS_DATA_TYPE_OCTET_STRING, signature)) != 0)
                {
                    return ret;
                }
            }
        }
        e.SetValue(data);
        return DLMS_ERROR_CODE_OK;
    }
    return DLMS_ERROR_CODE_INVALID_PARAMETER;
}

// Set value of given attribute.
int DLMSImageTransfer::SetValue(DLMSSettings& settings, DLMSValueEventArg& e)
{
    if (e.GetIndex() == 1)
    {
        return SetLogicalName(this, e.GetValue());
    }
    else if (e.GetIndex() == 2)
    {
        m_ImageBlockSize = e.GetValue().ToInteger();
    }
    else if (e.GetIndex() == 3)
    {
        m_ImageTransferredBlocksStatus = e.GetValue().ToString();
    }
    else if (e.GetIndex() == 4)
    {
        m_ImageFirstNotTransferredBlockNumber = e.GetValue().ToInteger();
    }
    else if (e.GetIndex() == 5)
    {
        m_ImageTransferEnabled = e.GetValue().boolVal;
    }
    else if (e.GetIndex() == 6)
    {
        m_ImageTransferStatus = (DLMS_IMAGE_TRANSFER_STATUS)e.GetValue().ToInteger();
    }
    else if (e.GetIndex() == 7)
    {
        for (std::vector<DLMSImageActivateInfo*>::iterator it = m_ImageActivateInfo.begin();
            it != m_ImageActivateInfo.end(); ++it)
        {
            delete *it;
        }
        m_ImageActivateInfo.clear();
        if (e.GetValue().vt == DLMS_DATA_TYPE_ARRAY)
        {
            DLMSVariant tmp;
            for (std::vector<DLMSVariant>::iterator it = e.GetValue().Arr.begin(); it != e.GetValue().Arr.end(); ++it)
            {
                DLMSImageActivateInfo* item = new DLMSImageActivateInfo();
                item->SetSize((*it).Arr[0].ToInteger());
                ByteBuffer id, sic;
                id.Set((*it).Arr[1].byteArr, (*it).Arr[1].GetSize());
                sic.Set((*it).Arr[2].byteArr, (*it).Arr[2].GetSize());
                item->SetIdentification(id);
                item->SetSignature(sic);
                m_ImageActivateInfo.push_back(item);
            }
        }
    }
    else
    {
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    return DLMS_ERROR_CODE_OK;
}
#endif //DLMS_IGNORE_IMAGE_TRANSFER
