#include "OSInitializer.hpp"
#include <QBluetoothLocalDevice>
#include <QtCore>
#include <QCoreApplication>
#include <QJniEnvironment>
#include <QJniObject>
#include <QtCore/private/qandroidextras_p.h>

bool InitializeOSResources()
{
    QStringList permissions;

    permissions.clear();

    auto resc1 = QtAndroidPrivate::checkPermission("android.permission.BLUETOOTH").result();
    if(resc1 != QtAndroidPrivate::PermissionResult::Authorized)
    {
        qDebug() << "android.permission.BLUETOOTH NOT AVAILABLE";
        permissions.append("android.permission.BLUETOOTH");
    }
    else
    {
        qDebug() << "android.permission.BLUETOOTH IS AVAILABLE";
    }

    auto resc2 = QtAndroidPrivate::checkPermission("android.permission.BLUETOOTH_ADMIN").result();
    if(resc2 != QtAndroidPrivate::PermissionResult::Authorized)
    {
        qDebug() << "android.permission.BLUETOOTH_ADMIN NOT AVAILABLE";
        permissions.append("android.permission.BLUETOOTH_ADMIN");
    }
    else
    {
        qDebug() << "android.permission.BLUETOOTH_ADMIN IS AVAILABLE";
    }

    auto resc3 = QtAndroidPrivate::checkPermission("android.permission.BLUETOOTH_SCAN").result();
    if(resc3 != QtAndroidPrivate::PermissionResult::Authorized)
    {
        qDebug() << "android.permission.BLUETOOTH_SCAN NOT AVAILABLE";
        permissions.append("android.permission.BLUETOOTH_SCAN");
    }
    else
    {
        qDebug() << "android.permission.BLUETOOTH_SCAN IS AVAILABLE";
    }

    auto resc4 = QtAndroidPrivate::checkPermission("android.permission.BLUETOOTH_CONNECT").result();
    if(resc4 != QtAndroidPrivate::PermissionResult::Authorized)
    {
        qDebug() << "android.permission.BLUETOOTH_CONNECT NOT AVAILABLE";
        permissions.append("android.permission.BLUETOOTH_CONNECT");
    }
    else
    {
        qDebug() << "android.permission.BLUETOOTH_CONNECT IS AVAILABLE";
    }

    auto resc5 = QtAndroidPrivate::checkPermission("android.permission.ACCESS_COARSE_LOCATION").result();
    if(resc5 != QtAndroidPrivate::PermissionResult::Authorized)
    {
        qDebug() << "android.permission.ACCESS_COARSE_LOCATION NOT AVAILABLE";
        permissions.append("android.permission.ACCESS_COARSE_LOCATION");
    }
    else
    {
        qDebug() << "android.permission.ACCESS_COARSE_LOCATION IS AVAILABLE";
    }

    auto resc6 = QtAndroidPrivate::checkPermission("android.permission.ACCESS_FINE_LOCATION").result();
    if(resc6 != QtAndroidPrivate::PermissionResult::Authorized)
    {
        qDebug() << "android.permission.ACCESS_FINE_LOCATION NOT AVAILABLE";
        permissions.append("android.permission.ACCESS_FINE_LOCATION");
    }
    else
    {
        qDebug() << "android.permission.ACCESS_FINE_LOCATION IS AVAILABLE";
    }

    auto resc7 = QtAndroidPrivate::checkPermission("android.permission.CAMERA").result();
    if(resc7 != QtAndroidPrivate::PermissionResult::Authorized)
    {
        qDebug() << "android.permission.CAMERA NOT AVAILABLE";
        permissions.append("android.permission.CAMERA");
    }
    else
    {
        qDebug() << "android.permission.CAMERA IS AVAILABLE";
    }

    auto resc8 = QtAndroidPrivate::checkPermission("android.permission.INTERNET").result();
    if(resc8 != QtAndroidPrivate::PermissionResult::Authorized)
    {
        qDebug() << "android.permission.INTERNET NOT AVAILABLE";
        permissions.append("android.permission.INTERNET");
    }
    else
    {
        qDebug() << "android.permission.INTERNET IS AVAILABLE";
    }

    auto resc9 = QtAndroidPrivate::checkPermission("android.permission.ACCESS_NETWORK_STATE").result();
    if(resc9 != QtAndroidPrivate::PermissionResult::Authorized)
    {
        qDebug() << "android.permission.ACCESS_NETWORK_STATE NOT AVAILABLE";
        permissions.append("android.permission.ACCESS_NETWORK_STATE");
    }
    else
    {
        qDebug() << "android.permission.ACCESS_NETWORK_STATE IS AVAILABLE";
    }

    if(permissions.count() > 0)
    {
        for(int i = 0; i < permissions.count(); i++)
        {
            QString strtstat;

            auto pr = QtAndroidPrivate::requestPermission(permissions.at(i)).result();

            if(pr == QtAndroidPrivate::PermissionResult::Authorized)
            {
                strtstat = "Granted";
            }
            else
            {
                strtstat = "Denied";
            }

            qDebug() << permissions.at(i) << strtstat;
        }
    }

    QBluetoothLocalDevice dev;

    if(dev.hostMode() == QBluetoothLocalDevice::HostPoweredOff)
    {
        dev.powerOn();
    }

    return true;
}
