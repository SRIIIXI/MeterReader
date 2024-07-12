import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.3
import QtQuick.Controls.Material 2.3

Page
{
    Material.theme: Material.Light
    Material.accent: applicationData.Theme.AccentColor

    id: loginpage

    property string consumerName: applicationData.CurrentUser.UserId
    signal passcodeEnter(int pass1);

    property int keycount: 0

    property string passcode : ""
    property double fontSizeNormal: fontlabel.font.pointSize
    property double fontSizeSmall: fontlabel.font.pointSize -  2.5
    property double fontSizeTiny: fontlabel.font.pointSize - 5.0
    property double fontSizeBig: fontlabel.font.pointSize +  2.5
    property double fontSizeLarge: fontlabel.font.pointSize + 5.0

    Component.onCompleted:
    {
        fontSizeNormal: fontlabel.font.pointSize
        fontSizeSmall: fontlabel.font.pointSize -  2.5
        fontSizeTiny: fontlabel.font.pointSize - 5.0
        fontSizeBig: fontlabel.font.pointSize +  2.5
        fontSizeLarge: fontlabel.font.pointSize + 5.0

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
        headerTitle: "Login"
        isMenuButtonVisible: false
        isMeterNameVisible: false
        isSyncDateVisible: false
        isConnectionIndicatorVisible: false
    }

    Label
    {
        id: fontlabel
        text: ""
    }

    Text
    {
        id: name
        text: "Welcome " + consumerName
        font.bold: true
        color: applicationData.Theme.AccentColor
        font.pointSize: fontSizeLarge
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: 0
        anchors.top: headerPanel.bottom
    }

    Text
    {
        id: enterPass1Txt
        text: "Please enter your passcode "
        color: applicationData.Theme.FontColor
        font.bold: true
        anchors.top: name.bottom
        anchors.horizontalCenter: parent.horizontalCenter
    }

    Rectangle
    {
        id: rectdummy
        width: parent.width * 0.6
        height: (parent.width * 0.6)/4
        anchors.top: enterPass1Txt.bottom
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
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.verticalCenter: parent.verticalCenter
                    color: applicationData.Theme.FontColor
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
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.verticalCenter: parent.verticalCenter
                    color: applicationData.Theme.FontColor
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
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.verticalCenter: parent.verticalCenter
                    color: applicationData.Theme.FontColor
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
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.verticalCenter: parent.verticalCenter
                    color: applicationData.Theme.FontColor
                }
            }
        }
    }

    NumberKeypad
    {
        id:numberpadId
        visible: true
        anchors.top: rectdummy.bottom
        anchors.topMargin: 0
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

            applicationData.invokeLogin(passcode)
        }
    }

    Label
    {
        id: errorLabel
        anchors.top: numberpadId.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: 0
        font.bold: true
        text: "Error"
        color: "red"
    }

    Label
    {
        id: txtforget
        text: "Forgot your passcode ?"
        font.bold: true
        anchors.bottom: btnReset.top
        anchors.bottomMargin: 0
        anchors.horizontalCenter: parent.horizontalCenter
        color: applicationData.Theme.FontColor
    }

    CustomButton
    {
        id:btnReset
        height: loginpage.width*0.125
        width: (loginpage.width*0.2)*2.5
        anchors.bottom: btnExit.top
        anchors.bottomMargin: 5
        anchors.horizontalCenter: parent.horizontalCenter
        text: "Reset Passcode"
        isDefault: true
        accentColor: applicationData.Theme.AccentColor
        textAlignment: Text.ElideRight
        onClicked:
        {
            onClicked: applicationData.pageAction(14)
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
        function onUserLoginFailed(reasonstr)
        {
            dot1.visible = false;
            dot2.visible = false;
            dot3.visible = false;
            dot4.visible = false;
            keycount = 0;
            passcode = "";
            errorLabel.text = reasonstr
            errorLabel.visible = true;
        }
    }
}

