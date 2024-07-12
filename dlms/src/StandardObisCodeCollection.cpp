#include "StandardObisCodeCollection.h"

int StandardObisCodeCollection::GetBytes(std::string ln, unsigned char* bytes)
{
    std::vector< std::string > tmp = Helpers::Split(ln, '.');
    if (tmp.size() != 6)
    {
        //Invalid OBIS Code.
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    int value;
#if _MSC_VER > 1000
    sscanf_s(tmp[0].c_str(), "%d", &value);
#else
    sscanf(tmp[0].c_str(), "%d", &value);
#endif
    bytes[0] = value;
#if _MSC_VER > 1000
    sscanf_s(tmp[1].c_str(), "%d", &value);
#else
    sscanf(tmp[1].c_str(), "%d", &value);
#endif
    bytes[1] = value;
#if _MSC_VER > 1000
    sscanf_s(tmp[2].c_str(), "%d", &value);
#else
    sscanf(tmp[2].c_str(), "%d", &value);
#endif
    bytes[2] = value;
#if _MSC_VER > 1000
    sscanf_s(tmp[3].c_str(), "%d", &value);
#else
    sscanf(tmp[3].c_str(), "%d", &value);
#endif
    bytes[3] = value;
#if _MSC_VER > 1000
    sscanf_s(tmp[4].c_str(), "%d", &value);
#else
    sscanf(tmp[4].c_str(), "%d", &value);
#endif
    bytes[4] = value;
#if _MSC_VER > 1000
    sscanf_s(tmp[5].c_str(), "%d", &value);
#else
    sscanf(tmp[5].c_str(), "%d", &value);
#endif
    bytes[5] = value;
    return DLMS_ERROR_CODE_OK;
}

bool StandardObisCodeCollection::EqualsInterface(StandardObisCode item, int ic)
{
    //If all interfaces are allowed.
    if (ic == 0 || item.GetInterfaces() == "*")
    {
        return true;
    }
    char type[11] = {0};

    snprintf(type, 10, "%d", ic);

    std::vector< std::string > tmp = Helpers::Split(item.GetInterfaces(), ',');
    for (std::vector< std::string >::iterator it = tmp.begin(); it != tmp.end(); ++it)
    {
        if (*it == type)
        {
            return true;
        }
    }
    return false;
}

// Check OBIS codes.
bool StandardObisCodeCollection::EqualsMask(std::string& obis, int ic)
{
    bool number = true;
    int value;
    if (obis.find(',') != std::string::npos)
    {
        std::vector< std::string > tmp = Helpers::Split(obis, ',');
        //for (std::string it : tmp)
        for (std::vector< std::string >::iterator it = tmp.begin(); it != tmp.end(); ++it)
        {
            if ((*it).find('-') != std::string::npos)
            {
                if (EqualsMask(*it, ic))
                {
                    return true;
                }
            }
            else
            {
#if _MSC_VER > 1000
                sscanf_s((*it).c_str(), "%d", &value);
#else
                sscanf((*it).c_str(), "%d", &value);
#endif
                if (value == ic)
                {
                    return true;
                }
            }
        }
        return false;
    }
    else if (obis.find('-') != std::string::npos)
    {
        number = false;
        std::vector< std::string > tmp = Helpers::Split(obis, '-');
        int value1, value2;
#if _MSC_VER > 1000
        sscanf_s(tmp[0].c_str(), "%d", &value1);
        sscanf_s(tmp[1].c_str(), "%d", &value2);
#else
        sscanf(tmp[0].c_str(), "%d", &value1);
        sscanf(tmp[1].c_str(), "%d", &value2);
#endif
        return ic >= value1 && ic <= value2;
    }
    if (number)
    {
        if (obis == "&")
        {
            return ic == 0 || ic == 1 || ic == 7;
        }
#if _MSC_VER > 1000
        sscanf_s(obis.c_str(), "%d", &value);
#else
        sscanf(obis.c_str(), "%d", &value);
#endif
        return value == ic;
    }
    return false;
}

// Check OBIS code.
bool StandardObisCodeCollection::EqualsObisCode(std::vector< std::string >& obisMask, unsigned char* ic)
{
    if (!EqualsMask(obisMask[0], ic[0]))
    {
        return false;
    }
    if (!EqualsMask(obisMask[1], ic[1]))
    {
        return false;
    }
    if (!EqualsMask(obisMask[2], ic[2]))
    {
        return false;
    }
    if (!EqualsMask(obisMask[3], ic[3]))
    {
        return false;
    }
    if (!EqualsMask(obisMask[4], ic[4]))
    {
        return false;
    }
    if (!EqualsMask(obisMask[5], ic[5]))
    {
        return false;
    }
    return true;
}

// Get description.
std::string StandardObisCodeCollection::GetDescription(std::string& str)
{
    str = Helpers::trim(str);
    if (str.size() == 0 || str[0] != '$')
    {
        return "";
    }
    int value;
#if _MSC_VER > 1000
    sscanf_s(str.substr(1).c_str(), "%d", &value);
#else
    sscanf(str.substr(1).c_str(), "%d", &value);
#endif
    const char* tmp;
    switch (value)
    {
    case 1:
        tmp = "Sum Li Active power+ (QI+QIV)";
        break;
    case 2:
        tmp = "Sum Li Active power- (QII+QIII)";
        break;
    case 3:
        tmp = "Sum Li Reactive power+ (QI+QII)";
        break;
    case 4:
        tmp = "Sum Li Reactive power- (QIII+QIV)";
        break;
    case 5:
        tmp = "Sum Li Reactive power QI";
        break;
    case 6:
        tmp = "Sum Li Reactive power QII";
        break;
    case 7:
        tmp = "Sum Li Reactive power QIII";
        break;
    case 8:
        tmp = "Sum Li Reactive power QIV";
        break;
    case 9:
        tmp = "Sum Li Apparent power+ (QI+QIV)";
        break;
    case 10:
        tmp = "Sum Li Apparent power- (QII+QIII)";
        break;
    case 11:
        tmp = "Current: any phase";
        break;
    case 12:
        tmp = "Voltage: any phase";
        break;
    case 13:
        tmp = "Sum Li Power factor";
        break;
    case 14:
        tmp = "Supply frequency";
        break;
    case 15:
        tmp = "Sum Li Active power (abs(QI+QIV)+abs(QII+QIII))";
        break;
    case 16:
        tmp = "Sum Li Active power (abs(QI+QIV)-abs(QII+QIII))";
        break;
    case 17:
        tmp = "Sum Li Active power QI";
        break;
    case 18:
        tmp = "Sum Li Active power QII";
        break;
    case 19:
        tmp = "Sum Li Active power QIII";
        break;
    case 20:
        tmp = "Sum Li Active power QIV";
        break;
    case 21:
        tmp = "L1 Active power+ (QI+QIV)";
        break;
    case 22:
        tmp = "L1 Active power- (QII+QIII)";
        break;
    case 23:
        tmp = "L1 Reactive power+ (QI+QII)";
        break;
    case 24:
        tmp = "L1 Reactive power- (QIII+QIV)";
        break;
    case 25:
        tmp = "L1 Reactive power QI";
        break;
    case 26:
        tmp = "L1 Reactive power QII";
        break;
    case 27:
        tmp = "L1 Reactive power QIII";
        break;
    case 28:
        tmp = "L1 Reactive power QIV";
        break;
    case 29:
        tmp = "L1 Apparent power+ (QI+QIV)";
        break;
    case 30:
        tmp = "L1 Apparent power- (QII+QIII)";
        break;
    case 31:
        tmp = "L1 Current";
        break;
    case 32:
        tmp = "L1 Voltage";
        break;
    case 33:
        tmp = "L1 Power factor";
        break;
    case 34:
        tmp = "L1 Supply frequency";
        break;
    case 35:
        tmp = "L1 Active power (abs(QI+QIV)+abs(QII+QIII))";
        break;
    case 36:
        tmp = "L1 Active power (abs(QI+QIV)-abs(QII+QIII))";
        break;
    case 37:
        tmp = "L1 Active power QI";
        break;
    case 38:
        tmp = "L1 Active power QII";
        break;
    case 39:
        tmp = "L1 Active power QIII";
        break;
    case 40:
        tmp = "L1 Active power QIV";
        break;
    case 41:
        tmp = "L2 Active power+ (QI+QIV)";
        break;
    case 42:
        tmp = "L2 Active power- (QII+QIII)";
        break;
    case 43:
        tmp = "L2 Reactive power+ (QI+QII)";
        break;
    case 44:
        tmp = "L2 Reactive power- (QIII+QIV)";
        break;
    case 45:
        tmp = "L2 Reactive power QI";
        break;
    case 46:
        tmp = "L2 Reactive power QII";
        break;
    case 47:
        tmp = "L2 Reactive power QIII";
        break;
    case 48:
        tmp = "L2 Reactive power QIV";
        break;
    case 49:
        tmp = "L2 Apparent power+ (QI+QIV)";
        break;
    case 50:
        tmp = "L2 Apparent power- (QII+QIII)";
        break;
    case 51:
        tmp = "L2 Current";
        break;
    case 52:
        tmp = "L2 Voltage";
        break;
    case 53:
        tmp = "L2 Power factor";
        break;
    case 54:
        tmp = "L2 Supply frequency";
        break;
    case 55:
        tmp = "L2 Active power (abs(QI+QIV)+abs(QII+QIII))";
        break;
    case 56:
        tmp = "L2 Active power (abs(QI+QIV)-abs(QI+QIII))";
        break;
    case 57:
        tmp = "L2 Active power QI";
        break;
    case 58:
        tmp = "L2 Active power QII";
        break;
    case 59:
        tmp = "L2 Active power QIII";
        break;
    case 60:
        tmp = "L2 Active power QIV";
        break;
    case 61:
        tmp = "L3 Active power+ (QI+QIV)";
        break;
    case 62:
        tmp = "L3 Active power- (QII+QIII)";
        break;
    case 63:
        tmp = "L3 Reactive power+ (QI+QII)";
        break;
    case 64:
        tmp = "L3 Reactive power- (QIII+QIV)";
        break;
    case 65:
        tmp = "L3 Reactive power QI";
        break;
    case 66:
        tmp = "L3 Reactive power QII";
        break;
    case 67:
        tmp = "L3 Reactive power QIII";
        break;
    case 68:
        tmp = "L3 Reactive power QIV";
        break;
    case 69:
        tmp = "L3 Apparent power+ (QI+QIV)";
        break;
    case 70:
        tmp = "L3 Apparent power- (QII+QIII)";
        break;
    case 71:
        tmp = "L3 Current";
        break;
    case 72:
        tmp = "L3 Voltage";
        break;
    case 73:
        tmp = "L3 Power factor";
        break;
    case 74:
        tmp = "L3 Supply frequency";
        break;
    case 75:
        tmp = "L3 Active power (abs(QI+QIV)+abs(QII+QIII))";
        break;
    case 76:
        tmp = "L3 Active power (abs(QI+QIV)-abs(QI+QIII))";
        break;
    case 77:
        tmp = "L3 Active power QI";
        break;
    case 78:
        tmp = "L3 Active power QII";
        break;
    case 79:
        tmp = "L3 Active power QIII";
        break;
    case 80:
        tmp = "L3 Active power QIV";
        break;
    case 82:
        tmp = "Unitless quantities (pulses or pieces)";
        break;
    case 84:
        tmp = "Sum Li Power factor-";
        break;
    case 85:
        tmp = "L1 Power factor-";
        break;
    case 86:
        tmp = "L2 Power factor-";
        break;
    case 87:
        tmp = "L3 Power factor-";
        break;
    case 88:
        tmp = "Sum Li A2h QI+QII+QIII+QIV";
        break;
    case 89:
        tmp = "Sum Li V2h QI+QII+QIII+QIV";
        break;
    case 90:
        tmp = "SLi current (algebraic sum of the - unsigned - value of the currents in all phases)";
        break;
    case 91:
        tmp = "Lo Current (neutral)";
        break;
    case 92:
        tmp = "Lo Voltage (neutral)";
        break;
    default:
        tmp = "";
        break;
    }
    return tmp;
}


int StandardObisCodeCollection::Find(std::string ln, DLMS_OBJECT_TYPE objectType, std::vector<StandardObisCode*>& list)
{
    int ret;
    unsigned char bytes[6];
    if ((ret = GetBytes(ln, bytes)) != DLMS_ERROR_CODE_OK)
    {
        return ret;
    }
    Find(bytes, objectType, list);
    return 0;
}

bool StandardObisCodeCollection::EqualsMask(std::string& obisMask, std::string& ln)
{
    unsigned char bytes[6];
    if (GetBytes(ln, bytes) != DLMS_ERROR_CODE_OK)
    {
        return false;
    }
    std::vector< std::string > tmp = Helpers::Split(obisMask, '.');
    return EqualsObisCode(tmp, bytes);
}

bool StandardObisCodeCollection::EqualsMask2(std::string obisMask, std::string& ln)
{
    return EqualsMask(obisMask, ln);
}

// Get N1C description.
static const char* GetN1CDescription(std::string& str)
{
    if (str.empty() || str[0] != '$')
    {
        return "";
    }
    int value;
#if _MSC_VER > 1000
    if (sscanf_s(str.c_str(), "%d", &value) == -1)
#else
    if (sscanf(str.c_str(), "%d", &value) == -1)
#endif
    {
        return "";;
    }
    const char* tmp = nullptr;
    switch (value)
    {
    case 41:
        tmp = "Absolute temperature";
        break;
    case 42:
        tmp = "Absolute pressure";
        break;
    case 44:
        tmp = "Velocity of sound";
        break;
    case 45:
        tmp = "Density(of gas)";
        break;
    case 46:
        tmp = "Relative density";
        break;
    case 47:
        tmp = "Gauge pressure";
        break;
    case 48:
        tmp = "Differential pressure";
        break;
    case 49:
        tmp = "Density of air";
        break;
    }
    return tmp;
}

// Find Standard OBIS Code description.
void StandardObisCodeCollection::Find(unsigned char* pObisCode, int IC, std::vector<StandardObisCode*>& list)
{
    char buff[7] = {0};
    std::string desc;
    for (std::vector<StandardObisCode*>::iterator it = this->begin(); it != this->end(); ++it)
    {
        //Interface is tested first because it's faster.
        if (EqualsInterface(*(*it), IC) && EqualsObisCode((*it)->GetOBIS(), pObisCode))
        {
            StandardObisCode* obj = new StandardObisCode();
            obj->SetOBIS((*it)->GetOBIS());
            obj->SetDescription((*it)->GetDescription());
            obj->SetInterfaces((*it)->GetInterfaces());
            obj->SetDataType((*it)->GetDataType());
            obj->SetUIDataType((*it)->GetUIDataType());
            std::vector< std::string > tmp2 = Helpers::Split((*it)->GetDescription(), ';');
            if (tmp2.size() > 1)
            {
                if (pObisCode != NULL && Helpers::trim(tmp2[1]).compare("$1") == 0)
                {
                    desc = "$";
                    desc += Helpers::IntToString(pObisCode[2]);
                    if (pObisCode[0] == 7)
                    {
                        desc = GetN1CDescription(desc);
                    }
                    else
                    {
                        desc = GetDescription(desc);
                    }
                    if (desc != "")
                    {
                        tmp2[1] = desc;
                        std::string builder;
                        for (std::vector< std::string >::iterator s = tmp2.begin(); s != tmp2.end(); ++s)
                        {
                            if (builder.size() != 0)
                            {
                                builder.append(";");
                            }
                            builder.append(*s);
                        }
                        obj->SetDescription(builder);
                    }
                }
            }
            std::vector< std::string > obis = obj->GetOBIS();

            snprintf(buff, 6, "%d", pObisCode[0]);

            obis[0] = buff;

            snprintf(buff, 6, "%d", pObisCode[1]);

            obis[1] = buff;

            snprintf(buff, 6, "%d", pObisCode[2]);

            obis[2] = buff;

            snprintf(buff, 6, "%d", pObisCode[3]);

            obis[3] = buff;

            snprintf(buff, 6, "%d", pObisCode[4]);

            obis[4] = buff;

            snprintf(buff, 6, "%d", pObisCode[5]);

            obis[5] = buff;

            obj->SetOBIS(obis);
            desc = obj->GetDescription();
            snprintf(buff, 6, "%d", pObisCode[0]);

            Helpers::Replace(desc, "$A", buff);

            snprintf(buff, 6, "%d", pObisCode[1]);

            Helpers::Replace(desc, "$B", buff);

            snprintf(buff, 6, "%d", pObisCode[2]);

            Helpers::Replace(desc, "$C", buff);

            snprintf(buff, 6, "%d", pObisCode[3]);

            Helpers::Replace(desc, "$D", buff);

            snprintf(buff, 6, "%d", pObisCode[4]);

            Helpers::Replace(desc, "$E", buff);

            snprintf(buff, 6, "%d", pObisCode[5]);

            Helpers::Replace(desc, "$F", buff);
            //Increase value
            int begin = (int)desc.find("#$");
            if (begin != -1)
            {
                int start = (int)desc.find('(');
                int end = (int)desc.find(')');
                char channel = desc[start + 1];
                int ch = 0;
                if (channel == 'A')
                {
                    ch = pObisCode[0];
                }
                else if (channel == 'B')
                {
                    ch = pObisCode[1];
                }
                else if (channel == 'C')
                {
                    ch = pObisCode[2];
                }
                else if (channel == 'D')
                {
                    ch = pObisCode[3];
                }
                else if (channel == 'E')
                {
                    ch = pObisCode[4];
                }
                else if (channel == 'F')
                {
                    ch = pObisCode[5];
                }
                int plus = (int)desc.find('+');
                if (plus != -1)
                {
                    int value;

                    sscanf(desc.substr(plus + 1, plus + 1 + end - plus - 1).c_str(), "%d", &value);

                    ch += value;
                }

                snprintf(buff, 6, "%d", ch);
                desc = desc.substr(0, begin).append(buff);
            }
            Helpers::Replace(desc, ";", " ");
            Helpers::Replace(desc, "  ", " ");
            Helpers::rtrim(desc);
            obj->SetDescription(desc);
            list.push_back(obj);
        }
    }
    //If invalid OBIS code.
    if (list.size() == 0)
    {
        StandardObisCode* obj = new StandardObisCode();
        std::string desc = "Invalid";
        obj->SetDescription(desc);

        snprintf(buff, 6, "%d", IC);

        std::string str = buff;
        obj->SetInterfaces(str);
        std::vector <std::string > obis;

        snprintf(buff, 6, "%d", pObisCode[0]);
        obis.push_back(buff);

        snprintf(buff, 6, "%d", pObisCode[1]);
        obis.push_back(buff);

        snprintf(buff, 6, "%d", pObisCode[2]);
        obis.push_back(buff);

        snprintf(buff, 6, "%d", pObisCode[3]);
        obis.push_back(buff);

        snprintf(buff, 6, "%d", pObisCode[4]);
        obis.push_back(buff);

        snprintf(buff, 6, "%d", pObisCode[5]);
        obis.push_back(buff);

        obj->SetOBIS(obis);
        list.push_back(obj);
    }
}
