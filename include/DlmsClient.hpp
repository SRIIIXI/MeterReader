#ifndef DLMS_CLIENT
#define DLMS_CLIENT

#include "DLMSSecureClient.h"
#include "DLMSAccount.h"
#include "DLMSTokenGateway.h"
#include <string>
#include <map>
#include <QMap>

class DlmsClient
{
public:
    DlmsClient(const unsigned char* isystemTitle,
               int isystemTitlelen,
               bool iuseLogicalNameReferencing,
               int iclientAddress,
               int iserverAddress,
               int iServerPhysicalAddress,
               int iServerAddressSize,
               DLMS_AUTHENTICATION iauthentication,
               unsigned const char* ipassword,
               int ipasswordlen,
               DLMS_INTERFACE_TYPE iinterfaceType,
               DLMS_SECURITY isecurity,
               bool iautoIncreaseInvokeID,
               const char* iinvocationCounter,
               const unsigned char* iauthenticationKey,
               int iauthenticationKeylen,
               const unsigned char* iblockCipherKey,
               int iblockCipherKeylen,
               const unsigned char* idedicatedKey,
               int idedicatedKeylen,
               const unsigned char* imanufacturerId,
               int imanufacturerIdlen,
               unsigned char igbtWindowSize,
               unsigned char iwindowSize,
               uint16_t imaxInfo,
               const unsigned char* customerpin);

    virtual ~DlmsClient();

    DlmsClient();

    bool InitializeClient();

    const char* ErrorMessage();
    int ErrorCode();
    unsigned long GetInvocationCounter();
    void SetInvocationCounter(unsigned long iFrameCounter);
    void EnableCustomChallenge(bool fl);

    void SetSystemTitle(const unsigned char* isystemTitle, int isystemTitlelen);
    void SetAuthenticationKey(const unsigned char* iauthKey, int iKeylen);
    void SetCipheringKey(const unsigned char* icipherKey, int iKeylen);
    ByteBuffer& GetSystemTitle();
    ByteBuffer& GetAuthenticationKey();
    ByteBuffer& GetCipheringKey();
    void UnwrapKeys(const unsigned char* random_octet, const int random_octet_len, const unsigned char* wrapped_key, const int wrapped_key_len, ByteBuffer& reply);
    void ExtractStringData(DLMSVariant &data, QString &strval);
    void ExtractIntegerData(DLMSVariant &data, long &numval);

    //
    void CreateSNRMFrame(QByteArray& buffer);
    bool CheckUARFrame(const QByteArray& buffer);

    void CreateAARQFrame(QByteArray& buffer);
    bool CheckAAREFrame(const QByteArray& buffer);

    void CreateFrameCounterFrame(QByteArray& buffer, QString obis= "");
    bool CheckFrameCounterFrame(const QByteArray& buffer, long &ioFrameCounter);

    void CreateAuthenticationFrame(QByteArray& buffer);
    bool CheckAuthenticationResponse(const QByteArray& buffer);

    void CreateObjectRetreiveRequestFrame(QByteArray& buffer, const QString &obiscode);
    bool CheckObjectRetreiveResponseFrame(const QByteArray& buffer, DLMSVariant& objectData, const QString &obiscode);

    void CreateRegisterParamRequestFrame(QByteArray& buffer, const QString &obiscode, int index = 2);
    bool CheckRegisterParamResponseFrame(const QByteArray& buffer, const QString &obiscode, float &value, int index = 2);

    void CreateAccountStatusRequestFrame(QByteArray& buffer, const QString &obiscode, int index);
    bool CheckAccountStatusResponseFrame(const QByteArray& buffer, const QString &obiscode, DLMSAccount &value, bool &isfound, int index);

    void CreateTokenTransferRequestFrame(QByteArray& buffer, const QString &obiscode, const QString &tokenstring);
    bool CheckTokenTransferResponseFrame(const QByteArray& buffer, const QString &obiscode, DLMSTokenGateway &value, bool &istransferred);

    void CreateTokenTransferStatusRequestFrame(QByteArray& buffer, const QString &obiscode);
    bool CheckTokenTransferStatusResponseFrame(const QByteArray& buffer, const QString &obiscode, DLMSTokenGateway &value);

    void CreateProfileGenericNumValueRequestFrame(QByteArray& buffer, int index);
    bool CheckProfileGenericNumValueResponseFrame(const QByteArray& buffer, int index, long &numvalue);

    void CreateProfileGenericCapturedObjectsRequestFrame(QByteArray& buffer);
    bool CheckProfileGenericCapturedObjectsResponseFrame(QString &colnames, long &colcount);

    void CreateProfileGenericFirstRecordRequestFrame(QByteArray& buffer);
    bool CheckProfileGenericFirstRecordResponseFrame(const QByteArray& buffer, long capcol, long &frowid, long &ftstamp);

    void CreateProfileGenericBatchRecordsRequestFrame(QByteArray& buffer, int start, int numofrecords);
    bool CheckProfileGenericBatchRecordsResponseFrame(std::vector<std::vector<DLMSVariant> > &buffer);

    void CreateDisconnectFrame(QByteArray &buffer);
    bool CheckDisconnectResponse(const QByteArray& buffer);
    void CreateReleaseFrame(QByteArray &buffer);
    bool CheckReleaseResponse(const QByteArray& buffer);

    void StartProfileGenericDialog(const QString &obiscode);
    void StartBatch();
    void AddToBatch(const QByteArray& buffer);
    void CreateReceiverReadyFrame(QByteArray& buffer);
    bool CheckBatchFrame(const QByteArray& buffer, bool &ispartial, bool &islast);

    bool IsAuthenticationRequired();
    int GetClientId();

    static void CreateClients();
    static DlmsClient* GetClient(const int clientid);
    static void ExtractKeys(DLMSVariant& data, DlmsClient* client103, DlmsClient* client104, DlmsClient* client105);
    //

private:
    DLMSSecureClient* client;
    ReplyData replyBlockBuffer;
    DLMSTokenGateway tokenGateway;
    DLMSProfileGeneric profileGeneric;

    bool useLogicalNameReferencing;
    int clientAddress;
    int serverAddress;
    int serverPhysicalAddress;
    int serverAddressSize;
    DLMS_AUTHENTICATION authentication;
    char* password;
    DLMS_INTERFACE_TYPE interfaceType;
    DLMS_SECURITY security;
    bool autoIncreaseInvokeID;
    char* invocationCounter;
    char* systemTitle;
    char* authenticationKey;
    char* blockCipherKey;
    char* dedicatedKey;
    unsigned char gbtWindowSize;
    unsigned char windowSize;
    uint16_t maxInfo;
    char* manufacturerId;
    int errorcode;

    int systlen;
    int passlen;
    int atkylen;
    int blcylen;
    int mskylen;
    int mfgrlen;
};

#endif
