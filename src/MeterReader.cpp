#include "MeterReader.hpp"
#include "EntityStatisticsEnergy.hpp"
#include "EntityAlarm.hpp"
#include "EntityMeter.hpp"
#include "EntityMeterSettings.hpp"
#include "EntityToken.hpp"
#include "EntityTokenHistory.hpp"
#include "EntityAlarmFilter.hpp"
#include "EntityUserInfo.hpp"
#include "Logger.hpp"

static float get_random_real(float lower, float upper);
static long long get_random_integer(long long lower, long long upper);

MeterReader* csa = nullptr;

MeterReader::MeterReader(int argc, char *argv[])
    :QGuiApplication(argc, argv)
{
    csa = this;
}

MeterReader::~MeterReader()
{
    if(dataInterfacePtr)
    {
        dataInterfacePtr->close();
    }
}

bool MeterReader::InitializeDatabase()
{
    QString logfilepath;
    QString dbffilepath;
    bool is_new_file = false;

    logfilepath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + QString("/log/");
    dbffilepath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + QString("/database/");

    QDir logDir(logfilepath);

    if(!logDir.exists())
    {
        qDebug() << "NO LOG DIR";
        if(!logDir.mkpath(logfilepath))
        {
            qDebug() << "Could not create log directory";
        }
    }

    QDir dbDir(dbffilepath);

    if(!dbDir.exists())
    {
        is_new_file = true;
        if(!dbDir.mkpath(dbffilepath))
        {
            return false;
        }
    }

    QString dbffilename =  dbffilepath + QString("EdmiCsa.sqlite");

    QFileInfo infodbf(dbffilename);

    if(!infodbf.exists())
    {
        is_new_file = true;
    }

    QString appnamestr = applicationName();

    Logger* logger = Logger::GetInstance();
    logger->setLogDirectory(logfilepath.toStdString());
    logger->setModuleName("EdmiCSa");
    logger->setLogFileSize(10);
    logger->startLogging();
    logger->write("Application started", LogInfo);

    if(dataInterfacePtr)
    {
        dataInterfacePtr->setParameters(dbffilename, appnamestr);

        //Check for SQLite DB's existence
        if(!dataInterfacePtr->open())
        {
            //Create if not present
            if(dataInterfacePtr->createFile(dbffilename))
            {
                is_new_file = true;
            }
            else
            {
                return false;
            }
        }

        //Now we can open the database
        if(dataInterfacePtr->open())
        {
            //Check whether the meter table is present or not
            new EntityMeter(dataInterfacePtr->getDatabase(), "meter");
            if(!dataInterfacePtr->contains("meter"))
            {
                meterEntityPtr->createTable();
            }

            //Check whether the meter settings table is present or not
            new EntityMeterSettings(dataInterfacePtr->getDatabase(), "meter_settings");
            if(!dataInterfacePtr->contains("meter_settings"))
            {
                meterSettingsEntityPtr->createTable();
            }

            //Check whether the alarm table is present or not
            new EntityAlarm(dataInterfacePtr->getDatabase(), "alarm");
            if(!dataInterfacePtr->contains("alarm"))
            {
                alarmEntityPtr->createTable();
            }

            //Check whether the alarm filter table is present or not
            new EntityAlarmFilter(dataInterfacePtr->getDatabase(), "alarm_filter");
            if(!dataInterfacePtr->contains("alarm_filter"))
            {
                alarmFilterEntityPtr->createTable();
            }

            //Check whether the token table is present or not
            new EntityToken(dataInterfacePtr->getDatabase(), "token");
            if(!dataInterfacePtr->contains("token"))
            {
                tokenEntityPtr->createTable();
            }

            //Check whether the token history table is present or not
            new EntityTokenHistory(dataInterfacePtr->getDatabase(), "token_history");
            if(!dataInterfacePtr->contains("token_history"))
            {
                tokenHistoryEntityPtr->createTable();
            }

            //Check whether the token table is present or not
            new EntityUserInfo(dataInterfacePtr->getDatabase(), "user_info");
            if(!dataInterfacePtr->contains("user_info"))
            {
                userinfoEntityPtr->createTable();
            }

            //Check whether the energy statistics table is present or not
            new EntityStatisticsEnergy(dataInterfacePtr->getDatabase(), "statistics_energy");
            if(!dataInterfacePtr->contains("statistics_energy"))
            {
                statisticsEnergyEntityPtr->createTable();
            }

            if(is_new_file)
            {
                populateAlarmFilters();
                processEvents();
                populateDemoMasterData();
                processEvents();
                populateMeterObjects();
                processEvents();
                ResetDemoStatistics();
            }
        }
    }

    return true;
}

void MeterReader::ResetDemoStatistics()
{
    statisticsEnergyEntityPtr->truncate();
    populateEnergyProfileData("EDM1DEMO");
    processEvents();
    populateEnergyProfileData("EDM2DEMO");
    processEvents();
}

void MeterReader::populateAlarmFilters()
{
    QString sqlstring;

    sqlstring = "insert into alarm_filter (alarm_id, alarm_name, is_set) values(0, 'Register 1 - Clock invalid', 1)";
    dataInterfacePtr->executeSQL(sqlstring);
    sqlstring = "insert into alarm_filter (alarm_id, alarm_name, is_set) values(1, 'Register 1 - Battery replaced', 1)";
    dataInterfacePtr->executeSQL(sqlstring);
    sqlstring = "insert into alarm_filter (alarm_id, alarm_name, is_set) values(2, 'Register 1 - Reserved for future use', 1)";
    dataInterfacePtr->executeSQL(sqlstring);
    sqlstring = "insert into alarm_filter (alarm_id, alarm_name, is_set) values(3, 'Register 1 - Reserved for future use', 1)";
    dataInterfacePtr->executeSQL(sqlstring);
    sqlstring = "insert into alarm_filter (alarm_id, alarm_name, is_set) values(4, 'Register 1 - Reserved for future use', 1)";
    dataInterfacePtr->executeSQL(sqlstring);
    sqlstring = "insert into alarm_filter (alarm_id, alarm_name, is_set) values(5, 'Register 1 - Reserved for future use', 1)";
    dataInterfacePtr->executeSQL(sqlstring);
    sqlstring = "insert into alarm_filter (alarm_id, alarm_name, is_set) values(6, 'Register 1 - Reserved for future use', 1)";
    dataInterfacePtr->executeSQL(sqlstring);
    sqlstring = "insert into alarm_filter (alarm_id, alarm_name, is_set) values(7, 'Register 1 - Reserved for future use', 1)";
    dataInterfacePtr->executeSQL(sqlstring);
    sqlstring = "insert into alarm_filter (alarm_id, alarm_name, is_set) values(8, 'Register 1 - Program memory error', 1)";
    dataInterfacePtr->executeSQL(sqlstring);
    sqlstring = "insert into alarm_filter (alarm_id, alarm_name, is_set) values(9, 'Register 1 - RAM error', 1)";
    dataInterfacePtr->executeSQL(sqlstring);
    sqlstring = "insert into alarm_filter (alarm_id, alarm_name, is_set) values(10, 'Register 1 - NV memory error', 1)";
    dataInterfacePtr->executeSQL(sqlstring);
    sqlstring = "insert into alarm_filter (alarm_id, alarm_name, is_set) values(11, 'Register 1 - Measurement system error', 1)";
    dataInterfacePtr->executeSQL(sqlstring);
    sqlstring = "insert into alarm_filter (alarm_id, alarm_name, is_set) values(12, 'Register 1 - Watchdog error', 1)";
    dataInterfacePtr->executeSQL(sqlstring);
    sqlstring = "insert into alarm_filter (alarm_id, alarm_name, is_set) values(13, 'Register 1 - Fraud attempt', 1)";
    dataInterfacePtr->executeSQL(sqlstring);
    sqlstring = "insert into alarm_filter (alarm_id, alarm_name, is_set) values(14, 'Register 1 - Reserved for future use', 1)";
    dataInterfacePtr->executeSQL(sqlstring);
    sqlstring = "insert into alarm_filter (alarm_id, alarm_name, is_set) values(15, 'Register 1 - Reserved for future use', 1)";
    dataInterfacePtr->executeSQL(sqlstring);
    sqlstring = "insert into alarm_filter (alarm_id, alarm_name, is_set) values(16, 'Register 1 - M-Bus communication error ch1', 1)";
    dataInterfacePtr->executeSQL(sqlstring);
    sqlstring = "insert into alarm_filter (alarm_id, alarm_name, is_set) values(17, 'Register 1 - M-Bus communication error ch1', 1)";
    dataInterfacePtr->executeSQL(sqlstring);
    sqlstring = "insert into alarm_filter (alarm_id, alarm_name, is_set) values(18, 'Register 1 - M-Bus communication error ch1', 1)";
    dataInterfacePtr->executeSQL(sqlstring);
    sqlstring = "insert into alarm_filter (alarm_id, alarm_name, is_set) values(19, 'Register 1 - M-Bus communication error ch1', 1)";
    dataInterfacePtr->executeSQL(sqlstring);
    sqlstring = "insert into alarm_filter (alarm_id, alarm_name, is_set) values(20, 'Register 1 - M-Bus fraud attempt ch1', 1)";
    dataInterfacePtr->executeSQL(sqlstring);
    sqlstring = "insert into alarm_filter (alarm_id, alarm_name, is_set) values(21, 'Register 1 - M-Bus fraud attempt ch1', 1)";
    dataInterfacePtr->executeSQL(sqlstring);
    sqlstring = "insert into alarm_filter (alarm_id, alarm_name, is_set) values(22, 'Register 1 - M-Bus fraud attempt ch1', 1)";
    dataInterfacePtr->executeSQL(sqlstring);
    sqlstring = "insert into alarm_filter (alarm_id, alarm_name, is_set) values(23, 'Register 1 - M-Bus fraud attempt ch1', 1)";
    dataInterfacePtr->executeSQL(sqlstring);
    sqlstring = "insert into alarm_filter (alarm_id, alarm_name, is_set) values(24, 'Register 1 - Permanent error M-bus ch1', 1)";
    dataInterfacePtr->executeSQL(sqlstring);
    sqlstring = "insert into alarm_filter (alarm_id, alarm_name, is_set) values(25, 'Register 1 - Permanent error M-bus ch1', 1)";
    dataInterfacePtr->executeSQL(sqlstring);
    sqlstring = "insert into alarm_filter (alarm_id, alarm_name, is_set) values(26, 'Register 1 - Permanent error M-bus ch1', 1)";
    dataInterfacePtr->executeSQL(sqlstring);
    sqlstring = "insert into alarm_filter (alarm_id, alarm_name, is_set) values(27, 'Register 1 - Permanent error M-bus ch1', 1)";
    dataInterfacePtr->executeSQL(sqlstring);
    sqlstring = "insert into alarm_filter (alarm_id, alarm_name, is_set) values(28, 'Register 1 - Battery low on M-bus ch1', 1)";
    dataInterfacePtr->executeSQL(sqlstring);
    sqlstring = "insert into alarm_filter (alarm_id, alarm_name, is_set) values(29, 'Register 1 - Battery low on M-bus ch1', 1)";
    dataInterfacePtr->executeSQL(sqlstring);
    sqlstring = "insert into alarm_filter (alarm_id, alarm_name, is_set) values(30, 'Register 1 - Battery low on M-bus ch1', 1)";
    dataInterfacePtr->executeSQL(sqlstring);
    sqlstring = "insert into alarm_filter (alarm_id, alarm_name, is_set) values(31, 'Register 1 - Battery low on M-bus ch1', 1)";
    dataInterfacePtr->executeSQL(sqlstring);

    processEvents();

    sqlstring = "insert into alarm_filter (alarm_id, alarm_name, is_set) values(32, 'Register 2 - Total Power Failure', 1)";
    dataInterfacePtr->executeSQL(sqlstring);
    sqlstring = "insert into alarm_filter (alarm_id, alarm_name, is_set) values(33, 'Register 2 - Power Resume', 1)";
    dataInterfacePtr->executeSQL(sqlstring);
    sqlstring = "insert into alarm_filter (alarm_id, alarm_name, is_set) values(34, 'Register 2 - Voltage Missing Phase L1', 1)";
    dataInterfacePtr->executeSQL(sqlstring);
    sqlstring = "insert into alarm_filter (alarm_id, alarm_name, is_set) values(35, 'Register 2 - Voltage Missing Phase L2', 1)";
    dataInterfacePtr->executeSQL(sqlstring);
    sqlstring = "insert into alarm_filter (alarm_id, alarm_name, is_set) values(36, 'Register 2 - Voltage Missing Phase L3', 1)";
    dataInterfacePtr->executeSQL(sqlstring);
    sqlstring = "insert into alarm_filter (alarm_id, alarm_name, is_set) values(37, 'Register 2 - Voltage Normal Phase L1', 1)";
    dataInterfacePtr->executeSQL(sqlstring);
    sqlstring = "insert into alarm_filter (alarm_id, alarm_name, is_set) values(38, 'Register 2 - Voltage Normal Phase L2', 1)";
    dataInterfacePtr->executeSQL(sqlstring);
    sqlstring = "insert into alarm_filter (alarm_id, alarm_name, is_set) values(39, 'Register 2 - Voltage Normal Phase L3', 1)";
    dataInterfacePtr->executeSQL(sqlstring);
    sqlstring = "insert into alarm_filter (alarm_id, alarm_name, is_set) values(40, 'Register 2 - Missing Neutral', 1)";
    dataInterfacePtr->executeSQL(sqlstring);
    sqlstring = "insert into alarm_filter (alarm_id, alarm_name, is_set) values(41, 'Register 2 - Phase Asymmetry', 1)";
    dataInterfacePtr->executeSQL(sqlstring);
    sqlstring = "insert into alarm_filter (alarm_id, alarm_name, is_set) values(42, 'Register 2 - Current Reversal', 1)";
    dataInterfacePtr->executeSQL(sqlstring);
    sqlstring = "insert into alarm_filter (alarm_id, alarm_name, is_set) values(43, 'Register 2 - Wrong Phase Sequence', 1)";
    dataInterfacePtr->executeSQL(sqlstring);
    sqlstring = "insert into alarm_filter (alarm_id, alarm_name, is_set) values(44, 'Register 2 - Unexpected Consumption', 1)";
    dataInterfacePtr->executeSQL(sqlstring);
    sqlstring = "insert into alarm_filter (alarm_id, alarm_name, is_set) values(45, 'Register 2 - Key Exchanged', 1)";
    dataInterfacePtr->executeSQL(sqlstring);
    sqlstring = "insert into alarm_filter (alarm_id, alarm_name, is_set) values(46, 'Register 2 - Bad Voltage Quality L1', 1)";
    dataInterfacePtr->executeSQL(sqlstring);
    sqlstring = "insert into alarm_filter (alarm_id, alarm_name, is_set) values(47, 'Register 2 - Bad Voltage Quality L2', 1)";
    dataInterfacePtr->executeSQL(sqlstring);
    sqlstring = "insert into alarm_filter (alarm_id, alarm_name, is_set) values(48, 'Register 2 - Bad Voltage Quality L3', 1)";
    dataInterfacePtr->executeSQL(sqlstring);
    sqlstring = "insert into alarm_filter (alarm_id, alarm_name, is_set) values(49, 'Register 2 - External Alert', 1)";
    dataInterfacePtr->executeSQL(sqlstring);
    sqlstring = "insert into alarm_filter (alarm_id, alarm_name, is_set) values(50, 'Register 2 - Local communication attempt', 1)";
    dataInterfacePtr->executeSQL(sqlstring);
    sqlstring = "insert into alarm_filter (alarm_id, alarm_name, is_set) values(51, 'Register 2 - New M-Bus Device Installed Ch1', 1)";
    dataInterfacePtr->executeSQL(sqlstring);
    sqlstring = "insert into alarm_filter (alarm_id, alarm_name, is_set) values(52, 'Register 2 - New M-Bus Device Installed Ch2', 1)";
    dataInterfacePtr->executeSQL(sqlstring);
    sqlstring = "insert into alarm_filter (alarm_id, alarm_name, is_set) values(53, 'Register 2 - New M-Bus Device Installed Ch3', 1)";
    dataInterfacePtr->executeSQL(sqlstring);
    sqlstring = "insert into alarm_filter (alarm_id, alarm_name, is_set) values(54, 'Register 2 - New M-Bus Device Installed Ch4', 1)";
    dataInterfacePtr->executeSQL(sqlstring);
    sqlstring = "insert into alarm_filter (alarm_id, alarm_name, is_set) values(55, 'Register 2 - Reserved for future use', 1)";
    dataInterfacePtr->executeSQL(sqlstring);
    sqlstring = "insert into alarm_filter (alarm_id, alarm_name, is_set) values(56, 'Register 2 - Reserved for future use', 1)";
    dataInterfacePtr->executeSQL(sqlstring);
    sqlstring = "insert into alarm_filter (alarm_id, alarm_name, is_set) values(57, 'Register 2 - Reserved for future use', 1)";
    dataInterfacePtr->executeSQL(sqlstring);
    sqlstring = "insert into alarm_filter (alarm_id, alarm_name, is_set) values(58, 'Register 2 - Reserved for future use', 1)";
    dataInterfacePtr->executeSQL(sqlstring);
    sqlstring = "insert into alarm_filter (alarm_id, alarm_name, is_set) values(59, 'Register 2 - M-Bus valve alarm Ch1', 1)";
    dataInterfacePtr->executeSQL(sqlstring);
    sqlstring = "insert into alarm_filter (alarm_id, alarm_name, is_set) values(60, 'Register 2 - M-Bus valve alarm Ch2', 1)";
    dataInterfacePtr->executeSQL(sqlstring);
    sqlstring = "insert into alarm_filter (alarm_id, alarm_name, is_set) values(61, 'Register 2 - M-Bus valve alarm Ch3', 1)";
    dataInterfacePtr->executeSQL(sqlstring);
    sqlstring = "insert into alarm_filter (alarm_id, alarm_name, is_set) values(62, 'Register 2 - M-Bus valve alarm Ch4', 1)";
    dataInterfacePtr->executeSQL(sqlstring);
    sqlstring = "insert into alarm_filter (alarm_id, alarm_name, is_set) values(63, 'Register 2 - Disconnect/Reconnect Failure', 1)";
    dataInterfacePtr->executeSQL(sqlstring);

    processEvents();

    sqlstring = "insert into alarm_filter (alarm_id, alarm_name, is_set) values(64, 'Register 3 - STS Token Error', 1)";
    dataInterfacePtr->executeSQL(sqlstring);
    sqlstring = "insert into alarm_filter (alarm_id, alarm_name, is_set) values(65, 'Register 3 - CIU Log Updated', 1)";
    dataInterfacePtr->executeSQL(sqlstring);
    sqlstring = "insert into alarm_filter (alarm_id, alarm_name, is_set) values(66, 'Register 3 - Reserved for future use', 1)";
    dataInterfacePtr->executeSQL(sqlstring);
    sqlstring = "insert into alarm_filter (alarm_id, alarm_name, is_set) values(67, 'Register 3 - Reserved for future use', 1)";
    dataInterfacePtr->executeSQL(sqlstring);
    sqlstring = "insert into alarm_filter (alarm_id, alarm_name, is_set) values(68, 'Register 3 - Meter change Prepaid', 1)";
    dataInterfacePtr->executeSQL(sqlstring);
    sqlstring = "insert into alarm_filter (alarm_id, alarm_name, is_set) values(69, 'Register 3 - Meter change Postpaid', 1)";
    dataInterfacePtr->executeSQL(sqlstring);
    sqlstring = "insert into alarm_filter (alarm_id, alarm_name, is_set) values(70, 'Register 3 - Reserved for future use', 1)";
    dataInterfacePtr->executeSQL(sqlstring);
    sqlstring = "insert into alarm_filter (alarm_id, alarm_name, is_set) values(71, 'Register 3 - Reserved for future use', 1)";
    dataInterfacePtr->executeSQL(sqlstring);
    sqlstring = "insert into alarm_filter (alarm_id, alarm_name, is_set) values(72, 'Register 3 - Neutral Current Sensing', 1)";
    dataInterfacePtr->executeSQL(sqlstring);
    sqlstring = "insert into alarm_filter (alarm_id, alarm_name, is_set) values(73, 'Register 3 - Prepayment Low Credit', 1)";
    dataInterfacePtr->executeSQL(sqlstring);
    sqlstring = "insert into alarm_filter (alarm_id, alarm_name, is_set) values(74, 'Register 3 - Prepayment Credit Exhausted', 1)";
    dataInterfacePtr->executeSQL(sqlstring);
    sqlstring = "insert into alarm_filter (alarm_id, alarm_name, is_set) values(75, 'Register 3 - Prepayment Credit Restored', 1)";
    dataInterfacePtr->executeSQL(sqlstring);
    sqlstring = "insert into alarm_filter (alarm_id, alarm_name, is_set) values(76, 'Register 3 - Load Control Threshold Exceed', 1)";
    dataInterfacePtr->executeSQL(sqlstring);
    sqlstring = "insert into alarm_filter (alarm_id, alarm_name, is_set) values(77, 'Register 3 - Load Control Threshold Restored', 1)";
    dataInterfacePtr->executeSQL(sqlstring);
    sqlstring = "insert into alarm_filter (alarm_id, alarm_name, is_set) values(78, 'Register 3 - Relay Connected', 1)";
    dataInterfacePtr->executeSQL(sqlstring);
    sqlstring = "insert into alarm_filter (alarm_id, alarm_name, is_set) values(79, 'Register 3 - Relay Disconnected', 1)";
    dataInterfacePtr->executeSQL(sqlstring);
    sqlstring = "insert into alarm_filter (alarm_id, alarm_name, is_set) values(80, 'Register 3 - Reserved for future use', 1)";
    dataInterfacePtr->executeSQL(sqlstring);
    sqlstring = "insert into alarm_filter (alarm_id, alarm_name, is_set) values(81, 'Register 3 - Reserved for future use', 1)";
    dataInterfacePtr->executeSQL(sqlstring);
    sqlstring = "insert into alarm_filter (alarm_id, alarm_name, is_set) values(82, 'Register 3 - Reserved for future use', 1)";
    dataInterfacePtr->executeSQL(sqlstring);
    sqlstring = "insert into alarm_filter (alarm_id, alarm_name, is_set) values(83, 'Register 3 - Reserved for future use', 1)";
    dataInterfacePtr->executeSQL(sqlstring);
    sqlstring = "insert into alarm_filter (alarm_id, alarm_name, is_set) values(84, 'Register 3 - Reserved for future use', 1)";
    dataInterfacePtr->executeSQL(sqlstring);
    sqlstring = "insert into alarm_filter (alarm_id, alarm_name, is_set) values(85, 'Register 3 - Reserved for future use', 1)";
    dataInterfacePtr->executeSQL(sqlstring);
    sqlstring = "insert into alarm_filter (alarm_id, alarm_name, is_set) values(86, 'Register 3 - Reserved for future use', 1)";
    dataInterfacePtr->executeSQL(sqlstring);
    sqlstring = "insert into alarm_filter (alarm_id, alarm_name, is_set) values(87, 'Register 3 - Reserved for future use', 1)";
    dataInterfacePtr->executeSQL(sqlstring);
    sqlstring = "insert into alarm_filter (alarm_id, alarm_name, is_set) values(88, 'Register 3 - Reserved for future use', 1)";
    dataInterfacePtr->executeSQL(sqlstring);
    sqlstring = "insert into alarm_filter (alarm_id, alarm_name, is_set) values(89, 'Register 3 - Reserved for future use', 1)";
    dataInterfacePtr->executeSQL(sqlstring);
    sqlstring = "insert into alarm_filter (alarm_id, alarm_name, is_set) values(90, 'Register 3 - Reserved for future use', 1)";
    dataInterfacePtr->executeSQL(sqlstring);
    sqlstring = "insert into alarm_filter (alarm_id, alarm_name, is_set) values(91, 'Register 3 - Reserved for future use', 1)";
    dataInterfacePtr->executeSQL(sqlstring);
    sqlstring = "insert into alarm_filter (alarm_id, alarm_name, is_set) values(92, 'Register 3 - Reserved for future use', 1)";
    dataInterfacePtr->executeSQL(sqlstring);
    sqlstring = "insert into alarm_filter (alarm_id, alarm_name, is_set) values(93, 'Register 3 - Reserved for future use', 1)";
    dataInterfacePtr->executeSQL(sqlstring);
    sqlstring = "insert into alarm_filter (alarm_id, alarm_name, is_set) values(94, 'Register 3 - Reserved for future use', 1)";
    dataInterfacePtr->executeSQL(sqlstring);
    sqlstring = "insert into alarm_filter (alarm_id, alarm_name, is_set) values(95, 'Register 3 - Reserved for future use', 1)";
    dataInterfacePtr->executeSQL(sqlstring);

    processEvents();
}

void MeterReader::populateDemoMasterData()
{
    char tsbuffer[32] = {0};

    meterEntityPtr->truncate();

    Meter m1;

    m1.SerialNo_ = "EDM1DEMO";
    m1.MeterName_ = "Demo device 1";
    m1.ModelNo_ = "DEMO";
    m1.SystemTitle_ = "69 68 77 49 68 69 77 79";
    m1.CustomerPIN_ = "00000000";
    m1.Keys103_ = "00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F D0 D1 D2 D3 D4 D5 D6 D7 D8 D9 DA DB DC DD DE DF";
    m1.Keys104_ = "00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F D0 D1 D2 D3 D4 D5 D6 D7 D8 D9 DA DB DC DD DE DF";
    m1.Keys105_ = "00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F D0 D1 D2 D3 D4 D5 D6 D7 D8 D9 DA DB DC DD DE DF";
    m1.LastSync_ = QDateTime::currentDateTime().toSecsSinceEpoch();
    m1.RSSIStr_ = "-70";
    m1.StatusStr_ = "0";
    m1.IsSelected_ = 0;
    m1.LastSyncStr_ = QDateTime::fromSecsSinceEpoch(m1.LastSync_).toString("dd MMM yy hh:mm");

    meterEntityPtr->createMeter(m1);

    Meter m2;

    m2.SerialNo_ = "EDM2DEMO";
    m2.MeterName_ = "Demo device 2";
    m2.ModelNo_ = "DEMO";
    m2.SystemTitle_ = "69 68 77 50 68 69 77 79";
    m2.CustomerPIN_ = "00000000";
    m2.Keys103_ = "00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F D0 D1 D2 D3 D4 D5 D6 D7 D8 D9 DA DB DC DD DE DF";
    m2.Keys104_ = "00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F D0 D1 D2 D3 D4 D5 D6 D7 D8 D9 DA DB DC DD DE DF";
    m2.Keys105_ = "00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F D0 D1 D2 D3 D4 D5 D6 D7 D8 D9 DA DB DC DD DE DF";
    m2.LastSync_ = QDateTime::currentDateTime().toSecsSinceEpoch();
    m2.RSSIStr_ = "-70";
    m2.StatusStr_ = "0";
    m2.IsSelected_ = 0;
    m2.LastSyncStr_ = QDateTime::fromSecsSinceEpoch(m2.LastSync_).toString("dd MMM yy hh:mm");

    meterEntityPtr->createMeter(m2);

    MeterSettings ms1_1;
    ms1_1.SerialNo_ = "EDM1DEMO";
    ms1_1.Key_ = ACCOUNT_CURRENCY;
    ms1_1.Value_ = "kWh";

    MeterSettings ms1_2;
    ms1_2.SerialNo_ = "EDM1DEMO";
    ms1_2.Key_ = ACCOUNT_CURRENCY_SCALAR;
    ms1_2.Value_ = "-2";

    MeterSettings ms1_3;
    ms1_3.SerialNo_ = "EDM1DEMO";
    ms1_3.Key_ = ACTIVE_ENERGY_SCALAR;
    ms1_3.Value_ = "1";

    MeterSettings ms1_4;
    ms1_4.SerialNo_ = "EDM1DEMO";
    ms1_4.Key_ = INSTANTENOUS_POWER_SCALAR;
    ms1_4.Value_ = "1";

    MeterSettings ms1_5;
    ms1_5.SerialNo_ = "EDM1DEMO";
    ms1_5.Key_ = ACTIVE_EXPORT_SCALAR;
    ms1_5.Value_ = "1";

    MeterSettings ms1_6;
    ms1_6.SerialNo_ = "EDM1DEMO";
    ms1_6.Key_ = ACTIVE_IMPORT_SCALAR;
    ms1_6.Value_ = "1";

    MeterSettings ms1_7;
    ms1_7.SerialNo_ = "EDM1DEMO";
    ms1_7.Key_ = REACTIVE_EXPORT_SCALAR;
    ms1_7.Value_ = "1";

    MeterSettings ms1_8;
    ms1_8.SerialNo_ = "EDM1DEMO";
    ms1_8.Key_ = REACTIVE_IMPORT_SCALAR;
    ms1_8.Value_ = "1";

    MeterSettings ms1_9;
    ms1_9.SerialNo_ = "EDM1DEMO";
    ms1_9.Key_ = ACCOUNT_TYPE;
    ms1_9.Value_ = "1";

    meterSettingsEntityPtr->createMeterSettings(ms1_1);
    meterSettingsEntityPtr->createMeterSettings(ms1_2);
    meterSettingsEntityPtr->createMeterSettings(ms1_3);
    meterSettingsEntityPtr->createMeterSettings(ms1_4);
    meterSettingsEntityPtr->createMeterSettings(ms1_5);
    meterSettingsEntityPtr->createMeterSettings(ms1_6);
    meterSettingsEntityPtr->createMeterSettings(ms1_7);
    meterSettingsEntityPtr->createMeterSettings(ms1_8);
    meterSettingsEntityPtr->createMeterSettings(ms1_9);
    processEvents();

    MeterSettings ms2_1;
    ms2_1.SerialNo_ = "EDM2DEMO";
    ms2_1.Key_ = ACCOUNT_CURRENCY;
    ms2_1.Value_ = "INR";

    MeterSettings ms2_2;
    ms2_2.SerialNo_ = "EDM2DEMO";
    ms2_2.Key_ = ACCOUNT_CURRENCY_SCALAR;
    ms2_2.Value_ = "-2";

    MeterSettings ms2_3;
    ms2_3.SerialNo_ = "EDM2DEMO";
    ms2_3.Key_ = ACTIVE_ENERGY_SCALAR;
    ms2_3.Value_ = "1";

    MeterSettings ms2_4;
    ms2_4.SerialNo_ = "EDM2DEMO";
    ms2_4.Key_ = INSTANTENOUS_POWER_SCALAR;
    ms2_4.Value_ = "1";

    MeterSettings ms2_5;
    ms2_5.SerialNo_ = "EDM2DEMO";
    ms2_5.Key_ = ACTIVE_EXPORT_SCALAR;
    ms2_5.Value_ = "1";

    MeterSettings ms2_6;
    ms2_6.SerialNo_ = "EDM2DEMO";
    ms2_6.Key_ = ACTIVE_IMPORT_SCALAR;
    ms2_6.Value_ = "1";

    MeterSettings ms2_7;
    ms2_7.SerialNo_ = "EDM2DEMO";
    ms2_7.Key_ = REACTIVE_EXPORT_SCALAR;
    ms2_7.Value_ = "1";

    MeterSettings ms2_8;
    ms2_8.SerialNo_ = "EDM2DEMO";
    ms2_8.Key_ = REACTIVE_IMPORT_SCALAR;
    ms2_8.Value_ = "1";

    MeterSettings ms2_9;
    ms2_9.SerialNo_ = "EDM2DEMO";
    ms2_9.Key_ = ACCOUNT_TYPE;
    ms2_9.Value_ = "0";

    meterSettingsEntityPtr->createMeterSettings(ms2_1);
    meterSettingsEntityPtr->createMeterSettings(ms2_2);
    meterSettingsEntityPtr->createMeterSettings(ms2_3);
    meterSettingsEntityPtr->createMeterSettings(ms2_4);
    meterSettingsEntityPtr->createMeterSettings(ms2_5);
    meterSettingsEntityPtr->createMeterSettings(ms2_6);
    meterSettingsEntityPtr->createMeterSettings(ms2_7);
    meterSettingsEntityPtr->createMeterSettings(ms2_8);
    meterSettingsEntityPtr->createMeterSettings(ms2_9);
    processEvents();
}

void MeterReader::populateMeterObjects()
{
    tokenEntityPtr->truncate();
    tokenHistoryEntityPtr->truncate();
    alarmEntityPtr->truncate();

    Token tk1_1;
    tk1_1.TokenId_ = "00000000000000000000";
    tk1_1.TokenValue_ = 250.35;
    tk1_1.SerialNo_ = "EDM1DEMO";
    tk1_1.IsTokenNew_= true;
    tk1_1.TokenStatusStr_ = "New Token";
    tokenEntityPtr->createToken(tk1_1);

    Token tk1_2;
    tk1_2.TokenId_ = "11111111111111111111";
    tk1_2.TokenValue_ = 350.45;
    tk1_2.SerialNo_ = "EDM1DEMO";
    tk1_2.IsTokenNew_= false;
    tk1_2.TokenStatusStr_ = "Validation Failed";
    tokenEntityPtr->createToken(tk1_2);

    TokenHistory tkh1_1;
    tkh1_1.TokenId_ = "22222222222222222222";
    tkh1_1.TokenValue_ = 250.35;
    tkh1_1.SerialNo_ = "EDM1DEMO";
    tkh1_1.AppliedDate_ = QDateTime::currentDateTime().toSecsSinceEpoch();
    tkh1_1.RowIndex_ = 1;
    tokenHistoryEntityPtr->createTokenHistory(tkh1_1);

    TokenHistory tkh1_2;
    tkh1_2.TokenId_ = "33333333333333333333";
    tkh1_2.TokenValue_ = 350.45;
    tkh1_2.SerialNo_ = "EDM1DEMO";
    tkh1_2.AppliedDate_ = QDateTime::currentDateTime().toSecsSinceEpoch();
    tkh1_2.RowIndex_ = 2;
    tokenHistoryEntityPtr->createTokenHistory(tkh1_2);

    Token tk2_1;
    tk2_1.TokenId_ = "44444444444444444444";
    tk2_1.TokenValue_ = 250.35;
    tk2_1.SerialNo_ = "EDM2DEMO";
    tk2_1.IsTokenNew_= true;
    tk2_1.TokenStatusStr_ = "New Token";
    tokenEntityPtr->createToken(tk2_1);

    Token tk2_2;
    tk2_2.TokenId_ = "55555555555555555555";
    tk2_2.TokenValue_ = 350.45;
    tk2_2.SerialNo_ = "EDM2DEMO";
    tk2_2.IsTokenNew_= false;
    tk2_2.TokenStatusStr_ = "Validation Failed";
    tokenEntityPtr->createToken(tk2_2);

    TokenHistory tkh2_1;
    tkh2_1.TokenId_ = "66666666666666666666";
    tkh2_1.TokenValue_ = 250.35;
    tkh2_1.SerialNo_ = "EDM2DEMO";
    tkh2_1.AppliedDate_ = QDateTime::currentDateTime().toSecsSinceEpoch();
    tkh2_1.RowIndex_ = 1;
    tokenHistoryEntityPtr->createTokenHistory(tkh2_1);

    TokenHistory tkh2_2;
    tkh2_2.TokenId_ = "77777777777777777777";
    tkh2_2.TokenValue_ = 350.45;
    tkh2_2.SerialNo_ = "EDM2DEMO";
    tkh2_2.AppliedDate_ = QDateTime::currentDateTime().toSecsSinceEpoch();
    tkh2_2.RowIndex_ = 2;
    tokenHistoryEntityPtr->createTokenHistory(tkh2_2);

    Alarm a1_1, a1_2;
    a1_1.SerialNo_ = "EDM1DEMO";
    a1_1.AlarmId_ = 40;
    a1_1.AlarmName_ = "Register 2 - Missing Neutral";
    a1_1.AlarmTimestamp_ = QDateTime::currentDateTime().toSecsSinceEpoch();
    alarmEntityPtr->createAlarm(a1_1);

    a1_2.SerialNo_ = "EDM1DEMO";
    a1_2.AlarmId_ = 0;
    a1_2.AlarmName_ = "Register 1 - Clock invalid";
    a1_2.AlarmTimestamp_ = QDateTime::currentDateTime().toSecsSinceEpoch();
    alarmEntityPtr->createAlarm(a1_2);

    Alarm a2_1, a2_2;
    a2_1.SerialNo_ = "EDM2DEMO";
    a2_1.AlarmId_ = 42;
    a2_1.AlarmName_ = "Register 2 - Current Reversal";
    a2_1.AlarmTimestamp_ = QDateTime::currentDateTime().toSecsSinceEpoch();
    alarmEntityPtr->createAlarm(a2_1);

    a2_2.SerialNo_ = "EDM2DEMO";
    a2_2.AlarmId_ = 63;
    a2_2.AlarmName_ = "Register 2 - Disconnect/Reconnect Failure";
    a2_2.AlarmTimestamp_ = QDateTime::currentDateTime().toSecsSinceEpoch();
    alarmEntityPtr->createAlarm(a2_2);
}

void MeterReader::populateEnergyProfileData(QString serialNo)
{
    QDateTime timestamp;
    QDateTime current_timestamp;
    char tsbuffer[33] = {0};
    int rowidx = 0;

    current_timestamp = QDateTime::currentDateTime();

    // Energy

    // Last two year excluding current
    memset(tsbuffer, 0, sizeof(tsbuffer));
    snprintf(tsbuffer, 32, "01-01-%04d 00:00:00", QDateTime::currentDateTime().date().year()-2);
    timestamp = QDateTime::fromString(tsbuffer, "dd-MM-yyyy hh:mm:ss");
    rowidx = 1;
    for(int x = 0; x < 24; x++)
    {
        StatisticsEnergy e1;

        e1.SerialNo_ = serialNo;
        e1.ActiveImportTariff1_ = get_random_real(20, 35);
        e1.ReactiveImportTariff1_ = get_random_real(5, 15);
        e1.ActiveExportTariff1_ = get_random_real(3, 9);
        e1.ReactiveExportTariff1_ = get_random_real(5, 10);
        e1.ActiveImportTariff2_ = get_random_real(15, 20);
        e1.ReactiveImportTariff2_ = get_random_real(3, 5);
        e1.ActiveExportTariff2_ = get_random_real(1, 8);
        e1.ReactiveExportTariff2_ = get_random_real(4, 7);
        e1.LoadProfileType_ = 1;
        e1.RecordTimestamp_ = timestamp.toSecsSinceEpoch();
        e1.RowIndex_ = rowidx;
        rowidx++;

        statisticsEnergyEntityPtr->createStatisticsEnergy(e1);
        timestamp = timestamp.addMonths(1);

        processEvents();
    }

    // First two months of the current year
    memset(tsbuffer, 0, sizeof(tsbuffer));
    snprintf(tsbuffer, 32, "01-01-%04d 00:00:00", QDateTime::currentDateTime().date().year());
    timestamp = QDateTime::fromString(tsbuffer, "dd-MM-yyyy hh:mm:ss");

    for(int x = 0; x < 62; x++)
    {
        StatisticsEnergy e1;

        e1.SerialNo_ = serialNo;
        e1.ActiveImportTariff1_ = get_random_real(20, 35);
        e1.ReactiveImportTariff1_ = get_random_real(5, 15);
        e1.ActiveExportTariff1_ = get_random_real(3, 9);
        e1.ReactiveExportTariff1_ = get_random_real(5, 10);
        e1.ActiveImportTariff2_ = get_random_real(15, 20);
        e1.ReactiveImportTariff2_ = get_random_real(3, 5);
        e1.ActiveExportTariff2_ = get_random_real(1, 8);
        e1.ReactiveExportTariff2_ = get_random_real(4, 7);
        e1.LoadProfileType_ = 1;
        e1.RecordTimestamp_ = timestamp.toSecsSinceEpoch();
        e1.RowIndex_ = rowidx;
        rowidx++;

        statisticsEnergyEntityPtr->createStatisticsEnergy(e1);
        timestamp = timestamp.addDays(1);

        processEvents();
    }

    // First two days of the third month of current year
    memset(tsbuffer, 0, sizeof(tsbuffer));
    snprintf(tsbuffer, 32, "01-03-%04d 00:00:00", QDateTime::currentDateTime().date().year());
    timestamp = QDateTime::fromString(tsbuffer, "dd-MM-yyyy hh:mm:ss");

    for(int x = 0; x < 96 ; x++)
    {
        StatisticsEnergy e1;

        e1.SerialNo_ = serialNo;
        e1.ActiveImportTariff1_ = get_random_real(20, 35);
        e1.ReactiveImportTariff1_ = get_random_real(5, 15);
        e1.ActiveExportTariff1_ = get_random_real(3, 9);
        e1.ReactiveExportTariff1_ = get_random_real(5, 10);
        e1.ActiveImportTariff2_ = get_random_real(15, 20);
        e1.ReactiveImportTariff2_ = get_random_real(4, 10);
        e1.ActiveExportTariff2_ = get_random_real(3, 7);
        e1.ReactiveExportTariff2_ = get_random_real(3, 8);
        e1.LoadProfileType_ = 0;
        e1.RecordTimestamp_ = timestamp.toSecsSinceEpoch();
        e1.RowIndex_ = rowidx;
        rowidx++;

        statisticsEnergyEntityPtr->createStatisticsEnergy(e1);
        timestamp = timestamp.addSecs(3600);

        processEvents();
    }
}

float get_random_real(float lower, float upper)
{
    return (float)get_random_integer((long long)lower, (long long)upper);
}

long long get_random_integer(long long lower, long long upper)
{
    int rand_num = (rand() % (upper - lower + 1)) + lower;
    return rand_num;
}
