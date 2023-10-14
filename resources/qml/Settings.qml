import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.3
import QtQuick.Layouts 1.3

Page
{
    id: settingsPage

    Material.theme: Material.Light
    Material.accent: "#961C1C"

    property string lastSyncTime: applicationData.CurrentMeter.LastSyncStr
    property string meterName: applicationData.CurrentMeter.SerialNo

    Header
    {
        id:headerID
        backBtn.icon.source :
        if(applicationData.IsDarkTheme === true)
        {
            return "../images/MenuWhite.png";
        }
        else
        {
            return "../images/MenuBlack.png";
        }
        backBtn.action      : openMenuAction
        headerTitle: "Settings"
        isOptionsBtnVisible:  false
        isMeterNameVisible: false
        isSyncDateVisible: false
        isInfoVisible: false
    }

    Component.onCompleted:
    {
        if(applicationData.IsDemoMode === true)
        {
            demoChoice.checked = true;
        }
        else
        {
            demoChoice.checked = false;
        }

        if(applicationData.IsDarkTheme === true)
        {
            themeChoice.text = "Dark Theme";
            themeChoice.checked = true;
        }
        else
        {
            themeChoice.text = "Light Theme"
            themeChoice.checked = false;
        }
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

    Switch
    {
        id: demoChoice
        anchors.top:  headerID.bottom
        text: "Demo Mode"
        Material.accent: "#961C1C"
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

        onClicked:
        {
            applicationData.IsDemoMode = checked
        }
    }

    Rectangle
    {
        id: line1
        width: settingsPage.width*0.9
        height: 2
        border.width: 1
        anchors.top: demoChoice.bottom
        anchors.topMargin: headerID.height*0.05
        anchors.horizontalCenter: parent.horizontalCenter
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
    }

    Switch
    {
        id: themeChoice
        anchors.top:  line1.bottom
        anchors.topMargin: headerID.height*0.05
        text: "Dark Theme"
        Material.accent: "#961C1C"
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

        onClicked:
        {
            applicationData.IsDarkTheme = checked

            if(applicationData.IsDarkTheme === true)
            {
                text = "Dark Theme";
            }
            else
            {
                text = "Light Theme";
            }
        }
    }

    Label
    {
        id: swVersion
        text: "Version " + applicationData.AppVersion
        color: "darkgrey"
        font.pointSize: headerID.fontSizeSmall
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottomMargin: 10
    }
}

