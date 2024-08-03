import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.3
import QtQuick.Layouts 1.3

Page
{
    id: alarmsPage

    Material.theme: Material.Light
    Material.accent: applicationData.Theme.AccentColor

    property variant activeAlarmsModel :  []

    Component.onCompleted:
    {
        errorLabel.text = ""
        activeAlarmsModel = applicationData.ActiveAlarms
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
        headerTitle: "Active Alarms"
        isMenuButtonVisible: true
        isMeterNameVisible: true
        isSyncDateVisible: true
        isConnectionIndicatorVisible: applicationData.IsAppConnected
    }

    ListView
    {
        id: alarmListView
        width: parent.width*0.90
        anchors.top:headerPanel.bottom
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
            color: applicationData.Theme.BackgroundColor

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
                font.pointSize: headerPanel.fontSizeSmall
                color: applicationData.Theme.FontColor
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
        anchors.bottom: alarmCommandRect.top
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
        id: alarmCommandRect
        width: alarmsPage.width
        height: alarmsPage.width*0.2
        radius: 5
        color: applicationData.Theme.NavigationBarColor

        anchors
        {
            bottom: parent.bottom
            horizontalCenter: parent.horizontalCenter
        }

        ToolButton
        {
            id: name
            action: navigatAlarmSettingsAction
            height: alarmCommandRect.height*0.75
            width: alarmCommandRect.height*0.75
            icon.source: "../images/Filter.png"
            icon.color: "transparent"
            icon.height: alarmCommandRect.height*0.5
            icon.width: alarmCommandRect.height*0.5
            anchors.horizontalCenter:alarmCommandRect.horizontalCenter

            background: Rectangle
            {
                color: applicationData.Theme.NavigationBarColor
            }
        }

        Label
        {
            text: "Configure Filters"
            color: applicationData.Theme.FontColor
            height: alarmCommandRect.height*0.5
            elide: Label.ElideRight
            horizontalAlignment: Qt.AlignHCenter
            verticalAlignment: Qt.AlignVCenter
            anchors.bottom: alarmCommandRect.bottom
            anchors.horizontalCenter:alarmCommandRect.horizontalCenter
        }
    }

    Connections
    {
        target: applicationData

        function onAlarmAction()
        {
            activeAlarmsModel = applicationData.ActiveAlarms
        }

        function onAlarmError(str)
        {
            errorLabel.text = str
        }
    }
}

