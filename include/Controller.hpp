#ifndef CONTROLLER_
#define CONTROLLER_

#include <QObject>
#include <QGuiApplication>
#include <QDateTime>
#include <QString>
#include <QStringList>
#include <QList>

#include <QUuid>
#include <QPalette>

#include "BluetoothInterface.hpp"
#include "EntityStatisticsEnergy.hpp"
#include "EntityMeter.hpp"
#include "EntityAlarm.hpp"
#include "EntityToken.hpp"
#include "EntityTokenHistory.hpp"
#include "EntityAlarmFilter.hpp"
#include "EntityUserInfo.hpp"
#include "ApplicationTheme.hpp"
#include "IWorkFlow.hpp"
#include "WorkFlowAddMeter.hpp"
#include "WorkFlowConnectMeter.hpp"
#include "WorkFlowDisconnectMeter.hpp"
#include "WorkFlowKeepAlive.hpp"
#include "WorkFlowTokenTransfer.hpp"
#include "CameraInterface.hpp"

typedef enum ScanMode
{
    PairingCode=0,
    STSToken=1
}ScanMode;

class Controller : public QObject
{
    Q_OBJECT

public:
    explicit Controller(QGuiApplication* appPtr, QObject *parent = nullptr);

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

    Q_INVOKABLE void invokeCancel();
    Q_INVOKABLE void invokeClearToken();
    Q_INVOKABLE QString invokeCheckQRImage(QImage img);
    Q_INVOKABLE void invokeMeterAdditionClose();
    Q_INVOKABLE void invokeExit();

    Q_INVOKABLE void invokeUpdateAlarmFilterState(int index, int state);
    Q_INVOKABLE void invokeReloadAlarmFilters();
    Q_INVOKABLE void invokeApplyAlarmFilters();

    Q_INVOKABLE void invokeEnergyStatistics(int visualizationmode, int parameter_type, int duration_type, QString tstr1, QString tstr2);

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
    Q_INVOKABLE void invokeLoadHelpStrings(uint32_t index);

    Q_PROPERTY (QString AppVersion READ getAppVersion NOTIFY AppVersionChanged)
    QString getAppVersion();

    Q_PROPERTY (bool IsDarkTheme READ getIsDarkTheme WRITE setIsDarkTheme NOTIFY IsDarkThemeChanged)
    bool getIsDarkTheme();
    void setIsDarkTheme(bool isdarktheme);

    Q_PROPERTY (bool IsDemoMode READ getIsDemoMode WRITE setIsDemoMode NOTIFY IsDemoModeChanged)
    bool getIsDemoMode();
    void setIsDemoMode(bool isdemomode);

    Q_PROPERTY (QString ChartTitle READ getChartTitle WRITE setChartTitle NOTIFY ChartTitleChanged)
    void setChartTitle(QString str);
    QString getChartTitle();

    //Energy
    Q_PROPERTY (QList<BarData> EnergyBarData READ getEnergyBarData NOTIFY EnergyBarDataChanged)
    QList<BarData> getEnergyBarData();
    Q_PROPERTY (QList<LineData> EnergyLineData READ getEnergyLineData NOTIFY EnergyLineDataChanged)
    QList<LineData> getEnergyLineData();

    Q_PROPERTY (QString XLabelEnergy READ getXLabelEnergy WRITE setXLabelEnergy NOTIFY XLabelEnergyChanged)
    void setXLabelEnergy(QString str);
    QString getXLabelEnergy();

    Q_PROPERTY (QString YLabelEnergy READ getYLabelEnergy WRITE setYLabelEnergy NOTIFY YLabelEnergyChanged)
    void setYLabelEnergy(QString str);
    QString getYLabelEnergy();

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


    Q_PROPERTY (unsigned long long DeliveredEnergy READ getDeliveredEnergy NOTIFY DeliveredEnergyChanged)
    unsigned long long getDeliveredEnergy();

    Q_PROPERTY (unsigned long long InstantaneousPower READ getInstantaneousPower NOTIFY InstantaneousPowerChanged)
    unsigned long long getInstantaneousPower();

    Q_PROPERTY (unsigned long long UsedCredit READ getUsedCredit NOTIFY UsedCreditChanged)
    unsigned long long getUsedCredit();

    Q_PROPERTY (unsigned long long GrossCredit READ getGrossCredit NOTIFY GrossCreditChanged)
    unsigned long long getGrossCredit();

    Q_PROPERTY (unsigned long long RemainingCredit READ getRemainingCredit NOTIFY RemainingCreditChanged)
    unsigned long long getRemainingCredit();

    Q_PROPERTY (QString DeliveredEnergyStr READ getDeliveredEnergyStr NOTIFY DeliveredEnergyStrChanged)
    QString getDeliveredEnergyStr();

    Q_PROPERTY (QString InstantaneousPowerStr READ getInstantaneousPowerStr NOTIFY InstantaneousPowerStrChanged)
    QString getInstantaneousPowerStr();

    Q_PROPERTY (QString UsedCreditStr READ getUsedCreditStr NOTIFY UsedCreditStrChanged)
    QString getUsedCreditStr();

    Q_PROPERTY (QString GrossCreditStr READ getGrossCreditStr NOTIFY GrossCreditStrChanged)
    QString getGrossCreditStr();

    Q_PROPERTY (QString RemainingCreditStr READ getRemainingCreditStr NOTIFY RemainingCreditStrChanged)
    QString getRemainingCreditStr();

    Q_PROPERTY (float RemainingCreditPercent READ getRemainingCreditPercent NOTIFY RemainingCreditPercentChanged)
    float getRemainingCreditPercent();

    Q_PROPERTY (bool IsAppConnected READ getIsAppConnected WRITE setIsAppConnected NOTIFY IsAppConnectedChanged)
    void setIsAppConnected(bool isappconnected);
    bool getIsAppConnected();

    Q_PROPERTY (QString CurrentMeterSerialNo READ getCurrentMeterSerialNo WRITE setCurrentMeterSerialNo NOTIFY CurrentMeterSerialNoChanged)
    void setCurrentMeterSerialNo(QString imetersno);
    QString getCurrentMeterSerialNo();

    Q_PROPERTY (UserInfo CurrentUser READ getCurrentUser WRITE setCurrentUser NOTIFY CurrentUserChanged)
    void setCurrentUser(UserInfo iuser);
    UserInfo getCurrentUser();

    Q_PROPERTY (QList<QString> HelpStrings READ getHelpStrings NOTIFY HelpStringsChanged)
    QList<QString> getHelpStrings();

    Q_PROPERTY (QList<Alarm> ActiveAlarms READ getActiveAlarms WRITE setActiveAlarms NOTIFY ActiveAlarmsChanged)
    void setActiveAlarms(QList<Alarm> activealarms);
    QList<Alarm> getActiveAlarms();

    Q_PROPERTY (QList<AlarmFilter> AlarmFilter READ getAlarmFilter WRITE setAlarmFilter NOTIFY AlarmFilterChanged)
    void setAlarmFilter(QList<AlarmFilter> alarmsfiler);
    QList<AlarmFilter> getAlarmFilter();

    Q_PROPERTY (QList<Meter> MeterList READ getMeterList WRITE setMeterList NOTIFY MeterListChanged)
    void setMeterList(QList<Meter> meterlist);
    QList<Meter> getMeterList();

    Q_PROPERTY (QList<Token> TokenList READ getTokenList WRITE setTokenList NOTIFY TokenListChanged)
    void setTokenList(QList<Token> tokenlist);
    QList<Token> getTokenList();

    Q_PROPERTY (QList<TokenHistory> TokenHistoryList READ getTokenHistoryList WRITE setTokenHistoryList NOTIFY TokenHistoryListChanged)
    void setTokenHistoryList(QList<TokenHistory> tokenHlist);
    QList<TokenHistory> getTokenHistoryList();

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

     Q_PROPERTY (QString AppSyncTime READ getAppSyncTime NOTIFY AppSyncTimeChanged)
    QString getAppSyncTime();

    Q_PROPERTY (QString CreditCurrency READ getCreditCurrency NOTIFY CreditCurrencyChanged)
    QString getCreditCurrency();

    Q_PROPERTY (bool ShowIndicator READ getShowIndicator NOTIFY ShowIndicatorChanged)
    bool getShowIndicator();

    Q_PROPERTY (CsaTheme Theme READ getTheme NOTIFY ThemeChanged)
    CsaTheme getTheme();

    Q_PROPERTY (QString TraceString READ getTraceString NOTIFY TraceStringChanged)
    QString getTraceString();

    Q_PROPERTY (bool IsTraceEnabled READ getIsTraceEnabled WRITE setIsTraceEnabled NOTIFY IsTraceEnabledChanged)
    void setIsTraceEnabled(bool istraceenabled);
    bool getIsTraceEnabled();

    Q_PROPERTY (bool IsDumpRxTxEnabled READ getIsDumpRxTxEnabled WRITE setIsDumpRxTxEnabled NOTIFY IsDumpRxTxEnabledChanged)
    void setIsDumpRxTxEnabled(bool isdumprxtxenabled);
    bool getIsDumpRxTxEnabled();

    Q_PROPERTY (bool IsCameraAccessEnabled READ getIsCameraAccessEnabled NOTIFY IsCameraAccessEnabledChanged)
    bool getIsCameraAccessEnabled() { return (bool)ENABLE_CAMERA_ACCESS; };

    void Initialize();
    void SignalInitializationComplete();
    void Trace(QString str);
    void ClearTrace();

signals:

    void ThemeChanged();
    void ShowIndicatorChanged();
    void CreditCurrencyChanged();
    void HelpStringsChanged();
    void AppSyncTimeChanged();
    void IsDarkThemeChanged();
    void IsDemoModeChanged();
    void AppVersionChanged();
    void IsTraceEnabledChanged();
    void TraceStringChanged();
    void IsDumpRxTxEnabledChanged();
    void IsCameraAccessEnabledChanged();

    //Property change signals
    void IsAppConnectedChanged();

    void DeliveredEnergyChanged();
    void GrossCreditChanged();
    void InstantaneousPowerChanged();
    void UsedCreditChanged();
    void RemainingCreditChanged();
    void ScanModeChanged();
    void QRTextChanged();
    void DeliveredEnergyStrChanged();
    void InstantaneousPowerStrChanged();
    void UsedCreditStrChanged();
    void GrossCreditStrChanged();
    void RemainingCreditStrChanged();
    void RemainingCreditPercentChanged();
    void CurrentUserChanged();
    void CurrentMeterSerialNoChanged();
    void ActiveAlarmsChanged();
    void AlarmFilterChanged();
    void MeterListChanged();
    void TokenListChanged();
    void TokenHistoryListChanged();
    void IsFirstRunChanged();
    void PaymentTypeChanged();

    //Add Meter events
    void meterPairingCodeVerification(bool fl, QString errorstr);
    void meterFound(bool fl, QString errorstr);
    void meterConnected(bool fl, QString errorstr);
    void meterConfigured(bool fl, QString errorstr);

    //UI events and operations states
    void authenticationSuccessfull();
    void authenticationFailed();
    void tokenAction();
    void tokenAdded();
    void meterAdded();
    void meterAction(bool fl);
    void alarmSettingsChanged();
    void alarmAction();
    void operationCancelled();
    void pageAction(int ipage);
    void userLoginSucceeded();
    void userLoginFailed(QString reason);
    void energyStatisticsRefreshed();

    //Data related
    void ChartTitleChanged();

    //Energy
    void EnergyBarDataChanged();
    void EnergyLineDataChanged();
    void XLabelEnergyChanged();
    void YLabelEnergyChanged();
    void DailyMaxTouChanged();
    void HourlyAverageTouChanged();
    void TotalTouChanged();
    void MinTimestampLP1Changed();
    void MaxTimestampLP1Changed();
    void MinTimestampLP2Changed();
    void MaxTimestampLP2Changed();

    //Specifc errors
    void alarmError(QString str);
    void tokenTransferState(bool status, QString str);
    void tokenError(QString str);
    void meterMessage(bool status, QString str);
    void powerQualityError(QString str);
    void energyError(QString str);

    //User management errors
    void userNameError();
    void passCodeMismatch();

    //Trace/progress
    void workflowProgress(uint32_t str);
    void initializationComplete();

    //UI Indicators
    void progressIndicatorsOff();
    void progressIndicatorsOn();
    void networkIndicatorsOff();
    void networkIndicatorsOn();
    void updateDashboard();

private slots:
    void OnDeviceDiscovered(const QString &address, const QString &name, const QString &rssi, const QMultiHash<quint16, QByteArray> &mdata);
    void OnScanComplete();

    void OnConnectFailed();
    void OnDeviceConnected(const QString &name);
    void OnDeviceDisconnected(const QString &name);
    void OnDataReceived(const QByteArray& data);
    void OnDataSent(const QByteArray& data);
    void OnCommunicationTimeout();

    void OnSendKeepAlive();
    void OnReset();

    void OnAddMeterCompleted(QString &serialNo, bool status);
    void OnConnectCompleted(QString &serialNo, bool status);
    void OnDisconnectCompleted(QString &serialNo, bool status);
    void OnKeepAliveCompleted(bool status);
    void OnTokenTransferCompleted(QString &serialNo, QString &tokenNo, QString &statusMessage, bool status);

private:
    void LoadConfigurationData();
    void LoadAllData();
    void LoadMeterSettings(QString &serialNo);
    bool IsTokenHistoryExisting(const QString &tokenstr);
    bool HasActiveMeter();

    void loadEnergyStatistics(int visualizationmode, int parameter_type, int duration_type, uint32_t ts1, uint32_t ts2);
    void setAllMetersDisconnected();
    void loadMeter(QString &serialNo);
    void updateDataRanges(QString &serialNo);
    void selectMeter(QString &serialNo, bool connected = true);
    void UpdateCurrentMeterTimestamp();
    void handleCommunicationDrop(const QString &errstr);
    void retryScan();
    void retryConnect();
    void generateDashboardData();

    QGuiApplication* app;
    QTimer keepAliveTimer;
    QTimer connectionResetTimer;

    WorkFlowAddMeter addMeterWF;
    WorkFlowConnectMeter connectMeterWF;
    WorkFlowDisconnectMeter disconnectMeterWF;
    WorkFlowKeepAlive keepAliveWF;
    WorkFlowTokenTransfer tokenTransferWF;
    ActiveWorkflow activeWF_;
    //Data
    //------------------------------------------

    //Object lists
    QList<Alarm> activeAlarms_;
    QList<AlarmFilter> alarmFilter_;
    QList<Meter> meterList_;
    QList<Token> tokenList_;
    QList<TokenHistory> tokenHistoryList_;
    QList<UserInfo> userList_;
    QList<StatisticsEnergy> energyList_;
    QList<QString> helpStringList_;
    QString helpImageUrl_;

    // Dashboard display
    unsigned long deliveredEnergy_;
    unsigned long instantaneousPower_;
    unsigned long remainingCredit_;
    unsigned long usedCredit_;
    unsigned long grossCredit_;
    int deliveredEnergy_scalar;
    int instantaneousPower_scalar;
    int paymentType_;
    QString creditCurrency_;

    QString deliveredEnergyStr_;
    QString instantaneousPowerStr_;
    QString remainingCreditStr_;
    QString usedCreditStr_;
    QString grossCreditStr_;
    float remainingCreditPercent_;

    // Load profile registers
    int activeImport_scalar;
    int activeExport_scalar;
    int reactiveImport_scalar;
    int reactiveExport_scalar;

    QString chartTitle_;

    //Energy
    QList<BarData> barDataEnergy_;
    QList<LineData> lineDataEnergy_;
    QString xLabelEnergy_;
    QString yLabelEnergy_;
    float dailyMaxTou_;
    float hourlyAverageTou_;
    float totalTou_;
    QString minTimestampLP1_;
    QString maxTimestampLP1_;
    QString minTimestampLP2_;
    QString maxTimestampLP2_;

    //Globals / Miscellanous
    int global_scalar_;
    bool is_first_run_;
    ScanMode scanmode_;
    QString qrtext_;
    QString lastPage_;
    QString appVerion_;
    bool isAppConnected_;
    bool isMultiPhase_;

    bool isDarkTheme_;
    bool isDemoMode_;
    bool isTraceEnabled_;
    bool isDumpRxTxEnabled_;
    CsaTheme theme_;
    QString traceString_;

    QString peripheralServiceIdString_;
    QString appSyncTime_;

    //Fixed objects
    UserInfo currentUser_;
    QString currentMeterSerialNo_;

    friend class WorkFlowAddMeter;
    friend class WorkFlowConnectMeter;
    friend class WorkFlowDisconnectMeter;
    friend class WorkFlowKeepAlive;
    friend class WorkFlowTokenTransfer;

    QDateTime sessionStart_;
    QDateTime sessionEnd_;
};

extern Controller* controllerInstance;

#endif
