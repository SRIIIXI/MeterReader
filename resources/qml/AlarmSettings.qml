import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.3
import QtQuick.Layouts 1.3

Page
{
    id: alarmsSettingsPage

    Material.theme: Material.Light
    Material.accent: "#961C1C"

    property string lastSyncTime: applicationData.CurrentMeter.LastSyncStr
    property string meterName: applicationData.CurrentMeter.SerialNo

    property variant alarmFilterModel :  []

    Header
    {
        id:headerID
        headerTitle: "Selected alarm to be displayed"
        isOptionsBtnVisible:  true
        isMeterNameVisible: false
        isSyncDateVisible: false
        isInfoVisible: false
        backBtn.action      : navigatActiveAlarms
        optionsBtn.icon.source:
        if(applicationData.IsDarkTheme === true)
        {
            return "../images/SaveWhite.png";
        }
        else
        {
            return "../images/SaveBlack.png";
        }
        onOptionsBtnClicked:
        {
            headerID.optionsBtn.enabled = false;
            applicationData.invokeApplyAlarmFilters();
            errorLabel.text = "Alarm filters saved";
        }
    }

    Component.onCompleted:
    {
        errorLabel.text = ""
        alarmFilterModel = applicationData.AlarmFilter
        headerID.optionsBtn.enabled = false;
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
        id: alarmSettingsListView
        width: parent.width*0.90
        anchors.top:headerID.bottom
        anchors.topMargin: 10
        anchors.bottom: errorLabel.top
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

            CheckBox
            {
                id:checkBoxId
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: availableAlarmSettingsItemID.left
                anchors.leftMargin: 30
                checked: alarmFilterModel[index].IsSet
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
                onCheckedChanged:
                {
                    errorLabel.text = ""
                    applicationData.invokeUpdateAlarmFilterState(index, checkState)
                    headerID.optionsBtn.enabled = true;
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
                verticalAlignment: Qt.AlignVCenter
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: checkBoxId.right
                anchors.leftMargin: 30
                text: alarmFilterModel[index].AlarmName
            }
        }
    }

    Label
    {
        id: errorLabel
        width: alarmsSettingsPage.width * 0.9
        height: alarmsSettingsPage.width * 0.05
        anchors.bottom: alarmSettingsDummyRect.top
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
        id: alarmSettingsDummyRect
        width: alarmsSettingsPage.width
        height: alarmsSettingsPage.width*0.1
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
}

