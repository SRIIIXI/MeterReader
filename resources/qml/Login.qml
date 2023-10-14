import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.3
import QtQuick.Controls.Material 2.3

Page
{
    Material.theme: Material.Light
    Material.accent: "#961C1C"

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

    Rectangle
    {
        id: headerID
        width: parent.width
        height: parent.width * 0.2
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
        id: fontlabel
        text: ""
    }

    Text
    {
        id: name
        text: "Welcome " + consumerName
        font.bold: true
        color: "#961C1C"
        font.pointSize: fontSizeLarge
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: 0
        anchors.top: headerID.bottom
    }

    Text
    {
        id: enterPass1Txt
        text: "Please enter your passcode "
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

        font.bold: true
        anchors.top: name.bottom
        anchors.topMargin: 5
        anchors.horizontalCenter: parent.horizontalCenter
    }

    Rectangle
    {
        id: rectdummy
        width: parent.width * 0.6
        height: (parent.width * 0.6)/4
        anchors.top: enterPass1Txt.bottom
        anchors.topMargin: 5
        anchors.horizontalCenter: parent.horizontalCenter

        Rectangle
        {
            anchors.fill: parent
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

        Grid
        {
            rows: 1
            columns: 4

            Rectangle
            {
                height: rectdummy.width/4
                width:  rectdummy.width/4
                radius: width*0.5

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

                Rectangle
                {
                    id: dot1
                    width: (parent.width*0.75)/3
                    height: (parent.width*0.75)/3
                    border.width: 1
                    radius: width*0.5
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.verticalCenter: parent.verticalCenter

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
            }

            Rectangle
            {
                height: rectdummy.width/4
                width:  rectdummy.width/4
                radius: width*0.5

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

                Rectangle
                {
                    id: dot2
                    width: (parent.width*0.75)/3
                    height: (parent.width*0.75)/3
                    border.width: 1
                    radius: width*0.5
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.verticalCenter: parent.verticalCenter

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
            }

            Rectangle
            {
                height: rectdummy.width/4
                width:  rectdummy.width/4
                radius: width*0.5

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

                Rectangle
                {
                    id: dot3
                    width: (parent.width*0.75)/3
                    height: (parent.width*0.75)/3
                    border.width: 1
                    radius: width*0.5
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.verticalCenter: parent.verticalCenter

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
            }

            Rectangle
            {
                height: rectdummy.width/4
                width:  rectdummy.width/4
                radius: width*0.5

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

                Rectangle
                {
                    id: dot4
                    width: (parent.width*0.75)/3
                    height: (parent.width*0.75)/3
                    border.width: 1
                    radius: width*0.5
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.verticalCenter: parent.verticalCenter
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
            }
        }
    }

    NumberKeypad
    {
        id:numberpadId
        visible: true
        anchors.top: rectdummy.bottom
        anchors.topMargin: 5
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
                return;
            }

            applicationData.invokeLogin(passcode)
        }
    }

    Label
    {
        id: txtforget
        text: "Forgot your passcode ?"
        font.bold: true
        anchors.top: numberpadId.bottom
        anchors.topMargin: 5
        anchors.horizontalCenter: parent.horizontalCenter
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

    Button
    {
        id:btnReset
        height: loginpage.width*0.15
        width: (loginpage.width*0.2)*2.5
        anchors.top: txtforget.bottom
        anchors.topMargin: 5
        anchors.horizontalCenter: parent.horizontalCenter
        text: "Reset Passcode"
        onClicked:
        {
            onClicked: applicationData.pageAction(14)
        }

        background: Rectangle
        {
            color: "#961C1C"
            border.width: 1
            border.color: "#961C1C"
            radius: 0.2  * btnReset.height
        }

        contentItem: Text
        {
           text: "Reset Passcode"
           font: btnReset.font
           opacity: enabled ? 1.0 : 0.3
           color: btnReset.down ? "gray" : "white"
           horizontalAlignment: Text.AlignHCenter
           verticalAlignment: Text.AlignVCenter
           elide: Text.ElideRight
        }
    }

    Label
    {
        id: errorLabel
        width: loginpage.width * 0.9
        height: loginpage.width * 0.05
        anchors.top: btnReset.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: headerID.height*0.55
        font.bold: true
        text: "Error"
        color: "red"
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

