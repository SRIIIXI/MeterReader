import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Window 2.12
import QtMultimedia

Page
{
    Material.theme: Material.Light
    Material.accent: "#961C1C"

    id: scannerPage
    visible: true

    Component.onCompleted:
    {
        qrcodetext.text = applicationData.QRText
        camera.start()
        console.log(Screen.height, headerID.height, infoBar.height, btnRect.height, videoRect.height)
        console.log(Screen.width, videoRect.width)
    }

    Rectangle
    {
        id: background
        width: parent.width
        height: parent.height - headerID.height
        anchors.top: headerID.bottom
        color:
        {
            if(applicationData.IsDarkTheme === true)
            {
                return "#1C2833";
            }
            else
            {
                return "whitesmoke";
            }
        }
    }

    Header
    {
        id: headerID
        headerTitle: "Scan QR Code"
        isInfoVisible: false
        optionsBtn.visible: false
        backBtn.action:
        {
            camera.stop();
            if(applicationData.ScanMode === 0)
            {
                return navigateAddMeter;
            }
            else
            {
                return navigateAvailableTokens;
            }
        }
    }

    Rectangle
    {
        id: infoBar
        height: headerID.height/3
        width: scannerPage.width
        anchors.top: headerID.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        color:
        {
            if(applicationData.IsDarkTheme === true)
            {
                return "#1C2833";
            }
            else
            {
                return "whitesmoke";
            }
        }

        Label
        {
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            id: qrcodetext
            font.pointSize: headerID.fontSizeBig
            font.bold: true
            color: "darkgreen"
            height: infoBar.height*0.7
            width: infoBar.width*0.9
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
                console.log("Error occurred", requestId, error, message);
            }
            onImageCaptured: function(requestId, previewImage)
            {
                console.log("Image captured", requestId, previewImage);
                qrcodetext.text = applicationData.invokeCheckQRImage(previewImage);
            }
            onImageSaved: function(requestId, path)
            {
                console.log("Image saved", requestId, path);
            }
        }
    }

    Rectangle
    {
        id: videoRect
        width: scannerPage.width
        height: scannerPage.height - (headerID.height + btnRect.height + infoBar.height)
        anchors.top: infoBar.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        color:
        {
            if(applicationData.IsDarkTheme === true)
            {
                return "#1C2833";
            }
            else
            {
                return "whitesmoke";
            }
        }

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
        height: scannerPage.width*0.15
        anchors.bottom: parent.bottom
        anchors.topMargin: 10
        anchors.bottomMargin: 10
        anchors.rightMargin: 0
        anchors.right: parent.right
        color:
        {
            if(applicationData.IsDarkTheme === true)
            {
                return "#1C2833";
            }
            else
            {
                return "whitesmoke";
            }
        }

        Grid
        {
            id: grid
            rows: 1
            columns: 3
            spacing: 10
            anchors.right: parent.right

            Button
            {
                id:btnCancel
                height: scannerPage.width*0.15
                width: (scannerPage.width*0.2)*2
                text: "Cancel"
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

                background: Rectangle
                {
                        color: "white"
                        border.width: 1
                        border.color: "#961C1C"
                        radius: 0.2  * btnCancel.height
                }


                contentItem: Text
                {
                       text: "Cancel"
                       font: btnCancel.font
                       opacity: enabled ? 1.0 : 0.3
                       color: btnCancel.down ? "gray" : "black"
                       horizontalAlignment: Text.AlignHCenter
                       verticalAlignment: Text.AlignVCenter
                       elide: Text.ElideRight
                }
            }

            Button
            {
                id:btnAdd
                height: scannerPage.width*0.15
                width: (scannerPage.width*0.2)*2
                text: "OK"
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

                background: Rectangle
                {
                        color: "#961C1C"
                        border.width: 1
                        border.color: "#961C1C"
                        radius: 0.2  * btnAdd.height
                }

                contentItem: Text
                {
                       text: "Save"
                       font: btnAdd.font
                       opacity: enabled ? 1.0 : 0.3
                       color: btnAdd.down ? "gray" : "white"
                       horizontalAlignment: Text.AlignHCenter
                       verticalAlignment: Text.AlignVCenter
                       elide: Text.ElideRight
                }
            }
        }
    }
}
