#include "UICommandHandler.hpp"
#include "Helpers.h"
#include "Logger.hpp"
#include "Mk7Mi.hpp"
#include "CommunicationClient.hpp"
#include "DataInterface.hpp"
#include "ZXingQtReader.h"

unsigned char system_title[8] = {0};

char pin_ble_pairing[7] = {0};
char pin_csa_customer[9] = {0};
long rssi = 0;

Meter currentMeter;
UserInfo currentUser;
QString connect_serial_no;
UICommandHandler* globalApplicationData = nullptr;
QString token_pending;

// Network state flags
bool is_adding_meter = false;
bool is_connecting_meter = false;
bool is_keep_alive_pending = false;
bool is_disconect_pending = false;
bool is_token_transfer_pending = false;
// Network state flags

//New User creation interpage variables
QString newUName, newConsId, newPassCode1, newPassCode2;
//New User creation interpage variables

//Reset passcode interpage variables
QString resetConsId, resetPassCode1, resetPassCode2;
//Reset passcode interpage variables

using namespace ZXingQt;

UICommandHandler::UICommandHandler(QGuiApplication *appPtr, QObject *parent)
    : QObject(parent)
{
    app = appPtr;
    appVerion_ = "3.0.1b";
    global_scalar_ = -2;
    deliveredEnergy_scalar = 1;
    instantaneousPower_scalar = 1;
    isAppConnected_ = false;
    globalApplicationData = this;
    deliveredEnergy_ = 0;
    instantaneousPower_ = 0;
    availableCredit_ = 100;
    grossCredit_ =  200;
    isMultiPhase_ = false;
    paymentType_ = 1;
    dailyMaxTou_ = 0;
    hourlyAverageTou_ = 0;
    totalTou_ = 0;
    currentTariff_ = 0;
    lastPage_ = "";
    isDarkTheme_ = true;
    isDemoMode_ = false;
    peripheralServiceIdString_ = "ED310001-C889-5D66-AE38-A7A01230635A";

    connect(&scanner, SIGNAL(deviceDiscovered(QBluetoothDeviceInfo)), this, SLOT(OnDeviceFound(QBluetoothDeviceInfo)));
    connect(&scanner, SIGNAL(canceled(void)), this, SLOT(OnScanComplete(void)));
    connect(&scanner, SIGNAL(finished(void)), this, SLOT(OnScanComplete(void)));

    connect(&scanTimer, SIGNAL(timeout()), this, SLOT(OnScanTimeout()));
    connect(&connectTimer, SIGNAL(timeout()), this, SLOT(OnConnectTimeout()));

    connect(&central, SIGNAL(DeviceConnected(const QString &)), this, SLOT(OnDeviceConnected(const QString &)));
    connect(&central, SIGNAL(DeviceDisconnected()), this, SLOT(OnDeviceDisconnected()));
    connect(&central, SIGNAL(DataReceived(const QByteArray&)), this, SLOT(OnDataReceived(const QByteArray&)));
    connect(&central, SIGNAL(DataSent(uint32_t)), this, SLOT(OnDataSent(uint32_t)));
    connect(&central, SIGNAL(CommunicationTimeout()), this, SLOT(OnCommunicationTimeout()));

    connect(&keepAliveTimer, SIGNAL(timeout()), this, SLOT(OnSendKeepAlive()));

    scanner.setLowEnergyDiscoveryTimeout(60000);

    alarmFilterEntityPtr->allAlarmFilters(alarmFilter_);
    userinfoEntityPtr->allUserInfo(userList_);

    meterEntityPtr->allMeters(meterList_, isDemoMode_);
    for(int x = 0; x < meterList_.count(); x++)
    {
        if(meterList_.at(x).Status_ == MeterConnectionState::Connected)
        {
            meterEntityPtr->update("serial_no", meterList_.at(x).SerialNo_, "meter_status", MeterConnectionState::NotConnected);
        }
    }

    meterList_.clear();
    meterEntityPtr->allMeters(meterList_, isDemoMode_);

    if(meterList_.count()>0)
    {
        currentMeter = meterList_.at(0);
        currentMeter.IsSelected_ = true;
        meterList_[0] = currentMeter;

        foreach(Meter mt, meterList_)
        {
            meterNames_.append(mt.SerialNo_ + " " + mt.MeterName_);
        }

        emit MeterNameListChanged();

        tokenEntityPtr->selectedTokens(tokenList_, "serial_no", currentMeter.SerialNo_);
        tariffEntityPtr->selectedTariffs(tariffList_, "serial_no", currentMeter.SerialNo_);
        alarmEntityPtr->selectedAlarms(activeAlarms_, "serial_no", currentMeter.SerialNo_);

        Tariff trfMax;
        trfMax.EffectiveDate_ = 0;

        foreach(Tariff tf, tariffList_)
        {
            if(tf.EffectiveDate_ >= trfMax.EffectiveDate_)
            {
                trfMax = tf;
            }
        }

        currentTariff_ = trfMax.Import1_;

        emit CurrentTariffChanged();

        uint32_t minlp1, maxlp1, minlp2, maxlp2;
        uint32_t x, y;
        statisticsEnergyEntityPtr->getRanges( currentMeter.SerialNo_, x, y, minlp1, maxlp1, 0);
        statisticsEnergyEntityPtr->getRanges( currentMeter.SerialNo_, x, y, minlp2, maxlp2, 1);

        minTimestampLP1_ = QDateTime::fromSecsSinceEpoch(minlp1).toString("dd MMM yyyy, hh:mm");
        maxTimestampLP1_ = QDateTime::fromSecsSinceEpoch(maxlp1).toString("dd MMM yyyy, hh:mm");
        minTimestampLP2_ = QDateTime::fromSecsSinceEpoch(minlp2).toString("dd MMM yyyy, hh:mm");
        maxTimestampLP2_ = QDateTime::fromSecsSinceEpoch(maxlp2).toString("dd MMM yyyy, hh:mm");

        emit MinTimestampLP1Changed();
        emit MaxTimestampLP1Changed();
        emit MinTimestampLP2Changed();
        emit MaxTimestampLP2Changed();

        uint32_t minpq, maxpq;
        statisticsPowerQualityEntityPtr->getRanges( currentMeter.SerialNo_, x, y, minpq, maxpq);
        minTimestampPQ_ = QDateTime::fromSecsSinceEpoch(minpq).toString("dd MMM yyyy, hh:mm");
        maxTimestampPQ_ = QDateTime::fromSecsSinceEpoch(maxpq).toString("dd MMM yyyy, hh:mm");
        emit MinTimestampPQChanged();
        emit MaxTimestampPQChanged();
    }
    else
    {
        minTimestampLP1_ = "START LP1";
        maxTimestampLP1_ = "END LP1";
        minTimestampLP2_ = "START LP2";
        maxTimestampLP2_ = "END LP2";
        minTimestampPQ_ = "START PQ";
        maxTimestampPQ_ = "END PQ";
    }

    if(userList_.count() > 0)
    {
        currentUser = userList_.at(0);
    }

    addMeterWF = new WorkFlowAddMeter();
    connectMeterWF = new WorkFlowConnectMeter();
    tokenTransferWF = new WorkFlowTokenTransfer();
    keepAliveWF = new WorkFlowKeepAlive();

    for(int x = 1; x < 32; x++)
    {
        QVariant dy((int)x);
        days_.append(dy.toString());
    }

    for(int x = 2020; x < 2051; x++)
    {
        QVariant yr((int)x);
        years_.append(yr.toString());
    }

    months_.append("January");
    months_.append("February");
    months_.append("March");
    months_.append("April");
    months_.append("May");
    months_.append("June");
    months_.append("July");
    months_.append("August");
    months_.append("September");
    months_.append("October");
    months_.append("November");
    months_.append("December");

    Help::LoadHelpStrings(helpStringList_);

    QList<MeterSettings> mslist;

    meterSettingsEntityPtr->selectedMetersSettings(mslist, "serial_no", currentMeter.SerialNo_);

    if(app->palette().window().color().value() > app->palette().windowText().color().value())
    {
        isDarkTheme_ = false;
    }
    else
    {
        isDarkTheme_ = true;
    }

    emit IsDarkThemeChanged();
}

QString UICommandHandler::getAppVersion()
{
    return appVerion_;
}

bool UICommandHandler::getIsMeterActive()
{
    if(!currentMeter.IsSelected_)
    {
        return false;
    }

    if(currentMeter.SerialNo_.length() < 1)
    {
        return false;
    }

    return true;
}

bool UICommandHandler::getIsDarkTheme()
{
    return isDarkTheme_;
}

void UICommandHandler::setIsDarkTheme(bool isdarktheme)
{
    isDarkTheme_ = isdarktheme;
    emit IsDarkThemeChanged();
}

QString UICommandHandler::getCreditCurrency()
{
    return creditCurrency_;
}


bool UICommandHandler::getIsDemoMode()
{
    return isDemoMode_;
}

void UICommandHandler::setIsDemoMode(bool isdemomode)
{
    isDemoMode_ = isdemomode;
    emit IsDemoModeChanged();

    meterNames_.clear();
    meterList_.clear();
    meterEntityPtr->allMeters(meterList_, isdemomode);
    foreach(Meter mt, meterList_)
    {
        meterNames_.append(mt.SerialNo_ + " " + mt.MeterName_);
    }

    if(meterList_.count() > 0)
    {
        currentMeter = meterList_.at(0);
        currentMeter.IsSelected_ = true;
        meterList_[0] = currentMeter;

        tokenEntityPtr->selectedTokens(tokenList_, "serial_no", currentMeter.SerialNo_);
        tariffEntityPtr->selectedTariffs(tariffList_, "serial_no", currentMeter.SerialNo_);
        alarmEntityPtr->selectedAlarms(activeAlarms_, "serial_no", currentMeter.SerialNo_);
    }

    emit MeterNameListChanged();
    emit MeterListChanged();
    emit meterAction();
}

QList<Help> UICommandHandler::getHelpStrings()
{
    return helpStringList_;
}

void UICommandHandler::invokeThemeUpdate()
{
    if(app->palette().window().color().value() > app->palette().windowText().color().value())
    {
        isDarkTheme_ = false;
    }
    else
    {
        isDarkTheme_ = true;
    }

    emit IsDarkThemeChanged();
}

void UICommandHandler::invokeSaveCurrentPage(QString page)
{
    lastPage_ = page;
}

QString UICommandHandler::invokeRetreiveLastPage()
{
    return lastPage_;
}

void UICommandHandler::setIsFirstRun(bool isfirstrun)
{
    is_first_run_ = isfirstrun;
    emit IsFirstRunChanged();
}

bool UICommandHandler::getIsFirstRun()
{
    if(userList_.count() < 1)
    {
        is_first_run_ = true;
    }
    else
    {
        is_first_run_ = false;
    }

    return is_first_run_;
}

void UICommandHandler::AppLogInformation(QString str)
{
    emit logMessage(str);
    Logger::GetInstance()->write(str.toStdString(), LogInfo);
}

void UICommandHandler::AppLogError(QString str)
{
    emit logMessage(str);
    Logger::GetInstance()->write(str.toStdString(), LogError);
}

void UICommandHandler::Initialize()
{
    CommunicationClient::CreateClients();
}

QStringList UICommandHandler::getDays()
{
    return days_;
}

QStringList UICommandHandler::getMonths()
{
    return months_;
}

QStringList UICommandHandler::getYears()
{
    return years_;
}

// Energy

QList<BarData> UICommandHandler::getEnergyBarData()
{
    return barDataEnergy_;
}

QList<LineData> UICommandHandler::getEnergyLineData()
{
    return lineDataEnergy_;
}

void UICommandHandler::setXLabelBarEnergy(QString str)
{
    xLabelBarEnergy_ = str;
    emit XLabelBarEnergyChanged();
}

QString UICommandHandler::getXLabelBarEnergy()
{
    return xLabelBarEnergy_;
}

void UICommandHandler::setYLabelBarEnergy(QString str)
{
    yLabelBarEnergy_ = str;
    emit YLabelBarEnergyChanged();
}

QString UICommandHandler::getYLabelBarEnergy()
{
    return yLabelBarEnergy_;
}

void UICommandHandler::setXLabelLineEnergy(QString str)
{
    xLabelLineEnergy_ = str;
    emit XLabelLineEnergyChanged();
}

QString UICommandHandler::getXLabelLineEnergy()
{
    return xLabelLineEnergy_;
}

void UICommandHandler::setYLabelLineEnergy(QString str)
{
    yLabelLineEnergy_ = str;
    emit YLabelLineEnergyChanged();
}

QString UICommandHandler::getYLabelLineEnergy()
{
    return yLabelLineEnergy_;
}

void UICommandHandler::setChartTitleEnergy(QString str)
{
    chartTitleEnergy_ = str;
    emit ChartTitleEnergyChanged();
}

QString UICommandHandler::getChartTitleEnergy()
{
    return chartTitleEnergy_;
}

QString UICommandHandler::getMinTimestampLP1()
{
    return minTimestampLP1_;
}

QString UICommandHandler::getMaxTimestampLP1()
{
    return maxTimestampLP1_;
}

QString UICommandHandler::getMinTimestampLP2()
{
    return minTimestampLP2_;
}

QString UICommandHandler::getMaxTimestampLP2()
{
    return maxTimestampLP2_;
}


//Power Quality
QList<BarData> UICommandHandler::getPowerQualityBarData()
{
    return barDataPowerQuality_;
}

QList<LineData> UICommandHandler::getPowerQualityLineData()
{
    return lineDataPowerQuality_;
}

void UICommandHandler::setXLabelBarPowerQuality(QString str)
{
    xLabelBarPowerQuality_ = str;
    emit XLabelBarPowerQualityChanged();
}

QString UICommandHandler::getXLabelBarPowerQuality()
{
    return xLabelBarPowerQuality_;
}

void UICommandHandler::setYLabelBarPowerQuality(QString str)
{
    yLabelBarPowerQuality_ = str;
    emit YLabelBarPowerQualityChanged();
}

QString UICommandHandler::getYLabelBarPowerQuality()
{
    return yLabelBarPowerQuality_;
}

void UICommandHandler::setXLabelLinePowerQuality(QString str)
{
    xLabelLinePowerQuality_ = str;
    emit XLabelLinePowerQualityChanged();
}

QString UICommandHandler::getXLabelLinePowerQuality()
{
    return xLabelLinePowerQuality_;
}

void UICommandHandler::setYLabelLinePowerQuality(QString str)
{
    yLabelLinePowerQuality_ = str;
    emit YLabelLinePowerQualityChanged();
}

QString UICommandHandler::getYLabelLinePowerQuality()
{
    return yLabelLinePowerQuality_;
}

void UICommandHandler::setChartTitlePowerQuality(QString str)
{
    chartTitlePowerQuality_ = str;
    emit ChartTitlePowerQualityChanged();
}

QString UICommandHandler::getChartTitlePowerQuality()
{
    return chartTitlePowerQuality_;
}

QString UICommandHandler::getMinTimestampPQ()
{
    qDebug() << "TS START PQ" << minTimestampPQ_;
    return minTimestampPQ_;
}

QString UICommandHandler::getMaxTimestampPQ()
{
    qDebug() << "TS END PQ" << maxTimestampPQ_;
    return maxTimestampPQ_;
}

float UICommandHandler::getPQMax1()
{
    return pqMax1_;
}

float UICommandHandler::getPQMax2()
{
    return pqMax2_;
}

float UICommandHandler::getPQMin1()
{
    return pqMin1_;
}

float UICommandHandler::getPQMin2()
{
    return pqMin2_;
}

QString UICommandHandler::getPQ1Unit()
{
    return pq1Unit_;
}

QString UICommandHandler::getPQ2Unit()
{
    return pq2Unit_;
}

// Scanner
void UICommandHandler::setScanMode(int scanmode)
{
    scanmode_ = (ScanMode)scanmode;
    emit ScanModeChanged();
}

int UICommandHandler::getPaymentType()
{
    return paymentType_;
}

int UICommandHandler::getScanMode()
{
    return (int)scanmode_;
}

void UICommandHandler::setQRText(QString iqrtext)
{
    qrtext_ = iqrtext;
    emit QRTextChanged();
}

QString UICommandHandler::getQRText()
{
    return qrtext_;
}

float UICommandHandler::getCurrentTariff()
{
    currentTariff_ = getTariff(QDateTime::currentDateTime().toSecsSinceEpoch());
    return currentTariff_;
}

float UICommandHandler::getDeliveredEnergy()
{
    return instantaneousPower_;
}

float UICommandHandler::getInstantaneousPower()
{
    return deliveredEnergy_;
}

float UICommandHandler::getAvailableCredit()
{
    return availableCredit_;
}

float UICommandHandler::getGrossCredit()
{
    return grossCredit_;
}

void UICommandHandler::setIsAppConnected(bool isappconnected)
{
    isAppConnected_ = isappconnected;
    emit IsAppConnectedChanged();
}

bool UICommandHandler::getIsAppConnected()
{
    return isAppConnected_;
}

void UICommandHandler::setCurrentMeter(Meter imeter)
{
    currentMeter = imeter;
    emit CurrentMeterChanged();
}

Meter UICommandHandler::getCurrentMeter()
{
    return currentMeter;
}

void UICommandHandler::setMeterNameList(QList<QString> meterNamelist)
{
    meterNames_.clear();

    foreach (QString str, meterNamelist)
    {
        meterNames_.append(str);
    }

    emit MeterNameListChanged();
}

QList<QString> UICommandHandler::getMeterNameList()
{
    return meterNames_;
}


void UICommandHandler::setCurrentUser(UserInfo iuser)
{
    currentUser = iuser;
    emit CurrentUserChanged();
}

UserInfo UICommandHandler::getCurrentUser()
{
    return currentUser;
}

void UICommandHandler::setAlarmFilter(QList<AlarmFilter> alarmsfiler)
{
    alarmFilter_.clear();

    foreach (AlarmFilter lrm, alarmsfiler)
    {
        alarmFilter_.append(lrm);
    }

    emit AlarmFilterChanged();
}

QList<AlarmFilter> UICommandHandler::getAlarmFilter()
{
    return alarmFilter_;
}

void UICommandHandler::setActiveAlarms(QList<Alarm> activealarms)
{
    activeAlarms_.clear();

    foreach (Alarm str, activealarms)
    {
        activeAlarms_.append(str);
    }

    emit ActiveAlarmsChanged();
}

QList<Alarm> UICommandHandler::getActiveAlarms()
{
    return activeAlarms_;
}

void UICommandHandler::setMeterList(QList<Meter> meterlist)
{
    meterList_.clear();

    foreach (Meter mtr, meterlist)
    {
        meterList_.append(mtr);
    }

    emit MeterListChanged();
}

QList<Meter> UICommandHandler::getMeterList()
{
    return meterList_;
}

void UICommandHandler::setTokenList(QList<Token> tokenlist)
{
    tokenList_.clear();

    foreach (Token tkn, tokenlist)
    {
        tokenList_.append(tkn);
    }

    emit TokenListChanged();
}

QList<Token> UICommandHandler::getTokenList()
{
    return tokenList_;
}

void UICommandHandler::setTariffList(QList<Tariff> tarifflist)
{
    tariffList_.clear();

    foreach (Tariff trf, tarifflist)
    {
        tariffList_.append(trf);
    }

    emit TariffListChanged();
}

QList<Tariff> UICommandHandler::getTariffList()
{
    return tariffList_;
}

float UICommandHandler::geDailyMaxTou()
{
    return dailyMaxTou_;
}

float UICommandHandler::getHourlyAverageTou()
{
    return hourlyAverageTou_;
}

float UICommandHandler::getTotalTou()
{
    return totalTou_;
}

void UICommandHandler::invokeClearToken()
{
    qrtext_ = "";
    emit QRTextChanged();
}

QString UICommandHandler::invokeCheckQRImage(QImage img)
{
    if (img.isNull())
    {
        qDebug() << "Invalid image";
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

    qDebug() << qrtext_;

    emit QRTextChanged();
    return qrtext_;
}

void UICommandHandler::invokeChangePage(int ipage)
{
    emit pageAction(ipage);
}

void UICommandHandler::invokeAddMeter(QString token)
{
    qrtext_ = "";
    emit QRTextChanged();

    memset(pin_ble_pairing, 0, sizeof(pin_ble_pairing));
    memset(pin_csa_customer, 0, sizeof(pin_csa_customer));

    is_connecting_meter = false;
    is_adding_meter = true;

    if(token.length() != 29 && token.length() != 23 && token.length() != 15 && token.length() != 9)
    {
        AppLogInformation("Checksum failed for pass code");
        emit meterPairingCodeVerification(false, "Validation failed");
        is_connecting_meter = false;
        is_adding_meter = false;
        return;
    }

    char* ptr = new char[token.toStdString().length()+1];
    memset(ptr, 0, token.toStdString().length()+1);
    memcpy(ptr, token.toStdString().c_str(), token.toStdString().length());

    if (!Helpers::checkLuhn(ptr))
    {
        AppLogInformation("Checksum failed for pass code");
        emit meterPairingCodeVerification(false, "Validation failed");
        is_connecting_meter = false;
        is_adding_meter = false;
        return;
    }

    std::string system_title_str;

    if(token.length() == 29)
    {
        int tlen = token.toStdString().length() - 14 -1;
        char* system_title_base10 = new char[tlen+1];
        memset(system_title_base10, 0, tlen+1);

        emit meterPairingCodeVerification(true, "Validation failed");

        memcpy(system_title_base10, ptr, tlen);
        memcpy(pin_ble_pairing, &ptr[20], 6);
        memcpy(pin_csa_customer, &ptr[26], 8);

        delete [] ptr;
        system_title_str = Helpers::hexString(system_title_base10);
        delete [] system_title_base10;
    }
    else
    {
        if(token.length() == 15)
        {
            int tlen = token.toStdString().length() - 1;
            char* system_title_base10 = new char[tlen+1];
            memset(system_title_base10, 0, tlen+1);

            emit meterPairingCodeVerification(true, "Validation failed");

            memcpy(system_title_base10, ptr, tlen);
            memcpy(pin_ble_pairing, "000000", 6);
            memcpy(pin_csa_customer, "00000000", 8);

            delete [] ptr;
            system_title_str = Helpers::hexString(system_title_base10);
            delete [] system_title_base10;
        }
    }

    ByteBuffer buff;
    Helpers::HexToBytes(system_title_str, buff);
    memset(system_title, 0, 8);
    system_title[0] = 'E';
    system_title[1] = 'D';
    memcpy(&system_title[2], buff.GetData(), buff.GetSize());

    std::string syst_hex_str = Helpers::BytesToHex(system_title, 8, 0);

    for(int x = 0; x < meterList_.count(); x++)
    {
       QString tempstr = meterList_.at(x).SystemTitle_;
       tempstr = tempstr.remove(" ");

       if(syst_hex_str == tempstr.toStdString())
       {
            qDebug() << "Already exists";
            emit meterPairingCodeVerification(false, "Meter exists");
            is_connecting_meter = false;
            is_adding_meter = false;
            return;
       }
    }

    AppLogInformation("Starting BLE scan");
    qDebug() << "Scanning for" << syst_hex_str.c_str();
    scanner.start(QBluetoothDeviceDiscoveryAgent::LowEnergyMethod);
    scanTimer.start(30000);
}

void UICommandHandler::invokeConnectMeter(QString serialNo)
{
    isAppConnected_ = false;
    emit IsAppConnectedChanged();

    is_connecting_meter = true;
    is_adding_meter = false;

    Meter mt;
    bool found = false;
    foreach(Meter mtr , meterList_)
    {
        if(mtr.SerialNo_ == serialNo)
        {
            mt = mtr;
            connect_serial_no = serialNo;
            found = true;
            break;
        }
    }

    if(found)
    {
        ByteBuffer buff;
        std::string syst_hex = mt.SystemTitle_.toStdString();
        Helpers::HexToBytes(syst_hex, buff);

        memset(system_title, 0, 8);
        memcpy(system_title, buff.GetData(), buff.GetSize());

        std::string syst_hex_str = Helpers::BytesToHex(system_title, 8, 0);
        qDebug() << syst_hex_str.c_str();
        scanner.start(QBluetoothDeviceDiscoveryAgent::LowEnergyMethod);
        scanTimer.start(5000);
    }
}

void UICommandHandler::invokeDisconnectMeter(QString serialNo)
{
    if(!isAppConnected_)
    {
        return;
    }

    if(is_keep_alive_pending)
    {
        is_disconect_pending = true;
    }
    else
    {
        keepAliveTimer.stop();
        is_disconect_pending = false;
        workflow = connectMeterWF;
        connect(connectMeterWF, SIGNAL(Completed(QList<QVariant>, WorkflowName, bool)), this, SLOT(OnCompleted(QList<QVariant>, WorkflowName, bool)));
        connectMeterWF->SetCentral(&central);
        QList<QVariant> startParamters;
        startParamters.append(QVariant(QByteArray((char*)system_title, 8)));
        startParamters.append(QVariant(true));
        connectMeterWF->StartWorkFlow(startParamters);
    }
}

void UICommandHandler::invokeAddToken(QString tokenId)
{
    qrtext_ = "";
    emit QRTextChanged();

    if(!getIsMeterActive())
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

    t1.IsApplied_ = false;
    t1.TokenId_ = tokenId;
    t1.TokenValue_ = 100;
    t1.AppliedDate_ = QDateTime::currentDateTime().toSecsSinceEpoch();
    t1.SerialNo_ = currentMeter.SerialNo_;
    t1.TokenStatusText_ = "New token";
    t1.RowIndex_ = -1;

    tokenEntityPtr->createToken(t1);
    tokenList_.append(t1);

    emit TokenListChanged();
    emit tokenAdded();
}

void UICommandHandler::invokeRemoveMeter(QString serialNo)
{
    QVariant var(serialNo);
    meterEntityPtr->remove("serial_no", var);
    alarmEntityPtr->remove("serial_no", var);
    tokenEntityPtr->remove("serial_no", var);
    tariffEntityPtr->remove("serial_no", var);
    statisticsEnergyEntityPtr->remove("serial_no", var);

    activeAlarms_.clear();
    tokenList_.clear();
    tariffList_.clear();
    alarmEntityPtr->selectedAlarms(activeAlarms_, "serial_no", currentMeter.SerialNo_);
    tokenEntityPtr->selectedTokens(tokenList_, "serial_no", currentMeter.SerialNo_);
    tariffEntityPtr->selectedTariffs(tariffList_, "serial_no", currentMeter.SerialNo_);

    meterList_.clear();
    meterEntityPtr->allMeters(meterList_);

    if(meterList_.count() < 1)
    {
        currentMeter.MeterName_ = "";
        currentMeter.Status_ = 2;
        currentMeter.SerialNo_ = "";
        currentMeter.LastSync_ = 0;
        currentMeter.RSSI_ = 0;
        currentMeter.ModelNo_ = "";
        currentMeter.BlePIN_ = "";
        currentMeter.CIUPIN_ = "";
        currentMeter.SystemTitle_ = "";
        currentMeter.Keys103_ = "";
        currentMeter.Keys104_ = "";
        currentMeter.Keys105_ = "";
        currentMeter.BleIdentifier_ = "";
        currentMeter.LastSyncStr_ = "";
    }

    emit ActiveAlarmsChanged();
    emit TokenListChanged();
    emit TariffListChanged();
    emit MeterListChanged();
    emit meterAction();
}

void UICommandHandler::invokeSelectMeter(QString serialNo)
{
    if(isAppConnected_)
    {
        return;
    }

    Meter mt;
    bool found = false;

    int index = 0;
    foreach(Meter mtr , meterList_)
    {
        if(mtr.SerialNo_ == serialNo)
        {
            mt = mtr;
            connect_serial_no = serialNo;
            found = true;
            break;
        }
        index++;
    }

    for(int x = 0; x < meterList_.count(); x++)
    {
        Meter mx = meterList_[x];
        mx.IsSelected_ = false;
        meterList_[x] = mx;
    }

    if(found)
    {
        currentMeter = mt;
        mt.IsSelected_ = true;
        meterList_[index] = mt;
        emit CurrentMeterChanged();
        emit MeterListChanged();
        emit meterAction();
        emit IsMeterActiveChanged();
    }
}

void UICommandHandler::invokeApplyToken(QString tokenId)
{
    if(!isAppConnected_)
    {
        return;
    }

    if(is_keep_alive_pending)
    {
        is_token_transfer_pending = true;
        token_pending = tokenId;
    }
    else
    {
        keepAliveTimer.stop();
        is_token_transfer_pending = false;
        workflow = tokenTransferWF;
        connect(tokenTransferWF, SIGNAL(Completed(QList<QVariant>, WorkflowName, bool)), this, SLOT(OnCompleted(QList<QVariant>, WorkflowName, bool)));
        tokenTransferWF->SetCentral(&central);
        QList<QVariant> startParameters;
        startParameters.append(tokenId);
        tokenTransferWF->StartWorkFlow(startParameters);
    }
}

void UICommandHandler::invokeRemoveToken(QString tokenId)
{
    QVariant var(tokenId);
    tokenEntityPtr->remove("token_id", var);
    tokenList_.clear();
    tokenEntityPtr->allTokens(tokenList_);
    emit TokenListChanged();
    emit tokenAction();
}

void UICommandHandler::invokeAddTariff(QString import1, QString export1, QString import2, QString export2, QString datestr)
{
     if(!getIsMeterActive())
    {
        emit tokenError("Please select a meter");
        return;
    }

    if(import1 == "")
    {
        import1 = "00.00";
    }

    if(export1 == "")
    {
        export1 = "00.00";
    }

    if(import2 == "")
    {
        import2 = "00.00";
    }

    if(export2 == "")
    {
        export2 = "00.00";
    }

    if(!IsFloatString(import1))
    {
        emit tariffError("Please re-enter Import 1 value as a valid decimal figure");
        return;
    }

    if(!IsFloatString(export1))
    {
        emit tariffError("Please re-enter Export 1 value as a valid decimal figure");
        return;
    }

    if(!IsFloatString(import2))
    {
        emit tariffError("Please re-enter Import 2 value as a valid decimal figure");
        return;
    }

    if(!IsFloatString(export2))
    {
        emit tariffError("Please re-enter Export 2 value as a valid decimal figure");
        return;
    }

    char temp[24] = {0};
    sprintf(temp, "%s 00:00:00", datestr.toStdString().c_str());
    qDebug() << temp;
    QDateTime dt = QDateTime::fromString(temp, "dd/MM/yyyy hh:mm:ss");
    uint32_t eposhtm = dt.toSecsSinceEpoch();

    foreach(Tariff tf, tariffList_)
    {
        if(eposhtm <= tf.EffectiveDate_)
        {
            emit tariffError("A tariff plan already exists until the provided effective date");
            return;
        }
    }

    Tariff tf1;

    tf1.TariffId_ = QUuid::createUuid().toString(QUuid::WithoutBraces).toUpper();
    tf1.IsApplied_ = false;
    tf1.SerialNo_ = currentMeter.SerialNo_;
    tf1.EffectiveDate_ = dt.toSecsSinceEpoch();
    tf1.Import1_ = import1.toDouble();
    tf1.Export1_ = export1.toDouble();
    tf1.Import2_ = import2.toDouble();
    tf1.Export2_ = export2.toDouble();

    tariffEntityPtr->createTariff(tf1);
    tariffList_.clear();
    tariffEntityPtr->selectedTariffs(tariffList_, "serial_no", currentMeter.SerialNo_);
    emit TariffListChanged();
    emit pageAction(17);

    QDateTime currdt = QDateTime::currentDateTime();
    uint32_t curreposhtm = dt.toSecsSinceEpoch();

    Tariff trfMax;
    trfMax.EffectiveDate_ = 0;

    foreach(Tariff tf, tariffList_)
    {
        if(tf.EffectiveDate_ >= trfMax.EffectiveDate_)
        {
            trfMax = tf;
        }
    }

    currentTariff_ = trfMax.Import1_;

    emit CurrentTariffChanged();
}

void UICommandHandler::invokeRemoveTariff(QString tariffId)
{
    QVariant var(tariffId);
    tariffEntityPtr->remove("tariff_id", var);
    tariffList_.clear();
    tariffEntityPtr->selectedTariffs(tariffList_, "serial_no", currentMeter.SerialNo_);
    emit TariffListChanged();
    emit tariffAction();

    Tariff trfMax;
    trfMax.EffectiveDate_ = 0;

    foreach(Tariff tf, tariffList_)
    {
        if(tf.EffectiveDate_ >= trfMax.EffectiveDate_)
        {
            trfMax = tf;
        }
    }

    currentTariff_ = trfMax.Import1_;

    emit CurrentTariffChanged();
}

void UICommandHandler::invokeMeterAdditionClose()
{
    emit meterAdded();
}

void UICommandHandler::invokeCancel()
{
   emit operationCancelled();
}

void UICommandHandler::invokeExit()
{
    dataInterfacePtr->close();
    lastPage_ = "";
    app->quit();
}

void UICommandHandler::invokeUpdateAlarmFilterState(int index, int state)
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

void UICommandHandler::invokeApplyAlarmFilters()
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

void UICommandHandler::invokeFetchEnergyStatistics(int visualizationmode, int parameter_type, int duration_type, QString tstr1, QString tstr2)
{
    char temp[24] = {0};

    memset(temp, 0, sizeof(temp));
    sprintf(temp, "%s 00:00:00", tstr1.toStdString().c_str());
    QDateTime dtst = QDateTime::fromString(temp, "dd/MM/yyyy hh:mm:ss");
    uint32_t epochst = dtst.toSecsSinceEpoch();

    memset(temp, 0, sizeof(temp));
    sprintf(temp, "%s 00:00:00", tstr2.toStdString().c_str());
    QDateTime dted = QDateTime::fromString(temp, "dd/MM/yyyy hh:mm:ss");
    uint32_t epoched = dted.toSecsSinceEpoch();

    loadEnergyStatistics(visualizationmode, parameter_type, duration_type, epochst, epoched);
}

void UICommandHandler::loadEnergyStatistics(int visualizationmode, int parameter_type, int duration_type, uint32_t ts1, uint32_t ts2)
{
    QString parameter_name = "";

    switch(parameter_type)
    {
        case 0:
        {
            parameter_name = "active_import_tariff1";
        chartTitleEnergy_ = "Import Active Energy Tariff 1";
            break;
        }
        case 1:
        {
            parameter_name = "active_export_tariff1";
            chartTitleEnergy_ = "Export Active Energy Tariff 1";
            break;
        }
        case 2:
        {
            parameter_name = "reactive_import_tariff1";
            chartTitleEnergy_ = "Import Reactive Energy Tariff 1";
            break;
        }
        case 3:
        {
            parameter_name = "reactive_export_tariff1";
            chartTitleEnergy_ = "Export Reactive Energy Tariff 1";
            break;
        }
        case 4:
        {
            parameter_name = "active_import_tariff2";
            chartTitleEnergy_ = "Import Active Energy Tariff 2";
            break;
        }
        case 5:
        {
            parameter_name = "active_export_tariff2";
            chartTitleEnergy_ = "Export Active Energy Tariff 2";
            break;
        }
        case 6:
        {
            parameter_name = "reactive_import_tariff2";
            chartTitleEnergy_ = "Import Reactive Energy Tariff 2";
            break;
        }
        case 7:
        {
            parameter_name = "reactive_export_tariff2";
            chartTitleEnergy_ = "Export Reactive Energy Tariff 2";
            break;
        }
        default:
        {
            break;
        }
    }

        yLabelBarEnergy_ = "kWh";
    yLabelLineEnergy_ = "kWh";

    // Daily - report by hours
    if(duration_type == 0)
    {
            xLabelLineEnergy_ = xLabelBarEnergy_ = "Hour";
            statisticsEnergyEntityPtr->selectedStatisticsEnergysByDay(barDataEnergy_, lineDataEnergy_, currentMeter.SerialNo_, parameter_name, ts1, ts2, dailyMaxTou_, totalTou_, hourlyAverageTou_);
    }

    // Weekly - report by weekdays / days
    if(duration_type == 1)
    {
            xLabelLineEnergy_ = xLabelBarEnergy_ = "Day of Week";
            statisticsEnergyEntityPtr->selectedStatisticsEnergysByWeek(barDataEnergy_, lineDataEnergy_, currentMeter.SerialNo_, parameter_name, ts1, ts2, dailyMaxTou_, totalTou_, hourlyAverageTou_);
    }

    // Monthly - report by days
    if(duration_type == 2)
    {
            xLabelLineEnergy_ = xLabelBarEnergy_ = "Day";
            statisticsEnergyEntityPtr->selectedStatisticsEnergysByMonth(barDataEnergy_, lineDataEnergy_, currentMeter.SerialNo_, parameter_name, ts1, ts2, dailyMaxTou_, totalTou_, hourlyAverageTou_);
    }

    // Yearly - report by months
    if(duration_type == 3)
    {
            xLabelLineEnergy_ = xLabelBarEnergy_ = "Month";
            statisticsEnergyEntityPtr->selectedStatisticsEnergysByYear(barDataEnergy_, lineDataEnergy_, currentMeter.SerialNo_, parameter_name, ts1, ts2, dailyMaxTou_, totalTou_, hourlyAverageTou_);
    }

    if(visualizationmode == 0)
    {
        // Multiply all readings by applicable tariff rates
    }

    emit XLabelBarEnergyChanged();
    emit XLabelLineEnergyChanged();
    emit YLabelBarEnergyChanged();
    emit YLabelLineEnergyChanged();
    emit ChartTitleEnergyChanged();
    emit DailyMaxTouChanged();
    emit HourlyAverageTouChanged();
    emit TotalTouChanged();
    emit EnergyBarDataChanged();
    emit EnergyLineDataChanged();
    emit energyStatisticsRefreshed();
}

void UICommandHandler::invokeLogin(QString pin)
{
    bool found = false;

    foreach (auto user, userList_)
    {
        if(user.PassCode1_ == pin)
        {
            found = true;
            currentUser = user;
            break;
        }
    }

    if(found)
    {
        emit userLoginSucceeded();
    }
    else
    {
        emit userLoginFailed("Passcode is incorrect. Please retry.");
    }
}

void UICommandHandler::invokeChangePassword(QString consumerid, QString pass)
{
    bool found = true;
    UserInfo u;
    foreach (auto user, userList_)
    {
        if(user.ConsumerId_ == consumerid)
        {
            found = true;
            u = user;
            break;
        }
    }

    userinfoEntityPtr->update("consumer_id", u.ConsumerId_, "passcode1", pass);
    userinfoEntityPtr->update("consumer_id", u.ConsumerId_, "passcode2", pass);
    currentUser = u;

    emit CurrentUserChanged();

    userList_.clear();
    userinfoEntityPtr->allUserInfo(userList_);

    emit pageAction(9);
}

void UICommandHandler::invokeCreateUserStage1(QString name, QString userid)
{
    bool errorset =  false;

    if(name.length() > 16 || name.length() < 4)
    {
        qDebug() << "length error";
        errorset = true;
    }

    for(int x = 0; x < name.length(); x++)
    {
        char ch = name.at(x).toLatin1();

        if(isalnum(ch) == 0)
        {
            if(ch != ' ')
            {
                qDebug() << "string error" << name.at(x);
                errorset = true;
                break;
            }
        }
    }

    if(errorset)
    {
        emit userNameError();
        return;
    }

    newUName = name;
    newConsId = userid;

    emit pageAction(11);
}

void UICommandHandler::invokeCreateUserStage2(QString passcode)
{
    newPassCode1 = passcode;
    emit pageAction(12);
}

void UICommandHandler::invokeCreateUserStage3(QString passcode)
{
    newPassCode2 = passcode;

    if(newPassCode1 != newPassCode2)
    {
        emit passCodeMismatch();
        return;
    }

    UserInfo u1;
    u1.ConsumerId_ = newConsId;
    u1.IsFirstLogin_ = true;
    u1.PassCode1_ = newPassCode1;
    u1.PassCode2_ = newPassCode2;
    u1.UserId_ = newUName;

    userinfoEntityPtr->createUserInfo(u1);

    userList_.clear();
    userinfoEntityPtr->allUserInfo(userList_);

    currentUser = u1;
    emit CurrentUserChanged();

    emit pageAction(9);
}

void UICommandHandler::invokeResetPasscodeStage1(QString userid)
{
    resetConsId = userid;
    emit pageAction(15);
}

void UICommandHandler::invokeResetPasscodeStage2(QString passcode)
{
    resetPassCode1 = passcode;
    emit pageAction(16);
}

void UICommandHandler::invokeResetPasscodeStage3(QString passcode)
{
    resetPassCode2 = passcode;

    qDebug() << resetConsId << resetPassCode1 << resetPassCode2;
    qDebug() << currentUser.PassCode1_ << currentUser.PassCode2_;

    if(resetPassCode1 != resetPassCode2)
    {
        emit passCodeMismatch();
        return;
    }

    foreach (auto user, userList_)
    {
        if(user.ConsumerId_ == resetConsId)
        {
            break;
        }
    }

    userinfoEntityPtr->update("consumer_id", resetConsId, "passcode1", resetPassCode1);
    userinfoEntityPtr->update("consumer_id", resetConsId, "passcode2", resetPassCode2);

    userList_.clear();
    userinfoEntityPtr->allUserInfo(userList_);

    if(userList_.count() > 0)
    {
        currentUser = userList_.at(0);
    }

    qDebug() << currentUser.PassCode1_ << currentUser.PassCode2_;

    emit CurrentUserChanged();

    emit pageAction(9);
}

void UICommandHandler::OnScanComplete()
{
    if(is_adding_meter || is_connecting_meter)
    {
        qDebug() << "Scan complete";
        qDebug() << QDateTime::currentDateTime().time().toString();
        emit meterFound(false);
    }
}

void UICommandHandler::OnDeviceFound(const QBluetoothDeviceInfo &device)
{
    qDebug() << "New LE device";
    //Determine the exposed services
    bool has_service = false;

    QList<QBluetoothUuid> servicelist = device.serviceUuids();
    for(int ctr = 0; ctr < servicelist.count(); ctr++)
    {
        QString temp = servicelist[ctr].toString(QUuid::WithoutBraces).toUpper();

        if(temp == peripheralServiceIdString_)
        {
            has_service = true;
            qDebug() << "Device found with requested service";
            break;
        }
    }

    if(!has_service)
    {
        return;
    }

    // Determine the system title using the manufacturer data
    bool has_device = false;
    QMultiHash<quint16, QByteArray> dataset = device.manufacturerData();
    QList<quint16> kl = dataset.keys();

    if(kl.count() > 0)
    {
        QByteArray brr = dataset.value(kl.at(0));
        unsigned char temp_syst[8] = {'E', 'D', 0, 0, 0, 0, 0, 0};
        memcpy(&temp_syst[2], brr.data(), 6);

        if(memcmp(temp_syst, system_title, 8) == 0)
        {
            has_device = true;
            qDebug() << "Target device found ...";
        }
        else
        {
            qDebug() << "No match ...";
            qDebug() << Helpers::BytesToHex((unsigned char*)temp_syst, 8, ' ');
            qDebug() << Helpers::BytesToHex((unsigned char*)system_title, 8, ' ');
        }
    }
    else
    {
        qDebug() << "Missing manufacturer data ...";
    }

    if(!has_device)
    {
        return;
    }

    scanTimer.stop();
    scanner.stop();

    if(is_adding_meter || is_connecting_meter)
    {
        if(is_adding_meter)
        {
            emit meterFound(true);
        }

        currentMeter.RSSI_ = device.rssi();
        rssi = device.rssi();
        connectTimer.start(15*1000);
        central.SetPeripheralDevice(device, EDMI_SERVICE_UUID);
        central.ConnectPeripheral();
    }
}

void UICommandHandler::OnDeviceConnected(const QString &name)
{
    connectTimer.stop();
    bool retval = false;

    AppLogInformation("Bluetooth Connected");

    if(is_adding_meter)
    {
        workflow = addMeterWF;
        connect(addMeterWF, SIGNAL(Completed(QList<QVariant>, WorkflowName, bool)), this, SLOT(OnCompleted(QList<QVariant>, WorkflowName, bool)));
        addMeterWF->SetCentral(&central);
        QList<QVariant> startParamters;
        //System title
        startParamters.append(QVariant(QByteArray((char*)system_title, 8)));
        //Network identity
        startParamters.append(QVariant(name.toUtf8()));
        addMeterWF->StartWorkFlow(startParamters);
    }

    if(is_connecting_meter)
    {
        Meter mt;
        bool found = false;
        foreach(Meter mtr , meterList_)
        {
            if(mtr.SerialNo_ == connect_serial_no)
            {
                mt = mtr;
                found = true;
                break;
            }
        }

        if(found)
        {
            qDebug() << "SYSTEM TITLES" << mt.SystemTitle_ << Helpers::BytesToHex(system_title, 8, ' ').c_str();
            workflow = connectMeterWF;
            connect(connectMeterWF, SIGNAL(Completed(QList<QVariant>, WorkflowName, bool)), this, SLOT(OnCompleted(QList<QVariant>, WorkflowName, bool)));
            connectMeterWF->SetCentral(&central);
            QList<QVariant> startParamters;
            startParamters.append(QVariant(QByteArray((char*)system_title, 8)));
            startParamters.append(QVariant(false));
            startParamters.append(QVariant(mt.Keys103_));
            startParamters.append(QVariant(mt.SerialNo_));
            connectMeterWF->StartWorkFlow(startParamters);
        }
    }
}

void UICommandHandler::OnSendKeepAlive()
{
    if(!isAppConnected_)
    {
        return;
    }

    is_keep_alive_pending = true;
    workflow = keepAliveWF;
    connect(keepAliveWF, SIGNAL(Completed(QList<QVariant>, WorkflowName, bool)), this, SLOT(OnCompleted(QList<QVariant>, WorkflowName, bool)));
    keepAliveWF->SetCentral(&central);
    QList<QVariant> startParamters;
    keepAliveTimer.stop();
    keepAliveWF->StartWorkFlow(startParamters);
    qDebug() << "Timer event";
}

void UICommandHandler::OnDataReceived(const QByteArray& data)
{
    workflow->ProcessReceivedFrame(data);
}

void UICommandHandler::OnDataSent(uint32_t len)
{

}

void  UICommandHandler::OnScanTimeout()
{
    qDebug() << "Scannner Timer event";
    scanTimer.stop();
    scanner.stop();

    if(is_adding_meter)
    {
        emit meterAdditionProgress("Scan timed out");
        QThread::currentThread()->sleep(2);
        emit meterFound(false);
        emit meterAuthenticated(false);
        emit meterCheckConfiguration(false);
        return;
    }

    if(is_connecting_meter)
    {
        emit connectionProgress("Scan timed out");
        QThread::currentThread()->sleep(2);
        emit meterError("Communication timed out. Please retry");
        return;
    }
}

void UICommandHandler::OnDeviceDisconnected()
{
    AppLogInformation("Bluetooth disconnected");

    if(isAppConnected_)
    {
        OnCommunicationTimeout();
    }
}

void UICommandHandler::OnCompleted(QList<QVariant> completionParamters, WorkflowName wfn, bool status)
{
    if(wfn == Addmeter)
    {
        if(status)
        {
            scanner.stop();

            // Disconnect BLE
            if(central.IsConnected())
            {
                central.DisconnectPeripheral();
            }

            int ts = QDateTime::currentDateTime().toSecsSinceEpoch();
            appSyncTime_ = QDateTime::fromSecsSinceEpoch(ts).toString("dd MMM yy hh:mm");
            emit AppSyncTimeChanged();

            Meter mt;
            
            mt.Status_ = MeterConnectionState::NotConnected;
            mt.LastSync_ = QDateTime::currentDateTime().toSecsSinceEpoch();
            mt.RSSI_ = rssi;
            mt.ModelNo_ = "Mk7Mi";
            mt.BlePIN_ = "";
            mt.CIUPIN_ = pin_csa_customer;

            mt.SystemTitle_ = completionParamters.at(0).toString();
            mt.BleIdentifier_ = completionParamters.at(1).toString();

            mt.Keys103_ = completionParamters.at(2).toString();
            mt.Keys104_ = completionParamters.at(3).toString();
            mt.Keys105_ = completionParamters.at(4).toString();
            mt.SerialNo_ = completionParamters.at(5).toString();
            mt.MeterName_ = completionParamters.at(6).toString();

            StatisticsEnergy egr1;
            egr1.RecordTimestamp_ =  completionParamters.at(7).toUInt();
            egr1.ActiveImportTariff1_ = completionParamters.at(8).toFloat();
            egr1.ActiveExportTariff1_ = completionParamters.at(9).toFloat();
            egr1.ReactiveImportTariff1_ = completionParamters.at(10).toFloat();
            egr1.ReactiveExportTariff1_ = completionParamters.at(11).toFloat();
            egr1.ActiveImportTariff2_ = 0;
            egr1.ActiveExportTariff2_ = 0;
            egr1.ReactiveImportTariff2_ = 0;
            egr1.ReactiveExportTariff2_ = 0;
            egr1.RowIndex_ = 1;
            egr1.LoadProfileType_ = 0;
            egr1.SerialNo_ = mt.SerialNo_;

            StatisticsEnergy egr2;
            egr2.RecordTimestamp_ =  completionParamters.at(12).toUInt();
            egr2.ActiveImportTariff1_ = completionParamters.at(13).toFloat();
            egr2.ActiveExportTariff1_ = completionParamters.at(14).toFloat();
            egr2.ReactiveImportTariff1_ = completionParamters.at(15).toFloat();
            egr2.ReactiveExportTariff1_ = completionParamters.at(16).toFloat();
            egr2.ActiveImportTariff2_ = 0;
            egr2.ActiveExportTariff2_ = 0;
            egr2.ReactiveImportTariff2_ = 0;
            egr2.ReactiveExportTariff2_ = 0;
            egr2.RowIndex_ = 1;
            egr2.LoadProfileType_ = 1;
            egr2.SerialNo_ = mt.SerialNo_;

            MeterSettings ms0;
            ms0.SerialNo_ = mt.SerialNo_;
            ms0.Key_ = ACCOUNT_TYPE;
            ms0.Value_ = completionParamters.at(17).toString();

            MeterSettings ms1;
            ms1.SerialNo_ = mt.SerialNo_;
            ms1.Key_ = ACCOUNT_CURRENCY;
            ms1.Value_ = completionParamters.at(18).toString();

            MeterSettings ms2;
            ms2.SerialNo_ = mt.SerialNo_;
            ms2.Key_ = ACCOUNT_CURRENCY_SCALAR;
            ms2.Value_ = completionParamters.at(19).toString();

            MeterSettings ms3;
            ms3.SerialNo_ = mt.SerialNo_;
            ms3.Key_ = ACTIVE_ENERGY_SCALAR;
            ms3.Value_ = completionParamters.at(20).toString();

            MeterSettings ms4;
            ms4.SerialNo_ = mt.SerialNo_;
            ms4.Key_ = INSTANTENOUS_POWER_SCALAR;
            ms4.Value_ = completionParamters.at(21).toString();

            MeterSettings ms5;
            ms5.SerialNo_ = mt.SerialNo_;
            ms5.Key_ = ACTIVE_IMPORT_SCALAR;
            ms5.Value_ = completionParamters.at(22).toString();

            MeterSettings ms6;
            ms6.SerialNo_ = mt.SerialNo_;
            ms6.Key_ = ACTIVE_EXPORT_SCALAR;
            ms6.Value_ = completionParamters.at(23).toString();

            MeterSettings ms7;
            ms7.SerialNo_ = mt.SerialNo_;
            ms7.Key_ = REACTIVE_EXPORT_SCALAR;
            ms7.Value_ = completionParamters.at(24).toString();

            MeterSettings ms8;
            ms8.SerialNo_ = mt.SerialNo_;
            ms8.Key_ = REACTIVE_IMPORT_SCALAR;
            ms8.Value_ = completionParamters.at(25).toString();

            // Set all connected meters to disconnected mode
            for(int x = 0; x < meterList_.count(); x++)
            {
                if(meterList_.at(x).Status_ == MeterConnectionState::Connected)
                {
                    meterEntityPtr->update("serial_no", meterList_.at(x).SerialNo_, "meter_status", MeterConnectionState::NotConnected);
                }
            }

            meterEntityPtr->createMeter(mt);          
            statisticsEnergyEntityPtr->createStatisticsEnergy(egr1);
            statisticsEnergyEntityPtr->createStatisticsEnergy(egr2);
            meterSettingsEntityPtr->createMeterSettings(ms0);
            meterSettingsEntityPtr->createMeterSettings(ms1);
            meterSettingsEntityPtr->createMeterSettings(ms2);
            meterSettingsEntityPtr->createMeterSettings(ms3);
            meterSettingsEntityPtr->createMeterSettings(ms4);
            meterSettingsEntityPtr->createMeterSettings(ms5);
            meterSettingsEntityPtr->createMeterSettings(ms6);
            meterSettingsEntityPtr->createMeterSettings(ms7);
            meterSettingsEntityPtr->createMeterSettings(ms8);

            meterList_.clear();
            meterEntityPtr->allMeters(meterList_);
            UpdateCurrentMeterTimestamp();

            is_connecting_meter = false;
            is_adding_meter = false;
            isAppConnected_ = false;

            emit MeterListChanged();
            emit meterAuthenticated(status);
            emit MinTimestampLP1Changed();
            emit MaxTimestampLP1Changed();
            emit MinTimestampLP2Changed();
            emit MaxTimestampLP2Changed();
            emit meterCheckConfiguration(status);
            emit IsAppConnectedChanged();
        }
        else
        {
            emit meterAuthenticated(status);
            emit meterCheckConfiguration(status);

            is_connecting_meter = false;
            is_adding_meter = false;
            scanner.stop();
            central.DisconnectPeripheral();

            isAppConnected_ = false;
        }

        disconnect(addMeterWF, SIGNAL(Completed(QList<QVariant>, WorkflowName, bool)), this, SLOT(OnCompleted(QList<QVariant>, WorkflowName, bool)));
    }

    if(wfn == ConnectMeter)
    {
        if(status)
        {
            int ts = QDateTime::currentDateTime().toSecsSinceEpoch();
            appSyncTime_ = QDateTime::fromSecsSinceEpoch(ts).toString("dd MMM yy hh:mm");
            emit AppSyncTimeChanged();

            QString serialNo = completionParamters.at(0).toString();
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

            if(completionParamters.count() > 0)
            {
                grossCredit_ = completionParamters.at(1).toInt();
                availableCredit_ = completionParamters.at(2).toInt();
                deliveredEnergy_ = completionParamters.at(3).toUInt();
                instantaneousPower_ = completionParamters.at(4).toUInt();

                if(availableCredit_ > grossCredit_)
                {
                        grossCredit_ = availableCredit_ * 2;
                }

                deliveredEnergy_ = deliveredEnergy_ * pow(10, deliveredEnergy_scalar);
                instantaneousPower_ = instantaneousPower_ * pow(10, instantaneousPower_scalar);
                availableCredit_ = availableCredit_ *  pow(10, global_scalar_);
                grossCredit_ = grossCredit_ * pow(10, global_scalar_);

                // Grab and set the current meter
                for(int x = 0; x < meterList_.count(); x++)
                {
                    if(meterList_.at(x).SerialNo_ == serialNo)
                    {
                        meterEntityPtr->update("serial_no", meterList_.at(x).SerialNo_, "meter_status", MeterConnectionState::Connected);
                        currentMeter = meterList_.at(x);
                        currentMeter.IsSelected_ = true;
                        emit IsMeterActiveChanged();
                    }
                    else
                    {
                        meterEntityPtr->update("serial_no", meterList_.at(x).SerialNo_, "meter_status", MeterConnectionState::NotConnected);
                    }
                }

                meterList_.clear();
                meterEntityPtr->allMeters(meterList_);

                activeAlarms_.clear();
                tokenList_.clear();
                tariffList_.clear();
                alarmEntityPtr->selectedAlarms(activeAlarms_, "serial_no", currentMeter.SerialNo_);
                tokenEntityPtr->selectedTokens(tokenList_, "serial_no", currentMeter.SerialNo_);
                tariffEntityPtr->selectedTariffs(tariffList_, "serial_no", currentMeter.SerialNo_);

                emit MeterListChanged();
                emit meterAction();
                emit ActiveAlarmsChanged();
                emit TokenListChanged();
                emit TariffListChanged();
                emit PaymentTypeChanged();
                emit DeliveredEnergyChanged();
                emit InstantaneousPowerChanged();
                emit AvailableCreditChanged();
                emit GrossCreditChanged();
                emit CreditCurrencyChanged();

                UpdateCurrentMeterTimestamp();

                isAppConnected_ = true;
                emit IsAppConnectedChanged();

                keepAliveTimer.start(15*1000);
                emit pageAction(13);

                uint32_t minlp1, maxlp1, minlp2, maxlp2;
                uint32_t x, y;
                statisticsEnergyEntityPtr->getRanges( currentMeter.SerialNo_, x, y, minlp1, maxlp1, 0);
                statisticsEnergyEntityPtr->getRanges( currentMeter.SerialNo_, x, y, minlp2, maxlp2, 1);

                minTimestampLP1_ = QDateTime::fromSecsSinceEpoch(minlp1).toString("dd MMM yyyy, hh:mm");
                maxTimestampLP1_ = QDateTime::fromSecsSinceEpoch(maxlp1).toString("dd MMM yyyy, hh:mm");
                minTimestampLP2_ = QDateTime::fromSecsSinceEpoch(minlp2).toString("dd MMM yyyy, hh:mm");
                maxTimestampLP2_ = QDateTime::fromSecsSinceEpoch(maxlp2).toString("dd MMM yyyy, hh:mm");

                emit MinTimestampLP1Changed();
                emit MaxTimestampLP1Changed();
                emit MinTimestampLP2Changed();
                emit MaxTimestampLP2Changed();
            }
            else
            {
                qDebug() << currentMeter.SerialNo_ << " disconnected";
                meterEntityPtr->update("serial_no", currentMeter.SerialNo_, "meter_status", MeterConnectionState::NotConnected);
                meterList_.clear();
                meterEntityPtr->allMeters(meterList_);
                emit MeterListChanged();
                emit meterAction();
                central.DisconnectPeripheral();

                isAppConnected_ = false;
                emit IsAppConnectedChanged();
            }
        }
        else
        {
            isAppConnected_ = false;
            emit meterError("Could not connect to meter");
        }

        scanner.stop();
        disconnect(connectMeterWF, SIGNAL(Completed(QList<QVariant>, WorkflowName, bool)), this, SLOT(OnCompleted(QList<QVariant>, WorkflowName, bool)));
    }

    if(wfn == DisconnectMeter)
    {
        isAppConnected_ = false;
        is_keep_alive_pending = false;
        central.DisconnectPeripheral();

        if(status)
        {
            meterEntityPtr->allMeters(meterList_);
            for(int x = 0; x < meterList_.count(); x++)
            {
                if(meterList_.at(x).SerialNo_ == currentMeter.SerialNo_)
                {
                    meterEntityPtr->update("serial_no", meterList_.at(x).SerialNo_, "meter_status", MeterConnectionState::NotConnected);
                }
            }
            meterList_.clear();
            meterEntityPtr->allMeters(meterList_);

            for(int x = 0; x < meterList_.count(); x++)
            {
                if(meterList_.at(x).SerialNo_ == currentMeter.SerialNo_)
                {
                    Meter mt = meterList_.at(x);
                    mt.IsSelected_ = true;
                    meterList_[x] = mt;
                    currentMeter = mt;
                }
            }

            emit MeterListChanged();
            emit CurrentMeterChanged();
        }

        scanner.stop();
        disconnect(connectMeterWF, SIGNAL(Completed(QList<QVariant>, WorkflowName, bool)), this, SLOT(OnCompleted(QList<QVariant>, WorkflowName, bool)));
    }

    if(wfn == TokenTransfer)
    {
        int ts = QDateTime::currentDateTime().toSecsSinceEpoch();
        appSyncTime_ = QDateTime::fromSecsSinceEpoch(ts).toString("dd MMM yy hh:mm");
        emit AppSyncTimeChanged();

        QString tokenstr = completionParamters.at(0).toString();
        uint32_t tokenStatus = completionParamters.at(1).toInt();
        QString tokenstatusstr = completionParamters.at(2).toString();

        tokenEntityPtr->remove("token_id", tokenstr);
        UpdateCurrentMeterTimestamp();
        emit tokenTransferState(tokenstatusstr, status);

        tokenList_.clear();
        tokenEntityPtr->selectedTokens(tokenList_, "serial_no", currentMeter.SerialNo_);
        emit TokenListChanged();
        emit tokenAction();

        disconnect(tokenTransferWF, SIGNAL(Completed(QList<QVariant>, WorkflowName, bool)), this, SLOT(OnCompleted(QList<QVariant>, WorkflowName, bool)));
        keepAliveTimer.start(15*1000);
    }

    if(wfn == KeepAlive)
    {
        int ts = QDateTime::currentDateTime().toSecsSinceEpoch();
        appSyncTime_ = QDateTime::fromSecsSinceEpoch(ts).toString("dd MMM yy hh:mm");
        emit AppSyncTimeChanged();

        disconnect(keepAliveWF, SIGNAL(Completed(QList<QVariant>, WorkflowName, bool)), this, SLOT(OnCompleted(QList<QVariant>, WorkflowName, bool)));
        is_keep_alive_pending = false;

        if(status)
        {
            qDebug() << "KEEP ALIVE COMPLETED";
            deliveredEnergy_ = completionParamters.at(0).toUInt();
            instantaneousPower_ = completionParamters.at(1).toUInt();
            availableCredit_ = completionParamters.at(2).toUInt();
            grossCredit_ = completionParamters.at(3).toUInt();

            if(availableCredit_ > grossCredit_)
            {
                grossCredit_ = availableCredit_*2;
            }

            deliveredEnergy_ = deliveredEnergy_ * pow(10, deliveredEnergy_scalar);
            instantaneousPower_ = instantaneousPower_ * pow(10, instantaneousPower_scalar);
            availableCredit_ = availableCredit_ *  pow(10, global_scalar_);
            grossCredit_ = grossCredit_ *  pow(10, global_scalar_);

            emit DeliveredEnergyChanged();
            emit InstantaneousPowerChanged();
            emit AvailableCreditChanged();
            emit GrossCreditChanged();

            QString alarms = completionParamters.at(3).toString();
            char* ptr = (char*)calloc(1, alarms.length()+1);
            memcpy(ptr, alarms.toStdString().c_str(), alarms.length());

            activeAlarms_.clear();
            QList<Alarm> curralarms;

            for(int x = 0; x < alarmFilter_.count(); x++)
            {
                if(ptr[x] == '1' && alarmFilter_.at(x).IsSet_)
                {
                    curralarms.append(Alarm(alarmFilter_[x].AlarmName_, currentMeter.SerialNo_, x, QDateTime::currentDateTime().currentSecsSinceEpoch()));
                }
            }

            setActiveAlarms(curralarms);

            alarmEntityPtr->remove("serial_no", currentMeter.SerialNo_);

            for(int x = 0; x < curralarms.count(); x++)
            {
                Alarm a1;

                a1.AlarmId_ = curralarms[x].AlarmId_;
                a1.AlarmName_ = curralarms[x].AlarmName_;
                a1.AlarmTimestamp_ = curralarms[x].AlarmTimestamp_;
                a1.SerialNo_ = currentMeter.SerialNo_;

                alarmEntityPtr->createAlarm(a1);
            }

            tokenList_.clear();

            tokenEntityPtr->selectedTokens(tokenList_, "serial_no", currentMeter.SerialNo_);

            free(ptr);
            emit alarmAction(true);
            UpdateCurrentMeterTimestamp();

            uint32_t minlp1, maxlp1, minlp2, maxlp2;
            uint32_t x, y;
            statisticsEnergyEntityPtr->getRanges( currentMeter.SerialNo_, x, y, minlp1, maxlp1, 0);
            statisticsEnergyEntityPtr->getRanges( currentMeter.SerialNo_, x, y, minlp2, maxlp2, 1);

            minTimestampLP1_ = QDateTime::fromSecsSinceEpoch(minlp1).toString("dd MMM yyyy, hh:mm");
            maxTimestampLP1_ = QDateTime::fromSecsSinceEpoch(maxlp1).toString("dd MMM yyyy, hh:mm");
            minTimestampLP2_ = QDateTime::fromSecsSinceEpoch(minlp2).toString("dd MMM yyyy, hh:mm");
            maxTimestampLP2_ = QDateTime::fromSecsSinceEpoch(maxlp2).toString("dd MMM yyyy, hh:mm");

            emit MinTimestampLP1Changed();
            emit MaxTimestampLP1Changed();
            emit MinTimestampLP2Changed();
            emit MaxTimestampLP2Changed();
            emit TokenListChanged();
            emit tokenAction();

            if(is_disconect_pending)
            {
                workflow = connectMeterWF;
                connect(connectMeterWF, SIGNAL(Completed(QList<QVariant>, WorkflowName, bool)), this, SLOT(OnCompleted(QList<QVariant>, WorkflowName, bool)));
                connectMeterWF->SetCentral(&central);
                QList<QVariant> startParamters;
                startParamters.append(QVariant(QByteArray((char*)system_title, 8)));
                startParamters.append(QVariant(true));
                keepAliveTimer.stop();
                connectMeterWF->StartWorkFlow(startParamters);
            }
            else
            {
                if(is_token_transfer_pending)
                {
                    is_token_transfer_pending = false;
                    workflow = tokenTransferWF;
                    connect(tokenTransferWF, SIGNAL(Completed(QList<QVariant>, WorkflowName, bool)), this, SLOT(OnCompleted(QList<QVariant>, WorkflowName, bool)));
                    tokenTransferWF->SetCentral(&central);
                    QList<QVariant> startParameters;
                    startParameters.append(token_pending);
                    tokenTransferWF->StartWorkFlow(startParameters);
                }
                else
                {
                    keepAliveTimer.start(15000);
                }
            }
        }
        else
        {
            qDebug() << "METER IS NOT RESPONDING";
            isAppConnected_ = false;
            is_keep_alive_pending = false;
            central.DisconnectPeripheral();

            if(status)
            {
                meterEntityPtr->allMeters(meterList_);
                for(int x = 0; x < meterList_.count(); x++)
                {
                    if(meterList_.at(x).SerialNo_ == currentMeter.SerialNo_)
                    {
                        meterEntityPtr->update("serial_no", meterList_.at(x).SerialNo_, "meter_status", MeterConnectionState::NotConnected);
                    }
                }
                meterList_.clear();
                meterEntityPtr->allMeters(meterList_);

                for(int x = 0; x < meterList_.count(); x++)
                {
                    if(meterList_.at(x).SerialNo_ == currentMeter.SerialNo_)
                    {
                        Meter mt = meterList_.at(x);
                        mt.IsSelected_ = true;
                        meterList_[x] = mt;
                        currentMeter = mt;
                    }
                }

                emit MeterListChanged();
                emit CurrentMeterChanged();
                emit pageAction(4);
            }
        }
    }
}

bool UICommandHandler::IsFloatString(const QString& str)
{
    int len = str.length();

    for(int x = 0; x < len; x++)
    {
        char ch = str.at(x).toLatin1();

        if(!isdigit(ch))
        {
            if(ch != '.')
            {
                return false;
            }
        }
    }

    return true;
}

void UICommandHandler::UpdateCurrentMeterTimestamp()
{
    int ts = QDateTime::currentDateTime().toSecsSinceEpoch();

    for(int x = 0; x < meterList_.count(); x++)
    {
        Meter mx = meterList_[x];
        if(meterList_.at(x).SerialNo_ == currentMeter.SerialNo_)
        {
            meterEntityPtr->update("serial_no", meterList_.at(x).SerialNo_, "last_sync", ts);
            currentMeter.LastSync_ = ts;
            currentMeter.LastSyncStr_ = QDateTime::fromSecsSinceEpoch(ts).toString("dd MMM yy hh:mm");
        }
        meterList_[x] = mx;
    }
}

QString  UICommandHandler::getAppSyncTime()
{
    return appSyncTime_;
}

float UICommandHandler::getTariff(unsigned long tstamp)
{
    if(tariffList_.count() < 1)
    {
        return 0;
    }

    if(tariffList_.count() == 1)
    {
        if(tariffList_.at(0).Import1_ <= tstamp)
        {
            return tariffList_.at(0).Import1_;
        }
        else
        {
            return 0;
        }
    }

    if(tariffList_.count() > 1)
    {
        if(tstamp > tariffList_.at(tariffList_.count() - 1).Import1_)
        {
            return tariffList_.at(tariffList_.count() - 1).Import1_;
        }
    }

    for(int x = 0; x < tariffList_.count(); x++)
    {
        if(tstamp < tariffList_.at(x).EffectiveDate_)
        {
            return tariffList_.at(x-1).Import1_;
        }
    }

    return 0;
}

void UICommandHandler::OnConnectTimeout()
{
    qDebug() << "CONNECT STEP TIMED OUT";
    central.DisconnectPeripheral();
    connectTimer.stop();

    if(is_adding_meter)
    {
        emit meterAuthenticated(false);
    }

    if(is_connecting_meter)
    {
        emit meterError("Connection and pairing timed out.\nPlease retry");
    }
}

void UICommandHandler::OnCommunicationTimeout()
{
    central.DisconnectPeripheral();

    meterEntityPtr->allMeters(meterList_);
    for(int x = 0; x < meterList_.count(); x++)
    {
        if(meterList_.at(x).SerialNo_ == currentMeter.SerialNo_)
        {
            meterEntityPtr->update("serial_no", meterList_.at(x).SerialNo_, "meter_status", MeterConnectionState::NotConnected);
        }
    }
    meterList_.clear();
    meterEntityPtr->allMeters(meterList_);

    for(int x = 0; x < meterList_.count(); x++)
    {
        if(meterList_.at(x).SerialNo_ == currentMeter.SerialNo_)
        {
            Meter mt = meterList_.at(x);
            mt.IsSelected_ = true;
            meterList_[x] = mt;
            currentMeter = mt;
        }
    }

    emit MeterListChanged();
    emit CurrentMeterChanged();


    QString err = "Communication timed out. Please retry";

    if(is_adding_meter)
    {
        emit meterAuthenticated(false);
        emit meterCheckConfiguration(false);
        return;
    }

    if(is_connecting_meter)
    {
        emit meterError(err);
        return;
    }

    if(lastPage_ == "Alarms.qml")
    {
        emit alarmError(err);
        return;
    }

    if(lastPage_ == "Dashboard.qml")
    {
        emit dashboardError("Communication timed out. Please retry");
        return;
    }

    if(lastPage_ == "AvailableTokens.qml")
    {
        emit tokenTransferState(err, false);
        return;
    }
}
