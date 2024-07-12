#include "Mk7Mi.hpp"

const char* EDMI_SERVICE_UUID = "ED310001-C889-5D66-AE38-A7A01230635A";

const int CLIENT_PUBLIC = 16;
const int CLIENT_PAIRING = 80;
const int CLIENT_SECURED_103 = 103;
const int CLIENT_SECURED_104 = 104;
const int CLIENT_SECURED_105 = 105;

const char* OBIS_SECURITY_SETUP_103 = "0.0.43.0.6.255";
const char* OBIS_SECURITY_SETUP_104 = "0.0.43.0.7.255"; 
const char* OBIS_SECURITY_SETUP_105 = "0.0.43.0.8.255";

const char* OBIS_FRAME_COUNTER_103 = "0.0.43.1.6.255";
const char* OBIS_FRAME_COUNTER_104 = "0.0.43.1.7.255";
const char* OBIS_FRAME_COUNTER_105 = "0.0.43.1.8.255";

const char* OBIS_CIU_KEYS = "0.128.0.0.6.255";

const char* OBIS_ACCOUNT_STATUS = "0.0.19.0.0.255";
const char* OBIS_TOKEN_GATEWAY = "0.0.19.40.0.255";

const char* OBIS_CLOCK = "0.0.1.0.0.255";
const char* OBIS_ALARMS = "0.128.0.0.4.255";
const char* OBIS_ALARMS_REGISTER1 = "0.0.97.98.0.255";
const char* OBIS_ALARMS_REGISTER2 = "0.128.0.0.4.255";
const char* OBIS_ALARMS_REGISTER3 = "0.0.97.98.2.255";

const char* OBIS_TAMPERS = "0.0.96.15.13.255";
const char* OBIS_METER_SERIAL_NO = "0.0.96.1.0.255";
const char* OBIS_SECURITY_SETUP_PUBLIC = "0.0.43.0.9.255";
const char* OBIS_FRAME_COUNTER_PAIRING = "0.0.43.1.9.255";
const char* OBIS_FIRMWARE_IDENTIFIER = "1.0.0.2.0.255";
const char* OBIS_LOGICAL_DEVICE_NAME = "0.0.42.0.0.255";

const char* OBIS_DELIVERED_ACTIVE_ENERGY = "1.0.1.8.0.255";
const char* OBIS_TOTAL_INSTANTANEOUS_ACTIVE_POWER = "1.0.1.7.0.255";

const char* OBIS_ACTIVE_ENERGY_IMPORT_TARIFF1 = "1.0.1.8.1.255";
const char* OBIS_ACTIVE_ENERGY_EXPORT_TARIFF1 = "1.0.2.8.1.255";
const char* OBIS_REACTIVE_ENERGY_IMPORT_TARIFF1 = "1.0.3.8.1.255";
const char* OBIS_REACTIVE_ENERGY_EXPORT_TARIFF1 = "1.0.4.8.1.255";

const char* OBIS_ACTIVE_ENERGY_IMPORT_TARIFF2 = "1.0.1.8.2.255";
const char* OBIS_ACTIVE_ENERGY_EXPORT_TARIFF2 = "1.0.2.8.2.255";
const char* OBIS_REACTIVE_ENERGY_IMPORT_TARIFF2 = "1.0.3.8.2.255";
const char* OBIS_REACTIVE_ENERGY_EXPORT_TARIFF2 = "1.0.4.8.2.255";

const char* OBIS_H_ACTIVE_ENERGY_IMPORT_TARIFF1 = "1.0.1.8.0.255";
const char* OBIS_H_ACTIVE_ENERGY_EXPORT_TARIFF1 = "1.0.2.8.0.255";
const char* OBIS_H_REACTIVE_ENERGY_IMPORT_TARIFF1 = "1.0.3.8.0.255";
const char* OBIS_H_REACTIVE_ENERGY_EXPORT_TARIFF1 = "1.0.4.8.0.255";

const char* OBIS_H_ACTIVE_ENERGY_IMPORT_TARIFF2 = "1.0.1.8.2.255";
const char* OBIS_H_ACTIVE_ENERGY_EXPORT_TARIFF2 = "1.0.2.8.2.255";
const char* OBIS_H_REACTIVE_ENERGY_IMPORT_TARIFF2 = "1.0.3.8.2.255";
const char* OBIS_H_REACTIVE_ENERGY_EXPORT_TARIFF2 = "1.0.4.8.2.255";

const char* OBIS_D_ACTIVE_ENERGY_IMPORT_TARIFF1 = "1.0.1.8.0.255";
const char* OBIS_D_ACTIVE_ENERGY_EXPORT_TARIFF1 = "1.0.2.8.0.255";
const char* OBIS_D_REACTIVE_ENERGY_IMPORT_TARIFF1 = "1.0.3.8.0.255";
const char* OBIS_D_REACTIVE_ENERGY_EXPORT_TARIFF1 = "1.0.4.8.0.255";

const char* OBIS_D_ACTIVE_ENERGY_IMPORT_TARIFF2 = "1.0.1.8.2.255";
const char* OBIS_D_ACTIVE_ENERGY_EXPORT_TARIFF2 = "1.0.2.8.2.255";
const char* OBIS_D_REACTIVE_ENERGY_IMPORT_TARIFF2 = "1.0.3.8.2.255";
const char* OBIS_D_REACTIVE_ENERGY_EXPORT_TARIFF2 = "1.0.4.8.2.255";

const char* OBIS_TOTAL_CREDIT_PURCHASED = "0.0.199.198.127.0";
const char* OBIS_TOTAL_CREDIT_USED = "0.0.199.198.128.0";

const char* OBIS_AVERAGE_VOLTAGE_A = "1.0.32.24.0.255";
const char* OBIS_AVERAGE_VOLTAGE_B = "1.0.52.24.0.255";
const char* OBIS_AVERAGE_VOLTAGE_C = "1.0.72.24.0.255";
const char* OBIS_AVERAGE_CURRENT_A = "1.0.31.24.0.255";
const char* OBIS_AVERAGE_CURRENT_B = "1.0.51.24.0.255";
const char* OBIS_AVERAGE_CURRENT_C = "1.0.71.24.0.255";
const char* OBIS_AVERAGE_POWER_FACTOR_TOTAL = "1.0.13.5.0.255";
const char* OBIS_AVERAGE_MCU_TEMPERTAURE = "0.0.96.9.0.255" ;
const char* OBIS_INSTANTENOUS_NET_FREQUENCY = "1.0.14.7.0.255";
const char* OBIS_AVERAGE_THD_VOLTAGE_A = "1.0.32.56.124.255";
const char* OBIS_AVERAGE_THD_VOLTAGE_B = "1.0.52.56.124.255";
const char* OBIS_AVERAGE_THD_VOLTAGE_C = "1.0.72.56.124.255";

const char* OBIS_TOKEN_HISTORY_CLOCK = "0.0.19.40.0.255";
const char* OBIS_TOKEN_HISTORY_TOKEN = "0.0.19.40.0.255";
const char* OBIS_TOKEN_HISTORY_VALUE = "0.0.131.0.50.255";

const char* OBIS_PROFILE_TOKEN_HISTORY = "0.0.99.15.0.255";
const char* OBIS_PROFILE_HOURLY_LOAD = "1.0.99.1.0.255";
const char* OBIS_PROFILE_DAILY_LOAD = "1.0.99.2.0.255";
const char* OBIS_PROFILE_POWER_QUALITY = "1.0.99.14.0.255";
