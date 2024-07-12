#include "CameraInterface.hpp"
#include <QPermissions>
#include <QCoreApplication>
#if defined (Q_OS_ANDROID)
#include <QtCore/private/qandroidextras_p.h>
#endif

CameraInterface::CameraInterface(QObject* parent): QObject(parent)
{
}

CameraInterface::~CameraInterface()
{

}

bool CameraInterface::Initialize()
{
#if defined (Q_OS_ANDROID)
    QStringList permissions;

    permissions.clear();

    auto resc = QtAndroidPrivate::checkPermission("android.permission.CAMERA").result();
    if(resc != QtAndroidPrivate::PermissionResult::Authorized)
    {
        permissions.append("android.permission.CAMERA");
    }

    if(permissions.count() > 0)
    {
        for(int i = 0; i < permissions.count(); i++)
        {
            auto pr = QtAndroidPrivate::requestPermission(permissions.at(i)).result();
        }
    }
#endif

    QCameraPermission camera_permission{};

    switch (qApp->checkPermission(camera_permission))
    {
    case Qt::PermissionStatus::Undetermined:
        qApp->requestPermission(camera_permission, this, &CameraInterface::OnPermissionsGranted);
        break;
    case Qt::PermissionStatus::Denied:
        return false;
    case Qt::PermissionStatus::Granted:
        break; // proceed to search
    }

    return true;
}

void CameraInterface::OnPermissionsGranted()
{

}
