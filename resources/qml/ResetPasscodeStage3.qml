import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.3
import QtQuick.Controls.Material 2.3

Page
{
    Material.theme: Material.Light
    Material.accent: applicationData.Theme.AccentColor

    id: resetPasscodeStage3Page

    property string consumerName: applicationData.CurrentUser.ConsumerId
    signal passcodeEnter(int pass1);
    signal resetPasscode();

    property int keycount: 0

    property string passcode : ""

    Component.onCompleted:
    {
        dot1.visible = false;
        dot2.visible = false;
        dot3.visible = false;
        dot4.visible = false;
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
        headerTitle: "Passcode Reset"
        isMenuButtonVisible: false
        isMeterNameVisible: false
        isSyncDateVisible: false
        isConnectionIndicatorVisible: false
    }

    Text
    {
        id: name
        text: "Welcome " + consumerName
        font.bold: true
        color: applicationData.Theme.AccentColor
        font.pointSize: headerPanel.fontSizeBig
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: headerPanel.height/2
        anchors.top: headerPanel.bottom
    }

    Text
    {
        id: enterPass1Txt
        text: "Please reconfirm your passcode "
        font.bold: true
        anchors.top: name.bottom
        anchors.topMargin: headerPanel.height/2
        anchors.horizontalCenter: parent.horizontalCenter
        color: applicationData.Theme.FontColor
    }

    Rectangle
    {
        id: rectdummy
        width: parent.width * 0.6
        height: (parent.width * 0.6)/4
        anchors.top: enterPass1Txt.bottom
        anchors.topMargin: 20
        anchors.horizontalCenter: parent.horizontalCenter

        Rectangle
        {
            anchors.fill: parent
            color: applicationData.Theme.BackgroundColor
        }

        Grid
        {
            rows: 1
            columns: 4

            Rectangle
            {
                height: rectdummy.width/4
                width:  rectdummy.width/4
                radius: width*0.5
                color: applicationData.Theme.BackgroundColor

                Rectangle
                {
                    id: dot1
                    width: (parent.width*0.75)/3
                    height: (parent.width*0.75)/3
                    border.width: 1
                    radius: width*0.5
                    color: applicationData.Theme.FontColor
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.verticalCenter: parent.verticalCenter
                }
            }

            Rectangle
            {
                height: rectdummy.width/4
                width:  rectdummy.width/4
                radius: width*0.5
                color: applicationData.Theme.BackgroundColor

                Rectangle
                {
                    id: dot2
                    width: (parent.width*0.75)/3
                    height: (parent.width*0.75)/3
                    border.width: 1
                    radius: width*0.5
                    color: applicationData.Theme.FontColor
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.verticalCenter: parent.verticalCenter
                }
            }

            Rectangle
            {
                height: rectdummy.width/4
                width:  rectdummy.width/4
                radius: width*0.5
                color: applicationData.Theme.BackgroundColor

                Rectangle
                {
                    id: dot3
                    width: (parent.width*0.75)/3
                    height: (parent.width*0.75)/3
                    border.width: 1
                    radius: width*0.5
                    color: applicationData.Theme.FontColor
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.verticalCenter: parent.verticalCenter
                }
            }

            Rectangle
            {
                height: rectdummy.width/4
                width:  rectdummy.width/4
                radius: width*0.5
                color: applicationData.Theme.BackgroundColor

                Rectangle
                {
                    id: dot4
                    width: (parent.width*0.75)/3
                    height: (parent.width*0.75)/3
                    border.width: 1
                    radius: width*0.5
                    color: applicationData.Theme.FontColor
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.verticalCenter: parent.verticalCenter
                }
            }
        }
    }

    NumberKeypad
    {
        id:numberpadId
        visible: true
        anchors.top: rectdummy.bottom
        anchors.topMargin: 10
        anchors.horizontalCenter: parent.horizontalCenter
        onButtonPressed :
        {
            if(keycount == 4)
            {
                errorLabel.text = "Passcode must be 4 digits";
                errorLabel.visible = true;
                return;
            }

            keycount = keycount + 1
            passcode = passcode + currentButtonPressed;

            if(keycount == 1)
            {
                dot1.visible = true;
                dot2.visible = false;
                dot3.visible = false;
                dot4.visible = false;
            }

            if(keycount == 2)
            {
                dot1.visible = true;
                dot2.visible = true;
                dot3.visible = false;
                dot4.visible = false;
            }

            if(keycount == 3)
            {
                dot1.visible = true;
                dot2.visible = true;
                dot3.visible = true;
                dot4.visible = false;
            }

            if(keycount == 4)
            {
                dot1.visible = true;
                dot2.visible = true;
                dot3.visible = true;
                dot4.visible = true;
            }

            if(keycount == 4)
            {
                numberpadId.isTickMarkVisible = true
            }
        }

        onCancelPressed:
        {
            keycount = keycount - 1
            errorLabel.text = ""
            errorLabel.visible = false;

            if(keycount == 0)
            {
                dot1.visible = false;
                dot2.visible = false;
                dot3.visible = false;
                dot4.visible = false;
            }

            if(keycount == 1)
            {
                dot1.visible = true;
                dot2.visible = false;
                dot3.visible = false;
                dot4.visible = false;
            }

            if(keycount == 2)
            {
                dot1.visible = true;
                dot2.visible = true;
                dot3.visible = false;
                dot4.visible = false;
            }

            if(keycount == 3)
            {
                dot1.visible = true;
                dot2.visible = true;
                dot3.visible = true;
                dot4.visible = false;
            }

            var str = passcode
            str = str.slice(0, -1);
            passcode = str

            if(keycount < 4)
            {
                numberpadId.isTickMarkVisible = false
            }
        }

        onEnterBtnPressed:
        {
            if(passcode.length != 4)
            {
                errorLabel.text = "Passcode must be 4 digits";
                errorLabel.visible = true;
                dot1.visible = false;
                dot2.visible = false;
                dot3.visible = false;
                dot4.visible = false;
                keycount = 0;
                passcode = ""
                return;
            }

            applicationData.invokeResetPasscodeStage3(passcode)
        }
    }

    Label
    {
        id: errorLabel
        anchors.top: numberpadId.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: headerPanel.height*0.55
        font.bold: true
        text: "Error"
        color: "red"
    }

    CustomButton
    {
        id:btnCancel
        height: parent.width*0.125
        width: (parent.width*0.2)*2.5
        text: "Cancel"
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 20
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
        function onPassCodeMismatch()
        {
            dot1.visible = false;
            dot2.visible = false;
            dot3.visible = false;
            dot4.visible = false;
            keycount = 0;
            passcode = "";
            errorLabel.text = "Passcode mismatch, please try again"
            errorLabel.visible = true;
        }
    }
}

