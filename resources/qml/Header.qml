import QtQuick 2.5
import QtQuick.Controls 2.5
import QtQuick.Controls.Material 2.3

Rectangle
{
    id: headerID

    Material.theme: Material.Light
    Material.accent: "#961C1C"

    height: parent.width*0.25
    width: parent.width

    property bool  isBackBtnVisible: true
    property bool  isOptionsBtnVisible: true

    property bool  isMeterNameVisible: true
    property bool  isSyncDateVisible: true
    property bool  isInfoVisible: true
    property bool  isTitleVisible: true

    property string headerTitle: ""
    property string textcolorname: ""
    property string meterName: applicationData.CurrentMeter.SerialNo
    property string syncDate: applicationData.AppSyncTime
    property alias backBtn: backBtnID
    property alias optionsBtn: optionsBtnID

    property double fontSizeNormal: fontlabel.font.pointSize
    property double fontSizeSmall: fontlabel.font.pointSize -  2.5
    property double fontSizeTiny: fontlabel.font.pointSize - 5.0
    property double fontSizeBig: fontlabel.font.pointSize +  2.5
    property double fontSizeLarge: fontlabel.font.pointSize + 5.0

    // Info section
    property bool bleConnected: false
    property bool progressVisible: false
    property string progressText: "0%"

    signal backBtnClicked();
    signal optionsBtnClicked();

    Component.onCompleted:
    {
        fontSizeNormal: fontlabel.font.pointSize
        fontSizeSmall: fontlabel.font.pointSize -  2.5
        fontSizeTiny: fontlabel.font.pointSize - 5.0
        fontSizeBig: fontlabel.font.pointSize +  2.5
        fontSizeLarge: fontlabel.font.pointSize + 5.0

        if(applicationData.IsDarkTheme === true)
        {
            textcolorname = "white";
        }
        else
        {
            textcolorname = "black";
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

        ToolButton
        {
            id: backBtnID            
            anchors.verticalCenter: parent.verticalCenter
            visible: isBackBtnVisible
            enabled: isBackBtnVisible
            action: navigateBackAction
            height: titleRect.height
            width: titleRect.height
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
                backBtnClicked()
            }

            icon.source :
            if(applicationData.IsDarkTheme === true)
            {
                return "../images/ArrowLeftWhite.png";
            }
            else
            {
                return "../images/ArrowLeftBlack.png";
            }
            icon.color: "transparent"
            icon.height: titleRect.height*0.5
            icon.width: titleRect.height*0.5

            background: Rectangle
            {
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
        }

        Label
        {
            text: headerTitle
            visible: isTitleVisible
            font.bold: true

            color:
            {
                if(applicationData.IsDarkTheme === true)
                {
                    return "white";
                }
                else
                {
                    return "color";
                }
            }

            elide: Label.ElideRight
            horizontalAlignment: Qt.AlignHCenter
            verticalAlignment: Qt.AlignVCenter
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            font.pointSize: fontSizeBig
        }

        Row
        {
            id:leftRowId
            anchors.right:titleRect.right
            anchors.verticalCenter: parent.verticalCenter
            spacing: 2

            ToolButton
            {
                id: optionsBtnID
                visible: isOptionsBtnVisible
                enabled: isOptionsBtnVisible
                height: titleRect.height
                width: titleRect.height
                onClicked:
                {
                    optionsBtnClicked()
                }
                icon.source:
                if(applicationData.IsDarkTheme === true)
                {
                    return "../images/FilterWhite.png";
                }
                else
                {
                    return "../images/FilterBlack.png";
                }
                icon.color: "transparent"
                icon.height: titleRect.height*0.5
                icon.width: titleRect.height*0.5

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

                background: Rectangle
                {
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
            }
        }
    }

    Rectangle
    {
        id: infoRect
        width: parent.width
        height: headerID.height * 0.4
        anchors.top: titleRect.bottom
        anchors.horizontalCenter: parent.horizontalCenter

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
            id: infoRectInterior
            height: parent.height
            width: parent.width

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

            ToolButton
            {
                id: bleStatusIcon
                enabled: bleConnected
                height: infoRect.height
                width: infoRect.height
                anchors.left: infoRectInterior.left
                visible: isMeterNameVisible

                onClicked:
                {
                    optionsBtnClicked()
                }

                icon.source:
                if(applicationData.IsAppConnected === true)
                {
                    return "../images/Bluetooth.png";
                }
                else
                {
                    return "../images/BluetoothGrey.png";
                }

                icon.color: "transparent"
                icon.height: titleRect.height*0.5
                icon.width: titleRect.height*0.5

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

                background: Rectangle
                {
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
            }

            Text
            {
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
                anchors.left: bleStatusIcon.right
                anchors.leftMargin: 5
                anchors.verticalCenter: parent.verticalCenter
                text:
                {
                    if(isInfoVisible === true)
                    {
                        return applicationData.CurrentMeter.SerialNo;
                    }
                    else
                    {
                        return " ";
                    }
                }
                font.pointSize: fontSizeSmall
                visible: isMeterNameVisible
            }

            Text
            {
                id: syncTimeStamp
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

                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter

                text:
                {
                    if(isInfoVisible === true)
                    {
                        return applicationData.AppSyncTime;
                    }
                    else
                    {
                        return " ";
                    }
                }
                font.pointSize: fontSizeSmall
            }

            Text
            {
                id: progressPerCent
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

                anchors.rightMargin: 5
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: dl.left
                text: "0%"
                font.pointSize: fontSizeSmall
                visible:
                {
                    if(isInfoVisible === true && progressVisible === true)
                    {
                        return true;
                    }
                    else
                    {
                        return false;
                    }
                }
            }

            BusyIndicator
            {
                id: dl
                height: infoRect.height*0.85
                width: infoRect.height*0.85
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: infoRectInterior.right
                anchors.rightMargin: 0
                running: true
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

                visible:
                {
                    if(isInfoVisible === true && progressVisible === true)
                    {
                        return true;
                    }
                    else
                    {
                        return false;
                    }
                }
            }
        }
    }

    Connections
    {
        target: applicationData
        function onUpdateSyncTime(fl, errorstr)
        {
        }

        function onConnectionProgress(str)
        {
            progressPerCent.text = str;
        }

        function onMeterAdditionProgress(str)
        {
            progressPerCent.text = str;
        }

        function onTokenTransferProgress(str)
        {
            progressPerCent.text = str;
        }
    }
}
