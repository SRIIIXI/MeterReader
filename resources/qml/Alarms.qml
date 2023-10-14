import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.3
import QtQuick.Layouts 1.3

Page
{
    id: alarmsPage

    Material.theme: Material.Light
    Material.accent: "#961C1C"

    property string lastSyncTime: applicationData.CurrentMeter.LastSyncStr
    property string meterName: applicationData.CurrentMeter.SerialNo

    property variant activeAlarmsModel :  []

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
        headerTitle: "Active Alarms"
        isOptionsBtnVisible:  true
        isMeterNameVisible: true
        isSyncDateVisible: true
        isInfoVisible: true
        optionsBtn.icon.source:
        if(applicationData.IsDarkTheme === true)
        {
            return "../images/FilterWhite.png";
        }
        else
        {
            return "../images/FilterBlack.png";
        }
        optionsBtn.action: navigatAlarmSettingsAction
    }

    Component.onCompleted:
    {
        errorLabel.text = ""
        activeAlarmsModel = applicationData.ActiveAlarms
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

    ListView
    {
        id: alarmListView
        width: parent.width*0.90
        anchors.top:headerID.bottom
        anchors.topMargin: 10
        anchors.bottom: errorLabel.top
        anchors.horizontalCenter: parent.horizontalCenter
        visible: true
        spacing: 10
        clip: true
        model: activeAlarmsModel
        delegate: alarmsListDelegate
    }

    Component
    {
        id: alarmsListDelegate

        Rectangle
        {
            id: activeAlarmItemID
            width: alarmListView.width
            height: alarmsPage.width*0.1
            radius: 5

            color:
            {
                if(applicationData.IsDarkTheme === true)
                {
                    return "#1C2833";
                }
                else
                {
                    return "whitesmoke";
                }
            }

            Image
            {
                id: activeAlarmImgID
                source: "../images/Warning.png"
                width: activeAlarmItemID.height*0.75
                height: activeAlarmItemID.height*0.75
                anchors.verticalCenter: parent.verticalCenter
                anchors
                {
                    left: parent.left
                    leftMargin:  30
                }
            }

            Label
            {
                id:activeAlarmNamelbl
                font.pointSize: headerID.fontSizeSmall

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

                elide: Label.ElideRight
                horizontalAlignment: Qt.AlignHCenter
                verticalAlignment: Qt.AlignVCenter
                anchors.verticalCenter: parent.verticalCenter
                anchors
                {
                    left: activeAlarmImgID.right
                    leftMargin: 30
                }
                text: activeAlarmsModel[index].AlarmName
            }
        }
    }

    Label
    {
        id: errorLabel
        width: alarmsPage.width * 0.9
        height: alarmsPage.width * 0.05
        anchors.bottom: alarmDummyRect.top
        anchors.horizontalCenter: parent.horizontalCenter
        font.bold: true
        text: "Error"
        color: "red"
        wrapMode: "Wrap"
        anchors.topMargin: 5
        anchors.bottomMargin: 5
    }

    Rectangle
    {
        id: alarmDummyRect
        width: alarmsPage.width
        height: alarmsPage.width*0.1
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

        anchors
        {
            bottom:parent.bottom
            horizontalCenter: parent.horizontalCenter
        }
    }

    Connections
    {
        target: applicationData

        function onAlarmAction(fl)
        {
            headerID.backBtn.enabled = true;
            headerID.optionsBtn.enabled = true;
            activeAlarmsModel = applicationData.ActiveAlarms
        }

        function onAlarmError(str)
        {
            errorLabel.text = str
        }
    }
}

