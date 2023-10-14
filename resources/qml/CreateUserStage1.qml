import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.3
import QtQuick.Layouts 1.15

Page
{
    id: createUserStage1Page

    Material.theme: Material.Light
    Material.accent: "#961C1C"

    property bool meterStatus: false
    property string lastSyncTime: applicationData.CurrentMeter.LastSyncStr
    property string meterName: applicationData.CurrentMeter.SerialNo

    Header
    {
        id:headerID
        headerTitle: "User Enrolment"
        isOptionsBtnVisible:  false
        isMeterNameVisible: false
        isSyncDateVisible: false
        isBackBtnVisible: false
        isInfoVisible: false
    }

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

    Label
    {
        id: welcomelbl
        width: createUserStage1Page.width * 0.9
        height: createUserStage1Page.width * 0.05
        anchors.top: headerID.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: headerID.height/2
        font.bold: true
        font.pointSize: headerID.fontSizeBig
        text: "Welcome"
        color: "#961C1C"
    }

    //
    Label
    {
        id: userNamelbl
        width: createUserStage1Page.width * 0.9
        height: createUserStage1Page.width * 0.05
        anchors.top: welcomelbl.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: headerID.height/2
        font.bold: true
        text: "Enter Name"
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
        anchors.topMargin: 10
        placeholderText: "16 character alphanumeric only"
        maximumLength: 16
        font.pointSize: headerID.fontSizeSmall
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
        anchors.topMargin: headerID.height*0.55
        font.bold: true
        text: "Enter Consumer Id"
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
        font.pointSize: headerID.fontSizeSmall
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
        width: createUserStage1Page.width * 0.9
        height: createUserStage1Page.width * 0.05
        anchors.top: userIdinput.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: headerID.height*0.55
        font.bold: true
        text: "Error"
        color: "red"
    }

    Button
    {
        id:btnNext
        height: parent.width*0.15
        width: (parent.width*0.2)*3
        text: "Next"
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 20
        anchors.horizontalCenter: parent.horizontalCenter

        onClicked:
        {
            if(userNameinput.text.length < 4 || userNameinput.text.length > 16)
            {
                errorLabel.text = "Username must be 4 to 16 characters long. Space is permitted"
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

        background: Rectangle
        {
            color: "#961C1C"
            border.width: 1
            border.color: "#961C1C"
            radius: 0.2  * btnNext.height
        }

        contentItem: Text
        {
           text: "Next"
           font: btnNext.font
           opacity: enabled ? 1.0 : 0.3
           color: btnNext.down ? "gray" : "white"
           horizontalAlignment: Text.AlignHCenter
           verticalAlignment: Text.AlignVCenter
           elide: Text.ElideRight
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

