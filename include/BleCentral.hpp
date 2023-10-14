#ifndef BLE_CENTRAL
#define BLE_CENTRAL

#include <QObject>
#include <QByteArray>
#include <QEventLoop>
#include <QMutex>
#include <QQueue>
#include <QMutexLocker>
#include <QTimer>
#include <QUuid>

#include <QLowEnergyController>
#include <QBluetoothAddress>
#include <QBluetoothLocalDevice>
#include <QBluetoothDeviceInfo>

class BleCentral : public QObject
{
    Q_OBJECT
public:
    BleCentral(QObject* parent = nullptr);
    virtual ~BleCentral();

    void SetPeripheralDevice(const QBluetoothDeviceInfo &device, QString servicestr);
    void ConnectPeripheral();
    void DisconnectPeripheral();
    quint32 SendData(QByteArray &buffer);

    bool IsConnected();
    bool HasErrors();

private slots:

    //Controller events/signals
    void OnServiceDiscovered(const QBluetoothUuid &gattservice);
    void OnServiceScanDone();
    void OnControllerError(QLowEnergyController::Error);
    void OnDeviceConnected();
    void OnDeviceDisconnected();

    //Characteristic (I/O) events
    void OnServiceStateChanged(QLowEnergyService::ServiceState state);
    void OnCharacteristicChanged(const QLowEnergyCharacteristic &characteristics, const QByteArray &value);
    void OnServiceError(QLowEnergyService::ServiceError error);
    void OnCharacteristicRead(const QLowEnergyCharacteristic &characteristics, const QByteArray &value);
    void OnCharacteristicWrite(const QLowEnergyCharacteristic &characteristics, const QByteArray &value);

    void OnDeviceConnected(const QBluetoothAddress &address);
    void OnDeviceDisconnected(const QBluetoothAddress &address);
    void OnErrorOccurred(QBluetoothLocalDevice::Error error);
    void OnHostModeStateChanged(QBluetoothLocalDevice::HostMode state);
    void OnPairingFinished(const QBluetoothAddress &address, QBluetoothLocalDevice::Pairing pairing);
    //Communication timeout
    void OnCommunicationTimeout();

signals:
    void DeviceConnected(const QString &name);
    void DeviceDisconnected();
    void DataReceived(const QByteArray& data);
    void DataSent(uint32_t len);
    void CommunicationTimeout();

private:
    void QueryServiceDetails();
    void GetCharachteristics();
    QLowEnergyController* uartCentral;
    QLowEnergyService* uartService;
    QLowEnergyCharacteristic readCharacteristic;
    QLowEnergyCharacteristic writeCharacteristic;
    QLowEnergyDescriptor readDescriptor;
    QLowEnergyDescriptor writeDescriptor;
    QBluetoothUuid peripheralGattService;
    QLowEnergyService::WriteMode mode;
    QBluetoothLocalDevice* localDevice;
    QBluetoothDeviceInfo remoteDevice;

    QUuid peripheralServiceId;
    QUuid peripheralTx;
    QUuid peripheralRx;
    QString peripheralServiceIdString;
    bool gattServiceFound;
    bool gattServiceScanComplete;
    bool readfound;
    bool writefound;
    QTimer commTimer;

    long connectionEvents;
    bool device_connected;
};

#endif
