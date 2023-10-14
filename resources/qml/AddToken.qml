import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.3
import QtQuick.Layouts 1.3

Page
{
    id: addTokenPage

    Material.theme: Material.Light
    Material.accent: "#961C1C"

    property bool meterStatus: false
    property string lastSyncTime: applicationData.CurrentMeter.LastSyncStr
    property string meterName: applicationData.CurrentMeter.SerialNo
    property variant availableMeterNamesModel :  []

    Keys.onReleased:
    {
        if(Qt.platform.os === "android")
        {
            if (event.key === Qt.Key_Back)
            {
                event.accepted = true
                applicationData.pageAction(5)
            }
        }
    }

    Component.onCompleted:
    {
        errorLabel.text = ""
        btnAdd.enabled = false;

        if(applicationData.QRText.length === 20)
        {
            btnAdd.enabled = true;
        }

        availableMeterNamesModel = applicationData.MeterNameList
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

    Header
    {
        id:headerID
        headerTitle: "Add New Token"
        meterName  : applicationData.CurrentMeter.SerialNo
        syncDate   : applicationData.CurrentMeter.LastSyncStr
        isOptionsBtnVisible:  false
        isMeterNameVisible: false
        isSyncDateVisible: false
        backBtn.action      : navigateAvailableTokens
    }

    Rectangle
    {
        id:tokenNamelblRect
        width: addTokenPage.width
        height: addTokenPage.width * 0.1
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
        anchors.topMargin: 30
        anchors.horizontalCenter: parent.horizontalCenter

        Label
        {
            id:tokenNamelbl
            text: "Enter Token"
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
       id:rectTokenEdit
       height: parent.width*0.1
       width: parent.width*0.96
       anchors.top: tokenNamelblRect.bottom
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
           height: rectTokenEdit.height
           width: rectTokenEdit.width - btnSCan.width*1.5
           anchors.left: parent.left
           anchors.verticalCenter: parent.verticalCenter
           maximumLength: 20
           placeholderText: "20 digits"
           inputMethodHints: Qt.ImhDigitsOnly
           onTextChanged:
           {
               errorLabel.text = ""
               if(text.length === 20)
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
           height: rectTokenEdit.height
           width: rectTokenEdit.height
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
           icon.height: rectTokenEdit.height*0.66
           icon.width: rectTokenEdit.height*0.66
           anchors.right :rectTokenEdit.right
           anchors.verticalCenter: rectTokenEdit.verticalCenter
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
               applicationData.ScanMode = 1;
               applicationData.invokeClearToken();
               applicationData.invokeChangePage(6);
           }
       }
    }

    Button
    {
        id:btnAdd
        height: addTokenPage.width*0.15
        width: (addTokenPage.width*0.2)*2
        anchors.top: rectTokenEdit.bottom
        anchors.topMargin: 20
        anchors.horizontalCenter: parent.horizontalCenter
        text: "OK"
        onClicked:
        {
            applicationData.invokeAddToken(textEdit.text)
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
           text: "OK"
           font: btnAdd.font
           opacity: enabled ? 1.0 : 0.3
           color: btnAdd.down ? "gray" : "white"
           horizontalAlignment: Text.AlignHCenter
           verticalAlignment: Text.AlignVCenter
           elide: Text.ElideRight
        }
    }

    Label
    {
        id: errorLabel
        width: addTokenPage.width * 0.9
        height: addTokenPage.width * 0.05
        anchors.bottom: btnAdd.top
        anchors.horizontalCenter: parent.horizontalCenter
        font.bold: true
        text: "Error"
        color: "red"
        wrapMode: "Wrap"
        anchors.topMargin: 5
        anchors.bottomMargin: 5
    }

    Connections
    {
        target: applicationData

        function onTokenError(str)
        {
            errorLabel.text = str
        }
    }
}

