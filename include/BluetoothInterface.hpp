#ifndef BLUETOOTH_NATIVE_INTERFACE
#define BLUETOOTH_NATIVE_INTERFACE

#include <QtCore>
#include <QBluetoothDeviceInfo>
#include <QBluetoothDeviceDiscoveryAgent>
#include <QLowEnergyController>
#include <QBluetoothLocalDevice>

class BluetoothInterface : public QObject
{
    Q_OBJECT
public:
    BluetoothInterface(QObject* parent = nullptr);
    virtual ~BluetoothInterface();
    bool Initialize();
    void StartScanning(uint32_t timeoutMSeconds);
    void StopScanning();
    void Connect(QString address, QString peripheralService, uint32_t timeoutMSeconds);
    void Disconnect(QString address);
    bool Send(QByteArray &hexdata);
    void PowerOnRadio();
    bool IsPowered();
    bool IsTranceiving();
    bool IsConnected();
    void ResetAdapter();
    void ConfirmConnected();
    void StopScanTimer();
    void StopConnectTimer();
    void StopCommTimer();

signals:
    void DeviceDiscovered(const QString &address, const QString &name, const QString &rssi, const QMultiHash<quint16, QByteArray> &mdata);
    void ScanComplete();

    void ConnectFailed();
    void DeviceConnected(const QString &address);
    void DeviceDisconnected(const QString &address);
    void DataReceived(const QByteArray& data);
    void DataSent(const QByteArray& data);
    void CommunicationTimedOut();

public slots:
    void OnPermissionsGranted();

    void OnScanTimerEvent();
    void OnConnectTimerEvent();
    void OnTransceiveTimerEvent();

    //Scanner
    void OnCanceled();
    void OnDeviceDiscovered(const QBluetoothDeviceInfo &info);
    void OnDeviceUpdated(const QBluetoothDeviceInfo &info, QBluetoothDeviceInfo::Fields updatedFields);
    void OnScannerError(QBluetoothDeviceDiscoveryAgent::Error error);
    void OnFinished();

    //Central
    //Controller events/signals
    void OnServiceDiscovered(const QBluetoothUuid &gattservice);
    void OnServiceScanDone();
    void OnControllerError(QLowEnergyController::Error);
    void OnControllerConnected();
    void OnControllerDisconnected();

    //Characteristic (I/O) events
    void OnServiceStateChanged(QLowEnergyService::ServiceState state);
    void OnCharacteristicChanged(const QLowEnergyCharacteristic &characteristics, const QByteArray &value);
    void OnServiceError(QLowEnergyService::ServiceError error);
    void OnCharacteristicRead(const QLowEnergyCharacteristic &characteristics, const QByteArray &value);
    void OnCharacteristicWrite(const QLowEnergyCharacteristic &characteristics, const QByteArray &value);

    //Adapter
    void OnDeviceConnected(const QBluetoothAddress &address);
    void OnDeviceDisconnected(const QBluetoothAddress &info);
    void OnDeviceError(QBluetoothLocalDevice::Error error);
    void OnHostModeStateChanged(QBluetoothLocalDevice::HostMode state);
    void OnPairingFinished(const QBluetoothAddress &address, QBluetoothLocalDevice::Pairing pairing);

private:
    QTimer scanTimer_;
    QTimer connectTimer_;
    QTimer transceiveTimer_;

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

    QUuid peripheralTx;
    QUuid peripheralRx;
    bool gattServiceFound;
    bool gattServiceScanComplete;
    bool readfound;
    bool writefound;
    bool is_transceiving;

    long connectionEvents;
    bool device_connected;
    bool is_pairing_finished;

    QBluetoothDeviceDiscoveryAgent discoveryAgent;
    QMap<QString, QBluetoothDeviceInfo> discoveredDevices;

};

extern BluetoothInterface* btInterfacePtr;

#endif
