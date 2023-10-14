#include "BleCentral.hpp"
#include "UICommandHandler.hpp"

#define CHUNK_SIZE 256

int step = 0;

QByteArray prevRecv;

BleCentral::BleCentral(QObject* parent):QObject(parent)
{
    connectionEvents = 0;
    readfound = false;
    writefound = false;
    gattServiceFound = false;
    gattServiceScanComplete = false;
    uartCentral = nullptr;
    localDevice = nullptr;
    device_connected = false;
    connect(&commTimer, SIGNAL(timeout()), this, SLOT(OnCommunicationTimeout()));
}

BleCentral::~BleCentral()
{

}

void BleCentral::SetPeripheralDevice(const QBluetoothDeviceInfo &device, QString servicestr)
{
    if(uartCentral != nullptr)
    {
        disconnect(uartCentral, SIGNAL(serviceDiscovered(QBluetoothUuid)), this, SLOT(OnServiceDiscovered(QBluetoothUuid)));
        disconnect(uartCentral, SIGNAL(discoveryFinished()), this, SLOT(OnServiceScanDone()));
        disconnect(uartCentral, SIGNAL(error(QLowEnergyController::Error)),  this, SLOT(OnControllerError(QLowEnergyController::Error)));
        disconnect(uartCentral, SIGNAL(connected()), this, SLOT(OnDeviceConnected()));
        disconnect(uartCentral, SIGNAL(disconnected()), this, SLOT(OnDeviceDisconnected()));
        delete uartCentral;
        uartCentral = nullptr;
    }

    if(localDevice != nullptr)
    {
        disconnect(localDevice, SIGNAL(deviceConnected(const QBluetoothAddress&)), this, SLOT(OnDeviceConnected(const QBluetoothAddress &)));
        disconnect(localDevice, SIGNAL(deviceDisconnected(const QBluetoothAddress&)), this, SLOT(OnDeviceDisconnected(const QBluetoothAddress &)));
        disconnect(localDevice, SIGNAL(error(QBluetoothLocalDevice::Error)), this, SLOT(OnErrorOccurred(QBluetoothLocalDevice::Error)));
        disconnect(localDevice, SIGNAL(hostModeStateChanged(QBluetoothLocalDevice::HostMode)), this, SLOT(OnHostModeStateChanged(QBluetoothLocalDevice::HostMode)));
        disconnect(localDevice, SIGNAL(pairingFinished(const QBluetoothAddress&, QBluetoothLocalDevice::Pairing)), this, SLOT(OnPairingFinished(const QBluetoothAddress&, QBluetoothLocalDevice::Pairing)));
        delete localDevice;
        localDevice = nullptr;
    }

    remoteDevice = device;

    uartCentral = QLowEnergyController::createCentral(device, nullptr);
    peripheralServiceId = QUuid::fromString(servicestr);
    peripheralServiceIdString = peripheralServiceId.toString(QUuid::WithoutBraces).toUpper();

    localDevice = new QBluetoothLocalDevice();
    connectionEvents = 0;
}

void BleCentral::ConnectPeripheral()
{
    prevRecv.clear();
    device_connected = false;

    if(localDevice)
    {
        connect(localDevice, SIGNAL(deviceConnected(const QBluetoothAddress&)), this, SLOT(OnDeviceConnected(const QBluetoothAddress &)));
        connect(localDevice, SIGNAL(deviceDisconnected(const QBluetoothAddress&)), this, SLOT(OnDeviceDisconnected(const QBluetoothAddress &)));
        connect(localDevice, SIGNAL(errorOccurred(QBluetoothLocalDevice::Error)), this, SLOT(OnErrorOccurred(QBluetoothLocalDevice::Error)));
        connect(localDevice, SIGNAL(hostModeStateChanged(QBluetoothLocalDevice::HostMode)), this, SLOT(OnHostModeStateChanged(QBluetoothLocalDevice::HostMode)));
        connect(localDevice, SIGNAL(pairingFinished(const QBluetoothAddress&, QBluetoothLocalDevice::Pairing)), this, SLOT(OnPairingFinished(const QBluetoothAddress&, QBluetoothLocalDevice::Pairing)));

        commTimer.start(15*1000);
    }

    if(uartCentral)
    {
        connect(uartCentral, SIGNAL(serviceDiscovered(QBluetoothUuid)), this, SLOT(OnServiceDiscovered(QBluetoothUuid)), Qt::QueuedConnection);
        connect(uartCentral, SIGNAL(discoveryFinished()), this, SLOT(OnServiceScanDone()));
        connect(uartCentral, SIGNAL(connected()), this, SLOT(OnDeviceConnected()));
        connect(uartCentral, SIGNAL(disconnected()), this, SLOT(OnDeviceDisconnected()));
        connect(uartCentral, SIGNAL(errorOccurred(QLowEnergyController::Error)),  this, SLOT(OnControllerError(QLowEnergyController::Error)));
    }

    uartCentral->connectToDevice();
    connectionEvents = 0;
}

void BleCentral::DisconnectPeripheral()
{
    uartCentral->disconnectFromDevice();
}

quint32 BleCentral::SendData(QByteArray &buffer)
{
    if(uartService && writeCharacteristic.isValid())
    {
        if(buffer.length() > CHUNK_SIZE)
        {
            int sentbytes = 0;

            while(sentbytes < buffer.length())
            {
                uartService->writeCharacteristic(writeCharacteristic, buffer.mid(sentbytes, CHUNK_SIZE), mode);
                sentbytes += CHUNK_SIZE;

                if(mode == QLowEnergyService::WriteWithResponse)
                {
                    QEventLoop loop;
                    connect(uartService, SIGNAL(characteristicWritten(QLowEnergyCharacteristic,QByteArray)), &loop, SLOT(quit()));
                    loop.exec();

                    if(uartService->error() != QLowEnergyService::NoError)
                    {
                        commTimer.start(15*1000);
                        return buffer.length();
                    }
                }
            }
        }
        else
        {
            uartService->writeCharacteristic(writeCharacteristic, buffer, mode);
            commTimer.start(15*1000);
            return buffer.length();
        }
    }

    return -1;
}

bool BleCentral::IsConnected()
{
    if(uartCentral)
    {
        if(readfound && writefound)
        {
            if(device_connected)
            {
                return true;
            }
        }
    }

    return false;
}

bool BleCentral::HasErrors()
{
    return false;
}

void BleCentral::QueryServiceDetails()
{
    uartService = uartCentral->createServiceObject(peripheralGattService, this);

    connect(uartService, SIGNAL(stateChanged(QLowEnergyService::ServiceState)), this, SLOT(OnServiceStateChanged(QLowEnergyService::ServiceState)));
    connect(uartService, SIGNAL(characteristicChanged(QLowEnergyCharacteristic,QByteArray)), this, SLOT(OnCharacteristicChanged(QLowEnergyCharacteristic,QByteArray)));
    connect(uartService, SIGNAL(characteristicRead(QLowEnergyCharacteristic,QByteArray)), this, SLOT(OnCharacteristicRead(QLowEnergyCharacteristic,QByteArray)));
    connect(uartService, SIGNAL(characteristicWritten(QLowEnergyCharacteristic,QByteArray)), this, SLOT(OnCharacteristicWrite(QLowEnergyCharacteristic,QByteArray)));
    connect(uartService, SIGNAL(errorOccurred(QLowEnergyService::ServiceError)), this, SLOT(OnServiceError(QLowEnergyService::ServiceError)));

    if(uartService->state() == QLowEnergyService::RemoteService)
    {
        uartService->discoverDetails();
    }
}

void BleCentral::GetCharachteristics()
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
                    globalApplicationData->AppLogError("Could not subscribe to notify notification");
                }

                peripheralTx = chr.uuid();
            }
        }
    }

    if(readfound && writefound && gattServiceFound)
    {
        device_connected = true;
        commTimer.stop();
        emit DeviceConnected(remoteDevice.name());
    }
}

//Controller events/signals

void BleCentral::OnServiceDiscovered(const QBluetoothUuid &gattservice)
{
    QString temp = gattservice.toString(QUuid::WithoutBraces).toUpper();

    if(temp == peripheralServiceIdString)
    {
        peripheralGattService = gattservice;
        gattServiceFound = true;

        if(gattServiceScanComplete && gattServiceFound)
        {
            QueryServiceDetails();
        }
    }
}

void BleCentral::OnServiceScanDone()
{
    gattServiceScanComplete = true;

    if(gattServiceScanComplete && gattServiceFound)
    {
        QueryServiceDetails();
    }
}

void BleCentral::OnControllerError(QLowEnergyController::Error)
{
}

void BleCentral::OnDeviceConnected()
{
    globalApplicationData->AppLogInformation("LE Connected");
    uartCentral->discoverServices();
}

void BleCentral::OnDeviceDisconnected()
{
    emit DeviceDisconnected();
}

//Characteristic (I/O) events

void BleCentral::OnServiceStateChanged(QLowEnergyService::ServiceState state)
{
    if(state == QLowEnergyService::RemoteServiceDiscovered)
    {
        GetCharachteristics();
    }
}

void BleCentral::OnCharacteristicChanged(const QLowEnergyCharacteristic &characteristics, const QByteArray &value)
{
    if(characteristics.uuid().toString() == peripheralTx.toString())
    {
        if(value != prevRecv)
        {
            prevRecv = value;
            commTimer.stop();
            emit DataReceived(value);
        }
    }
}

void BleCentral::OnServiceError(QLowEnergyService::ServiceError error)
{
    globalApplicationData->AppLogError("Service Error");
}

void BleCentral::OnCharacteristicRead(const QLowEnergyCharacteristic &characteristics, const QByteArray &value)
{
}

void BleCentral::OnCharacteristicWrite(const QLowEnergyCharacteristic &characteristics, const QByteArray &value)
{
}

//Adapter events for non Apple devices
void BleCentral::OnDeviceConnected(const QBluetoothAddress &address)
{
}

void BleCentral::OnDeviceDisconnected(const QBluetoothAddress &address)
{
    emit DeviceDisconnected();
}

void BleCentral::OnErrorOccurred(QBluetoothLocalDevice::Error error)
{

}

void BleCentral::OnHostModeStateChanged(QBluetoothLocalDevice::HostMode state)
{

}

void BleCentral::OnPairingFinished(const QBluetoothAddress &address, QBluetoothLocalDevice::Pairing pairing)
{
}

void BleCentral::OnCommunicationTimeout()
{
    qDebug() << "Communication timed out";
    commTimer.stop();
    emit CommunicationTimeout();
}
