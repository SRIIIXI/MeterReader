import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.3
import QtQuick.Layouts 1.3

Page
{
    id: addTokenPage

    Material.theme: Material.Light
    Material.accent: applicationData.Theme.AccentColor

    Keys.onReleased:
    {
        if (event.key === Qt.Key_Back)
        {
            event.accepted = true
            applicationData.pageAction(4)
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
        headerTitle: "Add New Token"
        isMenuButtonVisible: true
        isMeterNameVisible: true
        isSyncDateVisible: true
        isConnectionIndicatorVisible: applicationData.IsAppConnected
    }

    Rectangle
    {
        id:tokenNamelblRect
        width: addTokenPage.width
        height: addTokenPage.width * 0.1
        color: applicationData.Theme.BackgroundColor
        anchors.top: headerPanel.bottom
        anchors.topMargin: 30
        anchors.horizontalCenter: parent.horizontalCenter

        Label
        {
            id:tokenNamelbl
            text: "Enter Token"
            font.bold: true
            anchors.horizontalCenter: parent.horizontalCenter
            color: applicationData.Theme.FontColor
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
       color: applicationData.Theme.BackgroundColor

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
           icon.color: "transparent"
           icon.height: rectTokenEdit.height*0.66
           icon.width: rectTokenEdit.height*0.66
           anchors.right :rectTokenEdit.right
           anchors.verticalCenter: rectTokenEdit.verticalCenter
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
               applicationData.QRText = "";
               applicationData.ScanMode = 1;
               applicationData.invokeClearToken();
               applicationData.invokeChangePage(6);
           }
       }
    }

    CustomButton
    {
        id:btnAdd
        height: addTokenPage.width*0.125
        width: (addTokenPage.width*0.2)*2
        anchors.top: rectTokenEdit.bottom
        anchors.topMargin: 20
        anchors.horizontalCenter: parent.horizontalCenter
        text: "OK"
        isDefault: true
        accentColor: applicationData.Theme.AccentColor
        onClicked:
        {
            applicationData.invokeAddToken(textEdit.text)
        }
    }

    Label
    {
        id: errorLabel
        anchors.bottom: btnCancel.top
        anchors.horizontalCenter: parent.horizontalCenter
        font.bold: true
        text: "Error"
        color: "red"
        wrapMode: "Wrap"
        anchors.topMargin: 5
        anchors.bottomMargin: 5
    }

    CustomButton
    {
        id:btnCancel
        height: addTokenPage.width*0.125
        width: (addTokenPage.width*0.2)*2
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        text: "Cancel"
        isDefault: false
        accentColor: applicationData.Theme.AccentColor
        onClicked:
        {
            applicationData.invokeChangePage(5)
        }
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

