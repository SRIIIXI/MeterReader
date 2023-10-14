import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.3
import QtQuick.Layouts 1.3

Page
{
    id: helpPage

    Material.theme: Material.Light
    Material.accent: "#961C1C"

    property string lastSyncTime: applicationData.CurrentMeter.LastSyncStr
    property string meterName: applicationData.CurrentMeter.SerialNo

    property variant helpModel :  []

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
        headerTitle: "Help"
        isOptionsBtnVisible:  false
        isMeterNameVisible: false
        isSyncDateVisible: false
        isInfoVisible: false
    }

    Component.onCompleted:
    {
        helpModel = applicationData.HelpStrings
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
        id: helpListView
        width: parent.width*0.90
        anchors.top:headerID.bottom
        anchors.topMargin: 10
        anchors.bottom: helpDummyRect.top
        anchors.horizontalCenter: parent.horizontalCenter
        visible: true
        spacing: 10
        clip: true
        model: helpModel
        delegate: helpstringsListDelegate
    }

    Component
    {
        id: helpstringsListDelegate

        Rectangle
        {
            id: helpItemID
            width: helpListView.width
            height: headerID.height*3
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

            Label
            {
                id: title
                width: helpItemID.width*0.9
                horizontalAlignment: Qt.AlignHCenter
                verticalAlignment: Qt.AlignTop
                font.pointSize: headerID.fontSizeNormal
                color: "#961C1C"
                elide: Label.ElideRight
                text: helpModel[index].Title
                font.bold: true
            }

            Label
            {
                id: helptext
                width: helpItemID.width*0.9
                anchors.top: title.bottom
                horizontalAlignment: Qt.AlignLeft
                anchors.topMargin: 20
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
                text: helpModel[index].Description
            }
        }
    }


    Rectangle
    {
        id: helpDummyRect
        width: helpPage.width
        height: helpPage.width*0.1
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

