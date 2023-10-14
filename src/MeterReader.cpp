#include "EdmiCsa.hpp"
#include "EntityStatisticsEnergy.hpp"
#include "EntityStatisticsPowerQuality.hpp"
#include "EntityAlarm.hpp"
#include "EntityMeter.hpp"
#include "EntityMeterSettings.hpp"
#include "EntityTariff.hpp"
#include "EntityToken.hpp"
#include "EntityAlarmFilter.hpp"
#include "EntityUserInfo.hpp"
#include "Logger.hpp"
#include "OSInitializer.hpp"

static float get_random_real(float lower, float upper);
static long long get_random_integer(long long lower, long long upper);

EdmiCsa::EdmiCsa(int argc, char *argv[])
    :QGuiApplication(argc, argv)
{
}

EdmiCsa::~EdmiCsa()
{
    if(dataInterfacePtr)
    {
        dataInterfacePtr->close();
    }
}

bool EdmiCsa::StartApplication()
{
    QString logfilepath;
    QString dbffilepath;
    bool is_new_file = false;

    logfilepath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + QString("/log/");
    dbffilepath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + QString("/database/");

    qDebug() << logfilepath;
    qDebug() << dbffilepath;

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
        qDebug() << "NO DB DIR";
        if(!dbDir.mkpath(dbffilepath))
        {
            qDebug() << "Could not create DB directory";
        }
    }

    QString dbffilename =  dbffilepath + QString("EdmiCsa.sqlite");

    QFileInfo infodbf(dbffilename);

    if(infodbf.exists())
    {
        qDebug() << dbffilename << " exists";
    }
    else
    {
        qDebug() << dbffilename << " not found";
        is_new_file = true;
    }

    Logger* logger = Logger::GetInstance();
    logger->setLogDirectory(logfilepath.toStdString());
    logger->setModuleName(QCoreApplication::applicationName().toStdString());
    logger->setLogFileSize(10);
    logger->startLogging();
    logger->write("Application started", LogInfo);

    QString appnamestr = applicationName();

    if(dataInterfacePtr)
    {
        dataInterfacePtr->setParameters(dbffilename, appnamestr);

        //Check for SQLite DB's existence
        if(!dataInterfacePtr->open())
        {
             qDebug() << "COULD NOT FIND DB";
            //Create if not present
            if(dataInterfacePtr->createFile(dbffilename))
            {
                qDebug() << "CREATED DB";
                is_new_file = true;
            }
            else
            {
                qDebug() << "CANNOT CREATE DB";
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

            //Check whether the power quality statistics table is present or not
            new EntityStatisticsPowerQuality(dataInterfacePtr->getDatabase(), "statistics_power_quality");
            if(!dataInterfacePtr->contains("statistics_power_quality"))
            {
                statisticsPowerQualityEntityPtr->createTable();
            }

            //Check whether the tariff table is present or not
            new EntityTariff(dataInterfacePtr->getDatabase(), "tariff");
            if(!dataInterfacePtr->contains("tariff"))
            {
                tariffEntityPtr->createTable();
            }

            if(is_new_file)
            {
                populateAlarmFilters();
                populateDemoData();
            }
        }
        else
        {
            qDebug() << "COULD NOT OPEN DB";
        }
    }

    bool init_b = InitializeOSResources();

    return init_b;
}

void EdmiCsa::populateAlarmFilters()
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
}

void EdmiCsa::populateDemoData()
{
    char tsbuffer[32] = {0};

    meterEntityPtr->truncate();
    alarmEntityPtr->truncate();
    tokenEntityPtr->truncate();
    tariffEntityPtr->truncate();
    statisticsEnergyEntityPtr->truncate();
    statisticsPowerQualityEntityPtr->truncate();

    Meter m1;

    m1.Status_ = MeterConnectionState::Connected;
    m1.LastSync_ = QDateTime::currentDateTime().toSecsSinceEpoch();
    m1.SerialNo_ = "EDMIDEMO";
    m1.MeterName_ = "Demo device";
    m1.RSSI_ = -70;
    m1.ModelNo_ = "DEMO";
    m1.SystemTitle_ = "45 44 4D 49 44 45 4D 4F";
    m1.BlePIN_ = "000000";
    m1.CIUPIN_ = "00000000";
    m1.Keys103_ = "00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F D0 D1 D2 D3 D4 D5 D6 D7 D8 D9 DA DB DC DD DE DF";
    m1.Keys104_ = "00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F D0 D1 D2 D3 D4 D5 D6 D7 D8 D9 DA DB DC DD DE DF";
    m1.Keys105_ = "00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F D0 D1 D2 D3 D4 D5 D6 D7 D8 D9 DA DB DC DD DE DF";
    m1.BleIdentifier_ = "45 44 4D 49 44 45 4D 4F";

    meterEntityPtr->createMeter(m1);

    Tariff tr;
    tr.TariffId_ = "EDMIDEMO";
    tr.EffectiveDate_ = QDateTime::currentDateTime().toSecsSinceEpoch();
    tr.Import1_ =  100.01;
    tr.Export1_ = 110.01;
    tr.Import2_ = 200.01;
    tr.Export2_ = 220.01;
    tr.SerialNo_ = "EDMIDEMO";
    tr.IsApplied_ = true;

    tariffEntityPtr->createTariff(tr);

    Token tk1;
    tk1.TokenId_ = "99999999999999999999";
    tk1.TokenValue_ = 250.55;
    tk1.AppliedDate_ = QDateTime::currentDateTime().toSecsSinceEpoch();
    tk1.IsApplied_ = false;
    tk1.SerialNo_ = "EDMIDEMO";
    tk1.TokenStatusText_ = "New Token";
    tk1.RowIndex_ = -1;

    tokenEntityPtr->createToken(tk1);

    Token tk2;
    tk2.TokenId_ = "88888888888888888888";
    tk2.TokenValue_ = 250.55;
    tk2.AppliedDate_ = QDateTime::currentDateTime().toSecsSinceEpoch();
    tk2.IsApplied_ = true;
    tk2.SerialNo_ = "EDMIDEMO";
    tk2.TokenStatusText_ = "Execution OK";
    tk2.RowIndex_ = -1;

    tokenEntityPtr->createToken(tk2);

    Alarm a1, a2;
    a1.SerialNo_ = "EDMIDEMO";
    a1.AlarmId_ = 40;
    a1.AlarmName_ = "Register 2 - Missing Neutral";
    a1.AlarmTimestamp_ = QDateTime::currentDateTime().toSecsSinceEpoch();

    a2.SerialNo_ = "EDMIDEMO";
    a2.AlarmId_ = 0;
    a2.AlarmName_ = "Register 1 - Clock invalid";
    a2.AlarmTimestamp_ = QDateTime::currentDateTime().toSecsSinceEpoch();

    alarmEntityPtr->createAlarm(a1);
    alarmEntityPtr->createAlarm(a2);

    MeterSettings ms1;
    ms1.SerialNo_ = "EDMIDEMO";
    ms1.Key_ = ACCOUNT_CURRENCY;
    ms1.Value_ = "kWh";

    MeterSettings ms2;
    ms2.SerialNo_ = "EDMIDEMO";
    ms2.Key_ = ACCOUNT_CURRENCY_SCALAR;
    ms2.Value_ = "-2";

    MeterSettings ms3;
    ms3.SerialNo_ = "EDMIDEMO";
    ms3.Key_ = ACTIVE_ENERGY_SCALAR;
    ms3.Value_ = "1";

    MeterSettings ms4;
    ms4.SerialNo_ = "EDMIDEMO";
    ms4.Key_ = INSTANTENOUS_POWER_SCALAR;
    ms4.Value_ = "1";

    MeterSettings ms5;
    ms5.SerialNo_ = "EDMIDEMO";
    ms5.Key_ = ACTIVE_EXPORT_SCALAR;
    ms5.Value_ = "1";

    MeterSettings ms6;
    ms6.SerialNo_ = "EDMIDEMO";
    ms6.Key_ = ACTIVE_IMPORT_SCALAR;
    ms6.Value_ = "1";

    MeterSettings ms7;
    ms7.SerialNo_ = "EDMIDEMO";
    ms7.Key_ = REACTIVE_EXPORT_SCALAR;
    ms7.Value_ = "1";

    MeterSettings ms8;
    ms8.SerialNo_ = "EDMIDEMO";
    ms8.Key_ = REACTIVE_IMPORT_SCALAR;
    ms8.Value_ = "1";

    meterSettingsEntityPtr->createMeterSettings(ms1);
    meterSettingsEntityPtr->createMeterSettings(ms2);
    meterSettingsEntityPtr->createMeterSettings(ms3);
    meterSettingsEntityPtr->createMeterSettings(ms4);
    meterSettingsEntityPtr->createMeterSettings(ms5);
    meterSettingsEntityPtr->createMeterSettings(ms6);
    meterSettingsEntityPtr->createMeterSettings(ms7);
    meterSettingsEntityPtr->createMeterSettings(ms8);

    QDateTime timestamp;

    // Energy
    memset(tsbuffer, 0, sizeof(tsbuffer));
    sprintf(tsbuffer, "01/%02d/%04d 00:00:00", QDateTime::currentDateTime().date().month(), QDateTime::currentDateTime().date().year() - 2);
    timestamp = QDateTime::fromString(tsbuffer, "dd/MM/yyyy hh:mm:ss");

    for(int x = 0; x < 730; x++)
    {
        StatisticsEnergy e1, e2;

        e1.SerialNo_ = "EDMIDEMO";
        e1.ActiveImportTariff1_ = get_random_real(20, 35);
        e1.ReactiveImportTariff1_ = get_random_real(5, 15);
        e1.ActiveExportTariff1_ = get_random_real(1, 5);
        e1.ReactiveExportTariff1_ = get_random_real(5, 10);
        e1.ActiveImportTariff2_ = get_random_real(15, 20);
        e1.ReactiveImportTariff2_ = get_random_real(1, 5);
        e1.ActiveExportTariff2_ = get_random_real(1, 5);
        e1.ReactiveExportTariff2_ = get_random_real(1, 5);
        e1.LoadProfileType_ = 1;
        e1.RecordTimestamp_ = timestamp.toSecsSinceEpoch();
        e1.RowIndex_ = x+1;

        statisticsEnergyEntityPtr->createStatisticsEnergy(e1);
        timestamp = timestamp.addDays(1);
    }

    memset(tsbuffer, 0, sizeof(tsbuffer));
    sprintf(tsbuffer, "%02d/%02d/%04d 00:00:00", QDateTime::currentDateTime().date().day(), QDateTime::currentDateTime().date().month()-2, QDateTime::currentDateTime().date().year());
    timestamp = QDateTime::fromString(tsbuffer, "dd/MM/yyyy hh:mm:ss");

    for(int x = 0; x < 24*60 ; x++)
    {
        StatisticsEnergy e1, e2;

        e1.SerialNo_ = "EDMIDEMO";
        e1.ActiveImportTariff1_ = get_random_real(20, 35);
        e1.ReactiveImportTariff1_ = get_random_real(5, 15);
        e1.ActiveExportTariff1_ = get_random_real(1, 5);
        e1.ReactiveExportTariff1_ = get_random_real(5, 10);
        e1.ActiveImportTariff2_ = get_random_real(15, 20);
        e1.ReactiveImportTariff2_ = get_random_real(1, 5);
        e1.ActiveExportTariff2_ = get_random_real(1, 5);
        e1.ReactiveExportTariff2_ = get_random_real(1, 5);
        e1.LoadProfileType_ = 0;
        e1.RecordTimestamp_ = timestamp.toSecsSinceEpoch();
        e1.RowIndex_ = x+1;

        statisticsEnergyEntityPtr->createStatisticsEnergy(e1);
        timestamp = timestamp.addSecs(3600);
    }

    // Power Quality

    memset(tsbuffer, 0, sizeof(tsbuffer));
    sprintf(tsbuffer, "01/01/%04d 00:00:00", QDateTime::currentDateTime().date().year());
    timestamp = QDateTime::fromString(tsbuffer, "dd/MM/yyyy hh:mm:ss");

    int rowidx = 0;
    for(int x = 0; x < 360; x++)
    {
        StatisticsPowerQuality p1, p2;

        p1.SerialNo_ = "EDMIDEMO";
        p1.AverageVoltageA_ = get_random_real(215, 260);
        p1.AverageVoltageB_ = 0;
        p1.AverageVoltageC_ = 0;
        p1.AverageCurrentA_ = get_random_real(11, 55);
        p1.AverageCurrentB_ = 0;
        p1.AverageCurrentC_ = 0;
        p1.AveragePowerFactorTotal_ = get_random_real(0.85, 0.95);
        p1.AverageMCUTemperature_ = get_random_real(28, 32);
        p1.InstantenousNetFrequency_ = get_random_real(55, 60);
        p1.AverageTHDVoltageA_ = get_random_real(1, 5);
        p1.AverageTHDVoltageB_ = 0;
        p1.AverageTHDVoltageC_ = get_random_real(120, 240);
        p1.RecordTimestamp_ = timestamp.toSecsSinceEpoch();
        rowidx++;
        p1.RowIndex_ = rowidx;

        statisticsPowerQualityEntityPtr->createStatisticsPowerQuality(p1);
        timestamp = timestamp.addDays(1);
    }

    for(int x = 0; x < 24*5; x++)
    {
        StatisticsPowerQuality p1, p2;

        p1.SerialNo_ = "EDMIDEMO";
        p1.AverageVoltageA_ = get_random_real(215, 260);
        p1.AverageVoltageB_ = 0;
        p1.AverageVoltageC_ = 0;
        p1.AverageCurrentA_ = get_random_real(11, 55);
        p1.AverageCurrentB_ = 0;
        p1.AverageCurrentC_ = 0;
        p1.AveragePowerFactorTotal_ = get_random_real(0.85, 0.95);
        p1.AverageMCUTemperature_ = get_random_real(28, 32);
        p1.InstantenousNetFrequency_ = get_random_real(55, 60);
        p1.AverageTHDVoltageA_ = get_random_real(1, 5);
        p1.AverageTHDVoltageB_ = 0;
        p1.AverageTHDVoltageC_ = get_random_real(120, 240);
        p1.RecordTimestamp_ = timestamp.toSecsSinceEpoch();
        rowidx++;
        p1.RowIndex_ = rowidx;

        statisticsPowerQualityEntityPtr->createStatisticsPowerQuality(p1);
        timestamp = timestamp.addSecs(3600);
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
