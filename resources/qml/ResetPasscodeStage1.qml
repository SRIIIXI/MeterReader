import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.3
import QtQuick.Layouts 1.15

Page
{
    id: resetPasscodeStage1Page

    Material.theme: Material.Light
    Material.accent: "#961C1C"

    property string consumerName: applicationData.CurrentUser.UserId

    Header
    {
        id:headerID
        headerTitle: "Passcode Reset"
        isOptionsBtnVisible:  false
        isMeterNameVisible: false
        isSyncDateVisible: false
        isBackBtnVisible: false
        isInfoVisible: false
    }

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
        width: resetPasscodeStage1Page.width * 0.9
        height: resetPasscodeStage1Page.width * 0.05
        anchors.top: headerID.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: headerID.height/2
        font.bold: true
        font.pointSize: headerID.fontSizeBig
        text: "Welcome " + consumerName
        color: "#961C1C"
    }

    //

    Label
    {
        id: userIdlbl
        width: resetPasscodeStage1Page.width * 0.9
        height: resetPasscodeStage1Page.width * 0.05
        anchors.top: welcomelbl.bottom
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
        height: resetPasscodeStage1Page.width * 0.1
        width: resetPasscodeStage1Page.width * 0.9
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
        width: resetPasscodeStage1Page.width * 0.9
        height: resetPasscodeStage1Page.width * 0.05
        anchors.top: userIdinput.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: headerID.height*0.55
        font.bold: true
        text: "Error"
        color: "red"
    }

    Button
    {
        id: btnNext
        text: "Next"
        height: parent.width*0.15
        width: (parent.width*0.2)*2
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 20
        anchors.horizontalCenter: parent.horizontalCenter

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

        background: Rectangle
        {
            color: "#961C1C"
            border.width: 1
            border.color: "#961C1C"
            radius: 0.2  * btnNext.height
        }

        contentItem: Text
        {
           text: "Reset Passcode"
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
    }
}

