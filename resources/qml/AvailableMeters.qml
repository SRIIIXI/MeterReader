import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.3
import QtQuick.Layouts 1.3

Page
{
    id: availableMetersPage

    Material.theme: Material.Light
    Material.accent: applicationData.Theme.AccentColor

    property variant availableMetersModel :  []
    property string selectedMeter:  " "


    Component.onCompleted:
    {
        confirm.close()
        headerPanel.visible = true
        headerPanel.isSyncDateVisible = false
        availableMetersModel = applicationData.MeterList
        errorLabel.text = ""
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
        headerTitle: "Available Meters"
        isMenuButtonVisible: true
        isConnectionIndicatorVisible: applicationData.IsAppConnected
        isMeterNameVisible: applicationData.IsAppConnected
        isSyncDateVisible: applicationData.IsAppConnected
    }

    ListView
    {
        id: meterListView
        width: parent.width*0.90
        anchors.top:headerPanel.bottom
        anchors.topMargin: 5
        anchors.bottom: errorLabel.top
        anchors.horizontalCenter: parent.horizontalCenter
        visible: true
        spacing: 5
        clip: true
        model: availableMetersModel
        delegate: meterListDelegate
    }

    Component
    {
        id: meterListDelegate

        Rectangle
        {
            id: availableMetersItemID
            width: meterListView.width
            height: availableMetersPage.width*0.3
            radius: 5
            color: applicationData.Theme.ControlColor

            property string meterStatus  : availableMetersModel[index].StatusStr

            Column
            {
                id: imagecol
                anchors.left: availableMetersItemID.left
                height: parent.height
                width: availableMetersItemID.height*0.45

                Rectangle
                {
                    height: imagecol.height
                    width: imagecol.width
                    color: "transparent"

                   ToolButton
                   {
                       id: imageBtnID
                       height: imagecol.width*0.9
                       width: imagecol.width*0.9
                       anchors.verticalCenter: parent.verticalCenter

                       onClicked:
                       {
                           applicationData.invokeSelectMeter(availableMetersModel[index].SerialNo)
                       }

                       icon.source: "../images/MeterItem.png"
                       icon.color: "transparent"
                       icon.height: availableMetersItemID.height*0.35
                       icon.width: availableMetersItemID.height*0.35

                       background: Rectangle
                       {
                           color: applicationData.Theme.ControlColor
                       }
                   }
                }
            }

            Column
            {
                id: infocol
                anchors.verticalCenter: parent.verticalCenter
                anchors.margins: 20
                anchors.left: imagecol.right

                Rectangle
                {
                    id: rectData
                    height: availableMetersItemID.height
                    width:  availableMetersItemID.height*1.5
                    color: applicationData.Theme.ControlColor

                    Text
                    {
                        id: meterNamelabel
                        text: availableMetersModel[index].MeterName
                        font.pointSize: headerPanel.fontSizeSmall
                        font.bold: true
                        anchors.top: rectData.top
                        anchors.topMargin: rectData.height*0.1
                        color: applicationData.Theme.FontColor
                    }

                    Text
                    {
                        id: serialNolabel
                        anchors.top: meterNamelabel.bottom
                        text: availableMetersModel[index].SerialNo
                        font.pointSize: headerPanel.fontSizeNormal
                        font.bold: availableMetersModel[index].IsSelected ? true : false
                        font.underline: availableMetersModel[index].IsSelected ? true : false
                        color: applicationData.Theme.FontColor
                    }

                    Text
                    {
                        id: syncDatelabel
                        anchors.top: serialNolabel.bottom
                        text: availableMetersModel[index].LastSyncStr
                        font.pointSize: headerPanel.fontSizeSmall
                        font.bold: false
                        color: applicationData.Theme.FontColor
                    }

                    Text
                    {
                        id: rssilabel
                        anchors.top: syncDatelabel.bottom
                        text: "Signal " + availableMetersModel[index].RSSIStr
                        font.pointSize: headerPanel.fontSizeSmall
                        font.bold: false
                        color: applicationData.Theme.FontColor
                    }

                    Text
                    {
                        anchors.top: rssilabel.bottom
                        font.pointSize: headerPanel.fontSizeNormal
                        color:
                        {
                            if(meterStatus === "0")
                                return "red"
                            else
                                return "green"
                        }

                        font.bold:
                        {
                            if(meterStatus === "0")
                                return false
                            else
                                return true
                        }

                        elide: Label.ElideRight
                        horizontalAlignment: Qt.AlignHCenter
                        verticalAlignment: Qt.AlignVCenter
                        text:
                        {
                            if(meterStatus === "0")
                                return "Not Connected"
                            else
                                return "Connected"
                        }

                        visible:
                        {
                            if(availableMetersModel[index].SerialNo === "EDM1DEMO" || availableMetersModel[index].SerialNo === "EDM2DEMO")
                            {
                                return false
                            }
                            else
                            {
                                return true
                            }
                        }
                    }
                }
            }

            Column
            {
                id: commandcol
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: availableMetersItemID.right
                anchors.rightMargin: 10

                Rectangle
                {
                    id: rectButtons
                    height: availableMetersItemID.height
                    width: (availableMetersItemID.height*0.35)*3
                    color: applicationData.Theme.ControlColor

                    CustomButton
                    {
                        id: btnRemove
                        text: "Remove"
                        anchors.right: rectButtons.right
                        anchors.top: rectButtons.top
                        anchors.topMargin: rectButtons.height*0.1
                        width: (availableMetersItemID.height*0.35)*3
                        height: availableMetersItemID.height*0.35
                        isDefault: false
                        accentColor: applicationData.Theme.AccentColor
                        enabled:
                        {
                            if(meterStatus === "0")
                                return true
                            else
                                return false
                        }

                        visible: true

                        onClicked:
                        {
                            confirm.itemIndex = index;
                            selectedMeter = availableMetersModel[index].SerialNo;
                            confirm.open();
                        }
                    }

                    CustomButton
                    {
                        id: btnConnect
                        anchors.right: rectButtons.right
                        anchors.bottom: rectButtons.bottom
                        anchors.bottomMargin: rectButtons.height*0.1
                        width: (availableMetersItemID.height*0.35)*3
                        height: availableMetersItemID.height*0.35
                        isDefault: true
                        accentColor: applicationData.Theme.AccentColor
                        enabled:
                        {
                            if(meterStatus === "0" && applicationData.IsAppConnected === true)
                            {
                                return false;
                            }

                            if(meterStatus === "0" && applicationData.IsAppConnected === false)
                            {
                                return true;
                            }

                            if(meterStatus === "1" && applicationData.IsAppConnected === true)
                            {
                                return true;
                            }

                            if(meterStatus === "1" && applicationData.IsAppConnected === false)
                            {
                                return false;
                            }
                        }

                        visible: true

                        text:
                        {
                            if(meterStatus === "0")
                                return "Connect"
                            else
                                return "Disconnect"
                        }

                        onClicked:
                        {
                            errorLabel.text = ""
                            meterListView.enabled = false;
                            meterAddCommandRect.enabled = false;
                            addMeterBtn.enabled = false;
                            addMeterBtn.visible = false;
                            addLbl.visible = false;
                            headerPanel.isMenuButtonVisible = false
                            if(meterStatus === "0")
                            {
                                applicationData.invokeConnectMeter(availableMetersModel[index].SerialNo)
                            }
                            else
                            {
                                if(meterStatus === "1")
                                {
                                    applicationData.invokeDisconnectMeter(availableMetersModel[index].SerialNo)
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    Label
    {
        id: errorLabel
        anchors.bottom: meterAddCommandRect.top
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
        id: meterAddCommandRect
        width: availableMetersPage.width
        height: availableMetersPage.width*0.2
        radius: 5
        color: applicationData.Theme.NavigationBarColor
        anchors
        {
            bottom: parent.bottom
            horizontalCenter: parent.horizontalCenter
        }

        ToolButton
        {
            id: addMeterBtn
            action: navigateAddMeter
            height: meterAddCommandRect.height*0.75
            width: meterAddCommandRect.height*0.75
            icon.color: "transparent"
            icon.height: meterAddCommandRect.height*0.5
            icon.width: meterAddCommandRect.height*0.5
            anchors.horizontalCenter:meterAddCommandRect.horizontalCenter

            background: Rectangle
            {
                color: applicationData.Theme.NavigationBarColor
            }

            enabled:
            {
               if(applicationData.IsAppConnected === true)
               {
                   return false;
               }

               if(applicationData.IsAppConnected === false)
               {
                   return true;
               }
            }

            icon.source:
            {
                if(applicationData.IsAppConnected === true)
                {
                    return "../images/AddGrey.png";
                }

                if(applicationData.IsAppConnected === false)
                {
                    return "../images/Add.png";
                }
            }
        }

        Label
        {
            id: addLbl
            text: "Add a new Meter"
            color: applicationData.Theme.FontColor
            height: meterAddCommandRect.height*0.5
            elide: Label.ElideRight
            horizontalAlignment: Qt.AlignHCenter
            verticalAlignment: Qt.AlignVCenter
            anchors.bottom: meterAddCommandRect.bottom
            anchors.horizontalCenter:meterAddCommandRect.horizontalCenter
        }
    }

    Popup
    {
        id: confirm
        visible: false
        width: availableMetersPage.width*0.9
        height: (availableMetersPage.width*0.9)/1.5
        x: (availableMetersPage.width - confirm.width)/2
        y: parent.height/3
        padding: 0
        modal: true
        property int itemIndex: 0
        property string itemValue: ""

        contentItem: Rectangle
        {
            color: applicationData.Theme.ControlColor
            anchors.fill: parent
            border.color: applicationData.Theme.AccentColor

            Text
            {
                id: confirmText
                text: "Do you want to remove the meter " + selectedMeter + " ?"
                color: applicationData.Theme.FontColor
                wrapMode: "Wrap"
                width: confirm.width*0.9
                anchors.centerIn: parent
            }

            Grid
            {
                id: gridDlg
                rows: 1
                columns: 2
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 10
                anchors.rightMargin: 10
                anchors.right: parent.right
                spacing: 10

                CustomButton
                {
                    id:btnNo
                    width: meterListView.width*0.25
                    height: meterListView.width*0.125
                    text: "No"
                    isDefault: false
                    accentColor: applicationData.Theme.AccentColor
                    onClicked:
                    {
                        confirm.close();
                    }
                }

                CustomButton
                {
                    id:btnYes
                    width: meterListView.width*0.25
                    height: meterListView.width*0.125
                    text: "Yes"
                    isDefault: true
                    accentColor: applicationData.Theme.AccentColor
                    onClicked:
                    {
                        confirm.close()
                        applicationData.invokeRemoveMeter(availableMetersModel[confirm.itemIndex].SerialNo)
                    }
                }
            }
        }
    }

    Connections
    {
        target: applicationData

        function onMeterAction(fl)
        {
            confirm.close()
            availableMetersModel = applicationData.MeterList

            if(fl === true)
            {
                meterListView.enabled = true;
                meterAddCommandRect.enabled = true;
                headerPanel.isMeterNameVisible = true;
                headerPanel.isSyncDateVisible = true;
                headerPanel.isMenuButtonVisible = true
                addMeterBtn.enabled = true;
                addMeterBtn.visible = true;
                addLbl.visible = true;
            }
            else
            {
                meterListView.enabled = false;
                meterAddCommandRect.enabled = false;
                headerPanel.isMeterNameVisible = false;
                headerPanel.isSyncDateVisible = false;
                headerPanel.isMenuButtonVisible = false
                addMeterBtn.enabled = false;
                addMeterBtn.visible = false;
                addLbl.visible = false;
            }
        }

        function onMeterMessage(fl, str)
        {
            if(fl === true)
            {
                errorLabel.color = "green"
            }
            else
            {
                errorLabel.color = "red"
            }

            errorLabel.text = str
            confirm.close()
            availableMetersModel = applicationData.MeterList
        }
    }
}


