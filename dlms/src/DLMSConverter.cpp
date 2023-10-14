#include "DLMSConverter.h"
#include "ErrorCodes.h"
#include "OBiscodes.h"
#include "StandardObisCodeCollection.h"

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif //defined(_WIN32) || defined(_WIN64)

DLMSConverter::~DLMSConverter()
{
    for (std::vector<StandardObisCode*>::iterator it = m_Codes.begin(); it != m_Codes.end(); ++it)
    {
        delete* it;
    }
    m_Codes.clear();
}

const char* DLMSConverter::GetErrorMessage(int error)
{
    const char* str;
    if ((error & DLMS_ERROR_TYPE_EXCEPTION_RESPONSE) != 0)
    {
        switch (error & 0xFF)
        {
        case DLMS_EXCEPTION_SERVICE_ERROR_OPERATION_NOT_POSSIBLE:
            return "OperationNotPossible";
        case DLMS_EXCEPTION_SERVICE_ERROR_SERVICE_NOT_SUPPORTED:
            return "ServiceNotSupported";
        case DLMS_EXCEPTION_SERVICE_ERROR_OTHER_REASON:
            return "OtherReason";
        case DLMS_EXCEPTION_SERVICE_ERROR_PDU_TOO_LONG:
            return "PduTooLong";
        case DLMS_EXCEPTION_SERVICE_ERROR_DECIPHERING_ERROR:
            return "DecipheringError";
        case DLMS_EXCEPTION_SERVICE_ERROR_INVOCATION_COUNTER_ERROR:
            return "InvocationCounterError";
        default:
            return "Unknown Exception response.";
        }
    }
    else if ((error & DLMS_ERROR_TYPE_CONFIRMED_SERVICE_ERROR) != 0)
    {
        str = "Confirmed Service Error.";
    }
    else if ((error & DLMS_ERROR_TYPE_COMMUNICATION_ERROR) != 0)
    {
#if defined(_WIN32) || defined(_WIN64) || defined(__linux__)
#if defined(_WIN32) || defined(_WIN64)
        wchar_t* s = NULL;
        FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL, error & ~DLMS_ERROR_TYPE_COMMUNICATION_ERROR,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            (LPWSTR)&s, 0, NULL);
        fprintf(stderr, "%S\n", s);
        LocalFree(s);
        str = "Connection error.";
#else
        str = strerror(error & ~DLMS_ERROR_TYPE_COMMUNICATION_ERROR);
#endif
        str = "Connection error.";
#endif
    }
    else
    {
        switch (error)
        {
        case DLMS_ERROR_CODE_OK:
            str = "OK";
            break;
        case DLMS_ERROR_CODE_INVALID_PARAMETER:
            str = "Invalid parameter.";
            break;
        case DLMS_ERROR_CODE_NOT_INITIALIZED:
            str = "Server is not initialized.";
            break;
        case DLMS_ERROR_CODE_OUTOFMEMORY:
            str = "Not enough memory available.";
            break;
        case DLMS_ERROR_CODE_NOT_REPLY:
            str = "Packet is not a reply for a send packet.";
            break;
        case DLMS_ERROR_CODE_REJECTED:
            str = "Meter rejects send packet.";
            break;
        case DLMS_ERROR_CODE_INVALID_LOGICAL_NAME:
            str = "Invalid Logical Name.";
            break;
        case DLMS_ERROR_CODE_INVALID_CLIENT_ADDRESS:
            str = "Client HDLC Address is not set.";
            break;
        case DLMS_ERROR_CODE_INVALID_SERVER_ADDRESS:
            str = "Server HDLC Address is not set.";
            break;
        case DLMS_ERROR_CODE_INVALID_DATA_FORMAT:
            str = "Not a HDLC frame.";
            break;
        case DLMS_ERROR_CODE_INVALID_VERSION_NUMBER:
            str = "Invalid DLMS version number.";
            break;
        case DLMS_ERROR_CODE_CLIENT_ADDRESS_NO_NOT_MATCH:
            str = "Client addresses do not match.";
            break;
        case DLMS_ERROR_CODE_SERVER_ADDRESS_NO_NOT_MATCH:
            str = "Server addresses do not match.";
            break;
        case DLMS_ERROR_CODE_WRONG_CRC:
            str = "CRC do not match.";
            break;
        case DLMS_ERROR_CODE_INVALID_RESPONSE:
            str = "Invalid response";
            break;
        case DLMS_ERROR_CODE_INVALID_TAG:
            str = "Invalid Tag.";
            break;
        case DLMS_ERROR_CODE_ENCODING_FAILED:
            str = "Encoding failed. Not enough data.";
            break;
        case DLMS_ERROR_CODE_REJECTED_PERMAMENT:
            str = "Rejected permament.";
            break;
        case DLMS_ERROR_CODE_REJECTED_TRANSIENT:
            str = "Rejected transient.";
            break;
        case DLMS_ERROR_CODE_NO_REASON_GIVEN:
            str = "No reason given.";
            break;
        case DLMS_ERROR_CODE_APPLICATION_CONTEXT_NAME_NOT_SUPPORTED:
            str = "Application context name not supported.";
            break;
        case DLMS_ERROR_CODE_AUTHENTICATION_MECHANISM_NAME_NOT_RECOGNISED:
            str = "Authentication mechanism name not recognised.";
            break;
        case DLMS_ERROR_CODE_AUTHENTICATION_MECHANISM_NAME_REQUIRED:
            str = "Authentication mechanism name required.";
            break;
        case DLMS_ERROR_CODE_AUTHENTICATION_FAILURE:
            str = "Authentication failure.";
            break;
        case DLMS_ERROR_CODE_AUTHENTICATION_REQUIRED:
            str = "Authentication required.";
            break;
        case DLMS_ERROR_CODE_HARDWARE_FAULT:
            str = "Access Error : Device reports a hardware fault.";
            break;
        case DLMS_ERROR_CODE_TEMPORARY_FAILURE:
            str = "Access Error : Device reports a temporary failure.";
            break;
        case DLMS_ERROR_CODE_READ_WRITE_DENIED:
            str = "Access Error : Device reports Read-Write denied.";
            break;
        case DLMS_ERROR_CODE_UNDEFINED_OBJECT:
            str = "Access Error : Device reports a undefined object.";
            break;
        case DLMS_ERROR_CODE_INCONSISTENT_CLASS_OR_OBJECT:
            str = "Access Error : Device reports a inconsistent Class or Object.";
            break;
        case DLMS_ERROR_CODE_UNAVAILABLE_OBJECT:
            str = "Access Error : Device reports a unavailable object.";
            break;
        case DLMS_ERROR_CODE_UNMATCH_TYPE:
            str = "Access Error : Device reports a unmatched type.";
            break;
        case DLMS_ERROR_CODE_ACCESS_VIOLATED:
            str = "Access Error : Device reports scope of access violated.";
            break;
        case DLMS_ERROR_CODE_DATA_BLOCK_UNAVAILABLE:
            str = "Access Error : Data Block Unavailable.";
            break;
        case DLMS_ERROR_CODE_LONG_GET_OR_READ_ABORTED:
            str = "Access Error : Long Get Or Read Aborted.";
            break;
        case DLMS_ERROR_CODE_NO_LONG_GET_OR_READ_IN_PROGRESS:
            str = "Access Error : No Long Get Or Read In Progress.";
            break;
        case DLMS_ERROR_CODE_LONG_SET_OR_WRITE_ABORTED:
            str = "Access Error : Long Set Or Write Aborted.";
            break;
        case DLMS_ERROR_CODE_NO_LONG_SET_OR_WRITE_IN_PROGRESS:
            str = "Access Error : No Long Set Or Write In Progress.";
            break;
        case DLMS_ERROR_CODE_DATA_BLOCK_NUMBER_INVALID:
            str = "Access Error : Data Block Number Invalid.";
            break;
        case DLMS_ERROR_CODE_OTHER_REASON:
            str = "Access Error : Other Reason.";
            break;
        case DLMS_ERROR_CODE_UNKNOWN:
            str = "Unknown error.";
            break;
        case DLMS_ERROR_CODE_SEND_FAILED:
            str = "Data send failed.";
            break;
        case DLMS_ERROR_CODE_RECEIVE_FAILED:
            str = "Data receive failed.";
            break;
        case DLMS_ERROR_CODE_NOT_IMPLEMENTED:
            str = "Not implemeted.";
            break;
        case DLMS_ERROR_CODE_INVALID_INVOKE_ID:
            str = "Invalid Invoke ID.";
            break;
        case DLMS_ERROR_CODE_INVOCATION_COUNTER_TOO_SMALL:
            str = "Invocation counter value is too small.";
            break;
        case DLMS_ERROR_CODE_INVALID_DECIPHERING_ERROR:
            str = "Deciphering error.";
            break;
        case DLMS_ERROR_CODE_INVALID_SECURITY_SUITE:
            str = "Client try to connect with wrong security suite.";
            break;
        default:
            str = "Unknown error.";
            break;
        }
    }
    return str;
}

const char* DLMSConverter::ToString(DLMS_BAUD_RATE value)
{
    const char* str;
    switch (value)
    {
    case 0:
        str = "Baudrate300";
        break;
    case 1:
        str = "Baudrate600";
        break;
    case 2:
        str = "Baudrate1200";
        break;
    case 3:
        str = "Baudrate2400";
        break;
    case 4:
        str = "Baudrate4000";
        break;
    case 5:
        str = "Baudrate9600";
        break;
    case 6:
        str = "Baudrate19200";
        break;
    case 7:
        str = "Baudrate38400";
        break;
    case 8:
        str = "Baudrate57600";
        break;
    case 9:
        str = "Baudrate115200";
        break;
    default:
        str = "Unknown baudrate";
        break;
    }
    return str;
}

const char* DLMSConverter::ToString(DLMS_CLOCK_STATUS value)
{
    const char* str;
    switch (value)
    {
    case 0:
        str = "None";
        break;
    case 0x1:
        str = "InvalidValue";
        break;
    case 0x2:
        str = "DoubtfulValue";
        break;
    case 0x4:
        str = "DifferentClockBase";
        break;
    case 0x8:
        str = "Reserved1";
        break;
    case 0x10:
        str = "Reserved2";
        break;
    case 0x20:
        str = "Reserved3";
        break;
    case 0x40:
        str = "Reserved4";
        break;
    case 0x80:
        str = "DaylightSaveActive";
        break;
    default:
        str = "Unknown Clock status";
        break;
    }
    return str;
}

const char* DLMSConverter::ToString(DLMS_CLOCK_BASE value)
{
    const char* str;
    switch (value)
    {
    case 0:
        str = "None";
        break;
    case 1:
        str = "Crystal";
        break;
    case 2:
        str = "Frequency50";
        break;
    case 3:
        str = "Frequency60";
        break;
    case 4:
        str = "GPS";
        break;
    case 5:
        str = "Radio";
        break;
    default:
        str = "Unknown Clock base";
        break;
    }
    return str;
}

const char* DLMSConverter::GetUnitAsString(int unit)
{
    const char* ret;
    switch (unit)
    {
    case DLMS_UNIT_NONE:
        ret = "None";
        break;
    case DLMS_UNIT_YEAR:
        ret = "Year";
        break;
    case DLMS_UNIT_MONTH:
        ret = "Month";
        break;
    case DLMS_UNIT_WEEK:
        ret = "Week";
        break;
    case DLMS_UNIT_DAY:
        ret = "Day";
        break;
    case DLMS_UNIT_HOUR:
        ret = "Hour";
        break;
    case DLMS_UNIT_MINUTE:
        ret = "Minute";
        break;
    case DLMS_UNIT_SECOND:
        ret = "Second";
        break;
    case DLMS_UNIT_PHASE_ANGLE_DEGREE:
        ret = "PhaseAngle";
        break;
    case DLMS_UNIT_TEMPERATURE:
        ret = "Temperature";
        break;
    case DLMS_UNIT_LOCAL_CURRENCY:
        ret = "LocalCurrency";
        break;
    case DLMS_UNIT_LENGTH:
        ret = "Length";
        break;
    case DLMS_UNIT_SPEED:
        ret = "Speed";
        break;
    case DLMS_UNIT_VOLUME_CUBIC_METER:
        ret = "Volume Cubic Meter";
        break;
    case DLMS_UNIT_CORRECTED_VOLUME:
        ret = "Corrected volume";
        break;
    case DLMS_UNIT_VOLUME_FLUX_HOUR:
        ret = "Volume flux hour";
        break;
    case DLMS_UNIT_CORRECTED_VOLUME_FLUX_HOUR:
        ret = "Corrected volume flux hour";
        break;
    case DLMS_UNIT_VOLUME_FLUX_DAY:
        ret = "Volume flux day";
        break;
    case DLMS_UNIT_CORRECTED_VOLUME_FLUX_DAY:
        ret = "Corrected volume flux day";
        break;
    case DLMS_UNIT_VOLUME_LITER:
        ret = "Volume liter";
        break;
    case DLMS_UNIT_MASS_KG:
        ret = "Mass Kg";
        break;
    case DLMS_UNIT_FORCE:
        ret = "Force";
        break;
    case DLMS_UNIT_ENERGY:
        ret = "Energy";
        break;
    case DLMS_UNIT_PRESSURE_PASCAL:
        ret = "Pressure pascal";
        break;
    case DLMS_UNIT_PRESSURE_BAR:
        ret = "Pressure Bar";
        break;
    case DLMS_UNIT_ENERGY_JOULE:
        ret = "Energy joule";
        break;
    case DLMS_UNIT_THERMAL_POWER:
        ret = "Thermal power";
        break;
    case DLMS_UNIT_ACTIVE_POWER:
        ret = "Active power";
        break;
    case DLMS_UNIT_APPARENT_POWER:
        ret = "Apparent power";
        break;
    case DLMS_UNIT_REACTIVE_POWER:
        ret = "Reactive power";
        break;
    case DLMS_UNIT_ACTIVE_ENERGY:
        ret = "Active energy";
        break;
    case DLMS_UNIT_APPARENT_ENERGY:
        ret = "Apparent energy";
        break;
    case DLMS_UNIT_REACTIVE_ENERGY:
        ret = "Reactive energy";
        break;
    case DLMS_UNIT_CURRENT:
        ret = "Current";
        break;
    case DLMS_UNIT_ELECTRICAL_CHARGE:
        ret = "ElectricalCharge";
        break;
    case DLMS_UNIT_VOLTAGE:
        ret = "Voltage";
        break;
    case DLMS_UNIT_ELECTRICAL_FIELD_STRENGTH:
        ret = "Electrical field strength E V/m";
        break;
    case DLMS_UNIT_CAPACITY:
        ret = "Capacity C farad C/V = As/V";
        break;
    case DLMS_UNIT_RESISTANCE:
        ret = "Resistance";
        break;
    case DLMS_UNIT_RESISTIVITY:
        ret = "Resistivity";
        break;
    case DLMS_UNIT_MAGNETIC_FLUX:
        ret = "Magnetic flux F weber Wb = Vs";
        break;
    case DLMS_UNIT_INDUCTION:
        ret = "Induction T tesla Wb/m2";
        break;
    case DLMS_UNIT_MAGNETIC:
        ret = "Magnetic field strength H A/m";
        break;
    case DLMS_UNIT_INDUCTIVITY:
        ret = "Inductivity L henry H = Wb/A";
        break;
    case DLMS_UNIT_FREQUENCY:
        ret = "Frequency";
        break;
    case DLMS_UNIT_ACTIVE:
        ret = "Active energy";
        break;
    case DLMS_UNIT_REACTIVE:
        ret = "Reactive energy";
        break;
    case DLMS_UNIT_APPARENT:
        ret = "Apparent energy";
        break;
    case DLMS_UNIT_V260:
        ret = "V260*60s";
        break;
    case DLMS_UNIT_A260:
        ret = "A260*60s";
        break;
    case DLMS_UNIT_MASS_KG_PER_SECOND:
        ret = "Mass";
        break;
    case DLMS_UNIT_CONDUCTANCE:
        ret = "Conductance siemens";
        break;
    case DLMS_UNIT_KELVIN:
        ret = "Kelvin";
        break;
    case DLMS_UNIT_RU2H:
        ret = "RU2h";
        break;
    case DLMS_UNIT_RI2H:
        ret = "RI2h";
        break;
    case DLMS_UNIT_CUBIC_METER_RV:
        ret = "Cubic meter RV";
        break;
    case DLMS_UNIT_PERCENTAGE:
        ret = "Percentage";
        break;
    case DLMS_UNIT_AMPERE_HOURS:
        ret = "Ampere hours";
        break;
    case DLMS_UNIT_ENERGY_PER_VOLUME:
        ret = "Energy per volume";
        break;
    case DLMS_UNIT_WOBBE:
        ret = "Wobbe";
        break;
    case DLMS_UNIT_MOLE_PERCENT:
        ret = "Mole percent";
        break;
    case DLMS_UNIT_MASS_DENSITY:
        ret = "Mass density";
        break;
    case DLMS_UNIT_PASCAL_SECOND:
        ret = "Pascal second";
        break;
    case DLMS_UNIT_JOULE_KILOGRAM:
        ret = "Joule kilogram";
        break;
    case DLMS_UNIT_PRESSURE_GRAM_PER_SQUARE_CENTIMETER:
        ret = "Pressure, gram per square centimeter.";
        break;
    case DLMS_UNIT_PRESSURE_ATMOSPHERE:
        ret = "Pressure, atmosphere.";
        break;
    case DLMS_UNIT_SIGNAL_STRENGTH_MILLI_WATT:
        ret = "Signal strength, dB milliwatt";
        break;
    case DLMS_UNIT_SIGNAL_STRENGTH_MICRO_VOLT:
        //logarithmic unit that expresses the ratio between two values of a physical quantity
        ret = "Signal strength, dB microvolt";
        break;
    case DLMS_UNIT_DB:
        ret = "dB";
        break;
    case DLMS_UNIT_INCH:
        ret = "Inch";
        break;
    case DLMS_UNIT_FOOT:
        ret = "Foot";
        break;
    case DLMS_UNIT_POUND:
        ret = "Pound";
        break;
    case DLMS_UNIT_FAHRENHEIT:
        ret = "Fahrenheit";
        break;
    case DLMS_UNIT_RANKINE:
        ret = "Rankine";
        break;
    case DLMS_UNIT_SQUARE_INCH:
        ret = "Square inch";
        break;
    case DLMS_UNIT_SQUARE_FOOT:
        ret = "Square foot";
        break;
    case DLMS_UNIT_ACRE:
        ret = "Acre";
        break;
    case DLMS_UNIT_CUBIC_INCH:
        ret = "Cubic inch";
        break;
    case DLMS_UNIT_CUBIC_FOOT:
        ret = "Cubic foot";
        break;
    case DLMS_UNIT_ACRE_FOOT:
        ret = "Acre foot";
        break;
    case DLMS_UNIT_GALLON_IMPERIAL:
        ret = "Gallon Imperial";
        break;
    case DLMS_UNIT_GALLON_US:
        ret = "GallonUS";
        break;
    case DLMS_UNIT_POUND_FORCE:
        ret = "Pound force";
        break;
    case DLMS_UNIT_POUND_FORCE_PER_SQUARE_INCH:
        ret = "Pound force per square inch";
        break;
    case DLMS_UNIT_POUND_PER_CUBIC_FOOT:
        ret = "Pound per cubic foot";
        break;
    case DLMS_UNIT_POUND_PER_FOOT_SECOND:
        ret = "Pound per foot second";
        break;
    case DLMS_UNIT_BRITISH_THERMAL_UNIT:
        ret = "British thermal unit";
        break;
    case DLMS_UNIT_THERM_EU:
        ret = "Therm EU";
        break;
    case DLMS_UNIT_THERM_US:
        ret = "Therm US";
        break;
    case DLMS_UNIT_BRITISH_THERMAL_UNIT_PER_POUND:
        ret = "British thermal unit per pound";
        break;
    case DLMS_UNIT_BRITISH_THERMAL_UNIT_PER_CUBIC_FOOT:
        ret = "British thermal unit per cubic foot";
        break;
    case DLMS_UNIT_CUBIC_FEET:
        ret = "Cubic feet";
        break;
    case DLMS_UNIT_FOOT_PER_SECOND:
        ret = "Foot per second";
        break;
    case DLMS_UNIT_CUBIC_FOOT_PER_MIN:
        ret = "Foot per min";
        break;
    case DLMS_UNIT_CUBIC_FOOT_PER_DAY:
        ret = "Foot per day";
        break;
    case DLMS_UNIT_ACRE_FOOT_PER_SECOND:
        ret = "Acre foot per second";
        break;
    case DLMS_UNIT_ACRE_FOOT_PER_MIN:
        ret = "Acre foot per min";
        break;
    case DLMS_UNIT_ACRE_FOOT_PER_HOUR:
        ret = "Acre foot per hour";
        break;
    case DLMS_UNIT_ACRE_FOOT_PER_DAY:
        ret = "Acre foot per day";
        break;
    case DLMS_UNIT_IMPERIAL_GALLON:
        ret = "Imperial gallon";
        break;
    case DLMS_UNIT_IMPERIAL_GALLON_PER_SECOND:
        ret = "Imperial gallon per second";
        break;
    case DLMS_UNIT_IMPERIAL_GALLON_PER_MIN:
        ret = "Imperial gallon per min";
        break;
    case DLMS_UNIT_IMPERIAL_GALLON_PER_HOUR:
        ret = "Imperial gallon per hour";
        break;
    case DLMS_UNIT_IMPERIAL_GALLON_PER_DAY:
        ret = "Imperial gallon per day";
        break;
    case DLMS_UNIT_US_GALLON:
        ret = "US Gallon";
        break;
    case DLMS_UNIT_US_GALLON_PER_SECOND:
        ret = "US gallon per second";
        break;
    case DLMS_UNIT_US_GALLON_PER_MIN:
        ret = "US gallon per min";
        break;
    case DLMS_UNIT_US_GALLON_PER_HOUR:
        ret = "US gallon per hour";
        break;
    case DLMS_UNIT_US_GALLON_PER_DAY:
        ret = "US gallon per day";
        break;
    case DLMS_UNIT_BRITISH_THERMAL_UNIT_PER_SECOND:
        ret = "British thermal unit per second";
        break;
    case DLMS_UNIT_BRITISH_THERMAL_UNIT_PER_MIN:
        ret = "British thermal unit per min";
        break;
    case DLMS_UNIT_BRITISH_THERMAL_UNIT_PER_HOUR:
        ret = "British thermal unit per hour";
        break;
    case DLMS_UNIT_BRITISH_THERMAL_UNIT_PER_DAY:
        ret = "British thermal unit per day";
        break;
    case 254:
        ret = "Other unit";
        break;
    case 255:
        ret = "NoUnit";
        break;
    default:
        ret = NULL;
        break;
    }
    return ret;
}

const char* DLMSConverter::ToString(DLMS_ADDRESS_STATE value)
{
    const char* str;
    switch (value)
    {
    case 0:
        str = "None";
        break;
    case 1:
        str = "Assigned";
        break;
    default:
        str = "Unknown Clock base";
        break;
    }
    return str;
}

//Get Disconnect control control state as a std::string.
const char* DLMSConverter::ToString(DLMS_CONTROL_STATE value)
{
    const char* str;
    switch (value)
    {
    case 0:
        str = "Disconnected";
        break;
    case 1:
        str = "Connected";
        break;
    case 2:
        str = "ReadyForReconnection";
        break;
    default:
        str = "Unknown control state";
        break;
    }
    return str;
}

//Get Disconnect control control mode as a std::string.
const char* DLMSConverter::ToString(DLMS_CONTROL_MODE value)
{
    const char* str;
    switch (value)
    {
    case 0:
        str = "None";
        break;
    case 1:
        str = "Mode1";
        break;
    case 2:
        str = "Mode2";
        break;
    case 3:
        str = "Mode3";
        break;
    case 4:
        str = "Mode4";
        break;
    case 5:
        str = "Mode5";
        break;
    case 6:
        str = "Mode6";
        break;
    default:
        str = "Unknown control mode";
        break;
    }
    return str;
}

//Get Security Setup security policy as a std::string.
const char* DLMSConverter::ToString(DLMS_SECURITY_POLICY value)
{
    const char* str;
    switch (value)
    {
    case DLMS_SECURITY_POLICY_NOTHING:
        str = "Nothing";
        break;
    case DLMS_SECURITY_POLICY_AUTHENTICATED:
        str = "Authenticated";
        break;
    case DLMS_SECURITY_POLICY_ENCRYPTED:
        str = "Encrypted";
        break;
    case DLMS_SECURITY_POLICY_AUTHENTICATED_ENCRYPTED:
        str = "AuthenticatedEncrypted";
        break;
    default:
        str = "Unknown security policy";
        break;
    }
    return str;
}

//Get Security Setup security policy v1 as a std::string.
int DLMSConverter::ToString(DLMS_SECURITY_POLICY value, std::string& str)
{
    str.clear();
    if (value == DLMS_SECURITY_POLICY_NOTHING)
    {
        str = "Nothing";
    }
    else
    {
        if (value == DLMS_SECURITY_POLICY_AUTHENTICATED)
        {
            str.append("Authenticated");
            return 0;
        }
        if (value == DLMS_SECURITY_POLICY_ENCRYPTED)
        {
            str.append("Encrypted");
            return 0;
        }
        if (value == DLMS_SECURITY_POLICY_AUTHENTICATED_ENCRYPTED)
        {
            str.append("AuthenticatedEncrypted");
            return 0;
        }
        if ((value & DLMS_SECURITY_POLICY_AUTHENTICATED_REQUEST) != 0)
        {
            str.append("RequestAuthenticated");
        }
        if ((value & DLMS_SECURITY_POLICY_ENCRYPTED_REQUEST) != 0)
        {
            if (!str.empty())
            {
                str.append(", ");
            }
            str.append("RequestEncrypted");
        }
        if ((value & DLMS_SECURITY_POLICY_DIGITALLY_SIGNED_REQUEST) != 0)
        {
            if (!str.empty())
            {
                str.append(", ");
            }
            str.append("RequestDigitallySigned");
        }
        if ((value & DLMS_SECURITY_POLICY_AUTHENTICATED_RESPONSE) != 0)
        {
            if (!str.empty())
            {
                str.append(", ");
            }
            str.append("ResponseAuthenticated");
        }
        if ((value & DLMS_SECURITY_POLICY_AUTHENTICATED_RESPONSE) != 0)
        {
            if (!str.empty())
            {
                str.append(", ");
            }
            str.append("ResponseEncrypted");
        }
        if ((value & DLMS_SECURITY_POLICY_ENCRYPTED_RESPONSE) != 0)
        {
            if (!str.empty())
            {
                str.append(", ");
            }
            str.append("ResponseDigitallySigned");
        }
    }
    return 0;
}

//Get Security Setup security suite as a std::string.
const char* DLMSConverter::ToString(DLMS_SECURITY_SUITE value)
{
    const char* ret = NULL;
    switch (value)
    {
    case DLMS_SECURITY_SUITE_V0:
        ret = "Version0";
        break;
    case DLMS_SECURITY_SUITE_V1:
        ret = "Version1";
        break;
    case DLMS_SECURITY_SUITE_V2:
        ret = "Version2";
        break;
    default:
        ret = "Unknown security suite";
        break;
    }
    return ret;
}

const char* DLMSConverter::ToString(DLMS_ASSOCIATION_STATUS value)
{
    switch (value)
    {
    case DLMS_ASSOCIATION_STATUS_NON_ASSOCIATED:
        return "NonAssociated";
        break;
    case DLMS_ASSOCIATION_STATUS_ASSOCIATION_PENDING:
        return "AssociationPending";
        break;
    case DLMS_ASSOCIATION_STATUS_ASSOCIATED:
        return "Associated";
        break;
    default:
        break;
    }
    return "Unknown association status";
}

const char* DLMSConverter::ToString(DLMS_OPTICAL_PROTOCOL_MODE value)
{
    switch (value)
    {
    case DLMS_OPTICAL_PROTOCOL_MODE_DEFAULT:
        return "Default";
        break;
    case DLMS_OPTICAL_PROTOCOL_MODE_NET:
        return "Net";
        break;
    case DLMS_OPTICAL_PROTOCOL_MODE_UNKNOWN:
        return "Unknown";
        break;
    default:
        break;
    }
    return "Unknown protocol mode";
}

const char* DLMSConverter::ToString(DLMS_LOCAL_PORT_RESPONSE_TIME value)
{
    switch (value)
    {
    case DLMS_LOCAL_PORT_RESPONSE_TIME_20_MS:
        return "ms20";
        break;
    case DLMS_LOCAL_PORT_RESPONSE_TIME_200_MS:
        return "ms200";
        break;
    default:
        break;
    }
    return "Unknown response time";
}

const char* DLMSConverter::ToString(DLMS_GSM_STATUS value)
{
    const char* str;
    switch (value)
    {
    case DLMS_GSM_STATUS_NONE:
        str = "None";
        break;
    case DLMS_GSM_STATUS_HOME_NETWORK:
        str = "HomeNetwork";
        break;
    case DLMS_GSM_STATUS_SEARCHING:
        str = "Searching";
        break;
    case DLMS_GSM_STATUS_DENIED:
        str = "Denied";
        break;
    case DLMS_GSM_STATUS_UNKNOWN:
        str = "Unknown";
        break;
    case DLMS_GSM_STATUS_ROAMING:
        str = "Roaming";
        break;
    default:
        str = NULL;
    }
    return str;
};

const char* DLMSConverter::ToString(DLMS_GSM_CIRCUIT_SWITCH_STATUS value)
{
    const char* str;
    switch (value)
    {
    case DLMS_GSM_CIRCUIT_SWITCH_STATUS_INACTIVE:
        str = "Inactive";
        break;
    case DLMS_GSM_CIRCUIT_SWITCH_STATUS_INCOMING_CALL:
        str = "Incoming call";
        break;
    case DLMS_GSM_CIRCUIT_SWITCH_STATUS_ACTIVE:
        str = "Active";
        break;
    default:
        str = NULL;
    }
    return str;
};


const char* DLMSConverter::ToString(DLMS_GSM_PACKET_SWITCH_STATUS value)
{
    const char* str;
    switch (value)
    {
    case DLMS_GSM_PACKET_SWITCH_STATUS_INACTIVE:
        str = "Inactive";
        break;
    case DLMS_GSM_PACKET_SWITCH_STATUS_GPRS:
        str = "GPRS";
        break;
    case DLMS_GSM_PACKET_SWITCH_STATUS_EDGE:
        str = "EDGE";
        break;
    case DLMS_GSM_PACKET_SWITCH_STATUS_UMTS:
        str = "UMTS";
        break;
    case DLMS_GSM_PACKET_SWITCH_STATUS_HSDPA:
        str = "HSDPA";
        break;
    default:
        str = NULL;
    }
    return str;
};

const char* DLMSConverter::ToString(DLMS_AUTHENTICATION value)
{
    const char* str;
    switch (value)
    {
    case DLMS_AUTHENTICATION_HIGH:
        str = "High";
        break;
    case DLMS_AUTHENTICATION_HIGH_GMAC:
        str = "HighGMac";
        break;
    case DLMS_AUTHENTICATION_HIGH_MD5:
        str = "HighMd5";
        break;
    case DLMS_AUTHENTICATION_HIGH_SHA1:
        str = "HighSha1";
        break;
    case DLMS_AUTHENTICATION_HIGH_SHA256:
        str = "HighSha256";
        break;
    case DLMS_AUTHENTICATION_LOW:
        str = "Low";
        break;
    case DLMS_AUTHENTICATION_NONE:
        str = "None";
        break;
    default:
        str = "UNKNOWN";
        break;
    }
    return str;
}

DLMS_AUTHENTICATION DLMSConverter::ValueOfAuthentication(const char* value) {
    DLMS_AUTHENTICATION v;
    if (strcmp(value, "None") == 0)
    {
        v = DLMS_AUTHENTICATION_NONE;
    }
    else if (strcmp(value, "Low") == 0)
    {
        v = DLMS_AUTHENTICATION_LOW;
    }
    else if (strcmp(value, "High") == 0)
    {
        v = DLMS_AUTHENTICATION_HIGH;
    }
    else if (strcmp(value, "HighMd5") == 0)
    {
        v = DLMS_AUTHENTICATION_HIGH_MD5;
    }
    else if (strcmp(value, "HighSha1") == 0)
    {
        v = DLMS_AUTHENTICATION_HIGH_SHA1;
    }
    else if (strcmp(value, "HighSha256") == 0)
    {
        v = DLMS_AUTHENTICATION_HIGH_SHA256;
    }
    else if (strcmp(value, "HighGMac") == 0)
    {
        v = DLMS_AUTHENTICATION_HIGH_GMAC;
    }
    else {
        v = (DLMS_AUTHENTICATION)-1;
    }
    return v;
}

//Convert association enum value to string.
const char* DLMSConverter::ToString(DLMS_ASSOCIATION_RESULT value)
{
    const char* str;
    switch (value)
    {
    case DLMS_ASSOCIATION_RESULT_ACCEPTED:
        str = "ACCEPTED";
        break;
    case DLMS_ASSOCIATION_RESULT_PERMANENT_REJECTED:
        str = "PERMANENT_REJECTED";
        break;
    case DLMS_ASSOCIATION_RESULT_TRANSIENT_REJECTED:
        str = "TRANSIENT_REJECTED";
        break;
    default:
        str = "UNKNOWN";
        break;
    }
    return str;
}

DLMS_ASSOCIATION_RESULT DLMSConverter::ValueOfAssociation(const char* value)
{
    DLMS_ASSOCIATION_RESULT v;
    if (strcmp(value, "Accepted") == 0)
    {
        v = DLMS_ASSOCIATION_RESULT_ACCEPTED;
    }
    else if (strcmp(value, "PERMANENT_REJECTED") == 0)
    {
        v = DLMS_ASSOCIATION_RESULT_PERMANENT_REJECTED;
    }
    else if (strcmp(value, "TRANSIENT_REJECTED") == 0)
    {
        v = DLMS_ASSOCIATION_RESULT_TRANSIENT_REJECTED;
    }
    else {
        v = (DLMS_ASSOCIATION_RESULT)-1;
    }
    return v;
}

const char* DLMSConverter::ToString(DLMS_SOURCE_DIAGNOSTIC value)
{
    const char* str;
    switch (value)
    {
    case DLMS_SOURCE_DIAGNOSTIC_NONE:
        str = "NONE";
        break;
    case DLMS_SOURCE_DIAGNOSTIC_APPLICATION_CONTEXT_NAME_NOT_SUPPORTED:
        str = "APPLICATION_CONTEXT_NAME_NOT_SUPPORTED";
        break;
    case DLMS_SOURCE_DIAGNOSTIC_APPLICATION_CALLING_AP_TITLE_NOT_RECOGNIZED:
        str = "APPLICATION_CALLING_AP_TITLE_NOT_RECOGNIZED";
        break;
    case DLMS_SOURCE_DIAGNOSTIC_NO_REASON_GIVEN:
        str = "NO_REASON_GIVEN";
        break;
    case DLMS_SOURCE_DIAGNOSTIC_APPLICATION_CALLING_AP_INVOCATION_IDENTIFIER_NOT_RECOGNIZED:
        str = "APPLICATION_CALLING_AP_INVOCATION_IDENTIFIER_NOT_RECOGNIZED";
        break;
    case DLMS_SOURCE_DIAGNOSTIC_APPLICATION_CALLING_AE_QUALIFIER_NOT_RECOGNIZED:
        str = "APPLICATION_CALLING_AE_QUALIFIER_NOT_RECOGNIZED ";
        break;
    case DLMS_SOURCE_DIAGNOSTIC_APPLICATION_CALLING_AE_INVOCATION_IDENTIFIER_NOT_RECOGNIZED:
        str = "APPLICATION_CALLING_AE_INVOCATION_IDENTIFIER_NOT_RECOGNIZED";
        break;
    case DLMS_SOURCE_DIAGNOSTIC_APPLICATION_CALLED_AP_TITLE_NOT_RECOGNIZED:
        str = "APPLICATION_CALLED_AP_TITLE_NOT_RECOGNIZED";
        break;
    case DLMS_SOURCE_DIAGNOSTIC_APPLICATION_CALLED_AP_INVOCATION_IDENTIFIER_NOT_RECOGNIZED:
        str = "APPLICATION_CALLED_AP_INVOCATION_IDENTIFIER_NOT_RECOGNIZED";
        break;
    case DLMS_SOURCE_DIAGNOSTIC_APPLICATION_CALLED_AE_QUALIFIER_NOT_RECOGNIZED:
        str = "APPLICATION_CALLED_AE_QUALIFIER_NOT_RECOGNIZED";
        break;
    case DLMS_SOURCE_DIAGNOSTIC_APPLICATION_CALLED_AE_INVOCATION_IDENTIFIER_NOT_RECOGNIZED:
        str = "APPLICATION_CALLED_AE_INVOCATION_IDENTIFIER_NOT_RECOGNIZED";
        break;
    case DLMS_SOURCE_DIAGNOSTIC_AUTHENTICATION_MECHANISM_NAME_NOT_RECOGNISED:
        str = "AUTHENTICATION_MECHANISM_NAME_NOT_RECOGNISED";
        break;
    case DLMS_SOURCE_DIAGNOSTIC_AUTHENTICATION_MECHANISM_NAME_REQUIRED:
        str = "AUTHENTICATION_MECHANISM_NAME_REQUIRED";
        break;
    case DLMS_SOURCE_DIAGNOSTIC_AUTHENTICATION_FAILURE:
        str = "AUTHENTICATION_FAILURE";
        break;
    case DLMS_SOURCE_DIAGNOSTIC_AUTHENTICATION_REQUIRED:
        str = "AUTHENTICATION_REQUIRED";
        break;
    default:
        str = "UNKNOWN";
        break;
    }
    return str;
}

DLMS_SOURCE_DIAGNOSTIC DLMSConverter::ValueOfSourceDiagnostic(const char* value)
{
    DLMS_SOURCE_DIAGNOSTIC v;
    if (strcmp(value, "NONE") == 0)
    {
        v = DLMS_SOURCE_DIAGNOSTIC_NONE;
    }
    else if (strcmp(value, "APPLICATION_CONTEXT_NAME_NOT_SUPPORTED") == 0)
    {
        v = DLMS_SOURCE_DIAGNOSTIC_APPLICATION_CONTEXT_NAME_NOT_SUPPORTED;
    }
    else if (strcmp(value, "APPLICATION_CALLING_AP_TITLE_NOT_RECOGNIZED") == 0)
    {
        v = DLMS_SOURCE_DIAGNOSTIC_APPLICATION_CALLING_AP_TITLE_NOT_RECOGNIZED;
    }
    else if (strcmp(value, "NO_REASON_GIVEN") == 0)
    {
        v = DLMS_SOURCE_DIAGNOSTIC_NO_REASON_GIVEN;
    }
    else if (strcmp(value, "APPLICATION_CALLING_AP_INVOCATION_IDENTIFIER_NOT_RECOGNIZED") == 0)
    {
        v = DLMS_SOURCE_DIAGNOSTIC_APPLICATION_CALLING_AP_INVOCATION_IDENTIFIER_NOT_RECOGNIZED;
    }
    else if (strcmp(value, "APPLICATION_CALLING_AE_QUALIFIER_NOT_RECOGNIZED") == 0)
    {
        v = DLMS_SOURCE_DIAGNOSTIC_APPLICATION_CALLING_AE_QUALIFIER_NOT_RECOGNIZED;
    }
    else if (strcmp(value, "APPLICATION_CALLING_AE_INVOCATION_IDENTIFIER_NOT_RECOGNIZED") == 0)
    {
        v = DLMS_SOURCE_DIAGNOSTIC_APPLICATION_CALLING_AE_INVOCATION_IDENTIFIER_NOT_RECOGNIZED;
    }
    else if (strcmp(value, "APPLICATION_CALLED_AP_TITLE_NOT_RECOGNIZED") == 0)
    {
        v = DLMS_SOURCE_DIAGNOSTIC_APPLICATION_CALLED_AP_TITLE_NOT_RECOGNIZED;
    }
    else if (strcmp(value, "APPLICATION_CALLED_AP_INVOCATION_IDENTIFIER_NOT_RECOGNIZED") == 0)
    {
        v = DLMS_SOURCE_DIAGNOSTIC_APPLICATION_CALLED_AP_INVOCATION_IDENTIFIER_NOT_RECOGNIZED;
    }
    else if (strcmp(value, "APPLICATION_CALLED_AE_QUALIFIER_NOT_RECOGNIZED") == 0)
    {
        v = DLMS_SOURCE_DIAGNOSTIC_APPLICATION_CALLED_AE_QUALIFIER_NOT_RECOGNIZED;
    }
    else if (strcmp(value, "APPLICATION_CALLED_AE_INVOCATION_IDENTIFIER_NOT_RECOGNIZED") == 0)
    {
        v = DLMS_SOURCE_DIAGNOSTIC_APPLICATION_CALLED_AE_INVOCATION_IDENTIFIER_NOT_RECOGNIZED;
    }
    else if (strcmp(value, "AUTHENTICATION_MECHANISM_NAME_NOT_RECOGNISED") == 0)
    {
        v = DLMS_SOURCE_DIAGNOSTIC_AUTHENTICATION_MECHANISM_NAME_NOT_RECOGNISED;
    }
    else if (strcmp(value, "AUTHENTICATION_MECHANISM_NAME_REQUIRED") == 0)
    {
        v = DLMS_SOURCE_DIAGNOSTIC_AUTHENTICATION_MECHANISM_NAME_REQUIRED;
    }
    else if (strcmp(value, "AUTHENTICATION_FAILURE") == 0)
    {
        v = DLMS_SOURCE_DIAGNOSTIC_AUTHENTICATION_FAILURE;
    }
    else if (strcmp(value, "AUTHENTICATION_REQUIRED") == 0)
    {
        v = DLMS_SOURCE_DIAGNOSTIC_AUTHENTICATION_REQUIRED;
    }
    else {
        v = (DLMS_SOURCE_DIAGNOSTIC)-1;
    }
    return v;
}

const char* DLMSConverter::ToString(DLMS_SECURITY value)
{
    switch (value)
    {
    case DLMS_SECURITY_NONE:
        return "None";
    case DLMS_SECURITY_AUTHENTICATION:
        return "Authentication";
    case DLMS_SECURITY_ENCRYPTION:
        return "Encryption";
    case DLMS_SECURITY_AUTHENTICATION_ENCRYPTION:
        return "AuthenticationEncryption";
    default:
        break;
    }
    return "Invalid";
}

const char* DLMSConverter::ToString(DLMS_OBJECT_TYPE type)
{
    const char* str;
    switch (type)
    {
    case DLMS_OBJECT_TYPE_ACTION_SCHEDULE:
        str = "GXDLMSActionSchedule";
        break;
    case DLMS_OBJECT_TYPE_ACTIVITY_CALENDAR:
        str = "GXDLMSActivityCalendar";
        break;
    case DLMS_OBJECT_TYPE_ASSOCIATION_LOGICAL_NAME:
        str = "GXDLMSAssociationLogicalName";
        break;
    case DLMS_OBJECT_TYPE_ASSOCIATION_SHORT_NAME:
        str = "GXDLMSAssociationShortName";
        break;
    case DLMS_OBJECT_TYPE_AUTO_ANSWER:
        str = "GXDLMSAutoAnswer";
        break;
    case DLMS_OBJECT_TYPE_AUTO_CONNECT:
        str = "GXDLMSAutoConnect";
        break;
    case DLMS_OBJECT_TYPE_CLOCK:
        str = "GXDLMSClock";
        break;
    case DLMS_OBJECT_TYPE_DATA:
        str = "GXDLMSData";
        break;
    case DLMS_OBJECT_TYPE_DEMAND_REGISTER:
        str = "GXDLMSDemandRegister";
        break;
    case DLMS_OBJECT_TYPE_MAC_ADDRESS_SETUP:
        str = "GXDLMSMacAddressSetup";
        break;
    case DLMS_OBJECT_TYPE_EXTENDED_REGISTER:
        str = "GXDLMSExtendedRegister";
        break;
    case DLMS_OBJECT_TYPE_GPRS_SETUP:
        str = "GXDLMSGprsSetup";
        break;
    case DLMS_OBJECT_TYPE_SECURITY_SETUP:
        str = "GXDLMSSecuritySetup";
        break;
    case DLMS_OBJECT_TYPE_IEC_HDLC_SETUP:
        str = "GXDLMSIecHdlcSetup";
        break;
    case DLMS_OBJECT_TYPE_IEC_LOCAL_PORT_SETUP:
        str = "GXDLMSIecLocalPortSetup";
        break;
    case DLMS_OBJECT_TYPE_IEC_TWISTED_PAIR_SETUP:
        str = "GXDLMSIEC_TWISTED_PAIR_SETUP";
        break;
    case DLMS_OBJECT_TYPE_IP4_SETUP:
        str = "GXDLMSIp4Setup";
        break;
    case DLMS_OBJECT_TYPE_IP6_SETUP:
        str = "GXDLMSIp6Setup";
        break;
    case DLMS_OBJECT_TYPE_MBUS_SLAVE_PORT_SETUP:
        str = "GXDLMSMBusSlavePortSetup";
        break;
    case DLMS_OBJECT_TYPE_IMAGE_TRANSFER:
        str = "GXDLMSImageTransfer";
        break;
    case DLMS_OBJECT_TYPE_DISCONNECT_CONTROL:
        str = "GXDLMSDisconnectControl";
        break;
    case DLMS_OBJECT_TYPE_LIMITER:
        str = "GXDLMSLimiter";
        break;
    case DLMS_OBJECT_TYPE_MBUS_CLIENT:
        str = "GXDLMSMBusClient";
        break;
    case DLMS_OBJECT_TYPE_MODEM_CONFIGURATION:
        str = "GXDLMSModemConfiguration";
        break;
    case DLMS_OBJECT_TYPE_PPP_SETUP:
        str = "GXDLMSPppSetup";
        break;
    case DLMS_OBJECT_TYPE_PROFILE_GENERIC:
        str = "GXDLMSProfileGeneric";
        break;
    case DLMS_OBJECT_TYPE_REGISTER:
        str = "GXDLMSRegister";
        break;
    case DLMS_OBJECT_TYPE_REGISTER_ACTIVATION:
        str = "GXDLMSRegisterActivation";
        break;
    case DLMS_OBJECT_TYPE_REGISTER_MONITOR:
        str = "GXDLMSRegisterMonitor";
        break;
    case DLMS_OBJECT_TYPE_REGISTER_TABLE:
        str = "GXDLMSRegisterTable";
        break;
    case DLMS_OBJECT_TYPE_ZIG_BEE_SAS_STARTUP:
        str = "GXDLMSZigBeeSasStartup";
        break;
    case DLMS_OBJECT_TYPE_ZIG_BEE_SAS_JOIN:
        str = "GXDLMSZigBeeSasJoin";
        break;
    case DLMS_OBJECT_TYPE_ZIG_BEE_SAS_APS_FRAGMENTATION:
        str = "GXDLMSZigBeeSasApsFragmentation";
        break;
    case DLMS_OBJECT_TYPE_ZIG_BEE_NETWORK_CONTROL:
        str = "GXDLMSZigBeeNetworkControl";
        break;
    case DLMS_OBJECT_TYPE_SAP_ASSIGNMENT:
        str = "GXDLMSSapAssignment";
        break;
    case DLMS_OBJECT_TYPE_SCHEDULE:
        str = "GXDLMSSchedule";
        break;
    case DLMS_OBJECT_TYPE_SCRIPT_TABLE:
        str = "GXDLMSScriptTable";
        break;
    case DLMS_OBJECT_TYPE_SMTP_SETUP:
        str = "GXDLMSSMTPSetup";
        break;
    case DLMS_OBJECT_TYPE_SPECIAL_DAYS_TABLE:
        str = "GXDLMSSpecialDaysTable";
        break;
    case DLMS_OBJECT_TYPE_STATUS_MAPPING:
        str = "GXDLMSStatusMapping";
        break;
    case DLMS_OBJECT_TYPE_TCP_UDP_SETUP:
        str = "GXDLMSTcpUdpSetup";
        break;
    case DLMS_OBJECT_TYPE_UTILITY_TABLES:
        str = "GXDLMSUtilityTables";
        break;
    case DLMS_OBJECT_TYPE_MBUS_MASTER_PORT_SETUP:
        str = "GXDLMSMBusMasterPortSetup";
        break;
    case DLMS_OBJECT_TYPE_MESSAGE_HANDLER:
        str = "GXDLMSMessageHandler";
        break;
    case DLMS_OBJECT_TYPE_PUSH_SETUP:
        str = "GXDLMSPushSetup";
        break;
    case DLMS_OBJECT_TYPE_ACCOUNT:
        str = "GXDLMSAccount";
        break;
    case DLMS_OBJECT_TYPE_CREDIT:
        str = "GXDLMSCredit";
        break;
    case DLMS_OBJECT_TYPE_CHARGE:
        str = "GXDLMSCharge";
        break;
    case DLMS_OBJECT_TYPE_PARAMETER_MONITOR:
        str = "GXDLMSParameterMonitor";
        break;
    case DLMS_OBJECT_TYPE_TOKEN_GATEWAY:
        str = "GXDLMSTokenGateway";
        break;
    case DLMS_OBJECT_TYPE_GSM_DIAGNOSTIC:
        str = "GXDLMSGSMDiagnostic";
        break;
    case DLMS_OBJECT_TYPE_COMPACT_DATA:
        str = "GXDLMSCompactData";
        break;
    case DLMS_OBJECT_TYPE_LLC_SSCS_SETUP:
        str = "GXDLMSLlcSscsSetup";
        break;
    case DLMS_OBJECT_TYPE_PRIME_NB_OFDM_PLC_PHYSICAL_LAYER_COUNTERS:
        str = "GXDLMSPrimeNbOfdmPlcPhysicalLayerCounters";
        break;
    case DLMS_OBJECT_TYPE_PRIME_NB_OFDM_PLC_MAC_SETUP:
        str = "GXDLMSPrimeNbOfdmPlcMacSetup";
        break;
    case DLMS_OBJECT_TYPE_PRIME_NB_OFDM_PLC_MAC_FUNCTIONAL_PARAMETERS:
        str = "GXDLMSPrimeNbOfdmPlcMacFunctionalParameters";
        break;
    case DLMS_OBJECT_TYPE_PRIME_NB_OFDM_PLC_MAC_COUNTERS:
        str = "GXDLMSPrimeNbOfdmPlcMacCounters";
        break;
    case DLMS_OBJECT_TYPE_PRIME_NB_OFDM_PLC_MAC_NETWORK_ADMINISTRATION_DATA:
        str = "GXDLMSPrimeNbOfdmPlcMacNetworkAdministrationData";
        break;
    case DLMS_OBJECT_TYPE_PRIME_NB_OFDM_PLC_APPLICATIONS_IDENTIFICATION:
        str = "GXDLMSPrimeNbOfdmPlcApplicationsIdentification";
        break;
#ifndef DLMS_IGNORE_IEC_8802_LLC_TYPE1_SETUP
    case DLMS_OBJECT_TYPE_IEC_8802_LLC_TYPE1_SETUP:
        str = "GXDLMSIec8802LlcType1Setup";
        break;
#endif //DLMS_IGNORE_IEC_8802_LLC_TYPE1_SETUP
#ifndef DLMS_IGNORE_IEC_8802_LLC_TYPE2_SETUP
    case DLMS_OBJECT_TYPE_IEC_8802_LLC_TYPE2_SETUP:
        str = "GXDLMSIec8802LlcType2Setup";
        break;
#endif //DLMS_IGNORE_IEC_8802_LLC_TYPE2_SETUP
#ifndef DLMS_IGNORE_IEC_8802_LLC_TYPE3_SETUP
    case DLMS_OBJECT_TYPE_IEC_8802_LLC_TYPE3_SETUP:
        str = "GXDLMSIec8802LlcType3Setup";
        break;
#endif //DLMS_IGNORE_IEC_8802_LLC_TYPE3_SETUP
#ifndef DLMS_IGNORE_SFSK_REPORTING_SYSTEM_LIST
    case DLMS_OBJECT_TYPE_SFSK_REPORTING_SYSTEM_LIST:
        str = "GXDLMSSFSKReportingSystemList";
        break;
#endif //DLMS_IGNORE_SFSK_REPORTING_SYSTEM_LIST
#ifndef DLMS_IGNORE_ARBITRATOR
    case DLMS_OBJECT_TYPE_ARBITRATOR:
        str = "GXDLMSArbitrator";
        break;
#endif //DLMS_IGNORE_ARBITRATOR
#ifndef DLMS_IGNORE_SFSK_MAC_COUNTERS
    case DLMS_OBJECT_TYPE_SFSK_MAC_COUNTERS:
        str = "GXDLMSSFSKMacCounters";
        break;
#endif //DLMS_IGNORE_SFSK_MAC_COUNTERS
#ifndef DLMS_IGNORE_SFSK_MAC_SYNCHRONIZATION_TIMEOUTS
    case DLMS_OBJECT_TYPE_SFSK_MAC_SYNCHRONIZATION_TIMEOUTS:
        str = "GXDLMSSFSKMacSynchronizationTimeouts";
        break;
#endif //DLMS_IGNORE_SFSK_MAC_SYNCHRONIZATION_TIMEOUTS
#ifndef DLMS_IGNORE_SFSK_ACTIVE_INITIATOR
    case DLMS_OBJECT_TYPE_SFSK_ACTIVE_INITIATOR:
        str = "GXDLMSSFSKActiveInitiator";
        break;
#endif //DLMS_IGNORE_SFSK_ACTIVE_INITIATOR
#ifndef DLMS_IGNORE_SFSK_PHY_MAC_SETUP
    case DLMS_OBJECT_TYPE_SFSK_PHY_MAC_SETUP:
        str = "GXDLMSSFSKPhyMacSetUp";
        break;
#endif //DLMS_IGNORE_SFSK_PHY_MAC_SETUP
    default:
        str = "Manufacture spesific.";
        break;
    }
    return str;
}

DLMS_OBJECT_TYPE DLMSConverter::ValueOfObjectType(const char* value)
{
    DLMS_OBJECT_TYPE type;
    if (strcmp(value, "GXDLMSActionSchedule") == 0)
        type = DLMS_OBJECT_TYPE_ACTION_SCHEDULE;
    else if (strcmp(value, "GXDLMSActivityCalendar") == 0)
        type = DLMS_OBJECT_TYPE_ACTIVITY_CALENDAR;
    else if (strcmp(value, "GXDLMSAssociationLogicalName") == 0)
        type = DLMS_OBJECT_TYPE_ASSOCIATION_LOGICAL_NAME;
    else if (strcmp(value, "GXDLMSAssociationShortName") == 0)
        type = DLMS_OBJECT_TYPE_ASSOCIATION_SHORT_NAME;
    else if (strcmp(value, "GXDLMSAutoAnswer") == 0)
        type = DLMS_OBJECT_TYPE_AUTO_ANSWER;
    else if (strcmp(value, "GXDLMSAutoConnect") == 0)
        type = DLMS_OBJECT_TYPE_AUTO_CONNECT;
    else if (strcmp(value, "GXDLMSClock") == 0)
        type = DLMS_OBJECT_TYPE_CLOCK;
    else if (strcmp(value, "GXDLMSData") == 0)
        type = DLMS_OBJECT_TYPE_DATA;
    else if (strcmp(value, "GXDLMSDemandRegister") == 0)
        type = DLMS_OBJECT_TYPE_DEMAND_REGISTER;
    else if (strcmp(value, "GXDLMSMacAddressSetup") == 0)
        type = DLMS_OBJECT_TYPE_MAC_ADDRESS_SETUP;
    else if (strcmp(value, "GXDLMSExtendedRegister") == 0)
        type = DLMS_OBJECT_TYPE_EXTENDED_REGISTER;
    else if (strcmp(value, "GXDLMSGprsSetup") == 0)
        type = DLMS_OBJECT_TYPE_GPRS_SETUP;
    else if (strcmp(value, "GXDLMSSecuritySetup") == 0)
        type = DLMS_OBJECT_TYPE_SECURITY_SETUP;
    else if (strcmp(value, "GXDLMSIecHdlcSetup") == 0 || strcmp(value, "GXDLMSHdlcSetup") == 0)
        type = DLMS_OBJECT_TYPE_IEC_HDLC_SETUP;
    else if (strcmp(value, "GXDLMSIecLocalPortSetup") == 0 || strcmp(value, "GXDLMSIECOpticalPortSetup") == 0)
        type = DLMS_OBJECT_TYPE_IEC_LOCAL_PORT_SETUP;
    else if (strcmp(value, "GXDLMSIEC_TWISTED_PAIR_SETUP") == 0)
        type = DLMS_OBJECT_TYPE_IEC_TWISTED_PAIR_SETUP;
    else if (strcmp(value, "GXDLMSIp4Setup") == 0)
        type = DLMS_OBJECT_TYPE_IP4_SETUP;
    else if (strcmp(value, "GXDLMSIp6Setup") == 0)
        type = DLMS_OBJECT_TYPE_IP6_SETUP;
    else if (strcmp(value, "GXDLMSMBusSlavePortSetup") == 0)
        type = DLMS_OBJECT_TYPE_MBUS_SLAVE_PORT_SETUP;
    else if (strcmp(value, "GXDLMSImageTransfer") == 0)
        type = DLMS_OBJECT_TYPE_IMAGE_TRANSFER;
    else if (strcmp(value, "GXDLMSDisconnectControl") == 0)
        type = DLMS_OBJECT_TYPE_DISCONNECT_CONTROL;
    else if (strcmp(value, "GXDLMSLimiter") == 0)
        type = DLMS_OBJECT_TYPE_LIMITER;
    else if (strcmp(value, "GXDLMSMBusClient") == 0)
        type = DLMS_OBJECT_TYPE_MBUS_CLIENT;
    else if (strcmp(value, "GXDLMSModemConfiguration") == 0)
        type = DLMS_OBJECT_TYPE_MODEM_CONFIGURATION;
    else if (strcmp(value, "GXDLMSPppSetup") == 0)
        type = DLMS_OBJECT_TYPE_PPP_SETUP;
    else if (strcmp(value, "GXDLMSProfileGeneric") == 0)
        type = DLMS_OBJECT_TYPE_PROFILE_GENERIC;
    else if (strcmp(value, "GXDLMSRegister") == 0)
        type = DLMS_OBJECT_TYPE_REGISTER;
    else if (strcmp(value, "GXDLMSRegisterActivation") == 0)
        type = DLMS_OBJECT_TYPE_REGISTER_ACTIVATION;
    else if (strcmp(value, "GXDLMSRegisterMonitor") == 0)
        type = DLMS_OBJECT_TYPE_REGISTER_MONITOR;
    else if (strcmp(value, "GXDLMSRegisterTable") == 0)
        type = DLMS_OBJECT_TYPE_REGISTER_TABLE;
    else if (strcmp(value, "GXDLMSZigBeeSasStartup") == 0)
        type = DLMS_OBJECT_TYPE_ZIG_BEE_SAS_STARTUP;
    else if (strcmp(value, "GXDLMSZigBeeSasJoin") == 0)
        type = DLMS_OBJECT_TYPE_ZIG_BEE_SAS_JOIN;
    else if (strcmp(value, "GXDLMSZigBeeSasApsFragmentation") == 0)
        type = DLMS_OBJECT_TYPE_ZIG_BEE_SAS_APS_FRAGMENTATION;
    else if (strcmp(value, "GXDLMSZigBeeNetworkControl") == 0)
        type = DLMS_OBJECT_TYPE_ZIG_BEE_NETWORK_CONTROL;
    else if (strcmp(value, "GXDLMSSapAssignment") == 0)
        type = DLMS_OBJECT_TYPE_SAP_ASSIGNMENT;
    else if (strcmp(value, "GXDLMSSchedule") == 0)
        type = DLMS_OBJECT_TYPE_SCHEDULE;
    else if (strcmp(value, "GXDLMSScriptTable") == 0)
        type = DLMS_OBJECT_TYPE_SCRIPT_TABLE;
    else if (strcmp(value, "GXDLMSSMTPSetup") == 0)
        type = DLMS_OBJECT_TYPE_SMTP_SETUP;
    else if (strcmp(value, "GXDLMSSpecialDaysTable") == 0)
        type = DLMS_OBJECT_TYPE_SPECIAL_DAYS_TABLE;
    else if (strcmp(value, "GXDLMSStatusMapping") == 0)
        type = DLMS_OBJECT_TYPE_STATUS_MAPPING;
    else if (strcmp(value, "GXDLMSTcpUdpSetup") == 0)
        type = DLMS_OBJECT_TYPE_TCP_UDP_SETUP;
    else if (strcmp(value, "GXDLMSUtilityTables") == 0)
        type = DLMS_OBJECT_TYPE_UTILITY_TABLES;
    else if (strcmp(value, "GXDLMSMBusMasterPortSetup") == 0)
        type = DLMS_OBJECT_TYPE_MBUS_MASTER_PORT_SETUP;
    else if (strcmp(value, "GXDLMSMessageHandler") == 0)
        type = DLMS_OBJECT_TYPE_MESSAGE_HANDLER;
    else if (strcmp(value, "GXDLMSPushSetup") == 0)
        type = DLMS_OBJECT_TYPE_PUSH_SETUP;
    else if (strcmp(value, "GXDLMSAccount") == 0)
        type = DLMS_OBJECT_TYPE_ACCOUNT;
    else if (strcmp(value, "GXDLMSCredit") == 0)
        type = DLMS_OBJECT_TYPE_CREDIT;
    else if (strcmp(value, "GXDLMSCharge") == 0)
        type = DLMS_OBJECT_TYPE_CHARGE;
    else if (strcmp(value, "GXDLMSParameterMonitor") == 0)
        type = DLMS_OBJECT_TYPE_PARAMETER_MONITOR;
    else if (strcmp(value, "GXDLMSTokenGateway") == 0)
        type = DLMS_OBJECT_TYPE_TOKEN_GATEWAY;
    else if (strcmp(value, "GXDLMSGSMDiagnostic") == 0)
        type = DLMS_OBJECT_TYPE_GSM_DIAGNOSTIC;
    else if (strcmp(value, "GXDLMSCompactData") == 0)
        type = DLMS_OBJECT_TYPE_COMPACT_DATA;
    else if (strcmp(value, "GXDLMSLlcSscsSetup") == 0)
        type = DLMS_OBJECT_TYPE_LLC_SSCS_SETUP;
    else if (strcmp(value, "GXDLMSPrimeNbOfdmPlcPhysicalLayerCounters") == 0)
        type = DLMS_OBJECT_TYPE_PRIME_NB_OFDM_PLC_PHYSICAL_LAYER_COUNTERS;
    else if (strcmp(value, "GXDLMSPrimeNbOfdmPlcMacSetup") == 0)
        type = DLMS_OBJECT_TYPE_PRIME_NB_OFDM_PLC_MAC_SETUP;
    else if (strcmp(value, "GXDLMSPrimeNbOfdmPlcMacFunctionalParameters") == 0)
        type = DLMS_OBJECT_TYPE_PRIME_NB_OFDM_PLC_MAC_FUNCTIONAL_PARAMETERS;
    else if (strcmp(value, "GXDLMSPrimeNbOfdmPlcMacCounters") == 0)
        type = DLMS_OBJECT_TYPE_PRIME_NB_OFDM_PLC_MAC_COUNTERS;
    else if (strcmp(value, "GXDLMSPrimeNbOfdmPlcMacNetworkAdministrationData") == 0)
        type = DLMS_OBJECT_TYPE_PRIME_NB_OFDM_PLC_MAC_NETWORK_ADMINISTRATION_DATA;
    else if (strcmp(value, "GXDLMSPrimeNbOfdmPlcApplicationsIdentification") == 0)
        type = DLMS_OBJECT_TYPE_PRIME_NB_OFDM_PLC_APPLICATIONS_IDENTIFICATION;
#ifndef DLMS_IGNORE_IEC_8802_LLC_TYPE1_SETUP
    else if (strcmp(value, "GXDLMSIec8802LlcType1Setup") == 0)
        type = DLMS_OBJECT_TYPE_IEC_8802_LLC_TYPE1_SETUP;
#endif //DLMS_IGNORE_IEC_8802_LLC_TYPE1_SETUP
#ifndef DLMS_IGNORE_IEC_8802_LLC_TYPE2_SETUP
    else if (strcmp(value, "GXDLMSIec8802LlcType2Setup") == 0)
        type = DLMS_OBJECT_TYPE_IEC_8802_LLC_TYPE2_SETUP;
#endif //DLMS_IGNORE_IEC_8802_LLC_TYPE2_SETUP
#ifndef DLMS_IGNORE_IEC_8802_LLC_TYPE3_SETUP
    else if (strcmp(value, "GXDLMSIec8802LlcType3Setup") == 0)
        type = DLMS_OBJECT_TYPE_IEC_8802_LLC_TYPE3_SETUP;
#endif //DLMS_IGNORE_IEC_8802_LLC_TYPE3_SETUP
#ifndef DLMS_IGNORE_SFSK_REPORTING_SYSTEM_LIST
    else if (strcmp(value, "GXDLMSSFSKReportingSystemList") == 0)
        type = DLMS_OBJECT_TYPE_SFSK_REPORTING_SYSTEM_LIST;
#endif //DLMS_IGNORE_SFSK_REPORTING_SYSTEM_LIST
#ifndef DLMS_IGNORE_ARBITRATOR
    else if (strcmp(value, "GXDLMSArbitrator") == 0)
        type = DLMS_OBJECT_TYPE_ARBITRATOR;
#endif //DLMS_IGNORE_ARBITRATOR
#ifndef DLMS_IGNORE_SFSK_MAC_COUNTERS
    else if (strcmp(value, "GXDLMSSFSKMacCounters") == 0)
        type = DLMS_OBJECT_TYPE_SFSK_MAC_COUNTERS;
#endif //DLMS_IGNORE_SFSK_MAC_COUNTERS
#ifndef DLMS_IGNORE_SFSK_MAC_SYNCHRONIZATION_TIMEOUTS
    else if (strcmp(value, "GXDLMSSFSKMacSynchronizationTimeouts") == 0)
        type = DLMS_OBJECT_TYPE_SFSK_MAC_SYNCHRONIZATION_TIMEOUTS;
#endif //DLMS_IGNORE_SFSK_MAC_SYNCHRONIZATION_TIMEOUTS
#ifndef DLMS_IGNORE_SFSK_ACTIVE_INITIATOR
    else if (strcmp(value, "GXDLMSSFSKActiveInitiator") == 0)
        type = DLMS_OBJECT_TYPE_SFSK_ACTIVE_INITIATOR;
#endif //DLMS_IGNORE_SFSK_ACTIVE_INITIATOR
#ifndef DLMS_IGNORE_SFSK_PHY_MAC_SETUP
    else if (strcmp(value, "GXDLMSSFSKPhyMacSetUp") == 0)
        type = DLMS_OBJECT_TYPE_SFSK_PHY_MAC_SETUP;
#endif //DLMS_IGNORE_SFSK_PHY_MAC_SETUP
    else
        type = DLMS_OBJECT_TYPE_NONE;
    return type;
}

void DLMSConverter::UpdateObisCodes()
{
    if (m_Codes.empty())
    {
        std::stringstream sb;
        sb << OBIS_CODES_ABSTRACT_1;
        sb << OBIS_CODES_ABSTRACT_2;
#ifndef DLMS_IGNORE_OBIS_ELECTRICITY
        sb << OBIS_CODES_ELECTRICITY_1;
        sb << OBIS_CODES_ELECTRICITY_2;
        sb << OBIS_CODES_ELECTRICITY_3;
        sb << OBIS_CODES_ELECTRICITY_3;
        sb << OBIS_CODES_ELECTRICITY_4;
        sb << OBIS_CODES_ELECTRICITY_5;
#endif //DLMS_IGNORE_OBIS_ELECTRICITY
#ifndef DLMS_IGNORE_OBIS_GAS
        sb << OBIS_CODES_GAS_1;
        sb << OBIS_CODES_GAS_2;
        sb << OBIS_CODES_GAS_3;
        sb << OBIS_CODES_GAS_4;
        sb << OBIS_CODES_GAS_5;
        sb << OBIS_CODES_GAS_6;
#endif //DLMS_IGNORE_OBIS_GAS
        std::string str = sb.str();
        std::vector< std::string > rows = Helpers::Split(str, "\r\n", true);
        int row = 0;
        std::string last;
        for (std::vector< std::string >::iterator it = rows.begin(); it != rows.end(); ++it)
        {
            std::vector< std::string > items = Helpers::Split(*it, ";\r\n", false);
            if (items.size() != 8)
            {
                items = Helpers::Split(*it, ";\r\n", false);
            }
            assert(items.size() == 8);
            std::vector< std::string > obis = Helpers::Split(items[0], ".\r\n", false);
            if (obis.size() != 6)
            {
                obis = Helpers::Split(items[0], ".\r\n", false);
            }
            std::string str = items[3] + "; " + items[4] + "; " + items[5] + "; " + items[6] + "; " + items[7];
            m_Codes.push_back(new StandardObisCode(obis, str, items[1], items[2]));
            ++row;
            last = *it;
        }
    }
}

void DLMSConverter::GetDescription(std::string& logicalName, DLMS_OBJECT_TYPE type, std::vector< std::string >& descriptions)
{
    UpdateObisCodes();
    std::vector<StandardObisCode*> list;
    m_Codes.Find(logicalName, type, list);
    for (std::vector<StandardObisCode*>::iterator it = list.begin(); it != list.end(); ++it)
    {
        descriptions.push_back((*it)->GetDescription());
        delete* it;
    }
    list.clear();
}

void DLMSConverter::UpdateOBISCodeInformation(DLMSObjectCollection& objects)
{
    UpdateObisCodes();
    for (std::vector<DLMSObject*>::iterator it = objects.begin(); it != objects.end(); ++it)
    {
        std::string ln;
        (*it)->GetLogicalName(ln);
        std::vector<StandardObisCode*> list;
        m_Codes.Find(ln, (*it)->GetObjectType(), list);
        StandardObisCode* code = list.at(0);
        (*it)->SetDescription(code->GetDescription());
        //If std::string is used
        if (code->GetDataType().find("10") != std::string::npos)
        {
            code->SetUIDataType("10");
        }
        //If date time is used.
        else if (code->GetDataType().find("25") != std::string::npos ||
            code->GetDataType().find("26") != std::string::npos)
        {
            code->SetUIDataType("25");
        }
        else if (code->GetDataType().find("9"))
        {
            //Time stamps of the billing periods objects (first scheme if there are two)
            if ((StandardObisCodeCollection::EqualsMask2("0.0-64.96.7.10-14.255", ln) ||
                //Time stamps of the billing periods objects (second scheme)
                StandardObisCodeCollection::EqualsMask2("0.0-64.0.1.5.0-99,255", ln) ||
                //Time of power failure
                StandardObisCodeCollection::EqualsMask2("0.0-64.0.1.2.0-99,255", ln) ||
                //Time stamps of the billing periods objects (first scheme if there are two)
                StandardObisCodeCollection::EqualsMask2("1.0-64.0.1.2.0-99,255", ln) ||
                //Time stamps of the billing periods objects (second scheme)
                StandardObisCodeCollection::EqualsMask2("1.0-64.0.1.5.0-99,255", ln) ||
                //Time expired since last end of billing period
                StandardObisCodeCollection::EqualsMask2("1.0-64.0.9.0.255", ln) ||
                //Time of last reset
                StandardObisCodeCollection::EqualsMask2("1.0-64.0.9.6.255", ln) ||
                //Date of last reset
                StandardObisCodeCollection::EqualsMask2("1.0-64.0.9.7.255", ln) ||
                //Time expired since last end of billing period (Second billing period scheme)
                StandardObisCodeCollection::EqualsMask2("1.0-64.0.9.13.255", ln) ||
                //Time of last reset (Second billing period scheme)
                StandardObisCodeCollection::EqualsMask2("1.0-64.0.9.14.255", ln) ||
                //Date of last reset (Second billing period scheme)
                StandardObisCodeCollection::EqualsMask2("1.0-64.0.9.15.255", ln)))
            {
                code->SetUIDataType("25");
            }
            //Local time
            else if (StandardObisCodeCollection::EqualsMask2("1.0-64.0.9.1.255", ln))
            {
                code->SetUIDataType("27");
            }
            //Local date
            else if (StandardObisCodeCollection::EqualsMask2("1.0-64.0.9.2.255", ln))
            {
                code->SetUIDataType("26");
            }
            //Active firmware identifier
            else if (StandardObisCodeCollection::EqualsMask2("1.0.0.2.0.255", ln))
            {
                code->SetUIDataType("10");
            }
        }
        //Unix time
        else if ((*it)->GetObjectType() == DLMS_OBJECT_TYPE_DATA && StandardObisCodeCollection::EqualsMask2("0.0.1.1.0.255", ln))
        {
            code->SetUIDataType("25");
        }
        if (code->GetDataType() != "*" &&
            code->GetDataType() != "" &&
            code->GetDataType().find(",") == std::string::npos)
        {
            int value;
#if _MSC_VER > 1000
            sscanf_s(code->GetDataType().c_str(), "%d", &value);
#else
            sscanf(code->GetDataType().c_str(), "%d", &value);
#endif
            DLMS_DATA_TYPE type = (DLMS_DATA_TYPE)value;
            switch ((*it)->GetObjectType())
            {
            case DLMS_OBJECT_TYPE_DATA:
            case DLMS_OBJECT_TYPE_REGISTER:
            case DLMS_OBJECT_TYPE_REGISTER_ACTIVATION:
            case DLMS_OBJECT_TYPE_EXTENDED_REGISTER:
                (*it)->SetDataType(2, type);
                break;
            default:
                break;
            }
        }
        if (code->GetUIDataType() != "")
        {
            int value;
#if _MSC_VER > 1000
            sscanf_s(code->GetUIDataType().c_str(), "%d", &value);
#else
            sscanf(code->GetUIDataType().c_str(), "%d", &value);
#endif
            DLMS_DATA_TYPE type = (DLMS_DATA_TYPE)value;
            switch ((*it)->GetObjectType())
            {
            case DLMS_OBJECT_TYPE_DATA:
            case DLMS_OBJECT_TYPE_REGISTER:
            case DLMS_OBJECT_TYPE_REGISTER_ACTIVATION:
            case DLMS_OBJECT_TYPE_EXTENDED_REGISTER:
                (*it)->SetUIDataType(2, type);
                break;
            default:
                break;
            }
        }
        for (std::vector<StandardObisCode*>::iterator it = list.begin(); it != list.end(); ++it)
        {
            delete* it;
        }
        list.clear();
    }
}

const char* DLMSConverter::ToString(DLMS_X509_NAME value)
{
    switch (value)
    {
    case DLMS_X509_NAME_C:
        return "2.5.4.6";
    case DLMS_X509_NAME_O:
        return "2.5.4.10";
    case DLMS_X509_NAME_OU:
        return "2.5.4.11";
    case DLMS_X509_NAME_T:
        return "2.5.4.12";
    case DLMS_X509_NAME_CN:
        return "2.5.4.3";
    case DLMS_X509_NAME_STREET:
        return "2.5.4.9";
    case DLMS_X509_NAME_SERIAL_NUMBER:
        return "2.5.4.5";
    case DLMS_X509_NAME_L:
        return "2.5.4.7";
    case DLMS_X509_NAME_ST:
        return "2.5.4.8";
    case DLMS_X509_NAME_SUR_NAME:
        return "2.5.4.4";
    case DLMS_X509_NAME_GIVEN_NAME:
        return "2.5.4.42";
    case DLMS_X509_NAME_INITIALS:
        return "2.5.4.43";
    case DLMS_X509_NAME_GENERATION:
        return "2.5.4.44";
    case DLMS_X509_NAME_UNIQUE_IDENTIFIER:
        return "2.5.4.45";
    case DLMS_X509_NAME_BUSINESS_CATEGORY:
        return "2.5.4.15";
    case DLMS_X509_NAME_POSTAL_CODE:
        return "2.5.4.17";
    case DLMS_X509_NAME_DN_QUALIFIER:
        return "2.5.4.46";
    case DLMS_X509_NAME_PSEUDONYM:
        return "2.5.4.65";
    case DLMS_X509_NAME_DATE_OF_BIRTH:
        return "1.3.6.1.5.5.7.9.1";
    case DLMS_X509_NAME_PLACE_OF_BIRTH:
        return "1.3.6.1.5.5.7.9.2";
    case DLMS_X509_NAME_GENDER:
        return "1.3.6.1.5.5.7.9.3";
    case DLMS_X509_NAME_COUNTRY_OF_CITIZENSHIP:
        return "1.3.6.1.5.5.7.9.4";
    case DLMS_X509_NAME_COUNTRY_OF_RESIDENCE:
        return "1.3.6.1.5.5.7.9.5";
    case DLMS_X509_NAME_NAME_AT_BIRTH:
        return "1.3.36.8.3.14";
    case DLMS_X509_NAME_POSTAL_ADDRESS:
        return "2.5.4.16";
    case DLMS_X509_NAME_DMD_NAME:
        return "2.5.4.54";
    case DLMS_X509_NAME_TELEPHONE_NUMBER:
        return "2.5.4.20";
    case DLMS_X509_NAME_NAME:
        return "2.5.4.41";
    case DLMS_X509_NAME_E:
        return "1.2.840.113549.1.9.1";
    case DLMS_X509_NAME_DC:
        return "0.9.2342.19200300.100.1.25";
    case DLMS_X509_NAME_UID:
        return "0.9.2342.19200300.100.1.1";
    default:
        return NULL;
    }
}

const char* DLMSConverter::ToString(DLMS_X509_CERTIFICATE_TYPE value)
{
    const char* ret;
    switch (value)
    {
    case DLMS_X509_CERTIFICATE_TYPE_OLDAUTHORITY_KEY_IDENTIFIER:
        ret = "2.5.29.1";
        break;
    case DLMS_X509_CERTIFICATE_TYPE_OLD_PRIMARY_KEY_ATTRIBUTES:
        ret = "2.5.29.2";
        break;
    case DLMS_X509_CERTIFICATE_TYPE_CERTIFICATE_POLICIES:
        ret = "2.5.29.3";
        break;
    case DLMS_X509_CERTIFICATE_TYPE_ORIMARY_KEY_USAGE_RESTRICTION:
        ret = "2.5.29.4";
        break;
    case DLMS_X509_CERTIFICATE_TYPE_SUBJECT_DIRECTORY_ATTRIBUTES:
        ret = "2.5.29.9";
        break;
    case DLMS_X509_CERTIFICATE_TYPE_SUBJECT_KEY_IDENTIFIER:
        ret = "2.5.29.14";
        break;
    case DLMS_X509_CERTIFICATE_TYPE_KEY_USAGE:
        ret = "2.5.29.15";
        break;
    case DLMS_X509_CERTIFICATE_TYPE_PRIVATE_KEY_USAGE_PERIOD:
        ret = "2.5.29.16";
        break;
    case DLMS_X509_CERTIFICATE_TYPE_SUBJECT_ALTERNATIVE_NAME:
        ret = "2.5.29.17";
        break;
    case DLMS_X509_CERTIFICATE_TYPE_ISSUER_ALTERNATIVE_NAME:
        ret = "2.5.29.18";
        break;
    case DLMS_X509_CERTIFICATE_TYPE_BASIC_CONSTRAINTS:
        ret = "2.5.29.19";
        break;
    case DLMS_X509_CERTIFICATE_TYPE_CRL_NUMBER:
        ret = "2.5.29.20";
        break;
    case DLMS_X509_CERTIFICATE_TYPE_REASON_CODE:
        ret = "2.5.29.21";
        break;
    case DLMS_X509_CERTIFICATE_TYPE_HOLD_INSTRUCTIONCODE:
        ret = "2.5.29.23";
        break;
    case DLMS_X509_CERTIFICATE_TYPE_INVALIDITY_DATE:
        ret = "2.5.29.24";
        break;
    case DLMS_X509_CERTIFICATE_TYPE_DELTA_CRLINDICATOR:
        ret = "2.5.29.27";
        break;
    case DLMS_X509_CERTIFICATE_TYPE_ISSUING_DISTRIBUTION_POINT:
        ret = "2.5.29.28";
        break;
    case DLMS_X509_CERTIFICATE_TYPE_CERTIFICATE_ISSUER:
        ret = "2.5.29.29";
        break;
    case DLMS_X509_CERTIFICATE_TYPE_NAME_CONSTRAINTS:
        ret = "2.5.29.30";
        break;
    case DLMS_X509_CERTIFICATE_TYPE_CRL_DISTRIBUTION_POINTS:
        ret = "2.5.29.31";
        break;
    case DLMS_X509_CERTIFICATE_TYPE_CERTIFICATE_POLICIES2:
        ret = "2.5.29.32";
        break;
    case DLMS_X509_CERTIFICATE_TYPE_POLICY_MAPPINGS:
        ret = "2.5.29.33";
        break;
    case DLMS_X509_CERTIFICATE_TYPE_AUTHORITY_KEY_IDENTIFIER:
        ret = "2.5.29.35";
        break;
    case DLMS_X509_CERTIFICATE_TYPE_POLICY_CONSTRAINTS:
        ret = "2.5.29.36";
        break;
    case DLMS_X509_CERTIFICATE_TYPE_EXTENDED_KEY_USAGE:
        ret = "2.5.29.37";
        break;
    case DLMS_X509_CERTIFICATE_TYPE_FRESHEST_CRL:
        ret = "2.5.29.46";
        break;
    default:
        ret = 0;
    }
    return ret;
}

DLMS_X509_CERTIFICATE_TYPE DLMSConverter::ValueOfSourceDiagnosticX509CertificateType(const char* value)
{
    DLMS_X509_CERTIFICATE_TYPE ret;
    if (strcmp(value, "2.5.29.1") == 0)
    {
        ret = DLMS_X509_CERTIFICATE_TYPE_OLDAUTHORITY_KEY_IDENTIFIER;
    }
    else if (strcmp(value, "2.5.29.2") == 0)
    {
        ret = DLMS_X509_CERTIFICATE_TYPE_OLD_PRIMARY_KEY_ATTRIBUTES;
    }
    else if (strcmp(value, "2.5.29.3") == 0)
    {
        ret = DLMS_X509_CERTIFICATE_TYPE_CERTIFICATE_POLICIES;
    }
    else if (strcmp(value, "2.5.29.4") == 0)
    {
        ret = DLMS_X509_CERTIFICATE_TYPE_ORIMARY_KEY_USAGE_RESTRICTION;
    }
    else if (strcmp(value, "2.5.29.9") == 0)
    {
        ret = DLMS_X509_CERTIFICATE_TYPE_SUBJECT_DIRECTORY_ATTRIBUTES;
    }
    else if (strcmp(value, "2.5.29.14") == 0)
    {
        ret = DLMS_X509_CERTIFICATE_TYPE_SUBJECT_KEY_IDENTIFIER;
    }
    else if (strcmp(value, "2.5.29.15") == 0)
    {
        ret = DLMS_X509_CERTIFICATE_TYPE_KEY_USAGE;
    }
    else if (strcmp(value, "2.5.29.16") == 0)
    {
        ret = DLMS_X509_CERTIFICATE_TYPE_PRIVATE_KEY_USAGE_PERIOD;
    }
    else if (strcmp(value, "2.5.29.17") == 0)
    {
        ret = DLMS_X509_CERTIFICATE_TYPE_SUBJECT_ALTERNATIVE_NAME;
    }
    else if (strcmp(value, "2.5.29.18") == 0)
    {
        ret = DLMS_X509_CERTIFICATE_TYPE_ISSUER_ALTERNATIVE_NAME;
    }
    else if (strcmp(value, "2.5.29.19") == 0)
    {
        ret = DLMS_X509_CERTIFICATE_TYPE_BASIC_CONSTRAINTS;
    }
    else if (strcmp(value, "2.5.29.20") == 0)
    {
        ret = DLMS_X509_CERTIFICATE_TYPE_CRL_NUMBER;
    }
    else if (strcmp(value, "2.5.29.21") == 0)
    {
        ret = DLMS_X509_CERTIFICATE_TYPE_REASON_CODE;
    }
    else if (strcmp(value, "2.5.29.23") == 0)
    {
        ret = DLMS_X509_CERTIFICATE_TYPE_HOLD_INSTRUCTIONCODE;
    }
    else if (strcmp(value, "2.5.29.24") == 0)
    {
        ret = DLMS_X509_CERTIFICATE_TYPE_INVALIDITY_DATE;
    }
    else if (strcmp(value, "2.5.29.27") == 0)
    {
        ret = DLMS_X509_CERTIFICATE_TYPE_DELTA_CRLINDICATOR;
    }
    else if (strcmp(value, "2.5.29.28") == 0)
    {
        ret = DLMS_X509_CERTIFICATE_TYPE_ISSUING_DISTRIBUTION_POINT;
    }
    else if (strcmp(value, "2.5.29.29") == 0)
    {
        ret = DLMS_X509_CERTIFICATE_TYPE_CERTIFICATE_ISSUER;
    }
    else if (strcmp(value, "2.5.29.30") == 0)
    {
        ret = DLMS_X509_CERTIFICATE_TYPE_NAME_CONSTRAINTS;
    }
    else if (strcmp(value, "2.5.29.31") == 0)
    {
        ret = DLMS_X509_CERTIFICATE_TYPE_CRL_DISTRIBUTION_POINTS;
    }
    else if (strcmp(value, "2.5.29.32") == 0)
    {
        ret = DLMS_X509_CERTIFICATE_TYPE_CERTIFICATE_POLICIES2;
    }
    else if (strcmp(value, "2.5.29.33") == 0)
    {
        ret = DLMS_X509_CERTIFICATE_TYPE_POLICY_MAPPINGS;
    }
    else if (strcmp(value, "2.5.29.35") == 0)
    {
        ret = DLMS_X509_CERTIFICATE_TYPE_AUTHORITY_KEY_IDENTIFIER;
    }
    else if (strcmp(value, "2.5.29.36") == 0)
    {
        ret = DLMS_X509_CERTIFICATE_TYPE_POLICY_CONSTRAINTS;
    }
    else if (strcmp(value, "2.5.29.37") == 0)
    {
        ret = DLMS_X509_CERTIFICATE_TYPE_EXTENDED_KEY_USAGE;
    }
    else if (strcmp(value, "2.5.29.46") == 0)
    {
        ret = DLMS_X509_CERTIFICATE_TYPE_FRESHEST_CRL;
    }
    else
    {
        ret = DLMS_X509_CERTIFICATE_TYPE_NONE;
    }
    return ret;
}

DLMS_X509_NAME DLMSConverter::ValueOfx509Name(const char* value)
{
    if (strcmp(value, "2.5.4.6") == 0)
    {
        return DLMS_X509_NAME_C;
    }
    else if (strcmp(value, "2.5.4.10") == 0)
    {
        return DLMS_X509_NAME_O;
    }
    else if (strcmp(value, "2.5.4.11") == 0)
    {
        return DLMS_X509_NAME_OU;
    }
    else if (strcmp(value, "2.5.4.12") == 0)
    {
        return DLMS_X509_NAME_T;
    }
    else if (strcmp(value, "2.5.4.3") == 0)
    {
        return DLMS_X509_NAME_CN;
    }
    else if (strcmp(value, "2.5.4.9") == 0)
    {
        return DLMS_X509_NAME_STREET;
    }
    else if (strcmp(value, "2.5.4.5") == 0)
    {
        return DLMS_X509_NAME_SERIAL_NUMBER;
    }
    else if (strcmp(value, "2.5.4.7") == 0)
    {
        return DLMS_X509_NAME_L;
    }
    else if (strcmp(value, "2.5.4.8") == 0)
    {
        return DLMS_X509_NAME_ST;
    }
    else if (strcmp(value, "2.5.4.4") == 0)
    {
        return DLMS_X509_NAME_SUR_NAME;
    }
    else if (strcmp(value, "2.5.4.42") == 0)
    {
        return DLMS_X509_NAME_GIVEN_NAME;
    }
    else if (strcmp(value, "2.5.4.43") == 0)
    {
        return DLMS_X509_NAME_INITIALS;
    }
    else if (strcmp(value, "2.5.4.44") == 0)
    {
        return DLMS_X509_NAME_GENERATION;
    }
    else if (strcmp(value, "2.5.4.45") == 0)
    {
        return DLMS_X509_NAME_UNIQUE_IDENTIFIER;
    }
    else if (strcmp(value, "2.5.4.15") == 0)
    {
        return DLMS_X509_NAME_BUSINESS_CATEGORY;
    }
    else if (strcmp(value, "2.5.4.17") == 0)
    {
        return DLMS_X509_NAME_POSTAL_CODE;
    }
    else if (strcmp(value, "2.5.4.46") == 0)
    {
        return DLMS_X509_NAME_DN_QUALIFIER;
    }
    else if (strcmp(value, "2.5.4.65") == 0)
    {
        return DLMS_X509_NAME_PSEUDONYM;
    }
    else if (strcmp(value, "1.3.6.1.5.5.7.9.1") == 0)
    {
        return DLMS_X509_NAME_DATE_OF_BIRTH;
    }
    else if (strcmp(value, "1.3.6.1.5.5.7.9.2") == 0)
    {
        return DLMS_X509_NAME_PLACE_OF_BIRTH;
    }
    else if (strcmp(value, "1.3.6.1.5.5.7.9.3") == 0)
    {
        return DLMS_X509_NAME_GENDER;
    }
    else if (strcmp(value, "1.3.6.1.5.5.7.9.4") == 0)
    {
        return DLMS_X509_NAME_COUNTRY_OF_CITIZENSHIP;
    }
    else if (strcmp(value, "1.3.6.1.5.5.7.9.5") == 0)
    {
        return DLMS_X509_NAME_COUNTRY_OF_RESIDENCE;
    }
    else if (strcmp(value, "1.3.36.8.3.14") == 0)
    {
        return DLMS_X509_NAME_NAME_AT_BIRTH;
    }
    else if (strcmp(value, "2.5.4.16") == 0)
    {
        return DLMS_X509_NAME_POSTAL_ADDRESS;
    }
    else if (strcmp(value, "2.5.4.54") == 0)
    {
        return DLMS_X509_NAME_DMD_NAME;
    }
    else if (strcmp(value, "2.5.4.20") == 0)
    {
        return DLMS_X509_NAME_TELEPHONE_NUMBER;
    }
    else if (strcmp(value, "2.5.4.41") == 0)
    {
        return DLMS_X509_NAME_NAME;
    }
    else if (strcmp(value, "1.2.840.113549.1.9.1") == 0)
    {
        return DLMS_X509_NAME_E;
    }
    else if (strcmp(value, "0.9.2342.19200300.100.1.25") == 0)
    {
        return DLMS_X509_NAME_DC;
    }
    else if (strcmp(value, "0.9.2342.19200300.100.1.1") == 0)
    {
        return DLMS_X509_NAME_UID;
    }
    return DLMS_X509_NAME_NONE;
}

const char* DLMSConverter::ToString(DLMS_X9_OBJECT_IDENTIFIER value)
{
    switch (value)
    {
    case DLMS_X9_OBJECT_IDENTIFIER_ID_FIELD_TYPE:
        return "1.2.840.10045.1";
    case DLMS_X9_OBJECT_IDENTIFIER_PRIME_FIELD:
        return "1.2.840.10045.1";
    case DLMS_X9_OBJECT_IDENTIFIER_CHARACTERISTIC_TWO_FIELD:
        return "1.2.840.10045.1.2";
    case DLMS_X9_OBJECT_IDENTIFIER_GN_BASIS:
        return "1.2.840.10045.1.2.3.1";
    case DLMS_X9_OBJECT_IDENTIFIER_TP_BASIS:
        return "1.2.840.10045.1.2.3.2";
    case DLMS_X9_OBJECT_IDENTIFIER_PP_BASIS:
        return "1.2.840.10045.1.2.3.3";
    case DLMS_X9_OBJECT_IDENTIFIER_ECDSA_WITH_SHA1:
        return "1.2.840.10045.4.1";
    case DLMS_X9_OBJECT_IDENTIFIER_ID_EC_PUBLIC_KEY:
        return "1.2.840.10045.2.1";
    case DLMS_X9_OBJECT_IDENTIFIER_ECDSA_WITH_SHA2:
        return "1.2.840.10045.4.3";
    case DLMS_X9_OBJECT_IDENTIFIER_ECDSA_WITH_SHA224:
        return "1.2.840.10045.4.31";
    case DLMS_X9_OBJECT_IDENTIFIER_ECDSA_WITH_SHA256:
        return "1.2.840.10045.4.32";
    case DLMS_X9_OBJECT_IDENTIFIER_ECDSA_WITH_SHA_384:
        return "1.2.840.10045.4.33";
    case DLMS_X9_OBJECT_IDENTIFIER_ECDSA_WITH_SHA_512:
        return "1.2.840.10045.4.34";
    case DLMS_X9_OBJECT_IDENTIFIER_ELLIPTIC_CURVE:
        return "1.2.840.10045.3";
    case DLMS_X9_OBJECT_IDENTIFIER_C_TWO_CURVE:
        return "1.2.840.10045.3.0";
    case DLMS_X9_OBJECT_IDENTIFIER_C2_PNB_163_V1:
        return "1.2.840.10045.3.0.1";
    case DLMS_X9_OBJECT_IDENTIFIER_C2_PNB_163_V2:
        return "1.2.840.10045.3.0.2";
    case DLMS_X9_OBJECT_IDENTIFIER_C2_PNB_163_V3:
        return "1.2.840.10045.3.0.3";
    case DLMS_X9_OBJECT_IDENTIFIER_C2_PNB_176_W1:
        return "1.2.840.10045.3.0.4";
    case DLMS_X9_OBJECT_IDENTIFIER_C2_TNB_191_V1:
        return "1.2.840.10045.3.0.5";
    case DLMS_X9_OBJECT_IDENTIFIER_C2_TNB_191_V2:
        return "1.2.840.10045.3.0.6";
    case DLMS_X9_OBJECT_IDENTIFIER_C2_TNB_191_V3:
        return "1.2.840.10045.3.0.7";
    case DLMS_X9_OBJECT_IDENTIFIER_C2_ONB_191_V4:
        return "1.2.840.10045.3.0.8";
    case DLMS_X9_OBJECT_IDENTIFIER_C2_ONB_191_V5:
        return "1.2.840.10045.3.0.9";
    case DLMS_X9_OBJECT_IDENTIFIER_C2_PNB_208_W1:
        return "1.2.840.10045.3.0.10";
    case DLMS_X9_OBJECT_IDENTIFIER_C2_TNB_239_V1:
        return "1.2.840.10045.3.0.11";
    case DLMS_X9_OBJECT_IDENTIFIER_C2_TNB_239_V2:
        return "1.2.840.10045.3.0.12";
    case DLMS_X9_OBJECT_IDENTIFIER_C2_TNB_239_V3:
        return "1.2.840.10045.3.0.13";
    case DLMS_X9_OBJECT_IDENTIFIER_C2_ONB_239_V4:
        return "1.2.840.10045.3.0.14";
    case DLMS_X9_OBJECT_IDENTIFIER_C2_ONB_239_V5:
        return "1.2.840.10045.3.0.15";
    case DLMS_X9_OBJECT_IDENTIFIER_C2_PNB_272_W1:
        return "1.2.840.10045.3.0.16";
    case DLMS_X9_OBJECT_IDENTIFIER_C2_PNB_304_W1:
        return "1.2.840.10045.3.0.17";
    case DLMS_X9_OBJECT_IDENTIFIER_C2_TNB_359_V1:
        return "1.2.840.10045.3.0.18";
    case DLMS_X9_OBJECT_IDENTIFIER_C2_PNB_368_W1:
        return "1.2.840.10045.3.0.19";
    case DLMS_X9_OBJECT_IDENTIFIER_C2_TNB_431_R1:
        return "1.2.840.10045.3.0.20";
    case DLMS_X9_OBJECT_IDENTIFIER_PRIME_CURVE:
        return "1.2.840.10045.3.1";
    case DLMS_X9_OBJECT_IDENTIFIER_PRIME_192_V1:
        return "1.2.840.10045.3.1.1";
    case DLMS_X9_OBJECT_IDENTIFIER_PRIME_192_V2:
        return "1.2.840.10045.3.1.2";
    case DLMS_X9_OBJECT_IDENTIFIER_PRIME_192_V3:
        return "1.2.840.10045.3.1.3";
    case DLMS_X9_OBJECT_IDENTIFIER_PRIME_239_V1:
        return "1.2.840.10045.3.1.4";
    case DLMS_X9_OBJECT_IDENTIFIER_PRIME_239_V2:
        return "1.2.840.10045.3.1.5";
    case DLMS_X9_OBJECT_IDENTIFIER_PRIME_239_V3:
        return "1.2.840.10045.3.1.6";
    case DLMS_X9_OBJECT_IDENTIFIER_PRIME_256_V1:
        return "1.2.840.10045.3.1.7";
    case DLMS_X9_OBJECT_IDENTIFIER_ID_DSA:
        return "1.2.840.10040.4.1";
    case DLMS_X9_OBJECT_IDENTIFIER_ID_DSA_WITH_SHA1:
        return "1.2.840.10040.4.3";
    case DLMS_X9_OBJECT_IDENTIFIER_X9_X63_SCHEME:
        return "1.3.133.16.840.63.0";
    case DLMS_X9_OBJECT_IDENTIFIER_DH_SINGLE_PASS_STD_DH_SHA1_KDF_SCHEME:
        return "1.3.133.16.840.63.0.2";
    case DLMS_X9_OBJECT_IDENTIFIER_DH_SINGLE_PASS_COFACTOR_DH_SHA1_KDF_SCHEME:
        return "1.3.133.16.840.63.0.3";
    case DLMS_X9_OBJECT_IDENTIFIER_MQV_SINGLE_PASS_SHA1_KDF_SCHEME:
        return "1.3.133.16.840.63.0.16";
    case DLMS_X9_OBJECT_IDENTIFIER_ANSI_X9_42:
        return "1.2.840.10046";
    case DLMS_X9_OBJECT_IDENTIFIER_DH_PUBLIC_NUMBER:
        return "1.2.840.10046.2.1";
    case DLMS_X9_OBJECT_IDENTIFIER_X9_X42_SCHEMES:
        return "1.2.840.10046.2.3";
    case DLMS_X9_OBJECT_IDENTIFIER_DH_STATIC:
        return "1.2.840.10046.2.3.1";
    case DLMS_X9_OBJECT_IDENTIFIER_DH_EPHEM:
        return "1.2.840.10046.2.3.2";
    case DLMS_X9_OBJECT_IDENTIFIER_DH_ONE_FLOW:
        return "1.2.840.10046.2.3.3";
    case DLMS_X9_OBJECT_IDENTIFIER_DH_HYBRID1:
        return "1.2.840.10046.2.3.4";
    case DLMS_X9_OBJECT_IDENTIFIER_DH_HYBRID2:
        return "1.2.840.10046.2.3.5";
    case DLMS_X9_OBJECT_IDENTIFIER_DH_HYBRID_ONE_FLOW:
        return "1.2.840.10046.2.3.6";
    case DLMS_X9_OBJECT_IDENTIFIER_MQ_V2:
        return "1.2.840.10046.2.3.7";
    case DLMS_X9_OBJECT_IDENTIFIER_MQ_V1:
        return "1.2.840.10046.2.3.8";
    case DLMS_X9_OBJECT_IDENTIFIER_SECP_384_R1:
        return "1.3.132.0.34";
    default:
        return NULL;
    }
}

DLMS_X9_OBJECT_IDENTIFIER DLMSConverter::ValueOfX9Identifier(const char* value)
{
    if (strcmp(value, "1.2.840.10045.1") == 0)
        return DLMS_X9_OBJECT_IDENTIFIER_ID_FIELD_TYPE;
    else if (strcmp(value, "1.2.840.10045.1") == 0)
        return DLMS_X9_OBJECT_IDENTIFIER_PRIME_FIELD;
    else if (strcmp(value, "1.2.840.10045.1.2") == 0)
        return DLMS_X9_OBJECT_IDENTIFIER_CHARACTERISTIC_TWO_FIELD;
    else if (strcmp(value, "1.2.840.10045.1.2.3.1") == 0)
        return DLMS_X9_OBJECT_IDENTIFIER_GN_BASIS;
    else if (strcmp(value, "1.2.840.10045.1.2.3.2") == 0)
        return DLMS_X9_OBJECT_IDENTIFIER_TP_BASIS;
    else if (strcmp(value, "1.2.840.10045.1.2.3.3") == 0)
        return DLMS_X9_OBJECT_IDENTIFIER_PP_BASIS;
    else if (strcmp(value, "1.2.840.10045.4.1") == 0)
        return DLMS_X9_OBJECT_IDENTIFIER_ECDSA_WITH_SHA1;
    else if (strcmp(value, "1.2.840.10045.2.1") == 0)
        return DLMS_X9_OBJECT_IDENTIFIER_ID_EC_PUBLIC_KEY;
    else if (strcmp(value, "1.2.840.10045.4.3") == 0)
        return DLMS_X9_OBJECT_IDENTIFIER_ECDSA_WITH_SHA2;
    else if (strcmp(value, "1.2.840.10045.4.31") == 0)
        return DLMS_X9_OBJECT_IDENTIFIER_ECDSA_WITH_SHA224;
    else if (strcmp(value, "1.2.840.10045.4.32") == 0)
        return DLMS_X9_OBJECT_IDENTIFIER_ECDSA_WITH_SHA256;
    else if (strcmp(value, "1.2.840.10045.4.33") == 0)
        return DLMS_X9_OBJECT_IDENTIFIER_ECDSA_WITH_SHA_384;
    else if (strcmp(value, "1.2.840.10045.4.34") == 0)
        return DLMS_X9_OBJECT_IDENTIFIER_ECDSA_WITH_SHA_512;
    else if (strcmp(value, "1.2.840.10045.3") == 0)
        return DLMS_X9_OBJECT_IDENTIFIER_ELLIPTIC_CURVE;
    else if (strcmp(value, "1.2.840.10045.3.0") == 0)
        return DLMS_X9_OBJECT_IDENTIFIER_C_TWO_CURVE;
    else if (strcmp(value, "1.2.840.10045.3.0.1") == 0)
        return DLMS_X9_OBJECT_IDENTIFIER_C2_PNB_163_V1;
    else if (strcmp(value, "1.2.840.10045.3.0.2") == 0)
        return DLMS_X9_OBJECT_IDENTIFIER_C2_PNB_163_V2;
    else if (strcmp(value, "1.2.840.10045.3.0.3") == 0)
        return DLMS_X9_OBJECT_IDENTIFIER_C2_PNB_163_V3;
    else if (strcmp(value, "1.2.840.10045.3.0.4") == 0)
        return DLMS_X9_OBJECT_IDENTIFIER_C2_PNB_176_W1;
    else if (strcmp(value, "1.2.840.10045.3.0.5") == 0)
        return DLMS_X9_OBJECT_IDENTIFIER_C2_TNB_191_V1;
    else if (strcmp(value, "1.2.840.10045.3.0.6") == 0)
        return DLMS_X9_OBJECT_IDENTIFIER_C2_TNB_191_V2;
    else if (strcmp(value, "1.2.840.10045.3.0.7") == 0)
        return DLMS_X9_OBJECT_IDENTIFIER_C2_TNB_191_V3;
    else if (strcmp(value, "1.2.840.10045.3.0.8") == 0)
        return DLMS_X9_OBJECT_IDENTIFIER_C2_ONB_191_V4;
    else if (strcmp(value, "1.2.840.10045.3.0.9") == 0)
        return DLMS_X9_OBJECT_IDENTIFIER_C2_ONB_191_V5;
    else if (strcmp(value, "1.2.840.10045.3.0.10") == 0)
        return DLMS_X9_OBJECT_IDENTIFIER_C2_PNB_208_W1;
    else if (strcmp(value, "1.2.840.10045.3.0.11") == 0)
        return DLMS_X9_OBJECT_IDENTIFIER_C2_TNB_239_V1;
    else if (strcmp(value, "1.2.840.10045.3.0.12") == 0)
        return DLMS_X9_OBJECT_IDENTIFIER_C2_TNB_239_V2;
    else if (strcmp(value, "1.2.840.10045.3.0.13") == 0)
        return DLMS_X9_OBJECT_IDENTIFIER_C2_TNB_239_V3;
    else if (strcmp(value, "1.2.840.10045.3.0.14") == 0)
        return DLMS_X9_OBJECT_IDENTIFIER_C2_ONB_239_V4;
    else if (strcmp(value, "1.2.840.10045.3.0.15") == 0)
        return DLMS_X9_OBJECT_IDENTIFIER_C2_ONB_239_V5;
    else if (strcmp(value, "1.2.840.10045.3.0.16") == 0)
        return DLMS_X9_OBJECT_IDENTIFIER_C2_PNB_272_W1;
    else if (strcmp(value, "1.2.840.10045.3.0.17") == 0)
        return DLMS_X9_OBJECT_IDENTIFIER_C2_PNB_304_W1;
    else if (strcmp(value, "1.2.840.10045.3.0.18") == 0)
        return DLMS_X9_OBJECT_IDENTIFIER_C2_TNB_359_V1;
    else if (strcmp(value, "1.2.840.10045.3.0.19") == 0)
        return DLMS_X9_OBJECT_IDENTIFIER_C2_PNB_368_W1;
    else if (strcmp(value, "1.2.840.10045.3.0.20") == 0)
        return DLMS_X9_OBJECT_IDENTIFIER_C2_TNB_431_R1;
    else if (strcmp(value, "1.2.840.10045.3.1") == 0)
        return DLMS_X9_OBJECT_IDENTIFIER_PRIME_CURVE;
    else if (strcmp(value, "1.2.840.10045.3.1.1") == 0)
        return DLMS_X9_OBJECT_IDENTIFIER_PRIME_192_V1;
    else if (strcmp(value, "1.2.840.10045.3.1.2") == 0)
        return DLMS_X9_OBJECT_IDENTIFIER_PRIME_192_V2;
    else if (strcmp(value, "1.2.840.10045.3.1.3") == 0)
        return DLMS_X9_OBJECT_IDENTIFIER_PRIME_192_V3;
    else if (strcmp(value, "1.2.840.10045.3.1.4") == 0)
        return DLMS_X9_OBJECT_IDENTIFIER_PRIME_239_V1;
    else if (strcmp(value, "1.2.840.10045.3.1.5") == 0)
        return DLMS_X9_OBJECT_IDENTIFIER_PRIME_239_V2;
    else if (strcmp(value, "1.2.840.10045.3.1.6") == 0)
        return DLMS_X9_OBJECT_IDENTIFIER_PRIME_239_V3;
    else if (strcmp(value, "1.2.840.10045.3.1.7") == 0)
        return DLMS_X9_OBJECT_IDENTIFIER_PRIME_256_V1;
    else if (strcmp(value, "1.2.840.10040.4.1") == 0)
        return DLMS_X9_OBJECT_IDENTIFIER_ID_DSA;
    else if (strcmp(value, "1.2.840.10040.4.3") == 0)
        return DLMS_X9_OBJECT_IDENTIFIER_ID_DSA_WITH_SHA1;
    else if (strcmp(value, "1.3.133.16.840.63.0") == 0)
        return DLMS_X9_OBJECT_IDENTIFIER_X9_X63_SCHEME;
    else if (strcmp(value, "1.3.133.16.840.63.0.2") == 0)
        return DLMS_X9_OBJECT_IDENTIFIER_DH_SINGLE_PASS_STD_DH_SHA1_KDF_SCHEME;
    else if (strcmp(value, "1.3.133.16.840.63.0.3") == 0)
        return DLMS_X9_OBJECT_IDENTIFIER_DH_SINGLE_PASS_COFACTOR_DH_SHA1_KDF_SCHEME;
    else if (strcmp(value, "1.3.133.16.840.63.0.16") == 0)
        return DLMS_X9_OBJECT_IDENTIFIER_MQV_SINGLE_PASS_SHA1_KDF_SCHEME;
    else if (strcmp(value, "1.2.840.10046") == 0)
        return DLMS_X9_OBJECT_IDENTIFIER_ANSI_X9_42;
    else if (strcmp(value, "1.2.840.10046.2.1") == 0)
        return DLMS_X9_OBJECT_IDENTIFIER_DH_PUBLIC_NUMBER;
    else if (strcmp(value, "1.2.840.10046.2.3") == 0)
        return DLMS_X9_OBJECT_IDENTIFIER_X9_X42_SCHEMES;
    else if (strcmp(value, "1.2.840.10046.2.3.1") == 0)
        return DLMS_X9_OBJECT_IDENTIFIER_DH_STATIC;
    else if (strcmp(value, "1.2.840.10046.2.3.2") == 0)
        return DLMS_X9_OBJECT_IDENTIFIER_DH_EPHEM;
    else if (strcmp(value, "1.2.840.10046.2.3.3") == 0)
        return DLMS_X9_OBJECT_IDENTIFIER_DH_ONE_FLOW;
    else if (strcmp(value, "1.2.840.10046.2.3.4") == 0)
        return DLMS_X9_OBJECT_IDENTIFIER_DH_HYBRID1;
    else if (strcmp(value, "1.2.840.10046.2.3.5") == 0)
        return DLMS_X9_OBJECT_IDENTIFIER_DH_HYBRID2;
    else if (strcmp(value, "1.2.840.10046.2.3.6") == 0)
        return DLMS_X9_OBJECT_IDENTIFIER_DH_HYBRID_ONE_FLOW;
    else if (strcmp(value, "1.2.840.10046.2.3.7") == 0)
        return DLMS_X9_OBJECT_IDENTIFIER_MQ_V2;
    else if (strcmp(value, "1.2.840.10046.2.3.8") == 0)
        return DLMS_X9_OBJECT_IDENTIFIER_MQ_V1;
    else if (strcmp(value, "1.3.132.0.34") == 0)
        return DLMS_X9_OBJECT_IDENTIFIER_SECP_384_R1;
    return DLMS_X9_OBJECT_IDENTIFIER_NONE;
}

const char* DLMSConverter::ToString(DLMS_PKCS_OBJECT_IDENTIFIER value)
{
    switch (value)
    {
    case DLMS_PKCS_OBJECT_IDENTIFIER_RSA_ENCRYPTION:
        return "1.2.840.113549.1.1.1";
    case DLMS_PKCS_OBJECT_IDENTIFIER_MD2_WITH_RSA_ENCRYPTION:
        return "1.2.840.113549.1.1.2";
    case DLMS_PKCS_OBJECT_IDENTIFIER_MD4_WITH_RSA_ENCRYPTION:
        return "1.2.840.113549.1.1.3";
    case DLMS_PKCS_OBJECT_IDENTIFIER_MD5_WITH_RSA_ENCRYPTION:
        return "1.2.840.113549.1.1.4";
    case DLMS_PKCS_OBJECT_IDENTIFIER_SHA1_WITH_RSA_ENCRYPTION:
        return "1.2.840.113549.1.1.5";
    case DLMS_PKCS_OBJECT_IDENTIFIER_SRSA_OAEP_ENCRYPTIONSET:
        return "1.2.840.113549.1.1.6";
    case DLMS_PKCS_OBJECT_IDENTIFIER_ID_RSAES_OAEP:
        return "1.2.840.113549.1.1.7";
    case DLMS_PKCS_OBJECT_IDENTIFIER_ID_MGF1:
        return "1.2.840.113549.1.1.8";
    case DLMS_PKCS_OBJECT_IDENTIFIER_ID_PSPECIFIED:
        return "1.2.840.113549.1.1.9";
    case DLMS_PKCS_OBJECT_IDENTIFIER_ID_RSASSA_PSS:
        return "1.2.840.113549.1.1.10";
    case DLMS_PKCS_OBJECT_IDENTIFIER_SHA256_WITH_RSA_ENCRYPTION:
        return "1.2.840.113549.1.1.11";
    case DLMS_PKCS_OBJECT_IDENTIFIER_SHA384_WITH_RSA_ENCRYPTION:
        return "1.2.840.113549.1.1.12";
    case DLMS_PKCS_OBJECT_IDENTIFIER_SHA512_WITH_RSA_ENCRYPTION:
        return "1.2.840.113549.1.1.13";
    case DLMS_PKCS_OBJECT_IDENTIFIER_SHA224_WITH_RSA_ENCRYPTION:
        return "1.2.840.113549.1.1.14";
    case DLMS_PKCS_OBJECT_IDENTIFIER_DH_KEY_AGREEMENT:
        return "1.2.840.113549.1.3.1";
    case DLMS_PKCS_OBJECT_IDENTIFIER_PBE_WITH_MD2_AND_DES_CBC:
        return "1.2.840.113549.1.5.1";
    case DLMS_PKCS_OBJECT_IDENTIFIER_PBE_WITH_MD2_AND_RC2_CBC:
        return "1.2.840.113549.1.5.4";
    case DLMS_PKCS_OBJECT_IDENTIFIER_PBE_WITH_MD5_AND_DES_CBC:
        return "1.2.840.113549.1.5.3";
    case DLMS_PKCS_OBJECT_IDENTIFIER_PBE_WITH_MD5_AND_RC2_CBC:
        return "1.2.840.113549.1.5.6";
    case DLMS_PKCS_OBJECT_IDENTIFIER_PBE_WITH_SHA1_AND_DES_CBC:
        return "1.2.840.113549.1.5.10";
    case DLMS_PKCS_OBJECT_IDENTIFIER_PBE_WITH_SHA1_AND_RC2_CBC:
        return "1.2.840.113549.1.5.11";
    case DLMS_PKCS_OBJECT_IDENTIFIER_ID_PBE_S2:
        return "1.2.840.113549.1.5.13";
    case DLMS_PKCS_OBJECT_IDENTIFIER_ID_PBKD_F2:
        return "1.2.840.113549.1.5.12";
    case DLMS_PKCS_OBJECT_IDENTIFIER_DES_EDE3_CBC:
        return "1.2.840.113549.3.7";
    case DLMS_PKCS_OBJECT_IDENTIFIER_RC2_CBC:
        return "1.2.840.113549.3.2";
    case DLMS_PKCS_OBJECT_IDENTIFIER_MD2:
        return "1.2.840.113549.2.2";
    case DLMS_PKCS_OBJECT_IDENTIFIER_MD4:
        return "1.2.840.113549.2.4";
    case DLMS_PKCS_OBJECT_IDENTIFIER_MD5:
        return "1.2.840.113549.2.5";
    case DLMS_PKCS_OBJECT_IDENTIFIER_ID_HMAC_WITH_SHA1:
        return "1.2.840.113549.2.7";
    case DLMS_PKCS_OBJECT_IDENTIFIER_ID_HMAC_WITH_SHA224:
        return "1.2.840.113549.2.8";
    case DLMS_PKCS_OBJECT_IDENTIFIER_ID_HMAC_WITH_SHA256:
        return "1.2.840.113549.2.9";
    case DLMS_PKCS_OBJECT_IDENTIFIER_ID_HMAC_WITH_SHA384:
        return "1.2.840.113549.2.10";
    case DLMS_PKCS_OBJECT_IDENTIFIER_ID_HMAC_WITH_SHA512:
        return "1.2.840.113549.2.11";
    case DLMS_PKCS_OBJECT_IDENTIFIER_DATA:
        return "1.2.840.113549.1.7.1";
    case DLMS_PKCS_OBJECT_IDENTIFIER_SIGNED_DATA:
        return "1.2.840.113549.1.7.2";
    case DLMS_PKCS_OBJECT_IDENTIFIER_ENVELOPED_DATA:
        return "1.2.840.113549.1.7.3";
    case DLMS_PKCS_OBJECT_IDENTIFIER_SIGNED_AND_ENVELOPED_DATA:
        return "1.2.840.113549.1.7.4";
    case DLMS_PKCS_OBJECT_IDENTIFIER_DIGESTED_DATA:
        return "1.2.840.113549.1.7.5";
    case DLMS_PKCS_OBJECT_IDENTIFIER_ENCRYPTED_DATA:
        return "1.2.840.113549.1.7.6";
    case DLMS_PKCS_OBJECT_IDENTIFIER_PKCS9_AT_EMAIL_ADDRESS:
        return "1.2.840.113549.1.9.1";
    case DLMS_PKCS_OBJECT_IDENTIFIER_PKCS9_AT_UNSTRUCTURED_NAME:
        return "1.2.840.113549.1.9.2";
    case DLMS_PKCS_OBJECT_IDENTIFIER_PKCS9_AT_CONTENT_TYPE:
        return "1.2.840.113549.1.9.3";
    case DLMS_PKCS_OBJECT_IDENTIFIER_PKCS9_AT_MESSAGE_DIGEST:
        return "1.2.840.113549.1.9.4";
    case DLMS_PKCS_OBJECT_IDENTIFIER_PKCS9_AT_SIGNING_TIME:
        return "1.2.840.113549.1.9.5";
    case DLMS_PKCS_OBJECT_IDENTIFIER_PKCS9_AT_COUNTER_SIGNATURE:
        return "1.2.840.113549.1.9.6";
    case DLMS_PKCS_OBJECT_IDENTIFIER_PKCS9_AT_CHALLENGE_PASSWORD:
        return "1.2.840.113549.1.9.7";
    case DLMS_PKCS_OBJECT_IDENTIFIER_PKCS9_AT_UNSTRUCTURED_ADDRESS:
        return "1.2.840.113549.1.9.8";
    case DLMS_PKCS_OBJECT_IDENTIFIER_PKCS9_AT_EXTENDED_CERTIFICATE_ATTRIBUTES:
        return "1.2.840.113549.1.9.9";
    case DLMS_PKCS_OBJECT_IDENTIFIER_PKCS9_AT_SIGNING_DESCRIPTION:
        return "1.2.840.113549.1.9.13";
    case DLMS_PKCS_OBJECT_IDENTIFIER_PKCS9_AT_EXTENSION_REQUEST:
        return "1.2.840.113549.1.9.14";
    case DLMS_PKCS_OBJECT_IDENTIFIER_PKCS9_AT_SMIME_CAPABILITIES:
        return "1.2.840.113549.1.9.15";
    case DLMS_PKCS_OBJECT_IDENTIFIER_ID_SMIME:
        return "1.2.840.113549.1.9.16";
    case DLMS_PKCS_OBJECT_IDENTIFIER_PKCS9_AT_FRIENDLY_NAME:
        return "1.2.840.113549.1.9.20";
    case DLMS_PKCS_OBJECT_IDENTIFIER_PKCS9_AT_LOCAL_KEY_ID:
        return "1.2.840.113549.1.9.21";
    case DLMS_PKCS_OBJECT_IDENTIFIER_X509_CERTIFICATE:
        return "1.2.840.113549.1.9.22.1";
    case DLMS_PKCS_OBJECT_IDENTIFIER_SDSI_CERTIFICATE:
        return "1.2.840.113549.1.9.22.2";
    case DLMS_PKCS_OBJECT_IDENTIFIER_X509_CRL:
        return "1.2.840.113549.1.9.23.1";
    case DLMS_PKCS_OBJECT_IDENTIFIER_ID_ALG:
        return "1.2.840.113549.1.9.16.3";
    case DLMS_PKCS_OBJECT_IDENTIFIER_ID_ALG_ESDH:
        return "1.2.840.113549.1.9.16.3.5";
    case DLMS_PKCS_OBJECT_IDENTIFIER_ID_ALG_CMS3_DES_WRAP:
        return "1.2.840.113549.1.9.16.3.6";
    case DLMS_PKCS_OBJECT_IDENTIFIER_ID_ALG_CMS_RC2_WRAP:
        return "1.2.840.113549.1.9.16.3.7";
    case DLMS_PKCS_OBJECT_IDENTIFIER_ID_ALG_PWRI_KEK:
        return "1.2.840.113549.1.9.16.3.9";
    case DLMS_PKCS_OBJECT_IDENTIFIER_ID_ALG_SSDH:
        return "1.2.840.113549.1.9.16.3.10";
    case DLMS_PKCS_OBJECT_IDENTIFIER_ID_RSA_KEM:
        return "1.2.840.113549.1.9.16.3.14";
    case DLMS_PKCS_OBJECT_IDENTIFIER_PREFER_SIGNED_DATA:
        return "1.2.840.113549.1.9.15.1";
    case DLMS_PKCS_OBJECT_IDENTIFIER_CANNOT_DECRYPT_ANY:
        return "1.2.840.113549.1.9.15.2";
    case DLMS_PKCS_OBJECT_IDENTIFIER_SMIME_CAPABILITIES_VERSIONS:
        return "1.2.840.113549.1.9.15.3";
    case DLMS_PKCS_OBJECT_IDENTIFIER_ID_AA_RECEIPT_REQUEST:
        return "1.2.840.113549.1.9.16.2.1";
    case DLMS_PKCS_OBJECT_IDENTIFIER_ID_CT_AUTH_DATA:
        return "1.2.840.113549.1.9.16.1.2";
    case DLMS_PKCS_OBJECT_IDENTIFIER_ID_CT_TST_INFO:
        return "1.2.840.113549.1.9.16.1.4";
    case DLMS_PKCS_OBJECT_IDENTIFIER_ID_CT_COMPRESSED_DATA:
        return "1.2.840.113549.1.9.16.1.9";
    case DLMS_PKCS_OBJECT_IDENTIFIER_ID_CT_AUTH_ENVELOPED_DATA:
        return "1.2.840.113549.1.9.16.1.23";
    case DLMS_PKCS_OBJECT_IDENTIFIER_ID_CT_TIME_STAMPED_DATA:
        return "1.2.840.113549.1.9.16.1.31";
    case DLMS_PKCS_OBJECT_IDENTIFIER_ID_CTI_ETS_PROOF_OF_ORIGIN:
        return "1.2.840.113549.1.9.16.6.1";
    case DLMS_PKCS_OBJECT_IDENTIFIER_ID_CTI_ETS_PROOF_OF_RECEIPT:
        return "1.2.840.113549.1.9.16.6.2";
    case DLMS_PKCS_OBJECT_IDENTIFIER_ID_CTI_ETS_PROOF_OF_DELIVERY:
        return "1.2.840.113549.1.9.16.6.3";
    case DLMS_PKCS_OBJECT_IDENTIFIER_ID_CTI_ETS_PROOF_OF_SENDER:
        return "1.2.840.113549.1.9.16.6.4";
    case DLMS_PKCS_OBJECT_IDENTIFIER_ID_CTI_ETS_PROOF_OF_APPROVAL:
        return "1.2.840.113549.1.9.16.6.5";
    case DLMS_PKCS_OBJECT_IDENTIFIER_ID_CTI_ETS_PROOF_OF_CREATION:
        return "1.2.840.113549.1.9.16.6.6";
    case DLMS_PKCS_OBJECT_IDENTIFIER_ID_AA_CONTENT_HINT:
        return "1.2.840.113549.1.9.16.2.4";
    case DLMS_PKCS_OBJECT_IDENTIFIER_ID_AA_MSG_SIG_DIGEST:
        return "1.2.840.113549.1.9.16.2.5";
    case DLMS_PKCS_OBJECT_IDENTIFIER_ID_AA_CONTENT_REFERENCE:
        return "1.2.840.113549.1.9.16.2.10";
    case DLMS_PKCS_OBJECT_IDENTIFIER_ID_AA_ENCRYP_KEY_PREF:
        return "1.2.840.113549.1.9.16.2.11";
    case DLMS_PKCS_OBJECT_IDENTIFIER_ID_AA_SIGNING_CERTIFICATE:
        return "1.2.840.113549.1.9.16.2.12";
    case DLMS_PKCS_OBJECT_IDENTIFIER_ID_AA_SIGNING_CERTIFICATE_V2:
        return "1.2.840.113549.1.9.16.2.47";
    case DLMS_PKCS_OBJECT_IDENTIFIER_ID_AA_CONTENT_ID_ENTIFIER:
        return "1.2.840.113549.1.9.16.2.7";
    case DLMS_PKCS_OBJECT_IDENTIFIER_ID_AA_SIGNATURE_TIME_STAMP_TOKEN:
        return "1.2.840.113549.1.9.16.2.14";
    case DLMS_PKCS_OBJECT_IDENTIFIER_ID_AA_ETS_SIG_POLICY_ID:
        return "1.2.840.113549.1.9.16.2.15";
    case DLMS_PKCS_OBJECT_IDENTIFIER_ID_AA_ETS_COMMITMENT_TYPE:
        return "1.2.840.113549.1.9.16.2.16";
    case DLMS_PKCS_OBJECT_IDENTIFIER_ID_AA_ETS_SIGNER_LOCATION:
        return "1.2.840.113549.1.9.16.2.17";
    case DLMS_PKCS_OBJECT_IDENTIFIER_ID_AA_ETS_SIGNER_ATTR:
        return "1.2.840.113549.1.9.16.2.18";
    case DLMS_PKCS_OBJECT_IDENTIFIER_ID_AA_ETS_OTHER_SIG_CERT:
        return "1.2.840.113549.1.9.16.2.19";
    case DLMS_PKCS_OBJECT_IDENTIFIER_ID_AA_ETS_CONTENT_TIMESTAMP:
        return "1.2.840.113549.1.9.16.2.20";
    case DLMS_PKCS_OBJECT_IDENTIFIER_ID_AA_ETS_CERTIFICATE_REFS:
        return "1.2.840.113549.1.9.16.2.21";
    case DLMS_PKCS_OBJECT_IDENTIFIER_ID_AA_ETS_REVOCATION_REFS:
        return "1.2.840.113549.1.9.16.2.22";
    case DLMS_PKCS_OBJECT_IDENTIFIER_ID_AA_ETS_CERT_VALUES:
        return "1.2.840.113549.1.9.16.2.23";
    case DLMS_PKCS_OBJECT_IDENTIFIER_ID_AA_ETS_REVOCATION_VALUES:
        return "1.2.840.113549.1.9.16.2.24";
    case DLMS_PKCS_OBJECT_IDENTIFIER_ID_AA_ETS_ESC_TIME_STAMP:
        return "1.2.840.113549.1.9.16.2.25";
    case DLMS_PKCS_OBJECT_IDENTIFIER_ID_AA_ETS_CERT_CRL_TIMESTAMP:
        return "1.2.840.113549.1.9.16.2.26";
    case DLMS_PKCS_OBJECT_IDENTIFIER_ID_AA_ETS_ARCHIVE_TIMESTAMP:
        return "1.2.840.113549.1.9.16.2.27";
    case DLMS_PKCS_OBJECT_IDENTIFIER_ID_SPQ_ETS_URI:
        return "1.2.840.113549.1.9.16.5.1";
    case DLMS_PKCS_OBJECT_IDENTIFIER_ID_SPQ_ETS_UNOTICE:
        return "1.2.840.113549.1.9.16.5.2";
    case DLMS_PKCS_OBJECT_IDENTIFIER_KEY_BAG:
        return "1.2.840.113549.1.12.10.1.1";
    case DLMS_PKCS_OBJECT_IDENTIFIER_PKCS8_SHROUDED_KEY_BAG:
        return "1.2.840.113549.1.12.10.1.2";
    case DLMS_PKCS_OBJECT_IDENTIFIER_CERT_BAG:
        return "1.2.840.113549.1.12.10.1.3";
    case DLMS_PKCS_OBJECT_IDENTIFIER_CRL_BAG:
        return "1.2.840.113549.1.12.10.1.4";
    case DLMS_PKCS_OBJECT_IDENTIFIER_SECRET_BAG:
        return "1.2.840.113549.1.12.10.1.5";
    case DLMS_PKCS_OBJECT_IDENTIFIER_SAFECONTENTS_BAG:
        return "1.2.840.113549.1.12.10.1.6";
    case DLMS_PKCS_OBJECT_IDENTIFIER_PBE_WITH_SHA_AND_128_BIT_RC4:
        return "1.2.840.113549.1.12.1.1";
    case DLMS_PKCS_OBJECT_IDENTIFIER_PBE_WITH_SHA_AND_40_BIT_RC4:
        return "1.2.840.113549.1.12.1.2";
    case DLMS_PKCS_OBJECT_IDENTIFIER_PBE_WITH_SHA_AND_3KEY_TRIPLE_DES_CBC:
        return "1.2.840.113549.1.12.1.3";
    case DLMS_PKCS_OBJECT_IDENTIFIER_PBE_WITH_SHA_AND_2KEY_TRIPLE_DES_CBC:
        return "1.2.840.113549.1.12.1.4";
    case DLMS_PKCS_OBJECT_IDENTIFIER_PBE_WITH_SHA_AND_128_BIT_RC2_CBC:
        return "1.2.840.113549.1.12.1.5";
    case DLMS_PKCS_OBJECT_IDENTIFIER_PBE_WITH_SHA_AND_40_BIT_RC2_CBC:
        return "1.2.840.113549.1.12.1.6";
    default:
        return NULL;
    }
}

DLMS_PKCS_OBJECT_IDENTIFIER DLMSConverter::ValueOfPKCSObjectIdentifier(const char* value)
{
    if (strcmp(value, "1.2.840.113549.1.1.1") == 0)
    {
        return DLMS_PKCS_OBJECT_IDENTIFIER_RSA_ENCRYPTION;
    }
    else if (strcmp(value, "1.2.840.113549.1.1.2") == 0)
    {
        return DLMS_PKCS_OBJECT_IDENTIFIER_MD2_WITH_RSA_ENCRYPTION;
    }
    else if (strcmp(value, "1.2.840.113549.1.1.3") == 0)
    {
        return DLMS_PKCS_OBJECT_IDENTIFIER_MD4_WITH_RSA_ENCRYPTION;
    }
    else if (strcmp(value, "1.2.840.113549.1.1.4") == 0)
    {
        return DLMS_PKCS_OBJECT_IDENTIFIER_MD5_WITH_RSA_ENCRYPTION;
    }
    else if (strcmp(value, "1.2.840.113549.1.1.5") == 0)
    {
        return DLMS_PKCS_OBJECT_IDENTIFIER_SHA1_WITH_RSA_ENCRYPTION;
    }
    else if (strcmp(value, "1.2.840.113549.1.1.6") == 0)
    {
        return DLMS_PKCS_OBJECT_IDENTIFIER_SRSA_OAEP_ENCRYPTIONSET;
    }
    else if (strcmp(value, "1.2.840.113549.1.1.7") == 0)
    {
        return DLMS_PKCS_OBJECT_IDENTIFIER_ID_RSAES_OAEP;
    }
    else if (strcmp(value, "1.2.840.113549.1.1.8") == 0)
    {
        return DLMS_PKCS_OBJECT_IDENTIFIER_ID_MGF1;
    }
    else if (strcmp(value, "1.2.840.113549.1.1.9") == 0)
    {
        return DLMS_PKCS_OBJECT_IDENTIFIER_ID_PSPECIFIED;
    }
    else if (strcmp(value, "1.2.840.113549.1.1.10") == 0)
    {
        return DLMS_PKCS_OBJECT_IDENTIFIER_ID_RSASSA_PSS;
    }
    else if (strcmp(value, "1.2.840.113549.1.1.11") == 0)
    {
        return DLMS_PKCS_OBJECT_IDENTIFIER_SHA256_WITH_RSA_ENCRYPTION;
    }
    else if (strcmp(value, "1.2.840.113549.1.1.12") == 0)
    {
        return DLMS_PKCS_OBJECT_IDENTIFIER_SHA384_WITH_RSA_ENCRYPTION;
    }
    else if (strcmp(value, "1.2.840.113549.1.1.13") == 0)
    {
        return DLMS_PKCS_OBJECT_IDENTIFIER_SHA512_WITH_RSA_ENCRYPTION;
    }
    else if (strcmp(value, "1.2.840.113549.1.1.14") == 0)
    {
        return DLMS_PKCS_OBJECT_IDENTIFIER_SHA224_WITH_RSA_ENCRYPTION;
    }
    else if (strcmp(value, "1.2.840.113549.1.3.1") == 0)
    {
        return DLMS_PKCS_OBJECT_IDENTIFIER_DH_KEY_AGREEMENT;
    }
    else if (strcmp(value, "1.2.840.113549.1.5.1") == 0)
    {
        return DLMS_PKCS_OBJECT_IDENTIFIER_PBE_WITH_MD2_AND_DES_CBC;
    }
    else if (strcmp(value, "1.2.840.113549.1.5.4") == 0)
    {
        return DLMS_PKCS_OBJECT_IDENTIFIER_PBE_WITH_MD2_AND_RC2_CBC;
    }
    else if (strcmp(value, "1.2.840.113549.1.5.3") == 0)
    {
        return DLMS_PKCS_OBJECT_IDENTIFIER_PBE_WITH_MD5_AND_DES_CBC;
    }
    else if (strcmp(value, "1.2.840.113549.1.5.6") == 0)
    {
        return DLMS_PKCS_OBJECT_IDENTIFIER_PBE_WITH_MD5_AND_RC2_CBC;
    }
    else if (strcmp(value, "1.2.840.113549.1.5.10") == 0)
    {
        return DLMS_PKCS_OBJECT_IDENTIFIER_PBE_WITH_SHA1_AND_DES_CBC;
    }
    else if (strcmp(value, "1.2.840.113549.1.5.11") == 0)
    {
        return DLMS_PKCS_OBJECT_IDENTIFIER_PBE_WITH_SHA1_AND_RC2_CBC;
    }
    else if (strcmp(value, "1.2.840.113549.1.5.13") == 0)
    {
        return DLMS_PKCS_OBJECT_IDENTIFIER_ID_PBE_S2;
    }
    else if (strcmp(value, "1.2.840.113549.1.5.12") == 0)
    {
        return DLMS_PKCS_OBJECT_IDENTIFIER_ID_PBKD_F2;
    }
    else if (strcmp(value, "1.2.840.113549.3.7") == 0)
    {
        return DLMS_PKCS_OBJECT_IDENTIFIER_DES_EDE3_CBC;
    }
    else if (strcmp(value, "1.2.840.113549.3.2") == 0)
    {
        return DLMS_PKCS_OBJECT_IDENTIFIER_RC2_CBC;
    }
    else if (strcmp(value, "1.2.840.113549.2.2") == 0)
    {
        return DLMS_PKCS_OBJECT_IDENTIFIER_MD2;
    }
    else if (strcmp(value, "1.2.840.113549.2.4") == 0)
    {
        return DLMS_PKCS_OBJECT_IDENTIFIER_MD4;
    }
    else if (strcmp(value, "1.2.840.113549.2.5") == 0)
    {
        return DLMS_PKCS_OBJECT_IDENTIFIER_MD5;
    }
    else if (strcmp(value, "1.2.840.113549.2.7") == 0)
    {
        return DLMS_PKCS_OBJECT_IDENTIFIER_ID_HMAC_WITH_SHA1;
    }
    else if (strcmp(value, "1.2.840.113549.2.8") == 0)
    {
        return DLMS_PKCS_OBJECT_IDENTIFIER_ID_HMAC_WITH_SHA224;
    }
    else if (strcmp(value, "1.2.840.113549.2.9") == 0)
    {
        return DLMS_PKCS_OBJECT_IDENTIFIER_ID_HMAC_WITH_SHA256;
    }
    else if (strcmp(value, "1.2.840.113549.2.10") == 0)
    {
        return DLMS_PKCS_OBJECT_IDENTIFIER_ID_HMAC_WITH_SHA384;
    }
    else if (strcmp(value, "1.2.840.113549.2.11") == 0)
    {
        return DLMS_PKCS_OBJECT_IDENTIFIER_ID_HMAC_WITH_SHA512;
    }
    else if (strcmp(value, "1.2.840.113549.1.7.1") == 0)
    {
        return DLMS_PKCS_OBJECT_IDENTIFIER_DATA;
    }
    else if (strcmp(value, "1.2.840.113549.1.7.2") == 0)
    {
        return DLMS_PKCS_OBJECT_IDENTIFIER_SIGNED_DATA;
    }
    else if (strcmp(value, "1.2.840.113549.1.7.3") == 0)
    {
        return DLMS_PKCS_OBJECT_IDENTIFIER_ENVELOPED_DATA;
    }
    else if (strcmp(value, "1.2.840.113549.1.7.4") == 0)
    {
        return DLMS_PKCS_OBJECT_IDENTIFIER_SIGNED_AND_ENVELOPED_DATA;
    }
    else if (strcmp(value, "1.2.840.113549.1.7.5") == 0)
    {
        return DLMS_PKCS_OBJECT_IDENTIFIER_DIGESTED_DATA;
    }
    else if (strcmp(value, "1.2.840.113549.1.7.6") == 0)
    {
        return DLMS_PKCS_OBJECT_IDENTIFIER_ENCRYPTED_DATA;
    }
    else if (strcmp(value, "1.2.840.113549.1.9.1") == 0)
    {
        return DLMS_PKCS_OBJECT_IDENTIFIER_PKCS9_AT_EMAIL_ADDRESS;
    }
    else if (strcmp(value, "1.2.840.113549.1.9.2") == 0)
    {
        return DLMS_PKCS_OBJECT_IDENTIFIER_PKCS9_AT_UNSTRUCTURED_NAME;
    }
    else if (strcmp(value, "1.2.840.113549.1.9.3") == 0)
    {
        return DLMS_PKCS_OBJECT_IDENTIFIER_PKCS9_AT_CONTENT_TYPE;
    }
    else if (strcmp(value, "1.2.840.113549.1.9.4") == 0)
    {
        return DLMS_PKCS_OBJECT_IDENTIFIER_PKCS9_AT_MESSAGE_DIGEST;
    }
    else if (strcmp(value, "1.2.840.113549.1.9.5") == 0)
    {
        return DLMS_PKCS_OBJECT_IDENTIFIER_PKCS9_AT_SIGNING_TIME;
    }
    else if (strcmp(value, "1.2.840.113549.1.9.6") == 0)
    {
        return DLMS_PKCS_OBJECT_IDENTIFIER_PKCS9_AT_COUNTER_SIGNATURE;
    }
    else if (strcmp(value, "1.2.840.113549.1.9.7") == 0)
    {
        return DLMS_PKCS_OBJECT_IDENTIFIER_PKCS9_AT_CHALLENGE_PASSWORD;
    }
    else if (strcmp(value, "1.2.840.113549.1.9.8") == 0)
    {
        return DLMS_PKCS_OBJECT_IDENTIFIER_PKCS9_AT_UNSTRUCTURED_ADDRESS;
    }
    else if (strcmp(value, "1.2.840.113549.1.9.9") == 0)
    {
        return DLMS_PKCS_OBJECT_IDENTIFIER_PKCS9_AT_EXTENDED_CERTIFICATE_ATTRIBUTES;
    }
    else if (strcmp(value, "1.2.840.113549.1.9.13") == 0)
    {
        return DLMS_PKCS_OBJECT_IDENTIFIER_PKCS9_AT_SIGNING_DESCRIPTION;
    }
    else if (strcmp(value, "1.2.840.113549.1.9.14") == 0)
    {
        return DLMS_PKCS_OBJECT_IDENTIFIER_PKCS9_AT_EXTENSION_REQUEST;
    }
    else if (strcmp(value, "1.2.840.113549.1.9.15") == 0)
    {
        return DLMS_PKCS_OBJECT_IDENTIFIER_PKCS9_AT_SMIME_CAPABILITIES;
    }
    else if (strcmp(value, "1.2.840.113549.1.9.16") == 0)
    {
        return DLMS_PKCS_OBJECT_IDENTIFIER_ID_SMIME;
    }
    else if (strcmp(value, "1.2.840.113549.1.9.20") == 0)
    {
        return DLMS_PKCS_OBJECT_IDENTIFIER_PKCS9_AT_FRIENDLY_NAME;
    }
    else if (strcmp(value, "1.2.840.113549.1.9.21") == 0)
    {
        return DLMS_PKCS_OBJECT_IDENTIFIER_PKCS9_AT_LOCAL_KEY_ID;
    }
    else if (strcmp(value, "1.2.840.113549.1.9.22.1") == 0)
    {
        return DLMS_PKCS_OBJECT_IDENTIFIER_X509_CERTIFICATE;
    }
    else if (strcmp(value, "1.2.840.113549.1.9.22.2") == 0)
    {
        return DLMS_PKCS_OBJECT_IDENTIFIER_SDSI_CERTIFICATE;
    }
    else if (strcmp(value, "1.2.840.113549.1.9.23.1") == 0)
    {
        return DLMS_PKCS_OBJECT_IDENTIFIER_X509_CRL;
    }
    else if (strcmp(value, "1.2.840.113549.1.9.16.3") == 0)
    {
        return DLMS_PKCS_OBJECT_IDENTIFIER_ID_ALG;
    }
    else if (strcmp(value, "1.2.840.113549.1.9.16.3.5") == 0)
    {
        return DLMS_PKCS_OBJECT_IDENTIFIER_ID_ALG_ESDH;
    }
    else if (strcmp(value, "1.2.840.113549.1.9.16.3.6") == 0)
    {
        return DLMS_PKCS_OBJECT_IDENTIFIER_ID_ALG_CMS3_DES_WRAP;
    }
    else if (strcmp(value, "1.2.840.113549.1.9.16.3.7") == 0)
    {
        return DLMS_PKCS_OBJECT_IDENTIFIER_ID_ALG_CMS_RC2_WRAP;
    }
    else if (strcmp(value, "1.2.840.113549.1.9.16.3.9") == 0)
    {
        return DLMS_PKCS_OBJECT_IDENTIFIER_ID_ALG_PWRI_KEK;
    }
    else if (strcmp(value, "1.2.840.113549.1.9.16.3.10") == 0)
    {
        return DLMS_PKCS_OBJECT_IDENTIFIER_ID_ALG_SSDH;
    }
    else if (strcmp(value, "1.2.840.113549.1.9.16.3.14") == 0)
    {
        return DLMS_PKCS_OBJECT_IDENTIFIER_ID_RSA_KEM;
    }
    else if (strcmp(value, "1.2.840.113549.1.9.15.1") == 0)
    {
        return DLMS_PKCS_OBJECT_IDENTIFIER_PREFER_SIGNED_DATA;
    }
    else if (strcmp(value, "1.2.840.113549.1.9.15.2") == 0)
    {
        return DLMS_PKCS_OBJECT_IDENTIFIER_CANNOT_DECRYPT_ANY;
    }
    else if (strcmp(value, "1.2.840.113549.1.9.15.3") == 0)
    {
        return DLMS_PKCS_OBJECT_IDENTIFIER_SMIME_CAPABILITIES_VERSIONS;
    }
    else if (strcmp(value, "1.2.840.113549.1.9.16.2.1") == 0)
    {
        return DLMS_PKCS_OBJECT_IDENTIFIER_ID_AA_RECEIPT_REQUEST;
    }
    else if (strcmp(value, "1.2.840.113549.1.9.16.1.2") == 0)
    {
        return DLMS_PKCS_OBJECT_IDENTIFIER_ID_CT_AUTH_DATA;
    }
    else if (strcmp(value, "1.2.840.113549.1.9.16.1.4") == 0)
    {
        return DLMS_PKCS_OBJECT_IDENTIFIER_ID_CT_TST_INFO;
    }
    else if (strcmp(value, "1.2.840.113549.1.9.16.1.9") == 0)
    {
        return DLMS_PKCS_OBJECT_IDENTIFIER_ID_CT_COMPRESSED_DATA;
    }
    else if (strcmp(value, "1.2.840.113549.1.9.16.1.23") == 0)
    {
        return DLMS_PKCS_OBJECT_IDENTIFIER_ID_CT_AUTH_ENVELOPED_DATA;
    }
    else if (strcmp(value, "1.2.840.113549.1.9.16.1.31") == 0)
    {
        return DLMS_PKCS_OBJECT_IDENTIFIER_ID_CT_TIME_STAMPED_DATA;
    }
    else if (strcmp(value, "1.2.840.113549.1.9.16.6.1") == 0)
    {
        return DLMS_PKCS_OBJECT_IDENTIFIER_ID_CTI_ETS_PROOF_OF_ORIGIN;
    }
    else if (strcmp(value, "1.2.840.113549.1.9.16.6.2") == 0)
    {
        return DLMS_PKCS_OBJECT_IDENTIFIER_ID_CTI_ETS_PROOF_OF_RECEIPT;
    }
    else if (strcmp(value, "1.2.840.113549.1.9.16.6.3") == 0)
    {
        return DLMS_PKCS_OBJECT_IDENTIFIER_ID_CTI_ETS_PROOF_OF_DELIVERY;
    }
    else if (strcmp(value, "1.2.840.113549.1.9.16.6.4") == 0)
    {
        return DLMS_PKCS_OBJECT_IDENTIFIER_ID_CTI_ETS_PROOF_OF_SENDER;
    }
    else if (strcmp(value, "1.2.840.113549.1.9.16.6.5") == 0)
    {
        return DLMS_PKCS_OBJECT_IDENTIFIER_ID_CTI_ETS_PROOF_OF_APPROVAL;
    }
    else if (strcmp(value, "1.2.840.113549.1.9.16.6.6") == 0)
    {
        return DLMS_PKCS_OBJECT_IDENTIFIER_ID_CTI_ETS_PROOF_OF_CREATION;
    }
    else if (strcmp(value, "1.2.840.113549.1.9.16.2.4") == 0)
    {
        return DLMS_PKCS_OBJECT_IDENTIFIER_ID_AA_CONTENT_HINT;
    }
    else if (strcmp(value, "1.2.840.113549.1.9.16.2.5") == 0)
    {
        return DLMS_PKCS_OBJECT_IDENTIFIER_ID_AA_MSG_SIG_DIGEST;
    }
    else if (strcmp(value, "1.2.840.113549.1.9.16.2.10") == 0)
    {
        return DLMS_PKCS_OBJECT_IDENTIFIER_ID_AA_CONTENT_REFERENCE;
    }
    else if (strcmp(value, "1.2.840.113549.1.9.16.2.11") == 0)
    {
        return DLMS_PKCS_OBJECT_IDENTIFIER_ID_AA_ENCRYP_KEY_PREF;
    }
    else if (strcmp(value, "1.2.840.113549.1.9.16.2.12") == 0)
    {
        return DLMS_PKCS_OBJECT_IDENTIFIER_ID_AA_SIGNING_CERTIFICATE;
    }
    else if (strcmp(value, "1.2.840.113549.1.9.16.2.47") == 0)
    {
        return DLMS_PKCS_OBJECT_IDENTIFIER_ID_AA_SIGNING_CERTIFICATE_V2;
    }
    else if (strcmp(value, "1.2.840.113549.1.9.16.2.7") == 0)
    {
        return DLMS_PKCS_OBJECT_IDENTIFIER_ID_AA_CONTENT_ID_ENTIFIER;
    }
    else if (strcmp(value, "1.2.840.113549.1.9.16.2.14") == 0)
    {
        return DLMS_PKCS_OBJECT_IDENTIFIER_ID_AA_SIGNATURE_TIME_STAMP_TOKEN;
    }
    else if (strcmp(value, "1.2.840.113549.1.9.16.2.15") == 0)
    {
        return DLMS_PKCS_OBJECT_IDENTIFIER_ID_AA_ETS_SIG_POLICY_ID;
    }
    else if (strcmp(value, "1.2.840.113549.1.9.16.2.16") == 0)
    {
        return DLMS_PKCS_OBJECT_IDENTIFIER_ID_AA_ETS_COMMITMENT_TYPE;
    }
    else if (strcmp(value, "1.2.840.113549.1.9.16.2.17") == 0)
    {
        return DLMS_PKCS_OBJECT_IDENTIFIER_ID_AA_ETS_SIGNER_LOCATION;
    }
    else if (strcmp(value, "1.2.840.113549.1.9.16.2.18") == 0)
    {
        return DLMS_PKCS_OBJECT_IDENTIFIER_ID_AA_ETS_SIGNER_ATTR;
    }
    else if (strcmp(value, "1.2.840.113549.1.9.16.2.19") == 0)
    {
        return DLMS_PKCS_OBJECT_IDENTIFIER_ID_AA_ETS_OTHER_SIG_CERT;
    }
    else if (strcmp(value, "1.2.840.113549.1.9.16.2.20") == 0)
    {
        return DLMS_PKCS_OBJECT_IDENTIFIER_ID_AA_ETS_CONTENT_TIMESTAMP;
    }
    else if (strcmp(value, "1.2.840.113549.1.9.16.2.21") == 0)
    {
        return DLMS_PKCS_OBJECT_IDENTIFIER_ID_AA_ETS_CERTIFICATE_REFS;
    }
    else if (strcmp(value, "1.2.840.113549.1.9.16.2.22") == 0)
    {
        return DLMS_PKCS_OBJECT_IDENTIFIER_ID_AA_ETS_REVOCATION_REFS;
    }
    else if (strcmp(value, "1.2.840.113549.1.9.16.2.23") == 0)
    {
        return DLMS_PKCS_OBJECT_IDENTIFIER_ID_AA_ETS_CERT_VALUES;
    }
    else if (strcmp(value, "1.2.840.113549.1.9.16.2.24") == 0)
    {
        return DLMS_PKCS_OBJECT_IDENTIFIER_ID_AA_ETS_REVOCATION_VALUES;
    }
    else if (strcmp(value, "1.2.840.113549.1.9.16.2.25") == 0)
    {
        return DLMS_PKCS_OBJECT_IDENTIFIER_ID_AA_ETS_ESC_TIME_STAMP;
    }
    else if (strcmp(value, "1.2.840.113549.1.9.16.2.26") == 0)
    {
        return DLMS_PKCS_OBJECT_IDENTIFIER_ID_AA_ETS_CERT_CRL_TIMESTAMP;
    }
    else if (strcmp(value, "1.2.840.113549.1.9.16.2.27") == 0)
    {
        return DLMS_PKCS_OBJECT_IDENTIFIER_ID_AA_ETS_ARCHIVE_TIMESTAMP;
    }
    else if (strcmp(value, "1.2.840.113549.1.9.16.5.1") == 0)
    {
        return DLMS_PKCS_OBJECT_IDENTIFIER_ID_SPQ_ETS_URI;
    }
    else if (strcmp(value, "1.2.840.113549.1.9.16.5.2") == 0)
    {
        return DLMS_PKCS_OBJECT_IDENTIFIER_ID_SPQ_ETS_UNOTICE;
    }
    else if (strcmp(value, "1.2.840.113549.1.12.10.1.1") == 0)
    {
        return DLMS_PKCS_OBJECT_IDENTIFIER_KEY_BAG;
    }
    else if (strcmp(value, "1.2.840.113549.1.12.10.1.2") == 0)
    {
        return DLMS_PKCS_OBJECT_IDENTIFIER_PKCS8_SHROUDED_KEY_BAG;
    }
    else if (strcmp(value, "1.2.840.113549.1.12.10.1.3") == 0)
    {
        return DLMS_PKCS_OBJECT_IDENTIFIER_CERT_BAG;
    }
    else if (strcmp(value, "1.2.840.113549.1.12.10.1.4") == 0)
    {
        return DLMS_PKCS_OBJECT_IDENTIFIER_CRL_BAG;
    }
    else if (strcmp(value, "1.2.840.113549.1.12.10.1.5") == 0)
    {
        return DLMS_PKCS_OBJECT_IDENTIFIER_SECRET_BAG;
    }
    else if (strcmp(value, "1.2.840.113549.1.12.10.1.6") == 0)
    {
        return DLMS_PKCS_OBJECT_IDENTIFIER_SAFECONTENTS_BAG;
    }
    else if (strcmp(value, "1.2.840.113549.1.12.1.1") == 0)
    {
        return DLMS_PKCS_OBJECT_IDENTIFIER_PBE_WITH_SHA_AND_128_BIT_RC4;
    }
    else if (strcmp(value, "1.2.840.113549.1.12.1.2") == 0)
    {
        return DLMS_PKCS_OBJECT_IDENTIFIER_PBE_WITH_SHA_AND_40_BIT_RC4;
    }
    else if (strcmp(value, "1.2.840.113549.1.12.1.3") == 0)
    {
        return DLMS_PKCS_OBJECT_IDENTIFIER_PBE_WITH_SHA_AND_3KEY_TRIPLE_DES_CBC;
    }
    else if (strcmp(value, "1.2.840.113549.1.12.1.4") == 0)
    {
        return DLMS_PKCS_OBJECT_IDENTIFIER_PBE_WITH_SHA_AND_2KEY_TRIPLE_DES_CBC;
    }
    else if (strcmp(value, "1.2.840.113549.1.12.1.5") == 0)
    {
        return DLMS_PKCS_OBJECT_IDENTIFIER_PBE_WITH_SHA_AND_128_BIT_RC2_CBC;
    }
    else if (strcmp(value, "1.2.840.113549.1.12.1.6") == 0)
    {
        return DLMS_PKCS_OBJECT_IDENTIFIER_PBE_WITH_SHA_AND_40_BIT_RC2_CBC;
    }
    return DLMS_PKCS_OBJECT_IDENTIFIER_NONE;
}

const char* DLMSConverter::ToString(DLMS_HASH_ALGORITHM value)
{
    switch (value)
    {
    case DLMS_HASH_ALGORITHM_SHA1_RSA:
        return "1.2.840.113549.1.1.5";
    case DLMS_HASH_ALGORITHM_MD5_RSA:
        return "1.2.840.113549.1.1.4";
    case DLMS_HASH_ALGORITHM_SHA1_DSA:
        return "1.2.840.10040.4.3";
    case DLMS_HASH_ALGORITHM_SHA1_RSA1:
        return "1.3.14.3.2.29";
    case DLMS_HASH_ALGORITHM_SHA_RSA:
        return "1.3.14.3.2.15";
    case DLMS_HASH_ALGORITHM_MD5_RSA1:
        return "1.3.14.3.2.3";
    case DLMS_HASH_ALGORITHM_MD2_RSA1:
        return "1.2.840.113549.1.1.2";
    case DLMS_HASH_ALGORITHM_MD4_RSA:
        return "1.2.840.113549.1.1.3";
    case DLMS_HASH_ALGORITHM_MD4_RSA1:
        return "1.3.14.3.2.2";
    case DLMS_HASH_ALGORITHM_MD4_RSA2:
        return "1.3.14.3.2.4";
    case DLMS_HASH_ALGORITHM_MD2_RSA:
        return "1.3.14.7.2.3.1";
    case DLMS_HASH_ALGORITHM_SHA1_DSA1:
        return "1.3.14.3.2.13";
    case DLMS_HASH_ALGORITHM_DSA_SHA1:
        return "1.3.14.3.2.27";
    case DLMS_HASH_ALGORITHM_MOSAIC_UPDATED_SIG:
        return "2.16.840.1.101.2.1.1.19";
    case DLMS_HASH_ALGORITHM_SHA1_NO_SIGN:
        return "1.3.14.3.2.26";
    case DLMS_HASH_ALGORITHM_MD5_NO_SIGN:
        return "1.2.840.113549.2.5";
    case DLMS_HASH_ALGORITHM_SHA_256_NO_SIGN:
        return "2.16.840.1.101.3.4.2.1";
    case DLMS_HASH_ALGORITHM_SHA_384_NO_SIGN:
        return "2.16.840.1.101.3.4.2.2";
    case DLMS_HASH_ALGORITHM_SHA_512_NO_SIGN:
        return "2.16.840.1.101.3.4.2.3";
    case DLMS_HASH_ALGORITHM_SHA_256_RSA:
        return "1.2.840.113549.1.1.11";
    case DLMS_HASH_ALGORITHM_SHA_384_RSA:
        return "1.2.840.113549.1.1.12";
    case DLMS_HASH_ALGORITHM_SHA_512_RSA:
        return "1.2.840.113549.1.1.13";
    case DLMS_HASH_ALGORITHM_RSA_SSA_PSS:
        return "1.2.840.113549.1.1.10";
    case DLMS_HASH_ALGORITHM_SHA1_WITH_ECDSA:
        return "1.2.840.10045.4.1";
    case DLMS_HASH_ALGORITHM_SHA_256_WITH_ECDSA:
        return "1.2.840.10045.4.3.2";
    case DLMS_HASH_ALGORITHM_SHA_384_WITH_ECDSA:
        return "1.2.840.10045.4.3.3";
    case DLMS_HASH_ALGORITHM_SHA_512_WITH_ECDSA:
        return "1.2.840.10045.4.3.4";
    case DLMS_HASH_ALGORITHM_SPECIFIED_ECDSA:
        return "1.2.840.10045.4.3";
    default:
        return NULL;
    }
}

DLMS_HASH_ALGORITHM DLMSConverter::ValueOfHashAlgorithm(const char* value)
{
    if (strcmp(value, "1.2.840.113549.1.1.5") == 0)
    {
        return DLMS_HASH_ALGORITHM_SHA1_RSA;
    }
    else if (strcmp(value, "1.2.840.113549.1.1.4") == 0)
    {
        return DLMS_HASH_ALGORITHM_MD5_RSA;
    }
    else if (strcmp(value, "1.2.840.10040.4.3") == 0)
    {
        return DLMS_HASH_ALGORITHM_SHA1_DSA;
    }
    else if (strcmp(value, "1.3.14.3.2.29") == 0)
    {
        return DLMS_HASH_ALGORITHM_SHA1_RSA1;
    }
    else if (strcmp(value, "1.3.14.3.2.15") == 0)
    {
        return DLMS_HASH_ALGORITHM_SHA_RSA;
    }
    else if (strcmp(value, "1.3.14.3.2.3") == 0)
    {
        return DLMS_HASH_ALGORITHM_MD5_RSA1;
    }
    else if (strcmp(value, "1.2.840.113549.1.1.2") == 0)
    {
        return DLMS_HASH_ALGORITHM_MD2_RSA1;
    }
    else if (strcmp(value, "1.2.840.113549.1.1.3") == 0)
    {
        return DLMS_HASH_ALGORITHM_MD4_RSA;
    }
    else if (strcmp(value, "1.3.14.3.2.2") == 0)
    {
        return DLMS_HASH_ALGORITHM_MD4_RSA1;
    }
    else if (strcmp(value, "1.3.14.3.2.4") == 0)
    {
        return DLMS_HASH_ALGORITHM_MD4_RSA2;
    }
    else if (strcmp(value, "1.3.14.7.2.3.1") == 0)
    {
        return DLMS_HASH_ALGORITHM_MD2_RSA;
    }
    else if (strcmp(value, "1.3.14.3.2.13") == 0)
    {
        return DLMS_HASH_ALGORITHM_SHA1_DSA1;
    }
    else if (strcmp(value, "1.3.14.3.2.27") == 0)
    {
        return DLMS_HASH_ALGORITHM_DSA_SHA1;
    }
    else if (strcmp(value, "2.16.840.1.101.2.1.1.19") == 0)
    {
        return DLMS_HASH_ALGORITHM_MOSAIC_UPDATED_SIG;
    }
    else if (strcmp(value, "1.3.14.3.2.26") == 0)
    {
        return DLMS_HASH_ALGORITHM_SHA1_NO_SIGN;
    }
    else if (strcmp(value, "1.2.840.113549.2.5") == 0)
    {
        return DLMS_HASH_ALGORITHM_MD5_NO_SIGN;
    }
    else if (strcmp(value, "2.16.840.1.101.3.4.2.1") == 0)
    {
        return DLMS_HASH_ALGORITHM_SHA_256_NO_SIGN;
    }
    else if (strcmp(value, "2.16.840.1.101.3.4.2.2") == 0)
    {
        return DLMS_HASH_ALGORITHM_SHA_384_NO_SIGN;
    }
    else if (strcmp(value, "2.16.840.1.101.3.4.2.3") == 0)
    {
        return DLMS_HASH_ALGORITHM_SHA_512_NO_SIGN;
    }
    else if (strcmp(value, "1.2.840.113549.1.1.11") == 0)
    {
        return DLMS_HASH_ALGORITHM_SHA_256_RSA;
    }
    else if (strcmp(value, "1.2.840.113549.1.1.12") == 0)
    {
        return DLMS_HASH_ALGORITHM_SHA_384_RSA;
    }
    else if (strcmp(value, "1.2.840.113549.1.1.13") == 0)
    {
        return DLMS_HASH_ALGORITHM_SHA_512_RSA;
    }
    else if (strcmp(value, "1.2.840.113549.1.1.10") == 0)
    {
        return DLMS_HASH_ALGORITHM_RSA_SSA_PSS;
    }
    else if (strcmp(value, "1.2.840.10045.4.1") == 0)
    {
        return DLMS_HASH_ALGORITHM_SHA1_WITH_ECDSA;
    }
    else if (strcmp(value, "1.2.840.10045.4.3.2") == 0)
    {
        return DLMS_HASH_ALGORITHM_SHA_256_WITH_ECDSA;
    }
    else if (strcmp(value, "1.2.840.10045.4.3.3") == 0)
    {
        return DLMS_HASH_ALGORITHM_SHA_384_WITH_ECDSA;
    }
    else if (strcmp(value, "1.2.840.10045.4.3.4") == 0)
    {
        return DLMS_HASH_ALGORITHM_SHA_512_WITH_ECDSA;
    }
    else if (strcmp(value, "1.2.840.10045.4.3") == 0)
    {
        return DLMS_HASH_ALGORITHM_SPECIFIED_ECDSA;
    }
    return DLMS_HASH_ALGORITHM_NONE;
}
