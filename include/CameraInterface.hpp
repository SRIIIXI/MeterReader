#ifndef CAMERA_ACCESS
#define CAMERA_ACCESS
#include <QObject>

#define ENABLE_CAMERA_ACCESS 1


class CameraInterface : public QObject
{
    Q_OBJECT
public:
    CameraInterface(QObject* parent = nullptr);
    virtual ~CameraInterface();
    bool Initialize();

public slots:
    void OnPermissionsGranted();

};

#endif
