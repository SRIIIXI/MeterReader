#ifndef DLMS_CERTIFICATE_INFO_H
#define DLMS_CERTIFICATE_INFO_H

#include <string>
#include "Enums.h"

class DLMSCertificateInfo
{
private:
    /**
 * Used certificate entity.
 */
    DLMS_CERTIFICATE_ENTITY m_Entity;

    /**
     * Used certificate type.
     */
    DLMS_CERTIFICATE_TYPE m_Type;

    /**
     * Certificate serial number.
     */
    std::string m_SerialNumber;

    /**
     * Certificate issuer.
     */
    std::string m_Issuer;
    /**
     * Certificate subject.
     */
    std::string m_Subject;

    /**
     * Certificate subject alt name.
     */
    std::string m_SubjectAltName;

public:
    //Constructor.
    DLMSCertificateInfo();

    /**
     * Used certificate entity.
     */
    DLMS_CERTIFICATE_ENTITY GetEntity();
    /**
     * Used certificate entity.
     */
    void SetEntity(DLMS_CERTIFICATE_ENTITY value);

    /**
     * Used certificate type.
     */
    DLMS_CERTIFICATE_TYPE GetType();

    /**
     * Used certificate type.
     */
    void SetType(DLMS_CERTIFICATE_TYPE value);

    /**
     * @return Certificate serial number.
     */
    std::string GetSerialNumber();

    /**
     *  Certificate serial number.
     */
    void SetSerialNumber(std::string& value);

    /**
     * Certificate issuer.
     */
    std::string GetIssuer();

    /**
     * Certificate issuer.
     */
    void SetIssuer(std::string& value);

    /**
     * Certificate subject.
     */
    std::string GetSubject();

    /**
     * Certificate subject.
     */
    void SetSubject(std::string& value);

    /**
     * Certificate subject alt name.
     */
    std::string GetSubjectAltName();

    /**
     * Certificate subject alt name.
     */
    void SetSubjectAltName(std::string& value);
};
#endif
