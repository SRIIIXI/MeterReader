import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.3
import QtQuick.Layouts 1.15

Page
{
    id: resetPasscodeStage1Page

    Material.theme: Material.Light
    Material.accent: applicationData.Theme.AccentColor

    property string consumerName: applicationData.CurrentUser.UserId


    Component.onCompleted:
    {
        userIdinput.text = "";
        errorLabel.text = ""
        errorLabel.visible = true;
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
        headerTitle: "Passcode Reset"
        isMenuButtonVisible: false
        isMeterNameVisible: false
        isSyncDateVisible: false
        isConnectionIndicatorVisible: false
    }

    Label
    {
        id: welcomelbl
        width: resetPasscodeStage1Page.width * 0.9
        height: resetPasscodeStage1Page.width * 0.05
        anchors.top: headerPanel.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: 0
        font.bold: true
        font.pointSize: headerPanel.fontSizeBig
        text: "Welcome " + consumerName
        color: applicationData.Theme.AccentColor
    }

    //

    Label
    {
        id: userIdlbl
        width: resetPasscodeStage1Page.width * 0.9
        height: resetPasscodeStage1Page.width * 0.05
        anchors.top: welcomelbl.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        font.bold: true
        text: "Enter Consumer Id"
        color: applicationData.Theme.FontColor
    }

    TextField
    {
        id: userIdinput
        height: resetPasscodeStage1Page.width * 0.1
        width: resetPasscodeStage1Page.width * 0.9
        focus: true
        text: ""
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: userIdlbl.bottom
        anchors.topMargin: 0
        placeholderText: "8 digit customer Id"
        maximumLength: 8
        inputMethodHints: Qt.ImhDigitsOnly
        font.pointSize: headerPanel.fontSizeSmall
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

        onTextChanged:
        {
            errorLabel.text = ""
            errorLabel.visible = false;
        }
    }

    Label
    {
        id: errorLabel
        anchors.top: userIdinput.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: 0
        font.bold: true
        text: "Error"
        color: "red"
    }

    CustomButton
    {
        id: btnNext
        text: "Next"
        height: parent.width*0.125
        width: (parent.width*0.2)*2.5
        anchors.bottom: btnCancel.top
        anchors.bottomMargin: 5
        anchors.horizontalCenter: parent.horizontalCenter
        accentColor: applicationData.Theme.AccentColor
        isDefault: true

        onClicked:
        {
            if(userIdinput.text.length < 8)
            {
                errorLabel.text = "Consumer ID must be 8 digits";
                errorLabel.visible = true;
                return;
            }

            if(userIdinput.text !== applicationData.CurrentUser.ConsumerId)
            {
                errorLabel.text = "Incorrect Consumer ID provided"
                errorLabel.visible = true;
                return;
            }

            applicationData.invokeResetPasscodeStage1(userIdinput.text)
        }
    }

    CustomButton
    {
        id:btnCancel
        height: parent.width*0.125
        width: (parent.width*0.2)*2.5
        text: "Cancel"
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 5
        anchors.horizontalCenter: parent.horizontalCenter
        accentColor: applicationData.Theme.AccentColor
        isDefault: false

        onClicked:
        {
            applicationData.invokeChangePage(9)
        }
    }

    Connections
    {
        target: applicationData
    }
}

