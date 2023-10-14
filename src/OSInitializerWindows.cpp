#include "OSInitializer.hpp"
#include "EntityStatisticsEnergy.hpp"
#include "EntityAlarm.hpp"
#include "EntityMeter.hpp"
#include "EntityTariff.hpp"
#include "EntityToken.hpp"

void CreateTestDataInternalWin();
static float win_get_random_real(float lower, float upper);
static long long win_get_random_integer(long long lower, long long upper);


bool InitializeOSResources()
{
    //CreateTestDataInternalWin();
    return true;
}

void CreateTestDataInternalWin()
{
    meterEntityPtr->truncate();
    alarmEntityPtr->truncate();
    tokenEntityPtr->truncate();
    tariffEntityPtr->truncate();
    statisticsEnergyEntityPtr->truncate();

    Meter m1;

    m1.Status_ = MeterConnectionState::Connected;
    m1.LastSync_ = QDateTime::currentDateTime().toSecsSinceEpoch();
    m1.SerialNo_ = "131003001";
    m1.MeterName_ = "Lab Device 1";
    m1.RSSI_ = -70;
    m1.ModelNo_ = "Mk7Mi";
    m1.SystemTitle_ = "45 44 4D 67 12 18 EF BF";
    m1.BlePIN_ = "000000";
    m1.CIUPIN_ = "00000000";
    m1.Keys103_ = "00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F D0 D1 D2 D3 D4 D5 D6 D7 D8 D9 DA DB DC DD DE DF";
    m1.Keys104_ = "00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F D0 D1 D2 D3 D4 D5 D6 D7 D8 D9 DA DB DC DD DE DF";
    m1.Keys105_ = "00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F D0 D1 D2 D3 D4 D5 D6 D7 D8 D9 DA DB DC DD DE DF";
    m1.BleIdentifier_ = "45 44 4D 67 12 18 EF BF BD 4E";

    meterEntityPtr->createMeter(m1);

    Tariff tr;
    tr.TariffId_ = "131003001";
    tr.EffectiveDate_ = QDateTime::currentDateTime().toSecsSinceEpoch();
    tr.Import1_ =  100.01;
    tr.Export1_ = 110.01;
    tr.Import2_ = 200.01;
    tr.Export2_ = 220.01;
    tr.SerialNo_ = "131003001";
    tr.IsApplied_ = true;

    tariffEntityPtr->createTariff(tr);

    Token tk;
    tk.TokenId_ = "99999999999999999999";
    tk.TokenValue_ = 250.55;
    tk.AppliedDate_ = QDateTime::currentDateTime().toSecsSinceEpoch();
    tk.IsApplied_ = false;
    tk.SerialNo_ = "131003001";
    tk.TokenStatusText_ = "New Token";

    tokenEntityPtr->createToken(tk);

    Alarm a1, a2;
    a1.SerialNo_ = "131003001";
    a1.AlarmId_ = 40;
    a1.AlarmName_ = "Register 2 - Missing Neutral";
    a1.AlarmTimestamp_ = QDateTime::currentDateTime().toSecsSinceEpoch();

    a2.SerialNo_ = "131003001";
    a2.AlarmId_ = 0;
    a2.AlarmName_ = "Register 1 - Clock invalid";
    a2.AlarmTimestamp_ = QDateTime::currentDateTime().toSecsSinceEpoch();

    alarmEntityPtr->createAlarm(a1);
    alarmEntityPtr->createAlarm(a2);

    QDateTime timestamp1;

    timestamp1 = QDateTime::fromString("31/12/2021 00:00:00", "dd/MM/yyyy hh:mm:ss");
    timestamp1.setTimeZone(QTimeZone::utc());

    for(int x = 0; x < 730; x++)
    {
        StatisticsEnergy e1, e2;

        e1.SerialNo_ = "131003001";
        e1.ActiveImportTariff1_ = win_get_random_real(20, 35);
        e1.ReactiveImportTariff1_ = win_get_random_real(5, 15);
        e1.ActiveExportTariff1_ = win_get_random_real(1, 5);
        e1.ReactiveExportTariff1_ = win_get_random_real(5, 10);
        e1.ActiveImportTariff2_ = win_get_random_real(15, 20);
        e1.ReactiveImportTariff2_ = win_get_random_real(1, 5);
        e1.ActiveExportTariff2_ = win_get_random_real(1, 5);
        e1.ReactiveExportTariff2_ = win_get_random_real(1, 5);
        e1.LoadProfileType_ = 1;

        timestamp1 = timestamp1.addDays(1);
        e1.RecordTimestamp_ = timestamp1.toSecsSinceEpoch();
        e1.RowIndex_ = x+1;

        statisticsEnergyEntityPtr->createStatisticsEnergy(e1);
    }

    timestamp1 = QDateTime::fromString("30/05/2023 00:00:00", "dd/MM/yyyy hh:mm:ss");

    for(int x = 0; x < 48; x++)
    {
        StatisticsEnergy e1, e2;

        e1.SerialNo_ = "131003001";
        e1.ActiveImportTariff1_ = win_get_random_real(20, 35);
        e1.ReactiveImportTariff1_ = win_get_random_real(5, 15);
        e1.ActiveExportTariff1_ = win_get_random_real(1, 5);
        e1.ReactiveExportTariff1_ = win_get_random_real(5, 10);
        e1.ActiveImportTariff2_ = win_get_random_real(15, 20);
        e1.ReactiveImportTariff2_ = win_get_random_real(1, 5);
        e1.ActiveExportTariff2_ = win_get_random_real(1, 5);
        e1.ReactiveExportTariff2_ = win_get_random_real(1, 5);
        e1.LoadProfileType_ = 0;

        timestamp1 = timestamp1.addSecs(3600);
        e1.RecordTimestamp_ = timestamp1.toSecsSinceEpoch();
        e1.RowIndex_ = x+1;

        statisticsEnergyEntityPtr->createStatisticsEnergy(e1);
    }
}

float win_get_random_real(float lower, float upper)
{
    return (float)win_get_random_integer((long long)lower, (long long)upper);
}

long long win_get_random_integer(long long lower, long long upper)
{
    int rand_num = (rand() % (upper - lower + 1)) + lower;
    return rand_num;
}
