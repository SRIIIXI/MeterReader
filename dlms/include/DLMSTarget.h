#ifndef DLMS_TARGET_H
#define DLMS_TARGET_H

#include "DLMSObject.h"

class DLMSTarget
{
    friend class DLMSParameterMonitor;
    /**
   * Target COSEM object.
   */
    DLMSObject* m_Target;

    /**
     * Attribute Index of COSEM object.
     */
    unsigned char m_AttributeIndex;

    /**
     * Data index of COSEM object. All targets don't use this.
     */
    unsigned char m_DataIndex;

    /**
     * Data value of COSEM object. All targets don't use this.
     */
    DLMSVariant m_Value;
public:
    // Constructor.
    DLMSTarget();

    // Destructor.
    ~DLMSTarget();

    void Clear();

    DLMSObject* GetTarget();

    void SetTarget(DLMSObject* value);

    /**
     * @return Attribute Index of COSEM object.
     */
    unsigned char GetAttributeIndex();

    /**
     * @param value
     *            Attribute Index of COSEM object.
     */
    void SetAttributeIndex(unsigned char value);

    /**
     * @return Data index of COSEM object. All targets don't use this.
     */
    unsigned char GetDataIndex();

    /**
     * @param value
     *            Data index of COSEM object. All targets don't use this.
     */
    void SetDataIndex(unsigned char value);

    /**
     * @return Data value of COSEM object. All targets don't use this.
     */
    DLMSVariant& GetValue();

    /**
     * @param value
     *            Data value of COSEM object. All targets don't use this.
     */
    void SetValue(DLMSVariant& value);

    std::string ToString();
};
#endif
