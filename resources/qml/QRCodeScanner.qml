import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.3
import QtQuick.Window 2.12
import QtMultimedia

Page
{
    Material.theme: Material.Light
    Material.accent: applicationData.Theme.AccentColor

    id: scannerPage
    visible: true

    Component.onCompleted:
    {
        qrcodetext.text = applicationData.QRText
        camera.start()
   }

    Rectangle
    {
        id: background
        width: parent.width
        height: parent.height - headerPanel.height
        anchors.top: headerPanel.bottom
        color: applicationData.Theme.ControlColor
    }

    Header
    {
        id: headerPanel
        headerTitle: "Scan QR Code"
        isMenuButtonVisible: true
        isMeterNameVisible: true
        isSyncDateVisible: false
        isConnectionIndicatorVisible: false
    }

    Rectangle
    {
        id: infoBar
        height: headerPanel.height/3
        width: scannerPage.width
        anchors.top: headerPanel.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        color: applicationData.Theme.ControlColor

        Label
        {
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            id: qrcodetext
            font.pointSize: headerPanel.fontSizeBig
            font.bold: true
            color: "darkgreen"
        }
    }

    Camera
    {
        id: camera
        focusMode: Camera.FocusModeAuto
    }

    CaptureSession
    {
        camera: camera
        videoOutput: videoOutput
        imageCapture: ImageCapture
        {
            id: captureSession

            onErrorOccurred: function(requestId, error, message)
            {
            }
            onImageCaptured: function(requestId, previewImage)
            {
                qrcodetext.text = applicationData.invokeCheckQRImage(previewImage);
            }
            onImageSaved: function(requestId, path)
            {
            }
        }
    }

    Rectangle
    {
        id: videoRect
        width: scannerPage.width
        height: scannerPage.height - (headerPanel.height + btnRect.height + infoBar.height)
        anchors.top: infoBar.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        color: applicationData.Theme.ControlColor

        VideoOutput
        {
            id: videoOutput
            width: parent.width
            height: parent.width
            anchors.verticalCenter: parent.verticalCenter
            fillMode: VideoOutput.PreserveAspectCrop

            // used to get camera focus on touched point
            MouseArea
            {
                id: focusTouchArea
                anchors.fill: parent
                onClicked:
                {
                    captureSession.capture();
                }
            }
        }
    }

    Rectangle
    {
        id: btnRect
        width: scannerPage.width
        height: scannerPage.width*0.2
        anchors.bottom: parent.bottom
        anchors.topMargin: 10
        anchors.bottomMargin: 10
        anchors.rightMargin: 0
        anchors.right: parent.right
        color: applicationData.Theme.ControlColor

        Grid
        {
            id: grid
            rows: 1
            columns: 3
            spacing: 10
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter

            CustomButton
            {
                id:btnCancel
                height: scannerPage.width*0.15
                width: (scannerPage.width*0.2)*2
                text: "Cancel"
                isDefault: false
                accentColor: applicationData.Theme.AccentColor
                onClicked:
                {
                    camera.stop()
                    applicationData.QRText = ""
                    applicationData.invokeClearToken()
                    qrcodetext.text = ""

                    if(applicationData.ScanMode === 0)
                    {
                        applicationData.invokeChangePage(7)
                    }

                    if(applicationData.ScanMode === 1)
                    {
                        applicationData.invokeChangePage(8)
                    }
                }
            }

            CustomButton
            {
                id:btnAdd
                height: scannerPage.width*0.15
                width: (scannerPage.width*0.2)*2
                text: "OK"
                isDefault: true
                accentColor: applicationData.Theme.AccentColor
                onClicked:
                {
                    camera.stop()
                    applicationData.QRText = qrcodetext.text
                    qrcodetext.text = ""

                    if(applicationData.ScanMode === 0)
                    {
                        applicationData.invokeChangePage(7)
                    }

                    if(applicationData.ScanMode === 1)
                    {
                        applicationData.invokeChangePage(8)
                    }
                }
            }
        }
    }
}
