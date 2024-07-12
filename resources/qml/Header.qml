import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.3
import QtQuick.Layouts 1.15

Rectangle
{
    id: headerID

    Material.theme: Material.Light
    Material.accent: applicationData.Theme.AccentColor

    height: parent.width*0.225
    width: parent.width

    property string headerTitle: ""

    property bool isMenuButtonVisible: true
    property bool isMeterNameVisible: true
    property bool isSyncDateVisible: true
    property bool isConnectionIndicatorVisible: true

    property double fontSizeNormal: fontlabel.font.pointSize
    property double fontSizeSmall: fontlabel.font.pointSize -  2.5
    property double fontSizeTiny: fontlabel.font.pointSize - 5.0
    property double fontSizeBig: fontlabel.font.pointSize +  2.5
    property double fontSizeLarge: fontlabel.font.pointSize + 5.0

    signal menuButtonClicked();

    Component.onCompleted:
    {
        fontSizeNormal = fontlabel.font.pointSize
        fontSizeSmall = fontlabel.font.pointSize -  2.5
        fontSizeTiny = fontlabel.font.pointSize - 5.0
        fontSizeBig = fontlabel.font.pointSize +  2.5
        fontSizeLarge = fontlabel.font.pointSize + 5.0

        progressPerCent.visible = false
        progressAnimator.visible = false

        if(applicationData.IsAppConnected === true && isConnectionIndicatorVisible === true && applicationData.ShowIndicator)
        {
            connectionStatusIcon.visible = true
        }
        else
        {
            connectionStatusIcon.visible = false
        }
    }

    Label
    {
        id: fontlabel
        text: ""
    }

    Rectangle
    {
        id: titleRect
        width: parent.width
        height: headerID.height * 0.6
        color: applicationData.Theme.BackgroundColor

        ToolButton
        {
            id: backBtnID            
            anchors.verticalCenter: parent.verticalCenter
            height: titleRect.height
            width: titleRect.height
            visible: isMenuButtonVisible
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

            onClicked:
            {
                menuButtonClicked();
            }

            action:
            {
                openMenuAction
            }

            icon.source :
            if(applicationData.IsDarkTheme === true)
            {
                return "../images/MenuWhite.png";
            }
            else
            {
                return "../images/MenuBlack.png";
            }

            icon.color: "transparent"
            icon.height: titleRect.height*0.5
            icon.width: titleRect.height*0.5

            background: Rectangle
            {
                color: applicationData.Theme.BackgroundColor
            }
        }

        Label
        {
            text: headerTitle
            font.bold: true
            color: applicationData.Theme.FontColor
            elide: Label.ElideRight
            horizontalAlignment: Qt.AlignHCenter
            verticalAlignment: Qt.AlignVCenter
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            font.pointSize: fontSizeBig
        }
    }

    Rectangle
    {
        id: infoRect
        width: parent.width
        height: headerID.height * 0.4
        anchors.top: titleRect.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        color: applicationData.Theme.BackgroundColor

        Rectangle
        {
            id: infoRectInterior
            height: parent.height
            width: parent.width*0.94
            anchors.horizontalCenter: parent.horizontalCenter
            color: applicationData.Theme.BackgroundColor

            Image
            {
                id: connectionStatusIcon
                height: infoRectInterior.height*0.6
                width: infoRectInterior.height*0.6
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                source : "../images/GreenDot.png"
                visible: isConnectionIndicatorVisible
            }

            Text
            {
                color: applicationData.Theme.FontColor
                anchors.left: connectionStatusIcon.right
                anchors.leftMargin: 5
                anchors.verticalCenter: parent.verticalCenter
                text: applicationData.CurrentMeterSerialNo;
                font.pointSize: fontSizeSmall
                visible: isMeterNameVisible
            }

            Text
            {
                id: syncTimeStamp
                color: applicationData.Theme.FontColor
                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter
                text: applicationData.AppSyncTime;
                font.pointSize: fontSizeSmall
                visible: isSyncDateVisible
            }

            Text
            {
                id: progressPerCent
                color: applicationData.Theme.FontColor
                anchors.rightMargin: 5
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: progressAnimator.left
                text: "0%"
                font.pointSize: fontSizeSmall
                visible: isProgressStateVisible
            }

            BusyIndicator
            {
                id: progressAnimator
                height: infoRect.height*0.85
                width: infoRect.height*0.85
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: infoRectInterior.right
                anchors.rightMargin: 0
                running: true
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

                visible: isProgressStateVisible
            }
        }
    }

    Connections
    {
        target: applicationData

        function onWorkflowProgress(str)
        {
            if(str === 0)
            {
                progressPerCent.text = " ";
            }
            else
            {
                progressPerCent.text = str.toString()+"%";
            }
        }

        function onProgressIndicatorsOff()
        {
            progressPerCent.visible = false;
            progressAnimator.visible = false;
        }

        function onProgressIndicatorsOn()
        {
            if(applicationData.ShowIndicator)
            {
                progressPerCent.visible = true;
                progressAnimator.visible = true;
            }
        }

        function onNetworkIndicatorsOff()
        {
            connectionStatusIcon.visible = false
        }

        function onNetworkIndicatorsOn()
        {
            if(applicationData.ShowIndicator)
            {
                connectionStatusIcon.visible = true
            }
        }
    }
}
