#include "BluetoothInterface.hpp"
#include "Controller.hpp"

#include <QPermissions>

#if defined (Q_OS_ANDROID)
#include <QtCore/private/qandroidextras_p.h>
#endif

BluetoothInterface* btInterfacePtr = nullptr;

static QString currentDeviceAddress = "";
static QBluetoothDeviceInfo currentRemoteDevice;
static QString currentPeripheralService;

//Communication flags
static bool is_scanning = false;
static bool is_connecting = false;
static bool is_transceiving = false;
static bool is_ble_connected = false;
//Communication flags

#define CHUNK_SIZE 256

int step = 0;

QByteArray prevRecv;
QByteArray prevSend;

int retryCount = 0;

BluetoothInterface::BluetoothInterface(QObject* parent): QObject(parent)
{
    btInterfacePtr = this;
    uartCentral = nullptr;
    uartService = nullptr;
    localDevice = nullptr;
    connectionEvents = 0;
    readfound = false;
    writefound = false;
    gattServiceFound = false;
    gattServiceScanComplete = false;
    device_connected = false;

    connect(&connectTimer_, &QTimer::timeout, this, &BluetoothInterface::OnConnectTimerEvent);
    connect(&transceiveTimer_, &QTimer::timeout, this, &BluetoothInterface::OnTransceiveTimerEvent);

    connectTimer_.setSingleShot(true);
    transceiveTimer_.setSingleShot(true);

    localDevice = new QBluetoothLocalDevice();
}

BluetoothInterface::~BluetoothInterface()
{

}

bool BluetoothInterface::Initialize()
{
    #if defined (Q_OS_ANDROID)
        QStringList permissions;

        permissions.clear();

        auto resc1 = QtAndroidPrivate::checkPermission("android.permission.BLUETOOTH").result();
        if(resc1 != QtAndroidPrivate::PermissionResult::Authorized)
        {
            permissions.append("android.permission.BLUETOOTH");
        }

        auto resc2 = QtAndroidPrivate::checkPermission("android.permission.BLUETOOTH_ADMIN").result();
        if(resc2 != QtAndroidPrivate::PermissionResult::Authorized)
        {
            permissions.append("android.permission.BLUETOOTH_ADMIN");
        }

        auto resc3 = QtAndroidPrivate::checkPermission("android.permission.BLUETOOTH_SCAN").result();
        if(resc3 != QtAndroidPrivate::PermissionResult::Authorized)
        {
            permissions.append("android.permission.BLUETOOTH_SCAN");
        }

        auto resc4 = QtAndroidPrivate::checkPermission("android.permission.BLUETOOTH_CONNECT").result();
        if(resc4 != QtAndroidPrivate::PermissionResult::Authorized)
        {
            permissions.append("android.permission.BLUETOOTH_CONNECT");
        }

        auto resc5 = QtAndroidPrivate::checkPermission("android.permission.ACCESS_COARSE_LOCATION").result();
        if(resc5 != QtAndroidPrivate::PermissionResult::Authorized)
        {
            permissions.append("android.permission.ACCESS_COARSE_LOCATION");
        }

        auto resc6 = QtAndroidPrivate::checkPermission("android.permission.ACCESS_FINE_LOCATION").result();
        if(resc6 != QtAndroidPrivate::PermissionResult::Authorized)
        {
            permissions.append("android.permission.ACCESS_FINE_LOCATION");
        }

        auto resc7 = QtAndroidPrivate::checkPermission("android.permission.INTERNET").result();
        if(resc7 != QtAndroidPrivate::PermissionResult::Authorized)
        {
            permissions.append("android.permission.INTERNET");
        }

        auto resc8 = QtAndroidPrivate::checkPermission("android.permission.ACCESS_NETWORK_STATE").result();
        if(resc8 != QtAndroidPrivate::PermissionResult::Authorized)
        {
            permissions.append("android.permission.ACCESS_NETWORK_STATE");
        }

        if(permissions.count() > 0)
        {
            for(int i = 0; i < permissions.count(); i++)
            {
                auto pr = QtAndroidPrivate::requestPermission(permissions.at(i)).result();

                if(pr == QtAndroidPrivate::PermissionResult::Authorized)
                {
                    qDebug() << "Granted " << permissions.at(i);
                }
                else
                {
                    qDebug() << "Denied " << permissions.at(i);
                }
            }
        }
    #endif

    qDebug() << "Getting Bluetooth permissions";

    QBluetoothPermission ble_permission{};

    switch (qApp->checkPermission(ble_permission))
    {
        case Qt::PermissionStatus::Undetermined:
        qApp->requestPermission(ble_permission, this, &BluetoothInterface::OnPermissionsGranted);
            break;
        case Qt::PermissionStatus::Denied:
            return false;
        case Qt::PermissionStatus::Granted:
            break; // proceed to search
    }

    return true;
}

void BluetoothInterface::ResetAdapter()
{

}

bool BluetoothInterface::IsTranceiving()
{
    return is_transceiving;
}

void BluetoothInterface::ConfirmConnected()
{
    connectTimer_.stop();
    is_connecting = false;
}

void BluetoothInterface::OnScanTimerEvent()
{
}

void BluetoothInterface::OnConnectTimerEvent()
{
    if(is_connecting)
    {
        if(is_ble_connected)
        {
            Disconnect(currentDeviceAddress);
        }
    }
}

void BluetoothInterface::OnTransceiveTimerEvent()
{
    if(is_transceiving)
    {
        emit CommunicationTimedOut();
    }
}

void BluetoothInterface::StartScanning(uint32_t timeoutMSeconds)
{
    discoveredDevices.clear();
    is_scanning = true;
    discoveryAgent.setLowEnergyDiscoveryTimeout(timeoutMSeconds);
    discoveryAgent.start(QBluetoothDeviceDiscoveryAgent::LowEnergyMethod);
}

void BluetoothInterface::StopScanning()
{
    is_scanning = false;
    discoveryAgent.stop();
}

void BluetoothInterface::Connect(QString address, QString peripheralService, uint32_t timeoutMSeconds)
{
    if(!discoveredDevices.contains(address))
    {
        emit ConnectFailed();
        return;
    }

    if(uartCentral != nullptr)
    {
        disconnect(uartCentral, &QLowEnergyController::serviceDiscovered, this, &BluetoothInterface::OnServiceDiscovered);
        disconnect(uartCentral, &QLowEnergyController::discoveryFinished, this, &BluetoothInterface::OnServiceScanDone);
        disconnect(uartCentral, &QLowEnergyController::errorOccurred,  this, &BluetoothInterface::OnControllerError);
        disconnect(uartCentral, &QLowEnergyController::connected, this, &BluetoothInterface::OnControllerConnected);
        disconnect(uartCentral, &QLowEnergyController::disconnected, this, &BluetoothInterface::OnControllerDisconnected);
        delete uartCentral;
        uartCentral = nullptr;
    }

    if(localDevice != nullptr)
    {
        disconnect(localDevice, &QBluetoothLocalDevice::deviceConnected, this, &BluetoothInterface::OnDeviceConnected);
        disconnect(localDevice, &QBluetoothLocalDevice::deviceDisconnected, this, &BluetoothInterface::OnDeviceDisconnected);
        disconnect(localDevice, &QBluetoothLocalDevice::errorOccurred, this, &BluetoothInterface::OnDeviceError);
        disconnect(localDevice, &QBluetoothLocalDevice::hostModeStateChanged, this, &BluetoothInterface::OnHostModeStateChanged);
        disconnect(localDevice, &QBluetoothLocalDevice::pairingFinished, this, &BluetoothInterface::OnPairingFinished);
    }

    is_ble_connected = false;
    currentRemoteDevice = discoveredDevices[address];
    currentDeviceAddress = address;
    currentPeripheralService = peripheralService;

    is_scanning = false;
    is_connecting = false;
    uartCentral = QLowEnergyController::createCentral(currentRemoteDevice, nullptr);

    connect(uartCentral, &QLowEnergyController::serviceDiscovered, this, &BluetoothInterface::OnServiceDiscovered);
    connect(uartCentral, &QLowEnergyController::discoveryFinished, this, &BluetoothInterface::OnServiceScanDone);
    connect(uartCentral, &QLowEnergyController::errorOccurred,  this, &BluetoothInterface::OnControllerError);
    connect(uartCentral, &QLowEnergyController::connected, this, &BluetoothInterface::OnControllerConnected);
    connect(uartCentral, &QLowEnergyController::disconnected, this, &BluetoothInterface::OnControllerDisconnected);

    connect(localDevice, &QBluetoothLocalDevice::deviceConnected, this, &BluetoothInterface::OnDeviceConnected);
    connect(localDevice, &QBluetoothLocalDevice::deviceDisconnected, this, &BluetoothInterface::OnDeviceDisconnected);
    connect(localDevice, &QBluetoothLocalDevice::errorOccurred, this, &BluetoothInterface::OnDeviceError);
    connect(localDevice, &QBluetoothLocalDevice::hostModeStateChanged, this, &BluetoothInterface::OnHostModeStateChanged);
    connect(localDevice, &QBluetoothLocalDevice::pairingFinished, this, &BluetoothInterface::OnPairingFinished);

    connectionEvents = 0;

    if(!IsPowered())
    {
        PowerOnRadio();
    }

    #if defined (Q_OS_IOS) || defined (Q_OS_MACOS)
        uartCentral->connectToDevice();
    #endif

    #if defined (Q_OS_ANDROID)
        localDevice->requestPairing(currentRemoteDevice.address(), QBluetoothLocalDevice::Unpaired);
    #endif

    is_connecting = true;
    if(!connectTimer_.isActive())
    {
        connectTimer_.start(timeoutMSeconds);
    }
}

void BluetoothInterface::Disconnect(QString address)
{
    uartCentral->disconnectFromDevice();
}

bool BluetoothInterface::IsConnected()
{
    return device_connected;
}

bool BluetoothInterface::Send(QByteArray &hexdata)
{
    if(connectTimer_.isActive())
    {
        connectTimer_.stop();
    }

    if(transceiveTimer_.isActive())
    {
        transceiveTimer_.stop();
    }

    is_connecting = false;

    QThread::currentThread()->msleep(100);

    bool send_succeded = false;

    if(uartService && writeCharacteristic.isValid())
    {
        if(hexdata.length() > CHUNK_SIZE)
        {
            int sentbytes = 0;

            while(sentbytes < hexdata.length())
            {
                uartService->writeCharacteristic(writeCharacteristic, hexdata.mid(sentbytes, CHUNK_SIZE), mode);
                sentbytes += CHUNK_SIZE;

                if(mode == QLowEnergyService::WriteWithResponse)
                {
                    QEventLoop loop;
                    connect(uartService, SIGNAL(characteristicWritten(QLowEnergyCharacteristic,QByteArray)), &loop, SLOT(quit()));
                    loop.exec();

                    if(uartService->error() != QLowEnergyService::NoError)
                    {
                        send_succeded = true;
                    }
                }
            }
        }
        else
        {
            uartService->writeCharacteristic(writeCharacteristic, hexdata, mode);
            send_succeded = true;
        }
    }

    if(send_succeded)
    {
        is_transceiving = true;
        transceiveTimer_.setSingleShot(true);
        transceiveTimer_.start(5000);
    }

    return send_succeded;
}

void BluetoothInterface::PowerOnRadio()
{
    if(localDevice != nullptr)
    {
        if(localDevice->hostMode() == QBluetoothLocalDevice::HostPoweredOff)
        {
            localDevice->setHostMode(QBluetoothLocalDevice::HostConnectable);
        }
    }
}

bool BluetoothInterface::IsPowered()
{
    if(localDevice != nullptr)
    {
        if(localDevice->hostMode() != QBluetoothLocalDevice::HostPoweredOff)
        {
            return true;
        }
    }

    return false;
}

void BluetoothInterface::OnPermissionsGranted()
{
    connect(&discoveryAgent, &QBluetoothDeviceDiscoveryAgent::canceled, this, &BluetoothInterface::OnCanceled);
    connect(&discoveryAgent, &QBluetoothDeviceDiscoveryAgent::deviceDiscovered, this, &BluetoothInterface::OnDeviceDiscovered);
    connect(&discoveryAgent, &QBluetoothDeviceDiscoveryAgent::deviceUpdated, this, &BluetoothInterface::OnDeviceUpdated);
    connect(&discoveryAgent, &QBluetoothDeviceDiscoveryAgent::errorOccurred, this, &BluetoothInterface::OnScannerError);
    connect(&discoveryAgent, &QBluetoothDeviceDiscoveryAgent::finished, this, &BluetoothInterface::OnFinished);

    QBluetoothLocalDevice localdevice;
    localdevice.setHostMode(QBluetoothLocalDevice::HostConnectable);
}

void BluetoothInterface::StopScanTimer()
{
}

void BluetoothInterface::StopConnectTimer()
{
      if(connectTimer_.isActive())
      {
          connectTimer_.stop();
      }
}

void BluetoothInterface::StopCommTimer()
{
    if(transceiveTimer_.isActive())
    {
        transceiveTimer_.stop();
    }
}

//Scanner
void BluetoothInterface::OnCanceled()
{
    if(is_scanning)
    {
        is_scanning = false;
        emit ScanComplete();
    }
}

void BluetoothInterface::OnDeviceDiscovered(const QBluetoothDeviceInfo &info)
{
    QString temp_address;

    #if defined (Q_OS_IOS) || defined (Q_OS_MACOS)
        temp_address = info.deviceUuid().toString();
    #endif
    #if defined (Q_OS_ANDROID)
        temp_address = info.address().toString();
    #endif

    discoveredDevices[temp_address] = info;

    emit DeviceDiscovered(temp_address, info.name(), QVariant(info.rssi()).toString(), info.manufacturerData());
}

void BluetoothInterface::OnDeviceUpdated(const QBluetoothDeviceInfo &info, QBluetoothDeviceInfo::Fields updatedFields)
{

}

void BluetoothInterface::OnScannerError(QBluetoothDeviceDiscoveryAgent::Error error)
{
    is_scanning = false;
    discoveryAgent.stop();
}

void BluetoothInterface::OnFinished()
{
    if(is_scanning)
    {
        is_scanning = false;
        emit ScanComplete();
    }
}

//Controller events/signals

void BluetoothInterface::OnServiceDiscovered(const QBluetoothUuid &gattservice)
{
    QString temp = gattservice.toString(QUuid::WithoutBraces).toUpper();

    if(temp == currentPeripheralService)
    {
        peripheralGattService = gattservice;
        gattServiceFound = true;

        if(gattServiceScanComplete && gattServiceFound)
        {
            QueryServiceDetails();
        }
    }
}

void BluetoothInterface::OnServiceScanDone()
{
    gattServiceScanComplete = true;

    if(gattServiceScanComplete && gattServiceFound)
    {
        QueryServiceDetails();
    }
}

void BluetoothInterface::OnControllerError(QLowEnergyController::Error)
{
}

void BluetoothInterface::OnControllerConnected()
{
    if(device_connected)
    {
        connectionEvents = 0;
        // Since we are already connected, this is an error
        Disconnect(currentDeviceAddress);
        emit DeviceDisconnected(currentDeviceAddress);
    }

    controllerInstance->Trace("LE Connected");
    connectionEvents++;

    #if defined (Q_OS_IOS)
    if(connectionEvents == 1)
    {
        uartCentral->discoverServices();
    }
    #endif

    if(connectionEvents == 2)
    {
        uartCentral->discoverServices();
    }
}

void BluetoothInterface::OnControllerDisconnected()
{
    device_connected = false;
    emit DeviceDisconnected(currentDeviceAddress);
}

void BluetoothInterface::QueryServiceDetails()
{
    if(uartService != nullptr)
    {
        disconnect(uartService, &QLowEnergyService::stateChanged, this, &BluetoothInterface::OnServiceStateChanged);
        disconnect(uartService, &QLowEnergyService::characteristicChanged, this, &BluetoothInterface::OnCharacteristicChanged);
        disconnect(uartService, &QLowEnergyService::characteristicRead, this, &BluetoothInterface::OnCharacteristicRead);
        disconnect(uartService, &QLowEnergyService::characteristicWritten, this, &BluetoothInterface::OnCharacteristicWrite);
        disconnect(uartService, &QLowEnergyService::errorOccurred, this, &BluetoothInterface::OnServiceError);
        delete uartService;
        uartService = nullptr;
    }

    uartService = uartCentral->createServiceObject(peripheralGattService, this);

    connect(uartService, &QLowEnergyService::stateChanged, this, &BluetoothInterface::OnServiceStateChanged);
    connect(uartService, &QLowEnergyService::characteristicChanged, this, &BluetoothInterface::OnCharacteristicChanged);
    connect(uartService, &QLowEnergyService::characteristicRead, this, &BluetoothInterface::OnCharacteristicRead);
    connect(uartService, &QLowEnergyService::characteristicWritten, this, &BluetoothInterface::OnCharacteristicWrite);
    connect(uartService, &QLowEnergyService::errorOccurred, this, &BluetoothInterface::OnServiceError);

    if(uartService->state() == QLowEnergyService::RemoteService)
    {
        uartService->discoverDetails();
    }
}

void BluetoothInterface::GetCharachteristics()
{
    readfound = false;
    writefound = false;

    foreach (QLowEnergyCharacteristic chr, uartService->characteristics())
    {
        if(chr.isValid())
        {
            if (chr.properties() & QLowEnergyCharacteristic::WriteNoResponse)
            {
                writeCharacteristic = chr;
                writefound = true;
                mode = QLowEnergyService::WriteMode::WriteWithoutResponse;

                writeDescriptor = chr.descriptor(QBluetoothUuid::DescriptorType::ClientCharacteristicConfiguration);

                if (writeDescriptor.isValid())
                {
                    uartService->writeDescriptor(writeDescriptor, QByteArray::fromHex("0100"));
                }

                peripheralRx = chr.uuid();
            }

            if (chr.properties() & QLowEnergyCharacteristic::Write)
            {
                writeCharacteristic = chr;
                writefound = true;
                mode = QLowEnergyService::WriteMode::WriteWithResponse;

                writeDescriptor = chr.descriptor(QBluetoothUuid::DescriptorType::ClientCharacteristicConfiguration);

                if (writeDescriptor.isValid())
                {
                    uartService->writeDescriptor(writeDescriptor, QByteArray::fromHex("0100"));
                }

                peripheralRx = chr.uuid();
            }

            if (chr.properties() & QLowEnergyCharacteristic::Notify)
            {
                readCharacteristic = chr;
                readfound = true;

                readDescriptor = chr.descriptor(QBluetoothUuid::DescriptorType::ClientCharacteristicConfiguration);

                if (readDescriptor.isValid())
                {
                    uartService->writeDescriptor(readDescriptor, QByteArray::fromHex("0100"));
                }
                else
                {
                    controllerInstance->Trace("Could not subscribe to notify notification");
                }

                peripheralTx = chr.uuid();
            }
        }
    }

    if(readfound && writefound && gattServiceFound)
    {
        device_connected = true;
        if(connectTimer_.isActive())
        {
            connectTimer_.stop();
        }

        QBluetoothDeviceInfo remoteDevice = discoveredDevices[currentDeviceAddress];
        QString address;
        #if defined (Q_OS_IOS) || defined (Q_OS_MACOS)
                address = remoteDevice.deviceUuid().toString();
        #else
                address = remoteDevice.address().toString();
        #endif

        device_connected = false;
        emit DeviceConnected(address);
    }
}

//Characteristic (I/O) events

void BluetoothInterface::OnServiceStateChanged(QLowEnergyService::ServiceState state)
{
    if(state == QLowEnergyService::RemoteServiceDiscovered)
    {
        GetCharachteristics();
    }
}

void BluetoothInterface::OnCharacteristicChanged(const QLowEnergyCharacteristic &characteristics, const QByteArray &value)
{
    if(characteristics.uuid().toString() == peripheralTx.toString())
    {
        if(value != prevRecv)
        {
            prevRecv = value;
            is_transceiving = false;
            if(transceiveTimer_.isActive())
            {
                transceiveTimer_.stop();
            }
            emit DataReceived(value);
        }
    }

    if(characteristics.uuid().toString() == peripheralTx.toString())
    {
        emit DataSent(value);
    }
}

void BluetoothInterface::OnServiceError(QLowEnergyService::ServiceError error)
{
    controllerInstance->Trace("Service Error");
    device_connected = false;
    Disconnect(currentDeviceAddress);
    emit DeviceDisconnected(currentDeviceAddress);
}

void BluetoothInterface::OnCharacteristicRead(const QLowEnergyCharacteristic &characteristics, const QByteArray &value)
{
}

void BluetoothInterface::OnCharacteristicWrite(const QLowEnergyCharacteristic &characteristics, const QByteArray &value)
{
}

//Adapter events for non Apple devices
void BluetoothInterface::OnDeviceConnected(const QBluetoothAddress &address)
{
    if(device_connected)
    {
        connectionEvents =0;
        device_connected = false;
        // Since we are already connected, this is an error
        Disconnect(currentDeviceAddress);
        emit DeviceDisconnected(currentDeviceAddress);
    }

    controllerInstance->Trace("Classic Connected");
    connectionEvents++;

    if(connectionEvents == 2)
    {
        uartCentral->discoverServices();
    }
}

void BluetoothInterface::OnDeviceDisconnected(const QBluetoothAddress &info)
{
    device_connected = false;
    emit DeviceDisconnected(info.toString());
}

void BluetoothInterface::OnDeviceError(QBluetoothLocalDevice::Error error)
{
    device_connected = false;
    emit DeviceDisconnected(currentDeviceAddress);
}

void BluetoothInterface::OnHostModeStateChanged(QBluetoothLocalDevice::HostMode state)
{
    if(state == QBluetoothLocalDevice::HostPoweredOff)
    {
        device_connected = false;
        emit DeviceDisconnected(currentDeviceAddress);
    }
}

void BluetoothInterface::OnPairingFinished(const QBluetoothAddress &address, QBluetoothLocalDevice::Pairing pairing)
{
    if(pairing == QBluetoothLocalDevice::Unpaired)
    {
        localDevice->requestPairing(currentRemoteDevice.address(), QBluetoothLocalDevice::Paired);
    }
    else
    {
        if(pairing == QBluetoothLocalDevice::Paired)
        {
            controllerInstance->Trace("Pairing Finished");
            uartCentral->connectToDevice();
        }
    }
}
