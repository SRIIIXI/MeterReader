#ifndef MK7MI
#define MK7MI

#include <stdint.h>
#include <map>
#include <iostream>
#include <vector>

extern const char* EDMI_SERVICE_UUID;

extern const int CLIENT_PUBLIC;
extern const int CLIENT_PAIRING;
extern const int CLIENT_SECURED_103;
extern const int CLIENT_SECURED_104;
extern const int CLIENT_SECURED_105;

extern const char* OBIS_SECURITY_SETUP_103;
extern const char* OBIS_SECURITY_SETUP_104;
extern const char* OBIS_SECURITY_SETUP_105;

extern const char* OBIS_FRAME_COUNTER_PAIRING;
extern const char* OBIS_FRAME_COUNTER_103;
extern const char* OBIS_FRAME_COUNTER_104;
extern const char* OBIS_FRAME_COUNTER_105;

extern const char* OBIS_CIU_KEYS;

extern const char* OBIS_ACCOUNT_STATUS;
extern const char* OBIS_TOKEN_GATEWAY;

extern const char* OBIS_ALARMS;
extern const char* OBIS_ALARMS_REGISTER1;
extern const char* OBIS_ALARMS_REGISTER2;
extern const char* OBIS_ALARMS_REGISTER3;

extern const char* OBIS_TAMPERS;
extern const char* OBIS_METER_SERIAL_NO;
extern const char* OBIS_SECURITY_SETUP_PUBLIC;
extern const char* OBIS_FIRMWARE_IDENTIFIER;
extern const char* OBIS_LOGICAL_DEVICE_NAME;

extern const char* OBIS_DELIVERED_ACTIVE_ENERGY;
extern const char* OBIS_TOTAL_INSTANTANEOUS_ACTIVE_POWER;

extern const char* OBIS_ACTIVE_ENERGY_IMPORT_TARIFF1;
extern const char* OBIS_ACTIVE_ENERGY_EXPORT_TARIFF1;
extern const char* OBIS_REACTIVE_ENERGY_IMPORT_TARIFF1;
extern const char* OBIS_REACTIVE_ENERGY_EXPORT_TARIFF1;

extern const char* OBIS_ACTIVE_ENERGY_IMPORT_TARIFF2;
extern const char* OBIS_ACTIVE_ENERGY_EXPORT_TARIFF2;
extern const char* OBIS_REACTIVE_ENERGY_IMPORT_TARIFF2;
extern const char* OBIS_REACTIVE_ENERGY_EXPORT_TARIFF2;

extern const char* OBIS_TOTAL_CREDIT_PURCHASED;
extern const char* OBIS_TOTAL_CREDIT_USED;

extern const char* OBIS_AVERAGE_VOLTAGE_A;
extern const char* OBIS_AVERAGE_VOLTAGE_B;
extern const char* OBIS_AVERAGE_VOLTAGE_C;
extern const char* OBIS_AVERAGE_CURRENT_A;
extern const char* OBIS_AVERAGE_CURRENT_B;
extern const char* OBIS_AVERAGE_CURRENT_C;
extern const char* OBIS_AVERAGE_POWER_FACTOR_TOTAL;
extern const char* OBIS_AVERAGE_MCU_TEMPERTAURE;
extern const char* OBIS_INSTANTENOUS_NET_FREQUENCY;
extern const char* OBIS_AVERAGE_THD_VOLTAGE_A;
extern const char* OBIS_AVERAGE_THD_VOLTAGE_B;
extern const char* OBIS_AVERAGE_THD_VOLTAGE_C;

#endif
