#include "Mk7Mi.hpp"
#include "Controller.hpp"
#include "Helpers.h"

unsigned char system_title[8] = {0};

char pin_ble_pairing[7] = {'0','0','0','0','0','0', 0};
char pin_csa_customer[9] = {0};
QString rssi;

QString connect_serial_no;
Controller* controllerInstance = nullptr;
QString token_pending;

// Network state flags
bool is_meter_found = false;
bool is_adding_meter = false;
bool is_connecting_meter = false;
bool is_keep_alive_pending = false;
bool is_disconnect_pending = false;
bool is_ble_disconect_requested = false;
bool is_token_transfer_pending = false;
// Network state flags

int scan_tries = 0;
const int max_scan_retries = 5;

int connect_tries = 0;
const int max_connect_retries = 5;

int keep_alive_flow_failure_count = 0;

QString discovered_address;
QString discovered_name;
QList<QString> scan_results;

void Controller::invokeAddMeter(QString token)
{
    if(activeWF_ != Idle)
    {
        emit meterPairingCodeVerification(false, "App busy, please try later");
        return;
    }

    //if(btInterfacePtr->IsPowered() == false)
    //{
    //    emit meterPairingCodeVerification(false, "Please turn on Bluetooth and retry");
    //    return;
    //}

    deliveredEnergy_ = 0;
    instantaneousPower_ = 0;
    usedCredit_ = 0;
    grossCredit_ = 0;

    ClearTrace();

    qrtext_ = "";
    emit QRTextChanged();

    memset(pin_ble_pairing, 0, sizeof(pin_ble_pairing));
    memset(pin_csa_customer, 0, sizeof(pin_csa_customer));

    is_connecting_meter = false;
    is_adding_meter = true;
    is_meter_found = false;
    is_disconnect_pending = false;
    is_keep_alive_pending = false;

    int tlen = 0;

    if(token.length() > 29 || token.length() < 9)
    {
        Trace("Length check failed for pass code");
        emit meterPairingCodeVerification(false, "Validation failed");
        is_connecting_meter = false;
        is_adding_meter = false;
        return;
    }

    if(token.length() <= 15)
    {
        token += "00000000000000";
    }

    char* ptr = new char[token.toStdString().length()+1];
    memset(ptr, 0, token.toStdString().length()+1);
    memcpy(ptr, token.toStdString().c_str(), token.toStdString().length());

    std::string system_title_str;

    tlen = token.toStdString().length() - 14;
    char* system_title_base10 = new char[tlen+1];
    memset(system_title_base10, 0, tlen+1);

    emit meterPairingCodeVerification(true, "Validation okay");

    memcpy(system_title_base10, ptr, tlen);
    memcpy(pin_ble_pairing, &ptr[20], 6);
    memcpy(pin_csa_customer, &ptr[26], 8);

    delete [] ptr;
    ptr = nullptr;
    system_title_str = Helpers::hexString(system_title_base10);
    delete [] system_title_base10;

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
            emit meterPairingCodeVerification(false, "Meter exists");
            is_connecting_meter = false;
            is_adding_meter = false;
            return;
        }
    }
    
    Trace("Starting BLE scan");
    Trace("Scanning for " + QString(syst_hex_str.c_str()));
    emit progressIndicatorsOn();
    scan_tries = 0;
    connect_tries = 0;
    discovered_address = "";
    discovered_name = "";
    activeWF_ = AddMeter;
    scan_results.clear();
    scan_results.clear();
    btInterfacePtr->StartScanning(15000);
}

void Controller::invokeConnectMeter(QString serialNo)
{
    if(activeWF_ != Idle)
    {
        emit meterMessage(false, "App busy, please try later");
        emit meterAction(true);
        return;
    }

    //if(btInterfacePtr->IsPowered() == false)
    //{
    //    emit meterMessage(false, "Please turn on Bluetooth and retry");
    //    emit meterAction(true);
    //    return;
    //}

    selectMeter(serialNo);

    QString str = "Connecting to " + currentMeterSerialNo_;

    Trace(str);

    deliveredEnergy_ = 0;
    instantaneousPower_ = 0;
    usedCredit_ = 0;
    grossCredit_ = 0;

    ClearTrace();

    isAppConnected_ = false;
    emit IsAppConnectedChanged();

    is_connecting_meter = false;
    is_adding_meter = false;
    is_meter_found = false;
    is_disconnect_pending = false;
    is_keep_alive_pending = false;

    Meter mt;
    bool found = false;
    foreach(Meter mtr , meterList_)
    {
        if(mtr.SerialNo_ == currentMeterSerialNo_)
        {
            mt = mtr;
            connect_serial_no = currentMeterSerialNo_;
            found = true;
            break;
        }
    }

    if(found)
    {
        deliveredEnergy_ = 0;
        instantaneousPower_ = 0;
        usedCredit_ = 0;
        grossCredit_ = 0;

        ByteBuffer buff;
        std::string syst_hex = mt.SystemTitle_.toStdString();
        Helpers::HexToBytes(syst_hex, buff);

        memset(system_title, 0, 8);
        memcpy(system_title, buff.GetData(), buff.GetSize());

        memset(pin_csa_customer, 0, 9);
        memcpy(pin_csa_customer, mt.CustomerPIN_.toStdString().c_str(), 8);

        memset(pin_ble_pairing, 0, 7);
        memcpy(pin_ble_pairing, mt.PairingPIN_.toStdString().c_str(), 6);

        std::string syst_hex_str = Helpers::BytesToHex(system_title, 8, 0);
        emit progressIndicatorsOn();
        emit workflowProgress(0);
        scan_tries = 0;
        connect_tries = 0;
        discovered_address = "";
        discovered_name = "";
        activeWF_ = ConnectMeter;
        scan_results.clear();
        scan_results.clear();
        btInterfacePtr->StartScanning(15000);
        is_connecting_meter = true;
    }
}

void Controller::invokeDisconnectMeter(QString serialNo)
{  
    if(!isAppConnected_)
    {
        return;
    }

    if(is_keep_alive_pending)
    {
        is_disconnect_pending = true;
        return;
    }
    else
    {
        keepAliveTimer.stop();
        is_connecting_meter = false;
        QList<QVariant> startParamters;
        startParamters.append(QVariant(serialNo));
        emit progressIndicatorsOn();
        emit workflowProgress(0);
        activeWF_ = DisconnectMeter;
        disconnectMeterWF.StartWorkFlow(startParamters);
    }
}

void Controller::invokeApplyToken(QString tokenId)
{   
    if(activeWF_ != Idle)
    {
        emit tokenTransferState(false, "App busy, please try later");
        emit tokenAction();
        return;
    }

    //if(btInterfacePtr->IsPowered() == false)
    //{
    //    emit tokenTransferState(false, "Please turn on Bluetooth and retry");
    //    emit tokenAction();
    //    return;
    //}

    if(!isAppConnected_)
    {
        emit tokenTransferState(false, "Connect the meter first");
        emit tokenAction();
        return;
    }

    Trace("Starting a token transfer");

    if(is_keep_alive_pending)
    {
        is_token_transfer_pending = true;
        token_pending = tokenId;
    }
    else
    {
        keepAliveTimer.stop();
        is_token_transfer_pending = true;
        QList<QVariant> startParameters;
        startParameters.append(tokenId);
        emit progressIndicatorsOn();
        emit workflowProgress(0);
        activeWF_ = TokenTransfer;
        tokenTransferWF.StartWorkFlow(startParameters);
    }
}

// - Scanning results - Start

void Controller::OnDeviceDiscovered(const QString &address, const QString &name, const QString &rssi_str, const QMultiHash<quint16, QByteArray> &mdata)
{
    if(scan_results.contains(address))
    {
        return;
    }

    scan_results.append(address);

    QByteArray mfg_advertised_name;
    foreach(auto mfg, mdata)
    {
        //For EDMI meters, there will be only one entry, i.e. advertised system title
        mfg_advertised_name = mfg;
    }

    // Prefix E and D
    mfg_advertised_name.insert(0, 'D');
    mfg_advertised_name.insert(0, 'E');

    discovered_address = address;
    discovered_name = Helpers::BytesToHex((unsigned char*)mfg_advertised_name.data(), mfg_advertised_name.length(), 0).c_str();

    is_meter_found = false;
    std::string syst_hex_str = Helpers::BytesToHex(system_title, 8, 0);

    if(discovered_name.toStdString() == syst_hex_str)
    {
        Trace("Device found");
        is_meter_found = true;
    }
    else
    {
        Trace("Non matching device " + discovered_address);
    }

    if(is_meter_found)
    {
        scan_tries = 0;
        btInterfacePtr->StopScanTimer();
        btInterfacePtr->StopScanning();
        emit progressIndicatorsOff();

        if(is_adding_meter)
        {
            emit meterFound(true, "Meter found");
        }

        if(is_connecting_meter)
        {
            emit meterMessage(true, "Meter found");
        }

        rssi = rssi_str;
        btInterfacePtr->ResetAdapter();
        is_ble_disconect_requested = false;
        btInterfacePtr->StopScanning();
        btInterfacePtr->Connect(discovered_address, EDMI_SERVICE_UUID, 15000);
    }
}

void Controller::OnScanComplete()
{
    Trace("Scan complete");
    btInterfacePtr->StopScanTimer();

    emit progressIndicatorsOff();
    emit networkIndicatorsOff();
    // This line is necessary, in case scan complete is fired by discovery agent
    if(is_meter_found == false && is_adding_meter)
    {
        if(scan_tries < max_scan_retries)
        {
            retryScan();
            return;
        }

        emit workflowProgress(0);
        emit meterFound(false, "Scan failure");
        emit meterConnected(false, "...");
        emit meterConfigured(false, "...");
        activeWF_ = Idle;
        Trace("Scan timed out while adding meter");
        return;
    }

    if(is_meter_found == false && is_connecting_meter)
    {
        if(scan_tries < max_scan_retries)
        {
            emit meterAction(false);
            retryScan();
            return;
        }

        emit workflowProgress(0);
        emit meterMessage(false, "Scan timed out. Please retry");
        emit meterAction(true);
        activeWF_ = Idle;
        Trace("Scan timed out while connecting meter");
        return;
    }

    if(is_meter_found && is_adding_meter)
    {
        emit meterFound(true, "Meter found");
        return;
    }

    if(is_meter_found && is_connecting_meter)
    {
        emit meterMessage(true, "Meter found");
        return;
    }
}

void Controller::retryScan()
{
    scan_tries++;
    char buffer[65] = {0};
    snprintf(buffer, 64, "Retrying scan %d/%d", scan_tries, max_scan_retries);
    Trace(buffer);

    if(is_adding_meter)
    {
        emit meterFound(true, buffer);
    }

    if(is_connecting_meter)
    {
        emit meterMessage(true, buffer);
        emit meterAction(false);
    }

    QThread::currentThread()->msleep(500);
    emit progressIndicatorsOn();
    emit workflowProgress(0);
    scan_results.clear();
    btInterfacePtr->StartScanning(15000);
}

// - Scanning error conditions - End

// - Communication error conditions - Start

void Controller::OnConnectFailed()
{
    isAppConnected_ = false;
    emit IsAppConnectedChanged();

    QString str = "Connection setup error";
    Trace(str);    char buff[65] = {0};

    btInterfacePtr->StopConnectTimer();
    btInterfacePtr->StopCommTimer();

    emit progressIndicatorsOff();
    emit networkIndicatorsOff();

    if(is_adding_meter)
    {
        if(connect_tries < max_connect_retries)
        {
            snprintf(buff, 64, "Retrying connection %d/%d", connect_tries+1, max_connect_retries);
            emit meterFound(true, "Meter found");
            emit meterConnected(true, buff);
            retryConnect();
        }
        else
        {
            emit meterFound(true, "Meter found");
            emit meterConnected(false, str);
            emit meterConfigured(false, "...");
            activeWF_ = Idle;
        }
        return;
    }

    if(is_connecting_meter)
    {
        if(connect_tries < max_connect_retries)
        {
            snprintf(buff, 64, "Retrying connection %d/%d", connect_tries+1, max_connect_retries);
            emit meterMessage(true, buff);
            emit meterAction(false);
            retryConnect();
        }
        else
        {
            emit meterMessage(false, str + " please retry");
            emit meterAction(true);
            activeWF_ = Idle;
        }
        return;
    }
}

void Controller::OnDeviceDisconnected(const QString &name)
{
    isAppConnected_ = false;
    emit IsAppConnectedChanged();

    emit networkIndicatorsOff();
    setAllMetersDisconnected();
    btInterfacePtr->StopConnectTimer();
    btInterfacePtr->StopCommTimer();
    activeWF_ = Idle;

    QString str = "Bluetooth disconnected";
    Trace(str);

    if(is_ble_disconect_requested)
    {
        return;
    }
    else
    {
        // Disconnected by peer ??
        if(is_disconnect_pending)
        {
            OnDisconnectCompleted(currentMeterSerialNo_, true);
            return;
        }
    }

    char buff[65] = {0};

    if(is_disconnect_pending)
    {
        is_disconnect_pending = false;
        return;
    }

    if(is_adding_meter)
    {
        if(connect_tries < max_connect_retries)
        {
            snprintf(buff, 64, "Retrying connection %d/%d", connect_tries+1, max_connect_retries);
            emit meterConnected(true, buff);
            retryConnect();
        }
        else
        {
            emit meterFound(true, "Meter found");
            emit meterConnected(false, str);
            emit meterConfigured(false, "Not authenticated");
            activeWF_ = Idle;
        }
        return;
    }

    if(is_connecting_meter)
    {
        if(connect_tries < max_connect_retries)
        {
            snprintf(buff, 64, "Retrying connection %d/%d", connect_tries+1, max_connect_retries);
            emit meterMessage(true, buff);
            retryConnect();
        }
        else
        {
            emit meterMessage(false, str + ", please retry.");
            activeWF_ = Idle;
        }
        return;
    }

    if(btInterfacePtr->IsTranceiving())
    {
        handleCommunicationDrop(str);
        return;
    }
}

void Controller::OnCommunicationTimeout()
{
    isAppConnected_ = false;
    emit IsAppConnectedChanged();

    activeWF_ = Idle;
    btInterfacePtr->StopConnectTimer();

    emit progressIndicatorsOff();

    if(is_disconnect_pending)
    {
        setAllMetersDisconnected();
        emit MeterListChanged();
        emit meterAction(true);
        is_disconnect_pending = false;
        is_ble_disconect_requested = true;
        btInterfacePtr->Disconnect(discovered_address);
        activeWF_ = Idle;
        return;
    }

    controllerInstance->Trace("Communication timeout");

    if(is_connecting_meter)
    {
        emit meterMessage(false, "Communication timed out, please retry");
        setAllMetersDisconnected();
        emit MeterListChanged();
        emit meterAction(true);
        is_connecting_meter = false;
        activeWF_ = Idle;
        return;
    }

    if(is_adding_meter)
    {
        emit meterFound(true, "Meter found");
        emit meterConnected(true, "Bluetooth connected");
        emit meterConfigured(false, "Communication timed out");
        is_connecting_meter = false;
        activeWF_ = Idle;
        return;
    }

    if(is_token_transfer_pending)
    {
        emit tokenTransferState(true, "Reconnecting DLMS in 2 minutes");
    }

    if(is_keep_alive_pending)
    {
        is_keep_alive_pending = false;
    }

    if(btInterfacePtr->IsTranceiving())
    {
        keepAliveTimer.stop();
        btInterfacePtr->StopCommTimer();
        Trace("Reconnecting DLMS after 2 minutes");
        connectionResetTimer.start(125*1000);
    }
}

void Controller::handleCommunicationDrop(const QString &errstr)
{
    sessionEnd_ = QDateTime::currentDateTime();

    const qint64 millisecondsDiff = sessionStart_.msecsTo(sessionEnd_);

    char buffer[65] = {0};
    snprintf(buffer, 64, "Session was active for %lld seconds", millisecondsDiff/1000);
    Trace(buffer);

    emit progressIndicatorsOff();
    emit networkIndicatorsOff();

    isAppConnected_ = false;
    emit IsAppConnectedChanged();

    setAllMetersDisconnected();
    selectMeter(currentMeterSerialNo_, false);

    emit MeterListChanged();
    emit CurrentMeterSerialNoChanged();
    emit meterAction(true);

    QString err = errstr + ". Please retry";

    if(is_adding_meter)
    {
        emit meterConnected(true, "Meter connected");
        emit meterConfigured(false, "Communication timed out");
        return;
    }

    if(is_connecting_meter)
    {
        emit meterMessage(false, err);
        return;
    }

    if(lastPage_ == "Alarms.qml")
    {
        emit alarmError(err);
        return;
    }

    if(lastPage_ == "AvailableTokens.qml")
    {
        emit tokenTransferState(false, err);
        return;
    }
}

void Controller::retryConnect()
{
    connect_tries++;
    QThread::currentThread()->msleep(1000);
    is_ble_disconect_requested = false;
    btInterfacePtr->Connect(discovered_address, EDMI_SERVICE_UUID, 15000);
}

// - Communication error conditions - End

void Controller::OnDeviceConnected(const QString &name)
{
    btInterfacePtr->ConfirmConnected();

    connect_tries = 0;
    bool retval = false;
    
    Trace("Bluetooth Connected");
    sessionStart_ = QDateTime::currentDateTime();
    emit networkIndicatorsOn();
    emit workflowProgress(0);

    if(is_adding_meter)
    {
        emit meterConnected(true, "Bluetooth connected");
        QList<QVariant> startParamters;
        //System title
        startParamters.append(QVariant(QByteArray((char*)system_title, 8)));
        //CIU PIN
        startParamters.append(QVariant(QString(pin_csa_customer)));
        //BLE PIN
        startParamters.append(QVariant(QString(pin_ble_pairing)));
        //RSSI
        startParamters.append(QVariant(QString(rssi)));
        emit progressIndicatorsOn();
        activeWF_ = AddMeter;
        addMeterWF.StartWorkFlow(startParamters);
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
            emit meterMessage(true, "Bluetooth connected");
            emit meterAction(false);
            deliveredEnergy_ = 0;
            instantaneousPower_ = 0;
            usedCredit_ = 0;
            grossCredit_ =  0;
            creditCurrency_ = " ";

            emit DeliveredEnergyChanged();
            emit InstantaneousPowerChanged();
            emit UsedCreditChanged();
            emit GrossCreditChanged();
            emit CreditCurrencyChanged();

            QList<QVariant> startParamters;
            startParamters.append(QVariant(QByteArray((char*)system_title, 8)));
            startParamters.append(QVariant(false));
            startParamters.append(QVariant(mt.Keys103_));
            startParamters.append(QVariant(mt.SerialNo_));
            startParamters.append(QVariant(connect_serial_no));
            emit progressIndicatorsOn();
            activeWF_ = ConnectMeter;
            connectMeterWF.StartWorkFlow(startParamters);
        }
    }
}

void Controller::OnSendKeepAlive()
{
    if(!isAppConnected_)
    {
        return;
    }

    is_keep_alive_pending = true;
    QList<QVariant> startParamters;
    keepAliveTimer.stop();
    emit progressIndicatorsOn();
    emit workflowProgress(0);
    activeWF_ = KeepAlive;
    keepAliveWF.StartWorkFlow(startParamters);

    Trace("Timer event");
}

void Controller::OnReset()
{
    Trace("Resetting");

    Meter mt;
    bool found = false;
    foreach(Meter mtr , meterList_)
    {
        if(mtr.SerialNo_ == currentMeterSerialNo_)
        {
            mt = mtr;
            found = true;
            break;
        }
    }

    if(found)
    {
        deliveredEnergy_ = 0;
        instantaneousPower_ = 0;
        usedCredit_ = 0;
        grossCredit_ =  0;
        creditCurrency_ = " ";

        emit DeliveredEnergyChanged();
        emit InstantaneousPowerChanged();
        emit UsedCreditChanged();
        emit GrossCreditChanged();
        emit CreditCurrencyChanged();

        ByteBuffer buff;
        std::string syst_hex = mt.SystemTitle_.toStdString();
        Helpers::HexToBytes(syst_hex, buff);

        memset(system_title, 0, 8);
        memcpy(system_title, buff.GetData(), buff.GetSize());

        memset(pin_csa_customer, 0, 9);
        memcpy(pin_csa_customer, mt.CustomerPIN_.toStdString().c_str(), 8);

        memset(pin_ble_pairing, 0, 7);
        memcpy(pin_ble_pairing, mt.PairingPIN_.toStdString().c_str(), 6);

        QList<QVariant> startParamters;
        startParamters.append(QVariant(QByteArray((char*)system_title, 8)));
        startParamters.append(QVariant(false));
        startParamters.append(QVariant(mt.Keys103_));
        startParamters.append(QVariant(mt.SerialNo_));
        startParamters.append(QVariant(currentMeterSerialNo_));
        emit progressIndicatorsOn();
        activeWF_ = ConnectMeter;
        connectMeterWF.StartWorkFlow(startParamters);
    }
}

void Controller::OnDataReceived(const QByteArray& data)
{
    if(activeWF_ == KeepAlive)
    {
        keepAliveWF.SaveRx(data);

        if(isDumpRxTxEnabled_)
        {
            keepAliveWF.TraceLastRx();
        }

        keepAliveWF.ProcessReceivedFrame(data);
        return;
    }

    if(activeWF_ == AddMeter)
    {
        addMeterWF.SaveRx(data);

        if(isDumpRxTxEnabled_)
        {
            addMeterWF.TraceLastRx();
        }

        addMeterWF.ProcessReceivedFrame(data);
        return;
    }

    if(activeWF_ == ConnectMeter)
    {
        connectMeterWF.SaveRx(data);

        if(isDumpRxTxEnabled_)
        {
            connectMeterWF.TraceLastRx();
        }

        connectMeterWF.ProcessReceivedFrame(data);
        return;
    }

    if(activeWF_ == DisconnectMeter)
    {
        disconnectMeterWF.SaveRx(data);

        if(isDumpRxTxEnabled_)
        {
            disconnectMeterWF.TraceLastRx();
        }

        disconnectMeterWF.ProcessReceivedFrame(data);
        return;
    }

    if(activeWF_ == TokenTransfer)
    {
        tokenTransferWF.SaveRx(data);

        if(isDumpRxTxEnabled_)
        {
            tokenTransferWF.TraceLastRx();
        }

        tokenTransferWF.ProcessReceivedFrame(data);
        return;
    }
}

void Controller::OnDataSent(const QByteArray &data)
{
    if(activeWF_ == AddMeter)
    {
        addMeterWF.SaveTx(data);

        if(isDumpRxTxEnabled_)
        {
            addMeterWF.TraceLastTx();
        }

        return;
    }

    if(activeWF_ == ConnectMeter)
    {
        connectMeterWF.SaveTx(data);

        if(isDumpRxTxEnabled_)
        {
            connectMeterWF.TraceLastTx();
        }

        return;
    }

    if(activeWF_ == DisconnectMeter)
    {
        disconnectMeterWF.SaveTx(data);

        if(isDumpRxTxEnabled_)
        {
            disconnectMeterWF.TraceLastTx();
        }

        return;
    }

    if(activeWF_ == KeepAlive)
    {
        keepAliveWF.SaveTx(data);

        if(isDumpRxTxEnabled_)
        {
            keepAliveWF.TraceLastTx();
        }

        return;
    }

    if(activeWF_ == TokenTransfer)
    {
        tokenTransferWF.SaveTx(data);

        if(isDumpRxTxEnabled_)
        {
            tokenTransferWF.TraceLastTx();
        }

        return;
    }
}

void Controller::OnAddMeterCompleted(QString &serialNo, bool status)
{
    emit progressIndicatorsOff();
    emit workflowProgress(0);

    if(status)
    {
        Trace("Meter added");
        int ts = QDateTime::currentDateTime().toSecsSinceEpoch();
        appSyncTime_ = QDateTime::fromSecsSinceEpoch(ts).toString("dd MMM yy hh:mm");
        emit AppSyncTimeChanged();

        currentMeterSerialNo_ = serialNo;

        meterList_.clear();
        meterEntityPtr->allMeters(meterList_, isDemoMode_);
        UpdateCurrentMeterTimestamp();

        // Load meter settings
        LoadMeterSettings(currentMeterSerialNo_);
        // Now load the meter data
        loadMeter(currentMeterSerialNo_);

        is_connecting_meter = false;
        is_adding_meter = false;
        isAppConnected_ = true;

        emit MeterListChanged();
        emit CurrentMeterSerialNoChanged();
        emit meterConnected(status, "Meter connected");
        emit meterConfigured(status, "Meter authenticated");
        emit MinTimestampLP1Changed();
        emit MaxTimestampLP1Changed();
        emit MinTimestampLP2Changed();
        emit MaxTimestampLP2Changed();
        emit IsAppConnectedChanged();

        generateDashboardData();
        activeWF_ = Idle;
        keepAliveTimer.start(15*1000);
        emit pageAction(13);
    }
    else
    {
        emit meterConnected(true, "Meter connected");
        activeWF_ = Idle;
        emit meterConfigured(status, "Meter authentication failed");

        is_connecting_meter = false;
        is_adding_meter = false;

        isAppConnected_ = false;
    }
}

void Controller::OnConnectCompleted(QString &serialNo, bool status)
{
    emit progressIndicatorsOff();
    emit workflowProgress(0);

    if(status)
    {
        emit meterMessage(true, "Connected");
        Trace("DLMS Connected");
        int ts = QDateTime::currentDateTime().toSecsSinceEpoch();
        appSyncTime_ = QDateTime::fromSecsSinceEpoch(ts).toString("dd MMM yy hh:mm");
        emit AppSyncTimeChanged();

        currentMeterSerialNo_ = serialNo;

        // Set all meters to disconnected and deselected
        for(int x = 0; x < meterList_.count(); x++)
        {
            meterEntityPtr->update("serial_no", meterList_.at(x).SerialNo_, "meter_status", "0");
            meterEntityPtr->update("serial_no", meterList_.at(x).SerialNo_, "is_selected", "0");
        }

        // Set the connected meter selected
        meterEntityPtr->update("serial_no", currentMeterSerialNo_, "meter_status", "1");
        meterEntityPtr->update("serial_no", currentMeterSerialNo_, "is_selected", "1");
        UpdateCurrentMeterTimestamp();

        // Load meter settings
        LoadMeterSettings(currentMeterSerialNo_);
        // Now load the meter data
        loadMeter(currentMeterSerialNo_);

        // Reload
        meterList_.clear();
        meterEntityPtr->allMeters(meterList_, isDemoMode_);

        isAppConnected_ = true;
        emit IsAppConnectedChanged();
        emit CurrentMeterSerialNoChanged();
        emit MeterListChanged();
        emit meterAction(true);

        generateDashboardData();

        keep_alive_flow_failure_count = 0;

        if(is_token_transfer_pending)
        {
            QList<QVariant> startParameters;
            startParameters.append(token_pending);
            activeWF_ = TokenTransfer;
            tokenTransferWF.StartWorkFlow(startParameters);
        }
        else
        {
            activeWF_ = Idle;
            keepAliveTimer.start(15*1000);
            emit pageAction(13);
        }
    }
    else
    {
        activeWF_ = Idle;
        isAppConnected_ = false;
        emit meterMessage(false, "Could not authenticate");
        Trace("DLMS Authentication Flow failure");

        is_ble_disconect_requested = true;
        btInterfacePtr->Disconnect(discovered_address);
        setAllMetersDisconnected();
        emit meterAction(true);
    }

    is_connecting_meter = false;
    is_adding_meter = false;
}

void Controller::OnDisconnectCompleted(QString &serialNo, bool status)
{
    isAppConnected_ = false;
    emit IsAppConnectedChanged();

    activeWF_ = Idle;
    Trace("DLMS disconnection completed");
    emit progressIndicatorsOff();
    emit workflowProgress(0);

    keep_alive_flow_failure_count = 0;
    is_keep_alive_pending = false;
    is_ble_disconect_requested = true;
    is_disconnect_pending = true;
    btInterfacePtr->Disconnect(discovered_address);
    setAllMetersDisconnected();
    emit MeterListChanged();
    emit meterAction(true);
    emit tokenAction();
}

void Controller::OnKeepAliveCompleted(bool status)
{
    Trace("Keep Alive Completed");
    emit progressIndicatorsOff();
    emit workflowProgress(0);

    int ts = QDateTime::currentDateTime().toSecsSinceEpoch();
    appSyncTime_ = QDateTime::fromSecsSinceEpoch(ts).toString("dd MMM yy hh:mm");
    emit AppSyncTimeChanged();
    UpdateCurrentMeterTimestamp();

    is_keep_alive_pending = false;

    if(status)
    {
        tokenList_.clear();
        tokenHistoryList_.clear();
        tokenEntityPtr->selectedTokens(tokenList_, "serial_no", currentMeterSerialNo_);
        tokenHistoryEntityPtr->selectedTokenHistories(tokenHistoryList_, "serial_no", currentMeterSerialNo_);

        updateDataRanges(currentMeterSerialNo_);

        emit ActiveAlarmsChanged();
        emit alarmAction();
        emit TokenListChanged();
        emit TokenHistoryListChanged();
        emit CurrentMeterSerialNoChanged();

        generateDashboardData();

        is_keep_alive_pending = false;

        if(is_disconnect_pending)
        {
            keepAliveTimer.stop();
            is_connecting_meter = false;
            QList<QVariant> startParamters;
            startParamters.append(QVariant(currentMeterSerialNo_));
            emit progressIndicatorsOn();
            emit workflowProgress(0);
            activeWF_ = DisconnectMeter;
            disconnectMeterWF.StartWorkFlow(startParamters);
        }
        else
        {
            if(is_token_transfer_pending)
            {
                QList<QVariant> startParameters;
                startParameters.append(token_pending);
                emit progressIndicatorsOn();
                emit workflowProgress(0);
                activeWF_ = TokenTransfer;
                tokenTransferWF.StartWorkFlow(startParameters);
            }
            else
            {
                activeWF_ = Idle;
                keepAliveTimer.start(15000);
            }
        }
    }
    else
    {
        keep_alive_flow_failure_count++;

        if(keep_alive_flow_failure_count >= 5)
        {
        }

        activeWF_ = Idle;
        keepAliveTimer.start(15000);
    }

    emit meterAction(true);
    emit tokenAction();
}

void Controller::OnTokenTransferCompleted(QString &serialNo, QString &tokenNo, QString &statusMessage, bool status)
{
    Trace("Token transfer Completed");
    emit progressIndicatorsOff();
    emit workflowProgress(0);

    is_token_transfer_pending = false;
    int ts = QDateTime::currentDateTime().toSecsSinceEpoch();
    appSyncTime_ = QDateTime::fromSecsSinceEpoch(ts).toString("dd MMM yy hh:mm");
    emit AppSyncTimeChanged();
    UpdateCurrentMeterTimestamp();

    emit tokenTransferState(status, statusMessage);

    tokenList_.clear();
    tokenHistoryList_.clear();
    tokenEntityPtr->selectedTokens(tokenList_, "serial_no", currentMeterSerialNo_);
    tokenHistoryEntityPtr->selectedTokenHistories(tokenHistoryList_, "serial_no", currentMeterSerialNo_);
    emit TokenListChanged();
    emit TokenHistoryListChanged();
    emit tokenAction();
    emit meterAction(true);

    activeWF_ = Idle;
    keepAliveTimer.start(15*1000);
}
