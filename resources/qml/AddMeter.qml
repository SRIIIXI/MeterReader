import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.3
import QtQuick.Layouts 1.15

Page
{
    id: addMeterPage

    Material.theme: Material.Light
    Material.accent: "#961C1C"

    property bool meterStatus: false
    property string lastSyncTime: applicationData.CurrentMeter.LastSyncStr
    property string meterName: applicationData.CurrentMeter.SerialNo

    property bool stage1: false
    property bool stage2: false
    property bool stage3: false
    property bool stage4: false

    function getFontColor()
    {
        if(applicationData.IsDarkTheme === true)
        {
            return "black";
        }
        else
        {
            return "white";
        }
    }

    Keys.onReleased:
    {
        if(Qt.platform.os === "android")
        {
            if (event.key === Qt.Key_Back)
            {
                event.accepted = true
                applicationData.pageAction(4)
            }
        }
    }

    Header
    {
        id:headerID
        headerTitle: "Add New Meter"
        meterName  : applicationData.CurrentMeter.SerialNo
        syncDate   : applicationData.CurrentMeter.LastSyncStr
        isOptionsBtnVisible:  false
        isMeterNameVisible: false
        isSyncDateVisible: false
        isInfoVisible: false
        isTitleVisible: true
        backBtn.action      : navigateAvailableMeters
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
                return "black";
            }
            else
            {
                return "white";
            }
        }
    }

    Component.onCompleted:
    {
        headerID.isMeterNameVisible = true
        stage1ItemID.visible = false
        stage2ItemID.visible = false
        stage3ItemID.visible = false
        stage4ItemID.visible = false
        stage5ItemID.visible = false
        btnClose.visible = false

        btnAdd.enabled = false;

        console.log("Existing QR Text Lenght", applicationData.QRText.length, Qt.platform.os)

        if(applicationData.QRText.length === 29 || applicationData.QRText.length === 23 || applicationData.QRText.length === 15 || applicationData.QRText.length === 9)
        {
            btnAdd.enabled = true;
        }
        else
        {
            btnAdd.enabled = false;
        }
    }

    Rectangle
    {
        id:pairingCodelblRect
        width: addMeterPage.width
        height: addMeterPage.width * 0.1
        color:
        {
            if(applicationData.IsDarkTheme === true)
            {
                return "black";
            }
            else
            {
                return "white";
            }
        }
        anchors.top: headerID.bottom
        anchors.topMargin: 10
        anchors.horizontalCenter: parent.horizontalCenter

        Label
        {
            id:pairingCodelbl
            text: "Enter Pairing Code"
            font.bold: true
            anchors.horizontalCenter: parent.horizontalCenter
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
    }

    Rectangle
    {
        id:rectMeterEdit
        height: parent.width*0.1
        width: parent.width*0.96
        anchors.top: pairingCodelblRect.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        radius: 5
        color:
        {
            if(applicationData.IsDarkTheme === true)
            {
                return "black";
            }
            else
            {
                return "white";
            }
        }

       TextField
       {
           id: textEdit
           focus: true
           text: applicationData.QRText
           height: rectMeterEdit.height
           width: rectMeterEdit.width - btnSCan.width*1.5
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
               btnClose.visible = false

               if(text.length === 29 || text.length === 23 || text.length === 15 || text.length === 9)
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
           id: btnSCan
           height: rectMeterEdit.height
           width: rectMeterEdit.height
           icon.source:
           if(applicationData.IsDarkTheme === true)
           {
               return "../images/ScanWhite.png";
           }
           else
           {
               return "../images/ScanBlack.png";
           }
           icon.color: "transparent"
           icon.height: rectMeterEdit.height*0.66
           icon.width: rectMeterEdit.height*0.66
           anchors.right :rectMeterEdit.right
           anchors.verticalCenter: rectMeterEdit.verticalCenter
           enabled: true

           background: Rectangle
           {
               color:
               {
                   if(applicationData.IsDarkTheme === true)
                   {
                       return "black";
                   }
                   else
                   {
                       return "white";
                   }
               }
           }

           onClicked:
           {               
               applicationData.QRText = "";
               applicationData.ScanMode = 0;
               applicationData.invokeClearToken();
               applicationData.invokeChangePage(6);
           }
       }
    }

    Button
    {
        id: btnAdd
        text: "Add Meter"
        height: parent.width*0.15
        width: (parent.width*0.2)*2
        anchors.top: rectMeterEdit.bottom
        anchors.topMargin: 10
        anchors.horizontalCenter: parent.horizontalCenter
        onClicked:
        {
            stage1ItemID.visible = false
            stage2ItemID.visible = false
            stage3ItemID.visible = false
            stage4ItemID.visible = false
            stage5ItemID.visible = false
            btnClose.visible = false
            btnAdd.enabled = false;
            headerID.isInfoVisible = true;
            headerID.progressVisible = true;
            headerID.progressText = "0%";
            applicationData.invokeAddMeter(textEdit.text)
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
           text: "Add Meter"
           font: btnAdd.font
           opacity: enabled ? 1.0 : 0.3
           color: btnAdd.down ? "gray" : "white"
           horizontalAlignment: Text.AlignHCenter
           verticalAlignment: Text.AlignVCenter
           elide: Text.ElideRight
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
            font.pointSize: headerID.fontSizeSmall
            verticalAlignment: Qt.AlignVCenter
            anchors
            {
                left: stage1ImgID.right
                leftMargin: stage1ImgID.width*0.33
            }
            text: "Validating Code"
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
            font.pointSize: headerID.fontSizeSmall
            verticalAlignment: Qt.AlignVCenter
            anchors
            {
                left: stage2ImgID.right
                leftMargin: stage2ImgID.width*0.33
            }
            text: "Scanning for Meter"
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
            font.pointSize: headerID.fontSizeSmall
            verticalAlignment: Qt.AlignVCenter
            anchors
            {
                left: stage3ImgID.right
                leftMargin: stage3ImgID.width*0.33
            }
            text: "Verifying Credentials"
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
            font.pointSize: headerID.fontSizeSmall
            verticalAlignment: Qt.AlignVCenter
            anchors
            {
                left: stage4ImgID.right
                leftMargin: stage4ImgID.width*0.33
            }
            text: "Validating Configuration"
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
    }

    // Stage 5
    Rectangle
    {
        id:stage5ItemID
        width: addMeterPage.width * 0.1
        height: addMeterPage.width * 0.1
        color:"transparent"
        anchors.bottom: btnClose.top
        anchors.bottomMargin: (btnClose.y - btnAdd.y)/2 - btnAdd.height*2
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

    Button
    {
        id: btnClose
        text: "Close"
        height: parent.width*0.15
        width: (parent.width*0.2)*2
        visible: false
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 10
        anchors.horizontalCenter: parent.horizontalCenter
        onClicked:
        {
            applicationData.invokeMeterAdditionClose()
        }

        background: Rectangle
        {
            color:
            {
                if(applicationData.IsDarkTheme === true)
                {
                    return "white";
                }
                else
                {
                    return "black";
                }
            }
            border.width: 1
            border.color:
            {
                if(applicationData.IsDarkTheme === true)
                {
                    return "white";
                }
                else
                {
                    return "black";
                }
            }
            radius: 0.2  * btnClose.height
        }

        contentItem: Text
        {
           text: "Close"
           font: btnClose.font
           opacity: enabled ? 1.0 : 0.3
           color: btnClose.down ? "gray" : getFontColor()
           horizontalAlignment: Text.AlignHCenter
           verticalAlignment: Text.AlignVCenter
           elide: Text.ElideRight
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
                stage1Namelbl.text = "Validating Code"
                btnClose.visible = false
            }
            else
            {
                stage1ImgID.source = "../images/Fail.png"
                stage1Namelbl.text = errorstr
                btnClose.visible = true
                btnAdd.enabled = true;
            }

            console.log("Token verify " + fl)
            stage1ItemID.visible = true
        }

        function onMeterFound(fl)
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
                btnAdd.enabled = true;
            }

            console.log("Meter found " + fl)
            stage2ItemID.visible = true
        }

        function onMeterAuthenticated(fl)
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
                btnAdd.enabled = true;
            }

            console.log("Meter authenticate " + fl)
            stage3ItemID.visible = true
        }

        function onMeterCheckConfiguration(fl)
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
                btnAdd.enabled = true;
            }

            console.log("Meter Check Configuration " + fl)
            stage4ItemID.visible = true

            if(stage1 === true && stage2 === true && stage3 === true && stage4 === true)
            {
                stage5ItemID.visible = true
            }
            else
            {
                 stage5ItemID.visible = false
            }

            headerID.isInfoVisible = false;
            headerID.progressVisible = false;
            headerID.progressText = "0%";
            btnClose.visible = true
            btnAdd.enabled = true;
        }
    }
}
