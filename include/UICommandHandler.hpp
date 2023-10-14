#ifndef UI_COMMAND_HANDLER_H
#define UI_COMMAND_HANDLER_H

#include <QObject>
#include <QGuiApplication>
#include <QDateTime>
#include <QString>
#include <QStringList>
#include <QList>
#include <QDebug>
#include <QThread>
#include <string>
#include <strstream>
#include <sstream>
#include <QMap>
#include <QQueue>
#include <QPointF>
#include <QBluetoothDeviceDiscoveryAgent>
#include <QBluetoothDeviceInfo>
#include <QBluetoothUuid>
#include <QUuid>
#include <QPalette>
#include <ctype.h>

#include "Help.hpp"
#include "BleCentral.hpp"
#include "EntityStatisticsEnergy.hpp"
#include "EntityStatisticsPowerQuality.hpp"
#include "EntityTariff.hpp"
#include "EntityMeter.hpp"
#include "EntityMeterSettings.hpp"
#include "EntityAlarm.hpp"
#include "EntityToken.hpp"
#include "EntityAlarmFilter.hpp"
#include "EntityUserInfo.hpp"
#include "IWorkFlow.hpp"
#include "WorkFlowAddMeter.hpp"
#include "WorkFlowConnectMeter.hpp"
#include "WorkFlowKeepAlive.hpp"
#include "WorkFlowTokenTransfer.hpp"

typedef enum ScanMode
{
    AddMeter=0,
    AddToken=1
}ScanMode;

class UICommandHandler : public QObject
{
    Q_OBJECT
public:
    explicit UICommandHandler(QGuiApplication* appPtr, QObject *parent = nullptr);

    Q_INVOKABLE void invokeThemeUpdate();
    Q_INVOKABLE void invokeChangePage(int ipage);
    Q_INVOKABLE void invokeAddMeter(QString token);
    Q_INVOKABLE void invokeConnectMeter(QString serialNo);
    Q_INVOKABLE void invokeDisconnectMeter(QString serialNo);
    Q_INVOKABLE void invokeRemoveMeter(QString serialNo);
    Q_INVOKABLE void invokeSelectMeter(QString serialNo);

    Q_INVOKABLE void invokeAddToken(QString tokenId);
    Q_INVOKABLE void invokeApplyToken(QString tokenId);
    Q_INVOKABLE void invokeRemoveToken(QString tokenId);

    Q_INVOKABLE void invokeAddTariff(QString import1, QString export1, QString import2, QString export2, QString datestr);
    Q_INVOKABLE void invokeRemoveTariff(QString tariffId);

    Q_INVOKABLE void invokeCancel();
    Q_INVOKABLE void invokeClearToken();
    Q_INVOKABLE QString invokeCheckQRImage(QImage img);
    Q_INVOKABLE void invokeMeterAdditionClose();
    Q_INVOKABLE void invokeExit();

    Q_INVOKABLE void invokeUpdateAlarmFilterState(int index, int state);
    Q_INVOKABLE void invokeApplyAlarmFilters();

    Q_INVOKABLE void invokeFetchEnergyStatistics(int visualizationmode, int parameter_type, int duration_type, QString tstr1, QString tstr2);

    Q_INVOKABLE void invokeLogin(QString pin);
    Q_INVOKABLE void invokeChangePassword(QString consumerid, QString pass);

    Q_INVOKABLE void invokeCreateUserStage1(QString name, QString userid);
    Q_INVOKABLE void invokeCreateUserStage2(QString passcode);
    Q_INVOKABLE void invokeCreateUserStage3(QString passcode);

    Q_INVOKABLE void invokeResetPasscodeStage1(QString userid);
    Q_INVOKABLE void invokeResetPasscodeStage2(QString passcode);
    Q_INVOKABLE void invokeResetPasscodeStage3(QString passcode);

    Q_INVOKABLE void invokeSaveCurrentPage(QString page);
    Q_INVOKABLE QString invokeRetreiveLastPage();

    Q_PROPERTY (QString AppVersion READ getAppVersion NOTIFY AppVersionChanged)
    QString getAppVersion();

    Q_PROPERTY (bool IsMeterActive READ getIsMeterActive NOTIFY IsMeterActiveChanged)
    bool getIsMeterActive();

    Q_PROPERTY (bool IsDarkTheme READ getIsDarkTheme WRITE setIsDarkTheme NOTIFY IsDarkThemeChanged)
    bool getIsDarkTheme();
    void setIsDarkTheme(bool isdarktheme);

    Q_PROPERTY (bool IsDemoMode READ getIsDemoMode WRITE setIsDemoMode NOTIFY IsDemoModeChanged)
    bool getIsDemoMode();
    void setIsDemoMode(bool isdemomode);

    Q_PROPERTY (QStringList Days READ getDays NOTIFY DaysChanged)
    QStringList getDays();
    Q_PROPERTY (QStringList Months READ getMonths NOTIFY MonthsChanged)
    QStringList getMonths();
    Q_PROPERTY (QStringList Years READ getYears NOTIFY YearsChanged)
    QStringList getYears();

    //Energy
    Q_PROPERTY (QList<BarData> EnergyBarData READ getEnergyBarData NOTIFY EnergyBarDataChanged)
    QList<BarData> getEnergyBarData();
    Q_PROPERTY (QList<LineData> EnergyLineData READ getEnergyLineData NOTIFY EnergyLineDataChanged)
    QList<LineData> getEnergyLineData();

    Q_PROPERTY (QString XLabelBarEnergy READ getXLabelBarEnergy WRITE setXLabelBarEnergy NOTIFY XLabelBarEnergyChanged)
    void setXLabelBarEnergy(QString str);
    QString getXLabelBarEnergy();

    Q_PROPERTY (QString YLabelBarEnergy READ getYLabelBarEnergy WRITE setYLabelBarEnergy NOTIFY YLabelBarEnergyChanged)
    void setYLabelBarEnergy(QString str);
    QString getYLabelBarEnergy();

    Q_PROPERTY (QString XLabelLineEnergy READ getXLabelLineEnergy WRITE setXLabelLineEnergy NOTIFY XLabelLineEnergyChanged)
    void setXLabelLineEnergy(QString str);
    QString getXLabelLineEnergy();

    Q_PROPERTY (QString YLabelLineEnergy READ getYLabelLineEnergy WRITE setYLabelLineEnergy NOTIFY YLabelLineEnergyChanged)
    void setYLabelLineEnergy(QString str);
    QString getYLabelLineEnergy();

    Q_PROPERTY (QString ChartTitleEnergy READ getChartTitleEnergy WRITE setChartTitleEnergy NOTIFY ChartTitleEnergyChanged)
    void setChartTitleEnergy(QString str);
    QString getChartTitleEnergy();

    Q_PROPERTY (float DailyMaxTou READ geDailyMaxTou NOTIFY DailyMaxTouChanged)
    float geDailyMaxTou();
    Q_PROPERTY (float HourlyAverageTou READ getHourlyAverageTou NOTIFY HourlyAverageTouChanged)
    float getHourlyAverageTou();
    Q_PROPERTY (float TotalTou READ getTotalTou NOTIFY TotalTouChanged)
    float getTotalTou();

    Q_PROPERTY (QString MinTimestampLP1 READ getMinTimestampLP1 NOTIFY MinTimestampLP1Changed)
    QString getMinTimestampLP1();
    Q_PROPERTY (QString MaxTimestampLP1 READ getMaxTimestampLP1 NOTIFY MaxTimestampLP1Changed)
    QString getMaxTimestampLP1();
    Q_PROPERTY (QString MinTimestampLP2 READ getMinTimestampLP2 NOTIFY MinTimestampLP2Changed)
    QString getMinTimestampLP2();
    Q_PROPERTY (QString MaxTimestampLP2 READ getMaxTimestampLP2 NOTIFY MaxTimestampLP2Changed)
    QString getMaxTimestampLP2();
    //Energy

    //Powerquality
    Q_PROPERTY (QList<BarData> PowerQualityBarData READ getPowerQualityBarData NOTIFY PowerQualityBarDataChanged)
    QList<BarData> getPowerQualityBarData();
    Q_PROPERTY (QList<LineData> PowerQualityLineData READ getPowerQualityLineData NOTIFY PowerQualityLineDataChanged)
    QList<LineData> getPowerQualityLineData();

    Q_PROPERTY (QString XLabelBarPowerQuality READ getXLabelBarPowerQuality WRITE setXLabelBarPowerQuality NOTIFY XLabelBarPowerQualityChanged)
    void setXLabelBarPowerQuality(QString str);
    QString getXLabelBarPowerQuality();

    Q_PROPERTY (QString YLabelBarPowerQuality READ getYLabelBarPowerQuality WRITE setYLabelBarPowerQuality NOTIFY YLabelBarPowerQualityChanged)
    void setYLabelBarPowerQuality(QString str);
    QString getYLabelBarPowerQuality();

    Q_PROPERTY (QString XLabelLinePowerQuality READ getXLabelLinePowerQuality WRITE setXLabelLinePowerQuality NOTIFY XLabelLinePowerQualityChanged)
    void setXLabelLinePowerQuality(QString str);
    QString getXLabelLinePowerQuality();

    Q_PROPERTY (QString YLabelLinePowerQuality READ getYLabelLinePowerQuality WRITE setYLabelLinePowerQuality NOTIFY YLabelLinePowerQualityChanged)
    void setYLabelLinePowerQuality(QString str);
    QString getYLabelLinePowerQuality();

    Q_PROPERTY (QString ChartTitlePowerQuality READ getChartTitlePowerQuality WRITE setChartTitlePowerQuality NOTIFY ChartTitlePowerQualityChanged)
    void setChartTitlePowerQuality(QString str);
    QString getChartTitlePowerQuality();

    Q_PROPERTY (float PQMax1 READ getPQMax1 NOTIFY PQMax1Changed)
    float getPQMax1();
    Q_PROPERTY (float PQMax2 READ getPQMax2 NOTIFY PQMax2Changed)
    float getPQMax2();
    Q_PROPERTY (float PQMin1 READ getPQMin1 NOTIFY PQMin1Changed)
    float getPQMin1();
    Q_PROPERTY (float PQMin2 READ getPQMin2 NOTIFY PQMin2Changed)
    float getPQMin2();

    Q_PROPERTY (QString MaxTimestampPQ READ getMaxTimestampPQ NOTIFY MaxTimestampPQChanged)
    QString getMaxTimestampPQ();
    Q_PROPERTY (QString MinTimestampPQ READ getMinTimestampPQ NOTIFY MinTimestampPQChanged)
    QString getMinTimestampPQ();

    Q_PROPERTY (QString PQ1Unit READ getPQ1Unit NOTIFY PQ1UnitChanged)
    QString getPQ1Unit();
    Q_PROPERTY (QString PQ2Unit READ getPQ2Unit NOTIFY PQ2UnitChanged)
    QString getPQ2Unit();

    //Powerquality

    Q_PROPERTY (float DeliveredEnergy READ getDeliveredEnergy NOTIFY DeliveredEnergyChanged)
    float getDeliveredEnergy();

    Q_PROPERTY (float InstantaneousPower READ getInstantaneousPower NOTIFY InstantaneousPowerChanged)
    float getInstantaneousPower();

    Q_PROPERTY (float AvailableCredit READ getAvailableCredit NOTIFY AvailableCreditChanged)
    float getAvailableCredit();

    Q_PROPERTY (float GrossCredit READ getGrossCredit NOTIFY GrossCreditChanged)
    float getGrossCredit();

    Q_PROPERTY (bool IsAppConnected READ getIsAppConnected WRITE setIsAppConnected NOTIFY IsAppConnectedChanged)
    void setIsAppConnected(bool isappconnected);
    bool getIsAppConnected();

    Q_PROPERTY (Meter CurrentMeter READ getCurrentMeter WRITE setCurrentMeter NOTIFY CurrentMeterChanged)
    void setCurrentMeter(Meter imeter);
    Meter getCurrentMeter();

    Q_PROPERTY (UserInfo CurrentUser READ getCurrentUser WRITE setCurrentUser NOTIFY CurrentUserChanged)
    void setCurrentUser(UserInfo iuser);
    UserInfo getCurrentUser();

    Q_PROPERTY (QList<Help> HelpStrings READ getHelpStrings NOTIFY HelpStringsChanged)
    QList<Help> getHelpStrings();

    Q_PROPERTY (QList<Alarm> ActiveAlarms READ getActiveAlarms WRITE setActiveAlarms NOTIFY ActiveAlarmsChanged)
    void setActiveAlarms(QList<Alarm> activealarms);
    QList<Alarm> getActiveAlarms();

    Q_PROPERTY (QList<AlarmFilter> AlarmFilter READ getAlarmFilter WRITE setAlarmFilter NOTIFY AlarmFilterChanged)
    void setAlarmFilter(QList<AlarmFilter> alarmsfiler);
    QList<AlarmFilter> getAlarmFilter();

    Q_PROPERTY (QList<Meter> MeterList READ getMeterList WRITE setMeterList NOTIFY MeterListChanged)
    void setMeterList(QList<Meter> meterlist);
    QList<Meter> getMeterList();

    Q_PROPERTY (QList<QString> MeterNameList READ getMeterNameList WRITE setMeterNameList NOTIFY MeterNameListChanged)
    void setMeterNameList(QList<QString> meterNamelist);
    QList<QString> getMeterNameList();

    Q_PROPERTY (QList<Token> TokenList READ getTokenList WRITE setTokenList NOTIFY TokenListChanged)
    void setTokenList(QList<Token> tokenlist);
    QList<Token> getTokenList();

    Q_PROPERTY (QList<Tariff> TariffList READ getTariffList WRITE setTariffList NOTIFY TariffListChanged)
    void setTariffList(QList<Tariff> tarifflist);
    QList<Tariff> getTariffList();

    Q_PROPERTY (int PaymentType READ getPaymentType NOTIFY PaymentTypeChanged)
    int getPaymentType();

    Q_PROPERTY (int ScanMode READ getScanMode WRITE setScanMode NOTIFY ScanModeChanged)
    void setScanMode(int scanmode);
    int getScanMode();

    Q_PROPERTY (QString QRText READ getQRText WRITE setQRText NOTIFY QRTextChanged)
    void setQRText(QString iqrtext);
    QString getQRText();

    Q_PROPERTY (bool IsFirstRun READ getIsFirstRun WRITE setIsFirstRun NOTIFY IsFirstRunChanged)
    void setIsFirstRun(bool isfirstrun);
    bool getIsFirstRun();

    Q_PROPERTY (float CurrentTariff READ getCurrentTariff NOTIFY CurrentTariffChanged)
    float getCurrentTariff();

    Q_PROPERTY (QString AppSyncTime READ getAppSyncTime NOTIFY AppSyncTimeChanged)
    QString getAppSyncTime();

    Q_PROPERTY (QString CreditCurrency READ getCreditCurrency NOTIFY CreditCurrencyChanged)
    QString getCreditCurrency();

    void Initialize();
    void AppLogInformation(QString str);
    void AppLogError(QString str);

signals:

    void CreditCurrencyChanged();
    void HelpStringsChanged();
    void AppSyncTimeChanged();
    //Property change signals
    void IsMeterActiveChanged();
    void IsAppConnectedChanged();
    void IsDarkThemeChanged();
    void IsDemoModeChanged();
    void AppVersionChanged();

    void DeliveredEnergyChanged();
    void GrossCreditChanged();
    void InstantaneousPowerChanged();
    void AvailableCreditChanged();
    void ScanModeChanged();
    void QRTextChanged();

    void CurrentUserChanged();
    void CurrentMeterChanged();
    void ActiveAlarmsChanged();
    void AlarmFilterChanged();
    void MeterListChanged();
    void TokenListChanged();
    void TariffListChanged();
    void IsFirstRunChanged();
    void PaymentTypeChanged();
    void MeterNameListChanged();

    void updateSyncTime();
    //Add Meter events
    void meterPairingCodeVerification(bool fl, QString errorstr);
    void meterFound(bool fl);
    void meterAuthenticated(bool fl);
    void meterCheckConfiguration(bool fl);

    //UI events and operations states
    void authenticationSuccessfull();
    void authenticationFailed();
    void logMessage(QString str);
    void logError(QString str);
    void tokenAction();
    void tokenAdded();
    void tariffAdded();
    void tariffAction();
    void meterAdded();
    void meterAction();
    void alarmSettingsChanged();
    void alarmAction(bool fl);
    void operationCancelled();
    void pageAction(int ipage);
    void userLoginSucceeded();
    void userLoginFailed(QString reason);
    void dashboardRefreshed();
    void energyStatisticsRefreshed();
    void powerQualityStatisticsRefreshed();

    // Date related
    void DaysChanged();
    void MonthsChanged();
    void YearsChanged();

    //Data related
    //Energy
    void EnergyBarDataChanged();
    void EnergyLineDataChanged();
    void XLabelBarEnergyChanged();
    void YLabelBarEnergyChanged();
    void XLabelLineEnergyChanged();
    void YLabelLineEnergyChanged();
    void ChartTitleEnergyChanged();
    void DailyMaxTouChanged();
    void HourlyAverageTouChanged();
    void TotalTouChanged();
    void MinTimestampLP1Changed();
    void MaxTimestampLP1Changed();
    void MinTimestampLP2Changed();
    void MaxTimestampLP2Changed();

    //Power Quality
    void PowerQualityBarDataChanged();
    void PowerQualityLineDataChanged();
    void XLabelBarPowerQualityChanged();
    void YLabelBarPowerQualityChanged();
    void XLabelLinePowerQualityChanged();
    void YLabelLinePowerQualityChanged();
    void ChartTitlePowerQualityChanged();
    void MinTimestampPQChanged();
    void MaxTimestampPQChanged();
    void PQMax1Changed();
    void PQMax2Changed();
    void PQMin1Changed();
    void PQMin2Changed();
    void PQ1UnitChanged();
    void PQ2UnitChanged();

    //Tariff
    void CurrentTariffChanged();

    //Specifc errors
    void alarmError(QString str);
    void dashboardError(QString str);
    void tokenTransferState(QString str, bool status);
    void tokenError(QString str);
    void tariffError(QString str);
    void meterError(QString str);
    void powerQualityError(QString str);

    //User management errors
    void userNameError();
    void passCodeMismatch();

    //Trace/progress
    void connectionProgress(QString str);
    void meterAdditionProgress(QString str);
    void tokenTransferProgress(QString str);

private slots:
    void OnDeviceFound(const QBluetoothDeviceInfo &device);
    void OnScanComplete();

    void OnDeviceConnected(const QString &name);
    void OnDeviceDisconnected();
    void OnDataReceived(const QByteArray& data);
    void OnDataSent(uint32_t len);
    void OnCompleted(QList<QVariant> completionParamters, WorkflowName wfn, bool status);
    void OnSendKeepAlive();
    void OnScanTimeout();
    void OnConnectTimeout();
    void OnCommunicationTimeout();

private:
    void loadEnergyStatistics(int visualizationmode, int parameter_type, int duration_type, uint32_t ts1, uint32_t ts2);
    bool IsFloatString(const QString& str);
    void UpdateCurrentMeterTimestamp();
    float getTariff(unsigned long tstamp);
    void GetAssociationView();

    QGuiApplication* app;
    QBluetoothDeviceDiscoveryAgent scanner;
    BleCentral central;
    QTimer keepAliveTimer;
    QTimer scanTimer;
    QTimer connectTimer;

    IWorkFlow* workflow;

    WorkFlowAddMeter* addMeterWF;
    WorkFlowConnectMeter* connectMeterWF;
    WorkFlowTokenTransfer* tokenTransferWF;
    WorkFlowKeepAlive* keepAliveWF;

    //Data
    //------------------------------------------

    //Static
    QStringList days_;
    QStringList months_;
    QStringList years_;

    //Object lists
    QList<Alarm> activeAlarms_;
    QList<AlarmFilter> alarmFilter_;
    QList<Meter> meterList_;
    QList<Token> tokenList_;
    QList<Tariff> tariffList_;
    QList<UserInfo> userList_;
    QList<StatisticsEnergy> energyList_;
    QList<QString> meterNames_;
    QList<Help> helpStringList_;

    // Dashboard display
    float deliveredEnergy_;
    float instantaneousPower_;
    int deliveredEnergy_scalar;
    int instantaneousPower_scalar;
    float availableCredit_;
    float grossCredit_;
    int paymentType_;
    QString creditCurrency_;

    // Load profile registers
    int activeImport_scalar;
    int activeExport_scalar;
    int reactiveImport_scalar;
    int reactiveExport_scalar;


    //Energy
    QList<BarData> barDataEnergy_;
    QList<LineData> lineDataEnergy_;
    QString xLabelBarEnergy_;
    QString yLabelBarEnergy_;
    QString xLabelLineEnergy_;
    QString yLabelLineEnergy_;
    QString chartTitleEnergy_;
    float dailyMaxTou_;
    float hourlyAverageTou_;
    float totalTou_;
    QString minTimestampLP1_;
    QString maxTimestampLP1_;
    QString minTimestampLP2_;
    QString maxTimestampLP2_;

    //Power Quality
    QList<BarData> barDataPowerQuality_;
    QList<LineData> lineDataPowerQuality_;
    QString xLabelBarPowerQuality_;
    QString yLabelBarPowerQuality_;
    QString xLabelLinePowerQuality_;
    QString yLabelLinePowerQuality_;
    QString chartTitlePowerQuality_;
    QString maxTimestampPQ_;
    QString minTimestampPQ_;
    QString pq1Unit_;
    QString pq2Unit_;
    float pqMax1_;
    float pqMax2_;
    float pqMin1_;
    float pqMin2_;

    //Globals / Miscellanous
    int global_scalar_;
    bool is_first_run_;
    ScanMode scanmode_;
    QString qrtext_;
    float currentTariff_;
    QString lastPage_;
    QString appVerion_;
    bool isAppConnected_;
    bool isMultiPhase_;

    bool isDarkTheme_;
    bool isDemoMode_;

    QString peripheralServiceIdString_;
    QString appSyncTime_;
};

extern UICommandHandler* globalApplicationData;

#endif
