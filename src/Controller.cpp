#include "Controller.hpp"
#include "Helpers.h"
#include "DlmsClient.hpp"
#include "DataInterface.hpp"
#include "ZXingQtReader.h"
#include "Help.hpp"
#include "EntityMeterSettings.hpp"
#include "Version.hpp"
using namespace ZXingQt;

Controller::Controller(QGuiApplication *appPtr, QObject *parent)
    : QObject(parent)
{
    controllerInstance = this;
    app = appPtr;

    appVerion_ = csa_version;
    appVerion_ += ", Qt Version ";
    appVerion_ += qVersion();

    traceString_ = "";

    global_scalar_ = -2;
    deliveredEnergy_scalar = 1;
    instantaneousPower_scalar = 1;

    isAppConnected_ = false;

    deliveredEnergy_ = 0;
    instantaneousPower_ = 0;
    usedCredit_ = 100;
    grossCredit_ =  200;
    remainingCredit_ = 100;
    paymentType_ = 0;
    creditCurrency_ = "kWh";

    isMultiPhase_ = false;
    paymentType_ = 1;
    dailyMaxTou_ = 0;
    hourlyAverageTou_ = 0;
    totalTou_ = 0;

    lastPage_ = "";
    isDarkTheme_ = true;
    isDemoMode_ = true;
    isTraceEnabled_ = false;
    isDumpRxTxEnabled_ = false;
    peripheralServiceIdString_ = "ED310001-C889-5D66-AE38-A7A01230635A";

    qrtext_ = "";
    activeWF_ = Idle;

    connect(btInterfacePtr, &BluetoothInterface::DeviceDiscovered, this, &Controller::OnDeviceDiscovered);
    connect(btInterfacePtr, &BluetoothInterface::ScanComplete, this, &Controller::OnScanComplete);

    connect(btInterfacePtr, &BluetoothInterface::DeviceConnected, this, &Controller::OnDeviceConnected);
    connect(btInterfacePtr, &BluetoothInterface::DeviceDisconnected, this, &Controller::OnDeviceDisconnected);
    connect(btInterfacePtr, &BluetoothInterface::DataReceived, this, &Controller::OnDataReceived);
    connect(btInterfacePtr, &BluetoothInterface::DataSent, this, &Controller::OnDataSent);
    connect(btInterfacePtr, &BluetoothInterface::CommunicationTimedOut, this, &Controller::OnCommunicationTimeout);
    connect(btInterfacePtr, &BluetoothInterface::ConnectFailed, this, &Controller::OnConnectFailed);

    connect(&addMeterWF, &WorkFlowAddMeter::AddMeterCompleted, this, &Controller::OnAddMeterCompleted);
    connect(&connectMeterWF, &WorkFlowConnectMeter::ConnectCompleted, this, &Controller::OnConnectCompleted);
    connect(&disconnectMeterWF, &WorkFlowDisconnectMeter::DisconnectCompleted, this, &Controller::OnDisconnectCompleted);
    connect(&keepAliveWF, &WorkFlowKeepAlive::KeepAliveCompleted, this, &Controller::OnKeepAliveCompleted);
    connect(&tokenTransferWF, &WorkFlowTokenTransfer::TokenTransferCompleted, this, &Controller::OnTokenTransferCompleted);

    connect(&keepAliveTimer, &QTimer::timeout, this, &Controller::OnSendKeepAlive);
    connect(&connectionResetTimer, &QTimer::timeout, this, &Controller::OnReset);

    keepAliveTimer.setSingleShot(true);
    connectionResetTimer.setSingleShot(true);

    if(app->palette().window().color().value() > app->palette().windowText().color().value())
    {
        isDarkTheme_ = false;
        theme_.EnableLightMode();
    }
    else
    {
        isDarkTheme_ = true;
        theme_.EnableDarkMode();
    }

    barDataEnergy_.append(BarData(0, 0 ,0));
    lineDataEnergy_.append(LineData(0, 0 ,0, 0));
}

void Controller::Initialize()
{
    DlmsClient::CreateClients();

    Help::LoadHelpStrings(0, helpStringList_);
    emit HelpStringsChanged();

    LoadConfigurationData();
    LoadAllData();

    emit IsDarkThemeChanged();
    emit IsDemoModeChanged();
    emit IsTraceEnabledChanged();
    emit ThemeChanged();
}

void Controller::LoadConfigurationData()
{
    userList_.clear();
    userinfoEntityPtr->allUserInfo(userList_);
    if(userList_.count() > 0)
    {
        currentUser_ = userList_.at(0);
        emit IsFirstRunChanged();
    }
    emit CurrentUserChanged();

    alarmFilter_.clear();
    alarmFilterEntityPtr->allAlarmFilters(alarmFilter_);
    emit AlarmFilterChanged();
}

void Controller::LoadAllData()
{
    meterList_.clear();
    meterEntityPtr->allMeters(meterList_, isDemoMode_);

    if(meterList_.count()>0)
    {
        // Set all meters to disconnected and deselected
        for(int x = 0; x < meterList_.count(); x++)
        {
            meterEntityPtr->update("serial_no", meterList_.at(x).SerialNo_, "meter_status", "0");
            meterEntityPtr->update("serial_no", meterList_.at(x).SerialNo_, "is_selected", "0");
        }

        // Set the first meter to selected
        meterEntityPtr->update("serial_no", meterList_.at(0).SerialNo_, "is_selected", "1");
        currentMeterSerialNo_ = meterList_.at(0).SerialNo_;

        // Reload
        meterList_.clear();
        meterEntityPtr->allMeters(meterList_, isDemoMode_);

        LoadMeterSettings(currentMeterSerialNo_);
        loadMeter(currentMeterSerialNo_);
        emit MeterListChanged();
        emit CurrentMeterSerialNoChanged();
        emit meterAction(true);
    }
    else
    {
        minTimestampLP1_ = "START LP1";
        maxTimestampLP1_ = "END LP1";
        minTimestampLP2_ = "START LP2";
        maxTimestampLP2_ = "END LP2";
    }
}

void Controller::selectMeter(QString &serialNo, bool connected)
{
    for(int x = 0; x < meterList_.count(); x++)
    {
        meterEntityPtr->update("serial_no",meterList_.at(x).SerialNo_, "meter_status", "0");
        meterEntityPtr->update("serial_no", meterList_.at(x).SerialNo_, "is_selected", "0");
    }

    currentMeterSerialNo_ = serialNo;
    meterEntityPtr->update("serial_no", serialNo, "is_selected", "1");

    meterList_.clear();
    meterEntityPtr->allMeters(meterList_, isDemoMode_);

    emit MeterListChanged();
    emit CurrentMeterSerialNoChanged();
    emit meterAction(true);
}

void Controller::loadMeter(QString &serialNo)
{
    activeAlarms_.clear();
    tokenList_.clear();
    tokenHistoryList_.clear();

    alarmEntityPtr->selectedAlarms(activeAlarms_, "serial_no", currentMeterSerialNo_);
    tokenEntityPtr->selectedTokens(tokenList_, "serial_no", currentMeterSerialNo_);
    tokenHistoryEntityPtr->selectedTokenHistories(tokenHistoryList_, "serial_no", currentMeterSerialNo_);

    emit ActiveAlarmsChanged();
    emit TokenListChanged();
    emit TokenHistoryListChanged();
    emit PaymentTypeChanged();

    generateDashboardData();
    emit CreditCurrencyChanged();

    updateDataRanges(serialNo);
}

void Controller::LoadMeterSettings(QString &serialNo)
{
    QList<MeterSettings> msettingslist;

    meterSettingsEntityPtr->selectedMetersSettings(msettingslist, "serial_no", serialNo);

    for(int i = 0; i < msettingslist.count(); i++)
    {
        switch(msettingslist.at(i).Key_)
        {
        case 1:
        {
            paymentType_ = msettingslist.at(i).Value_.toInt();
            break;
        }
        case 2:
        {
            creditCurrency_ = msettingslist.at(i).Value_;
            break;
        }
        case 3:
        {
            global_scalar_ = msettingslist.at(i).Value_.toInt();
            break;
        }
        case 4:
        {
            deliveredEnergy_scalar = msettingslist.at(i).Value_.toInt();
            break;
        }
        case 5:
        {
            instantaneousPower_scalar = msettingslist.at(i).Value_.toInt();
            break;
        }
        case 6:
        {
            activeImport_scalar = msettingslist.at(i).Value_.toInt();
            break;
        }
        case 7:
        {
            activeExport_scalar = msettingslist.at(i).Value_.toInt();
            break;
        }
        case 8:
        {
            reactiveImport_scalar = msettingslist.at(i).Value_.toInt();
            break;
        }
        case 9:
        {
            reactiveExport_scalar = msettingslist.at(i).Value_.toInt();
            break;
        }
        default:
            break;
        }
    }

    emit PaymentTypeChanged();
    emit GrossCreditChanged();
}

void Controller::setAllMetersDisconnected()
{
    meterEntityPtr->allMeters(meterList_, isDemoMode_);
    for(int x = 0; x < meterList_.count(); x++)
    {
        if(meterList_.at(x).SerialNo_ == currentMeterSerialNo_)
        {
            meterEntityPtr->update("serial_no", meterList_.at(x).SerialNo_, "meter_status", "0");
        }
    }
    meterList_.clear();
    meterEntityPtr->allMeters(meterList_, isDemoMode_);

    for(int x = 0; x < meterList_.count(); x++)
    {
        if(meterList_.at(x).SerialNo_ == currentMeterSerialNo_)
        {
            Meter mt = meterList_.at(x);
            mt.IsSelected_ = true;
            meterList_[x] = mt;
            currentMeterSerialNo_ = mt.SerialNo_;
        }
    }

    emit MeterListChanged();
    emit CurrentMeterSerialNoChanged();
    emit meterAction(true);
    isAppConnected_ = false;
    emit IsAppConnectedChanged();
}

void Controller::SignalInitializationComplete()
{
    emit initializationComplete();
}

QString Controller::getCreditCurrency()
{
    return creditCurrency_;
}

// Energy

QList<BarData> Controller::getEnergyBarData()
{
    return barDataEnergy_;
}

QList<LineData> Controller::getEnergyLineData()
{
    return lineDataEnergy_;
}

void Controller::setXLabelEnergy(QString str)
{
    xLabelEnergy_ = str;
    emit XLabelEnergyChanged();
}

QString Controller::getXLabelEnergy()
{
    return xLabelEnergy_;
}

void Controller::setYLabelEnergy(QString str)
{
    yLabelEnergy_ = str;
    emit YLabelEnergyChanged();
}

QString Controller::getYLabelEnergy()
{
    return yLabelEnergy_;
}

void Controller::setChartTitle(QString str)
{
    chartTitle_ = str;
    emit ChartTitleChanged();
}

QString Controller::getChartTitle()
{
    return chartTitle_;
}

QString Controller::getMinTimestampLP1()
{
    return minTimestampLP1_;
}

QString Controller::getMaxTimestampLP1()
{
    return maxTimestampLP1_;
}

QString Controller::getMinTimestampLP2()
{
    return minTimestampLP2_;
}

QString Controller::getMaxTimestampLP2()
{
    return maxTimestampLP2_;
}

// Scanner
void Controller::setScanMode(int scanmode)
{
    scanmode_ = (ScanMode)scanmode;
    emit ScanModeChanged();
}

int Controller::getPaymentType()
{
    return paymentType_;
}

int Controller::getScanMode()
{
    return (int)scanmode_;
}

void Controller::setQRText(QString iqrtext)
{
    qrtext_ = iqrtext;
    emit QRTextChanged();
}

QString Controller::getQRText()
{
    return qrtext_;
}

unsigned long long Controller::getDeliveredEnergy()
{
    return deliveredEnergy_;
}

unsigned long long Controller::getInstantaneousPower()
{
    return instantaneousPower_;
}

unsigned long long Controller::getUsedCredit()
{
    return usedCredit_;
}

unsigned long long Controller::getGrossCredit()
{
    return grossCredit_;
}

unsigned long long Controller::getRemainingCredit()
{
    return remainingCredit_;
}

QString Controller::getDeliveredEnergyStr()
{
    return deliveredEnergyStr_;
}

QString Controller::getInstantaneousPowerStr()
{
    return instantaneousPowerStr_;
}

QString Controller::getUsedCreditStr()
{
    return usedCreditStr_;
}

QString Controller::getGrossCreditStr()
{
    return grossCreditStr_;
}

QString Controller::getRemainingCreditStr()
{
    return remainingCreditStr_;
}

float Controller::getRemainingCreditPercent()
{
    return remainingCreditPercent_;
}

void Controller::generateDashboardData()
{
    emit DeliveredEnergyChanged();
    emit InstantaneousPowerChanged();
    emit UsedCreditChanged();
    emit GrossCreditChanged();
    emit RemainingCreditChanged();

    long double val = 0;
    char valstr[257] = {0};

    val = 0;
    memset(valstr, 0, sizeof(valstr));
    val = deliveredEnergy_ * pow(10, deliveredEnergy_scalar);
    snprintf(valstr, 256, "%.2Lf", val);
    deliveredEnergyStr_ = valstr;
    emit DeliveredEnergyStrChanged();

    val = 0;
    memset(valstr, 0, sizeof(valstr));
    val = instantaneousPower_ * pow(10, instantaneousPower_scalar);
    snprintf(valstr, 256, "%.2Lf", val);
    instantaneousPowerStr_ = valstr;
    emit InstantaneousPowerStrChanged();

    val = 0;
    memset(valstr, 0, sizeof(valstr));
    val = usedCredit_ * pow(10, global_scalar_);
    snprintf(valstr, 256, "%.2Lf", val);
    usedCreditStr_ = valstr;
    emit UsedCreditStrChanged();

    val = 0;
    memset(valstr, 0, sizeof(valstr));
    val = grossCredit_ * pow(10, global_scalar_);
    snprintf(valstr, 256, "%.2Lf", val);
    grossCreditStr_ = valstr;
    emit GrossCreditStrChanged();

    val = 0;
    memset(valstr, 0, sizeof(valstr));
    val = remainingCredit_ * pow(10, global_scalar_);
    snprintf(valstr, 256, "%.2Lf", val);
    remainingCreditStr_ = valstr;

    emit RemainingCreditStrChanged();

    emit RemainingCreditPercentChanged();
    emit CreditCurrencyChanged();

    emit updateDashboard();
}

void Controller::setIsAppConnected(bool isappconnected)
{
    isAppConnected_ = isappconnected;
    emit IsAppConnectedChanged();
}

bool Controller::getIsAppConnected()
{
    return isAppConnected_;
}

void Controller::setCurrentMeterSerialNo(QString imetersno)
{
    currentMeterSerialNo_ = imetersno;
    emit CurrentMeterSerialNoChanged();
}

QString Controller::getCurrentMeterSerialNo()
{
    return currentMeterSerialNo_;
}

void Controller::setAlarmFilter(QList<AlarmFilter> alarmsfiler)
{
    alarmFilter_.clear();

    foreach (AlarmFilter lrm, alarmsfiler)
    {
        alarmFilter_.append(lrm);
    }

    emit AlarmFilterChanged();
}

QList<AlarmFilter> Controller::getAlarmFilter()
{
    return alarmFilter_;
}

void Controller::setActiveAlarms(QList<Alarm> activealarms)
{
    activeAlarms_.clear();

    foreach (Alarm str, activealarms)
    {
        activeAlarms_.append(str);
    }

    emit ActiveAlarmsChanged();
}

QList<Alarm> Controller::getActiveAlarms()
{
    return activeAlarms_;
}

void Controller::setMeterList(QList<Meter> meterlist)
{
    meterList_.clear();

    foreach (Meter mtr, meterlist)
    {
        meterList_.append(mtr);
    }

    emit MeterListChanged();
}

QList<Meter> Controller::getMeterList()
{
    return meterList_;
}

void Controller::setTokenList(QList<Token> tokenlist)
{
    tokenList_.clear();

    foreach (Token tkn, tokenlist)
    {
        tokenList_.append(tkn);
    }

    emit TokenListChanged();
}

QList<Token> Controller::getTokenList()
{
    return tokenList_;
}

void Controller::setTokenHistoryList(QList<TokenHistory> tokenHlist)
{
    tokenHistoryList_.clear();

    foreach (TokenHistory tkn, tokenHlist)
    {
        tokenHistoryList_.append(tkn);
    }

    emit TokenHistoryListChanged();
}

QList<TokenHistory> Controller::getTokenHistoryList()
{
    return tokenHistoryList_;
}

float Controller::geDailyMaxTou()
{
    return dailyMaxTou_;
}

float Controller::getHourlyAverageTou()
{
    return hourlyAverageTou_;
}

float Controller::getTotalTou()
{
    return totalTou_;
}

void Controller::invokeClearToken()
{
    qrtext_ = "";
    emit QRTextChanged();
}

QString Controller::invokeCheckQRImage(QImage img)
{
    if (img.isNull())
    {
        return "";
    }

    DecodeHints hints;
    hints.setTryRotate(false);
    hints.setMaxNumberOfSymbols(10);
    hints.setFormats(ZXing::BarcodeFormat::QRCode);

    auto results = ReadBarcodes(img, hints);
    qrtext_ = "";

    for (auto& result : results)
    {
        qrtext_ = result.text();
        if(qrtext_.length() > 0)
        {
            break;
        }
    }

    emit QRTextChanged();
    return qrtext_;
}

void Controller::invokeAddToken(QString tokenId)
{
    qrtext_ = "";
    emit QRTextChanged();

    if(!HasActiveMeter())
    {
        emit tokenError("Please select a meter");
        return;
    }

    if(tokenId.length() != 20)
    {
        emit tokenError("Token format invalid");
        return;
    }

    for(int x = 0; x < tokenId.length(); x++ )
    {
        if(tokenId.at(x).toLatin1() < 48 || tokenId.at(x).toLatin1() > 57)
        {
            emit tokenError("Token format invalid");
            return;
        }
    }

    foreach(Token tk, tokenList_)
    {
        if(tk.TokenId_ == tokenId)
        {
            emit tokenError("Token already exists");
            return;
        }
    }

    Token t1;
    
    t1.IsTokenNew_ = true;
    t1.TokenStatusStr_ = "New token";
    t1.TokenId_ = tokenId;
    t1.TokenValue_ = 0;
    t1.SerialNo_ = currentMeterSerialNo_;

    tokenEntityPtr->createToken(t1);
    tokenList_.append(t1);

    emit TokenListChanged();
    emit tokenAdded();
}

void Controller::invokeRemoveMeter(QString serialNo)
{
    QVariant var(serialNo);
    meterEntityPtr->remove("serial_no", var);
    alarmEntityPtr->remove("serial_no", var);
    tokenEntityPtr->remove("serial_no", var);
    tokenHistoryEntityPtr->remove("serial_no", var);
    statisticsEnergyEntityPtr->remove("serial_no", var);

    currentMeterSerialNo_ = "";

    emit CurrentMeterSerialNoChanged();

    activeAlarms_.clear();
    tokenList_.clear();
    tokenHistoryList_.clear();

    meterList_.clear();

    meterEntityPtr->allMeters(meterList_, isDemoMode_);
    emit MeterListChanged();

    if(meterList_.count() > 0)
    {
        currentMeterSerialNo_ = meterList_.at(0).SerialNo_;
        loadMeter(currentMeterSerialNo_);
        emit CurrentMeterSerialNoChanged();
    }

    emit meterAction(true);
}

void Controller::invokeSelectMeter(QString serialNo)
{
    if(isAppConnected_)
    {
        emit meterMessage(false, "Disconnect the active meter first");
        return;
    }

    currentMeterSerialNo_ =  serialNo;
    selectMeter(currentMeterSerialNo_, false);
    loadMeter(currentMeterSerialNo_);
}

bool Controller::HasActiveMeter()
{
    for(int x = 0; x < meterList_.count(); x++)
    {
        if(meterList_[x].IsSelected_ == true)
        {
            return true;
        }
    }

    return false;
}


void Controller::invokeRemoveToken(QString tokenId)
{
    QVariant var(tokenId);
    tokenEntityPtr->remove("token_id", var);
    tokenList_.clear();
    tokenEntityPtr->selectedTokens(tokenList_, "serial_no", currentMeterSerialNo_);
    emit TokenListChanged();
    emit tokenAction();
}

void Controller::invokeMeterAdditionClose()
{
    emit meterAdded();
}

void Controller::invokeCancel()
{
   emit operationCancelled();
}

void Controller::invokeExit()
{
    dataInterfacePtr->close();
    lastPage_ = "";
    exit(0);
}

void Controller::invokeUpdateAlarmFilterState(int index, int state)
{
    AlarmFilter af = alarmFilter_.at(index);
    if(state == 0)
    {
        af.IsSet_ = false;
    }
    else
    {
        af.IsSet_ = true;
    }

    alarmFilter_[index] = af;
}

void Controller::invokeReloadAlarmFilters()
{
    alarmFilter_.clear();
    alarmFilterEntityPtr->allAlarmFilters(alarmFilter_);
    emit AlarmFilterChanged();
}

void Controller::invokeApplyAlarmFilters()
{
    for(int x = 0; x < alarmFilter_.count(); x++)
    {
        int toggle = 0;

        if(alarmFilter_.at(x).IsSet_)
        {
            toggle = 1;
        }
        else
        {
            toggle = 0;
        }

        alarmFilterEntityPtr->update("alarm_id", x, "is_set", toggle);
    }

    alarmFilter_.clear();
    alarmFilterEntityPtr->allAlarmFilters(alarmFilter_);
    emit AlarmFilterChanged();
}

void Controller::invokeEnergyStatistics(int visualizationmode, int parameter_type, int duration_type, QString tstr1, QString tstr2)
{
    char temp[25] = {0};
    QString errorstr = "";
    bool is_out_of_range = false;
    uint32_t x = 0;
    uint32_t y = 0;

    memset(temp, 0, sizeof(temp));
    snprintf(temp, 24, "%s 23:59:59", tstr1.toStdString().c_str());
    QDateTime dtst = QDateTime::fromString(temp, "dd/MM/yyyy hh:mm:ss");
    uint32_t epochst = dtst.toSecsSinceEpoch();

    memset(temp, 0, sizeof(temp));
    snprintf(temp, 24, "%s 00:00:00", tstr2.toStdString().c_str());
    QDateTime dted = QDateTime::fromString(temp, "dd/MM/yyyy hh:mm:ss");
    uint32_t epoched = dted.toSecsSinceEpoch();

    uint32_t minlp = 0;
    uint32_t maxlp = 0;

    if(duration_type == 0)
    {
        statisticsEnergyEntityPtr->getTimestampRanges( currentMeterSerialNo_, minlp, maxlp, 0);
    }
    else
    {
        statisticsEnergyEntityPtr->getTimestampRanges( currentMeterSerialNo_, minlp, maxlp, 1);
    }

    if(epochst < minlp)
    {
        is_out_of_range = true;
        errorstr = "Input start time is below range";
    }
    else
    {
        if(epoched > maxlp)
        {
            is_out_of_range = true;
            errorstr = "Input finish time is above range";
        }
        else
        {
            is_out_of_range = false;
        }
    }

    if(is_out_of_range)
    {
        emit energyError(errorstr);
        return;
    }

    loadEnergyStatistics(visualizationmode, parameter_type, duration_type, epochst, epoched);
}

void Controller::loadEnergyStatistics(int visualizationmode, int parameter_type, int duration_type, uint32_t ts1, uint32_t ts2)
{
    QString parameter_name = "";

    switch(parameter_type)
    {
        case 0:
        {
            parameter_name = "active_import_tariff1";
            chartTitle_ = "Import Active Energy Tariff 1";
            break;
        }
        case 1:
        {
            parameter_name = "active_export_tariff1";
            chartTitle_ = "Export Active Energy Tariff 1";
            break;
        }
        case 2:
        {
            parameter_name = "reactive_import_tariff1";
            chartTitle_ = "Import Reactive Energy Tariff 1";
            break;
        }
        case 3:
        {
            parameter_name = "reactive_export_tariff1";
            chartTitle_ = "Export Reactive Energy Tariff 1";
            break;
        }
        case 4:
        {
            parameter_name = "active_import_tariff2";
            chartTitle_ = "Import Active Energy Tariff 2";
            break;
        }
        case 5:
        {
            parameter_name = "active_export_tariff2";
            chartTitle_ = "Export Active Energy Tariff 2";
            break;
        }
        case 6:
        {
            parameter_name = "reactive_import_tariff2";
            chartTitle_ = "Import Reactive Energy Tariff 2";
            break;
        }
        case 7:
        {
            parameter_name = "reactive_export_tariff2";
            chartTitle_ = "Export Reactive Energy Tariff 2";
            break;
        }
        default:
        {
            break;
        }
    }

    yLabelEnergy_ = "kWh";

    // Daily - report by hours
    if(duration_type == 0)
    {
        xLabelEnergy_ = "Hour";
        statisticsEnergyEntityPtr->selectedStatisticsEnergysByDay(barDataEnergy_, lineDataEnergy_, currentMeterSerialNo_, parameter_name, ts1, ts2, dailyMaxTou_, totalTou_, hourlyAverageTou_);
    }

    // Weekly - report by weekdays / days
    if(duration_type == 1)
    {
        xLabelEnergy_ = "Day of Week";
        statisticsEnergyEntityPtr->selectedStatisticsEnergysByWeek(barDataEnergy_, lineDataEnergy_, currentMeterSerialNo_, parameter_name, ts1, ts2, dailyMaxTou_, totalTou_, hourlyAverageTou_);
    }

    // Monthly - report by days
    if(duration_type == 2)
    {
        xLabelEnergy_ = "Day";
        statisticsEnergyEntityPtr->selectedStatisticsEnergysByMonth(barDataEnergy_, lineDataEnergy_, currentMeterSerialNo_, parameter_name, ts1, ts2, dailyMaxTou_, totalTou_, hourlyAverageTou_);
    }

    // Yearly - report by months
    if(duration_type == 3)
    {
        xLabelEnergy_ = "Month";
        statisticsEnergyEntityPtr->selectedStatisticsEnergysByYear(barDataEnergy_, lineDataEnergy_, currentMeterSerialNo_, parameter_name, ts1, ts2, dailyMaxTou_, totalTou_, hourlyAverageTou_);
    }

    if(visualizationmode == 0)
    {
        // Multiply all readings by applicable tariff rates
    }

    emit XLabelEnergyChanged();
    emit YLabelEnergyChanged();
    emit ChartTitleChanged();
    emit DailyMaxTouChanged();
    emit HourlyAverageTouChanged();
    emit TotalTouChanged();
    emit EnergyBarDataChanged();
    emit EnergyLineDataChanged();
    emit energyStatisticsRefreshed();
}

void Controller::UpdateCurrentMeterTimestamp()
{
    int ts = QDateTime::currentDateTime().toSecsSinceEpoch();
    appSyncTime_ = QDateTime::fromSecsSinceEpoch(ts).toString("dd MMM yy hh:mm");;

    for(int x = 0; x < meterList_.count(); x++)
    {
        if(meterList_.at(x).SerialNo_ == currentMeterSerialNo_)
        {
            meterEntityPtr->update("serial_no", meterList_.at(x).SerialNo_, "last_sync", ts);
        }
    }
}

QString  Controller::getAppSyncTime()
{
    return appSyncTime_;
}

bool Controller::IsTokenHistoryExisting(const QString &tokenstr)
{
    for(int x = 0; x < tokenHistoryList_.count(); x++)
    {
        if(tokenHistoryList_.at(x).TokenId_ == tokenstr)
        {
            return true;
        }
    }

    return false;
}

void Controller::updateDataRanges(QString &serialNo)
{
    uint32_t minlp1, maxlp1, minlp2, maxlp2;
    uint32_t x, y;
    statisticsEnergyEntityPtr->getTimestampRanges( currentMeterSerialNo_, minlp1, maxlp1, 0);
    statisticsEnergyEntityPtr->getTimestampRanges( currentMeterSerialNo_, minlp2, maxlp2, 1);

    minTimestampLP1_ = QDateTime::fromSecsSinceEpoch(minlp1).toString("dd MMM yyyy, hh:mm");
    maxTimestampLP1_ = QDateTime::fromSecsSinceEpoch(maxlp1).toString("dd MMM yyyy, hh:mm");
    minTimestampLP2_ = QDateTime::fromSecsSinceEpoch(minlp2).toString("dd MMM yyyy, hh:mm");
    maxTimestampLP2_ = QDateTime::fromSecsSinceEpoch(maxlp2).toString("dd MMM yyyy, hh:mm");

    emit MinTimestampLP1Changed();
    emit MaxTimestampLP1Changed();
    emit MinTimestampLP2Changed();
    emit MaxTimestampLP2Changed();
}
