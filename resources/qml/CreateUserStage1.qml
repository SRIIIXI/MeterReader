import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.3
import QtQuick.Layouts 1.15

Page
{
    id: createUserStage1Page

    Material.theme: Material.Light
    Material.accent: applicationData.Theme.AccentColor

    Component.onCompleted:
    {
        userNameinput.text = "";
        userIdinput.text = "";
        errorLabel.text = ""
        errorLabel.visible = false;
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
        headerTitle: "User Enrolment"
        isMenuButtonVisible: false
        isMeterNameVisible: false
        isSyncDateVisible: false
        isConnectionIndicatorVisible: false
    }

    Label
    {
        id: welcomelbl
        width: createUserStage1Page.width * 0.9
        height: createUserStage1Page.width * 0.05
        anchors.top: headerPanel.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: 0
        font.bold: true
        font.pointSize: headerPanel.fontSizeBig
        text: "Welcome"
        color: applicationData.Theme.AccentColor
    }

    //
    Label
    {
        id: userNamelbl
        width: createUserStage1Page.width * 0.9
        height: createUserStage1Page.width * 0.05
        anchors.top: welcomelbl.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        font.bold: true
        text: "Enter Name"
        color: applicationData.Theme.FontColor
    }

    TextField
    {
        id: userNameinput
        height: createUserStage1Page.width * 0.1
        width: createUserStage1Page.width * 0.9
        focus: true
        text: ""
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: userNamelbl.bottom
        anchors.topMargin: 0
        placeholderText: "16 character alphanumeric only"
        maximumLength: 16
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

    //

    Label
    {
        id: userIdlbl
        width: createUserStage1Page.width * 0.9
        height: createUserStage1Page.width * 0.05
        anchors.top: userNameinput.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: 10
        font.bold: true
        text: "Enter Consumer Id"
        color: applicationData.Theme.FontColor
    }

    TextField
    {
        id: userIdinput
        height: createUserStage1Page.width * 0.1
        width: createUserStage1Page.width * 0.9
        focus: true
        text: ""
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: userIdlbl.bottom
        anchors.topMargin: 10
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
        id:btnNext
        height: parent.width*0.125
        width: (parent.width*0.2)*2.5
        text: "Next"
        anchors.bottom: btnExit.top
        anchors.bottomMargin: 5
        anchors.horizontalCenter: parent.horizontalCenter
        accentColor: applicationData.Theme.AccentColor
        isDefault: true

        onClicked:
        {
            if(userNameinput.text.length < 4 || userNameinput.text.length > 16)
            {
                errorLabel.text = "Username must be 4 to 16 characters long.\n Space is permitted"
                errorLabel.visible = true;
                return;
            }

            if(userIdinput.text.length < 8)
            {
                errorLabel.text = "Consumer ID must be 8 digits"
                errorLabel.visible = true;
                return;
            }

            applicationData.invokeCreateUserStage1(userNameinput.text, userIdinput.text)
        }
    }

    CustomButton
    {
        id:btnExit
        height: parent.width*0.125
        width: (parent.width*0.2)*2.5
        text: "Exit"
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 5
        anchors.horizontalCenter: parent.horizontalCenter
        accentColor: applicationData.Theme.AccentColor
        isDefault: false

        onClicked:
        {
            applicationData.invokeExit()
        }
    }

    Connections
    {
        target: applicationData

        function onUserNameError()
        {
            errorLabel.text = "Username must be 4 to 16 characters long. Space is permitted"
            errorLabel.visible = true
        }
    }
}

