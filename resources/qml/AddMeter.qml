import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.3
import QtQuick.Layouts 1.15

Page
{
    id: addMeterPage

    Material.theme: Material.Light
    Material.accent: applicationData.Theme.AccentColor

    property bool stage1: false
    property bool stage2: false
    property bool stage3: false
    property bool stage4: false

    Component.onCompleted:
    {
        stage1ItemID.visible = false
        stage2ItemID.visible = false
        stage3ItemID.visible = false
        stage4ItemID.visible = false
        stage5ItemID.visible = false

        if(applicationData.QRText.length > 8 && applicationData.QRText.length < 30)
        {
            btnAdd.enabled = true;
            textEdit.text = applicationData.QRText;
        }
        else
        {
            btnAdd.enabled = false;
        }
    }

    Rectangle
    {
        id: background
        width: parent.width
        height: parent.height - headerPanel.height
        anchors.top: headerPanel.bottom
        color: applicationData.Theme.BackgroundColor
    }

    Header
    {
        id:headerPanel
        headerTitle: "Add New Meter"
        isMenuButtonVisible: true
        isMeterNameVisible: false
        isSyncDateVisible: false
        isConnectionIndicatorVisible: applicationData.IsAppConnected
    }

    Rectangle
    {
        id:pairingCodelblRect
        width: addMeterPage.width
        height: addMeterPage.width * 0.1
        color: applicationData.Theme.BackgroundColor
        anchors.top: headerPanel.bottom
        anchors.topMargin: 10
        anchors.horizontalCenter: parent.horizontalCenter

        Label
        {
            id:pairingCodelbl
            text: "Enter Pairing Code"
            font.bold: true
            anchors.horizontalCenter: parent.horizontalCenter
            color: applicationData.Theme.FontColor
        }
    }

    Rectangle
    {
        id:rectMeterEdit
        height: parent.width*0.1
        width: parent.width*0.96
        anchors.top: pairingCodelblRect.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        radius: 5
        color: applicationData.Theme.BackgroundColor

       TextField
       {
           id: textEdit
           focus: true
           text: applicationData.QRText
           height: rectMeterEdit.height
           width: rectMeterEdit.width - btnScan.width*1.5
           anchors.left: parent.left
           anchors.verticalCenter: parent.verticalCenter
           maximumLength: 29
           placeholderText: "29 digits"
           inputMethodHints: Qt.ImhDigitsOnly
           onTextChanged:
           {
               stage1ItemID.visible = false
               stage2ItemID.visible = false
               stage3ItemID.visible = false
               stage4ItemID.visible = false
               stage5ItemID.visible = false

               if(text.length <= 29 || text.length === 23 || text.length === 15 || text.length === 9)
               {
                   btnAdd.enabled = true;
               }
               else
               {
                   btnAdd.enabled = false;
               }
           }

           Material.theme:
           {
               if(applicationData.IsDarkTheme === true)
               {
                   return Material.Dark;
               }
               else
               {
                   return Material.Light;
               }
           }
       }

       ToolButton
       {
           id: btnScan
           height: rectMeterEdit.height
           width: rectMeterEdit.height
           icon.color: "transparent"
           icon.height: rectMeterEdit.height*0.66
           icon.width: rectMeterEdit.height*0.66
           anchors.right :rectMeterEdit.right
           anchors.verticalCenter: rectMeterEdit.verticalCenter
           enabled: applicationData.IsCameraAccessEnabled

           icon.source:
           {
               if(applicationData.IsCameraAccessEnabled === true)
               {
                   return "../images/Scan.png";
               }
               else
               {
                   return "../images/ScanGrey.png"
               }
           }

           background: Rectangle
           {
               color: applicationData.Theme.BackgroundColor
           }

           onClicked:
           {
               btnScan.enabled = false;
               applicationData.QRText = "";
               applicationData.ScanMode = 0;
               applicationData.invokeClearToken();
               applicationData.invokeChangePage(6);
           }
       }
    }

    CustomButton
    {
        id: btnAdd
        text: "Add Meter"
        height: parent.width*0.125
        width: (parent.width*0.2)*2
        anchors.top: rectMeterEdit.bottom
        anchors.topMargin: 10
        anchors.horizontalCenter: parent.horizontalCenter
        textAlignment: Text.ElideRight
        isDefault: true
        accentColor: applicationData.Theme.AccentColor
        onClicked:
        {
            var str = textEdit.text
            stage1ItemID.visible = false
            stage2ItemID.visible = false
            stage3ItemID.visible = false
            stage4ItemID.visible = false
            stage5ItemID.visible = false
            btnClose.visible = false
            btnAdd.enabled = false
            btnAdd.visible = false
            btnScan.enabled = false
            btnClose.visible = false
            headerPanel.isMenuButtonVisible = false
            applicationData.QRText = ""
            textEdit.text = ""
            applicationData.invokeAddMeter(str)
        }
    }

    // Stage 1
    Rectangle
    {
        id:stage1ItemID
        width: addMeterPage.width * 0.1
        height: addMeterPage.width * 0.1
        color:"transparent"
        anchors.bottom: stage2ItemID.top
        anchors.bottomMargin: 10
        anchors.left: btnAdd.left

        Image
        {
            id: stage1ImgID
            source: "../images/Pass.png"
            width: stage1ItemID.width
            height: stage1ItemID.height
            verticalAlignment: Qt.AlignVCenter
            anchors.verticalCenter: parent.verticalCenter
        }

        Label
        {
            id:stage1Namelbl
            elide: Label.ElideRight
            height: stage1ItemID.height
            font.pointSize: headerPanel.fontSizeSmall
            verticalAlignment: Qt.AlignVCenter
            anchors
            {
                left: stage1ImgID.right
                leftMargin: stage1ImgID.width*0.33
            }
            text: "Validating Code"
            color: applicationData.Theme.FontColor
        }
    }

    // Stage 2
    Rectangle
    {
        id:stage2ItemID
        width: addMeterPage.width * 0.1
        height: addMeterPage.width * 0.1
        color:"transparent"
        anchors.bottom: stage3ItemID.top
        anchors.bottomMargin: 10
        anchors.left: btnAdd.left

        Image
        {
            id: stage2ImgID
            source: "../images/Pass.png"
            width: stage2ItemID.width
            height: stage2ItemID.height
            verticalAlignment: Qt.AlignVCenter
            anchors.verticalCenter: parent.verticalCenter
        }

        Label
        {
            id:stage2Namelbl
            elide: Label.ElideRight
            height: stage2ItemID.height
            font.pointSize: headerPanel.fontSizeSmall
            verticalAlignment: Qt.AlignVCenter
            anchors
            {
                left: stage2ImgID.right
                leftMargin: stage2ImgID.width*0.33
            }
            text: "Scanning for Meter"
            color: applicationData.Theme.FontColor
        }
    }

    // Stage 3
    Rectangle
    {
        id:stage3ItemID
        width: addMeterPage.width * 0.1
        height: addMeterPage.width * 0.1
        color:"transparent"
        anchors.bottom: stage4ItemID.top
        anchors.bottomMargin: 10
        anchors.left: btnAdd.left

        Image
        {
            id: stage3ImgID
            source: "../images/Pass.png"
            width: stage3ItemID.width
            height: stage3ItemID.height
            verticalAlignment: Qt.AlignVCenter
            anchors.verticalCenter: parent.verticalCenter
        }

        Label
        {
            id:stage3Namelbl
            elide: Label.ElideRight
            height: stage4ItemID.height
            font.pointSize: headerPanel.fontSizeSmall
            verticalAlignment: Qt.AlignVCenter
            anchors
            {
                left: stage3ImgID.right
                leftMargin: stage3ImgID.width*0.33
            }
            text: "Verifying Credentials"
            color: applicationData.Theme.FontColor
        }
    }

    // Stage 4
    Rectangle
    {
        id:stage4ItemID
        width: addMeterPage.width * 0.1
        height: addMeterPage.width * 0.1
        color:"transparent"
        anchors.bottom: stage5ItemID.top
        anchors.bottomMargin: 10
        anchors.left: btnAdd.left

        Image
        {
            id: stage4ImgID
            source: "../images/Pass.png"
            width: stage4ItemID.width
            height: stage4ItemID.height
            verticalAlignment: Qt.AlignVCenter
            anchors.verticalCenter: parent.verticalCenter
        }

        Label
        {
            id:stage4Namelbl
            elide: Label.ElideRight
            height: stage4ItemID.height
            font.pointSize: headerPanel.fontSizeSmall
            verticalAlignment: Qt.AlignVCenter
            anchors
            {
                left: stage4ImgID.right
                leftMargin: stage4ImgID.width*0.33
            }
            text: "Validating Configuration"
            color: applicationData.Theme.FontColor
        }
    }

    // Stage 5
    Rectangle
    {
        id:stage5ItemID
        width: addMeterPage.width * 0.1
        height: addMeterPage.width * 0.1
        color:"transparent"
        anchors.bottom: btnClose.top
        anchors.bottomMargin: btnClose.height
        anchors.horizontalCenter: parent.horizontalCenter

        Image
        {
            id: stage5ImgID
            source: "../images/Done.png"
            width: stage5ItemID.width
            height: stage5ItemID.height
            anchors.verticalCenter: parent.verticalCenter
        }
    }

    CustomButton
    {
        id: btnClose
        text: "Close"
        height: parent.width*0.125
        width: (parent.width*0.2)*2
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 10
        anchors.horizontalCenter: parent.horizontalCenter
        isDefault: false
        accentColor: applicationData.Theme.AccentColor
        onClicked:
        {
            applicationData.invokeMeterAdditionClose()
        }
    }

    Connections
    {
        target: applicationData

        function onMeterPairingCodeVerification(fl, errorstr)
        {
            stage1 = fl;

            if(fl === true)
            {
                stage1ImgID.source = "../images/Pass.png"
                btnClose.visible = false
            }
            else
            {
                stage1ImgID.source = "../images/Fail.png"
                stage1Namelbl.text = errorstr
                btnClose.visible = true
                headerPanel.isMenuButtonVisible = true
            }

           stage1Namelbl.text = errorstr
           stage1ItemID.visible = true
        }

        function onMeterFound(fl, errorstr)
        {
            stage2 = fl;

            if(fl === true)
            {
                stage2ImgID.source = "../images/Pass.png"
                btnClose.visible = false
            }
            else
            {
                stage2ImgID.source = "../images/Fail.png"
                btnClose.visible = true
                headerPanel.isMenuButtonVisible = true
            }

            stage2Namelbl.text = errorstr
            stage2ItemID.visible = true
       }

        function onMeterConnected(fl, errorstr)
        {
            stage3 = fl;

            if(fl === true)
            {
                stage3ImgID.source = "../images/Pass.png"
                btnClose.visible = false
            }
            else
            {
                stage3ImgID.source = "../images/Fail.png"
                btnClose.visible = true
                headerPanel.isMenuButtonVisible = true
            }

            stage3Namelbl.text = errorstr
            stage3ItemID.visible = true
        }

        function onMeterConfigured(fl, errorstr)
        {
            stage4 = fl;

            if(fl === true)
            {
                stage4ImgID.source = "../images/Pass.png"
                btnClose.visible = false
            }
            else
            {
                stage4ImgID.source = "../images/Fail.png"
                btnClose.visible = true
            }

            stage4Namelbl.text = errorstr
            stage4ItemID.visible = true

            if(stage1 === true && stage2 === true && stage3 === true && stage4 === true)
            {
                stage5ItemID.visible = true
            }
            else
            {
                 stage5ItemID.visible = false
            }

            btnClose.visible = true
            btnAdd.enabled = false
            btnAdd.visible = true
            btnScan.enabled = true
            headerPanel.isMenuButtonVisible = true
        }
    }
}
