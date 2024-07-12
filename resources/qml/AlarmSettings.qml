import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.3
import QtQuick.Layouts 1.3

Page
{
    id: alarmsSettingsPage

    Material.theme: Material.Light
    Material.accent: applicationData.Theme.AccentColor

    property variant alarmFilterModel :  []


    Component.onCompleted:
    {
        alarmFilterModel = applicationData.AlarmFilter
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
        headerTitle: "Alarm Filters"
        isMenuButtonVisible: true
        isConnectionIndicatorVisible: false
        isMeterNameVisible: false
        isSyncDateVisible: false
    }

    ListView
    {
        id: alarmSettingsListView
        width: parent.width*0.90
        anchors.top:headerPanel.bottom
        anchors.topMargin: 10
        anchors.bottom: btnRect.top
        anchors.horizontalCenter: parent.horizontalCenter
        visible: true
        spacing: 10
        clip: true
        model: alarmFilterModel
        delegate: alarmSettingsListDelegate
    }

    Component
    {
        id: alarmSettingsListDelegate

        Rectangle
        {
            id: availableAlarmSettingsItemID
            width: alarmSettingsListView.width
            height: alarmsSettingsPage.width*0.1
            radius: 5
            color: applicationData.Theme.ControlColor

            CheckBox
            {
                id:checkBoxId
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: availableAlarmSettingsItemID.left
                anchors.leftMargin: 30
                checked: alarmFilterModel[index].IsSet
                Material.accent: applicationData.Theme.AccentColor
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
                onCheckedChanged:
                {
                    applicationData.invokeUpdateAlarmFilterState(index, checkState)
                }
            }

            Label
            {
                id:activeAlarmNamelbl
                font.pointSize: headerPanel.fontSizeSmall
                color: applicationData.Theme.FontColor
                elide: Label.ElideRight
                verticalAlignment: Qt.AlignVCenter
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: checkBoxId.right
                anchors.leftMargin: 30
                text: alarmFilterModel[index].AlarmName
            }
        }
    }

    Rectangle
    {
        id: btnRect
        width: alarmsSettingsPage.width
        height: alarmsSettingsPage.width*0.2
        anchors.bottom: parent.bottom
        anchors.topMargin: 10
        anchors.bottomMargin: 10
        anchors.rightMargin: 0
        anchors.right: parent.right
        color: applicationData.Theme.ControlColor

        Grid
        {
            id: grid
            rows: 1
            columns: 3
            spacing: 10
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter

            CustomButton
            {
                id:btnCancel
                height: alarmsSettingsPage.width*0.125
                width: (alarmsSettingsPage.width*0.2)*2
                text: "Cancel"
                isDefault: false
                accentColor: applicationData.Theme.AccentColor
                onClicked:
                {
                    applicationData.invokeReloadAlarmFilters();
                    applicationData.invokeChangePage(3)
                }
            }

            CustomButton
            {
                id:btnAdd
                height: alarmsSettingsPage.width*0.125
                width: (alarmsSettingsPage.width*0.2)*2
                text: "OK"
                isDefault: true
                accentColor: applicationData.Theme.AccentColor
                onClicked:
                {
                    applicationData.invokeApplyAlarmFilters();
                    applicationData.invokeChangePage(3)
                }
            }
        }
    }
}

