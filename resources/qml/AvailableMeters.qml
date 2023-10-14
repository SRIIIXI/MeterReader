import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.3
import QtQuick.Layouts 1.3

Page
{
    id: availableMetersPage

    Material.theme: Material.Light
    Material.accent: "#961C1C"

    property variant availableMetersModel :  []
    property string errorstring: ""

    Header
    {
        id:headerID
        headerTitle: "Available Meters"
        isOptionsBtnVisible:  false
        isMeterNameVisible: false
        isSyncDateVisible: false
        isInfoVisible: false
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
    }

    Component.onCompleted:
    {
        confirm.close()
        headerID.visible = true
        headerID.isSyncDateVisible = false
        availableMetersModel = applicationData.MeterList
        errorLabel.text = ""
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
        id: meterListView
        width: parent.width*0.90
        anchors.top:headerID.bottom
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

            property string meterStatus  : availableMetersModel[index].StatusStr

            Column
            {
                id: infocol

                anchors.verticalCenter: parent.verticalCenter
                anchors.margins: 20
                anchors.left: availableMetersItemID.left


                Rectangle
                {
                    id: rectData
                    height: availableMetersItemID.height
                    width:  availableMetersItemID.height*1.5

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

                    Text
                    {
                        id: meterNamelabel
                        text: availableMetersModel[index].MeterName
                        font.pointSize: headerID.fontSizeSmall
                        font.bold: true
                        anchors.top: rectData.top
                        anchors.topMargin: rectData.height*0.1

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

                    Text
                    {
                        id: serialNolabel
                        anchors.top: meterNamelabel.bottom
                        text: availableMetersModel[index].SerialNo
                        font.pointSize: headerID.fontSizeSmall
                        font.bold: availableMetersModel[index].IsSelected ? true : false
                        font.underline: availableMetersModel[index].IsSelected ? true : false

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

                    Text
                    {
                        id: syncDatelabel
                        anchors.top: serialNolabel.bottom
                        text: availableMetersModel[index].LastSyncStr
                        font.pointSize: headerID.fontSizeSmall
                        font.bold: false

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

                    Text
                    {
                        id: rssilabel
                        anchors.top: syncDatelabel.bottom
                        text: "Signal " + availableMetersModel[index].RSSIStr
                        font.pointSize: headerID.fontSizeSmall
                        font.bold: false

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

                    Text
                    {
                        anchors.top: rssilabel.bottom
                        font.pointSize: headerID.fontSizeNormal
                        color:
                        {
                            if(meterStatus === '0')
                                return "red"
                            else
                                return "green"
                        }

                        font.bold:
                        {
                            if(meterStatus === '0')
                                return false
                            else
                                return true
                        }

                        elide: Label.ElideRight
                        horizontalAlignment: Qt.AlignHCenter
                        verticalAlignment: Qt.AlignVCenter
                        text:
                        {
                            if(meterStatus === '0')
                                return "Not Connected"
                            else
                                return "Connected"
                        }
                    }
                }
            }

            Column
            {
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: availableMetersItemID.right
                anchors.rightMargin: 10

                Rectangle
                {
                    id: rectButtons
                    height: availableMetersItemID.height
                    width: (availableMetersItemID.height*0.35)*3

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

                    Button
                    {
                        id: btnRemove
                        anchors.right: rectButtons.right
                        anchors.top: rectButtons.top
                        anchors.topMargin: rectButtons.height*0.1
                        width: (availableMetersItemID.height*0.35)*3
                        height: availableMetersItemID.height*0.4
                        enabled:
                        {
                            if(meterStatus === '0')
                                return true
                            else
                                return false
                        }

                        text: "Remove"
                        onClicked:
                        {
                            confirm.itemIndex = index;
                            confirm.itemValue = availableMetersModel[index].SerialNo;
                            confirm.open();
                        }

                        background: Rectangle
                        {
                            color: "white"
                            border.width: 1
                            border.color: "#961C1C"
                            radius: 0.2  * btnRemove.height
                        }

                        contentItem: Text
                        {
                           text: "Remove"
                           font: btnRemove.font
                           opacity: enabled ? 1.0 : 0.3
                           color: btnRemove.down ? "gray" : "black"
                           horizontalAlignment: Text.AlignHCenter
                           verticalAlignment: Text.AlignVCenter
                           elide: Text.ElideRight
                        }
                    }

                    Button
                    {
                        id: btnConnect
                        anchors.right: rectButtons.right
                        anchors.bottom: rectButtons.bottom
                        anchors.bottomMargin: rectButtons.height*0.1
                        width: (availableMetersItemID.height*0.35)*3
                        height: availableMetersItemID.height*0.4
                        enabled: true

                        text:
                        {
                            if(meterStatus === '0')
                                return "Connect"
                            else
                                return "Disconnect"
                        }

                        onClicked:
                        {
                            errorLabel.text = ""
                            meterListView.enabled = false;
                            headerID.backBtn.enabled = false;
                            meterAddCommandRect.enabled = false;
                            if(meterStatus === '0')
                            {
                                headerID.isInfoVisible = true;
                                headerID.progressVisible = true;
                                headerID.progressText = "0%";
                                applicationData.invokeConnectMeter(availableMetersModel[index].SerialNo)
                            }
                            else
                            {
                                if(meterStatus === '1')
                                {
                                    headerID.isInfoVisible = true;
                                    headerID.progressVisible = true;
                                    headerID.progressText = "0%";
                                    applicationData.invokeDisconnectMeter(availableMetersModel[index].SerialNo)
                                }
                            }
                        }

                        background: Rectangle
                        {
                            color: "#961C1C"
                            border.width: 1
                            border.color: "#961C1C"
                            radius: 0.2  * btnConnect.height
                        }

                        contentItem: Text
                        {
                           text: btnConnect.text
                           font: btnConnect.font
                           opacity: enabled ? 1.0 : 0.3
                           color: btnConnect.down ? "gray" : "white"
                           horizontalAlignment: Text.AlignHCenter
                           verticalAlignment: Text.AlignVCenter
                           elide: Text.ElideRight
                        }
                    }
                }
            }
        }
    }

    Label
    {
        id: errorLabel
        width: availableMetersPage.width * 0.9
        height: availableMetersPage.width * 0.05
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

        anchors
        {
            bottom: parent.bottom
            horizontalCenter: parent.horizontalCenter
        }

        ToolButton
        {
            id: name
            action: navigateAddMeter
            height: meterAddCommandRect.height*0.75
            width: meterAddCommandRect.height*0.75
            icon.source:
            if(applicationData.IsDarkTheme === true)
            {
                return "../images/AddWhite.png";
            }
            else
            {
                return "../images/AddBlack.png";
            }
            icon.color: "transparent"
            icon.height: meterAddCommandRect.height*0.5
            icon.width: meterAddCommandRect.height*0.5
            anchors.horizontalCenter:meterAddCommandRect.horizontalCenter

            background: Rectangle
            {
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
            }
        }

        Label
        {
            text: "Add a new Meter"

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
            anchors.fill: parent
            border.color: "#961C1C"

            Text
            {
                id: confirmText
                text: "Do you want to remove the meter " + availableMetersModel[confirm.itemIndex].SerialNo + " ?"
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
                Button
                {
                    id:btnNo
                    width: meterListView.width*0.25
                    height: meterListView.width*0.125
                    text: "No"
                    onClicked:
                    {
                        confirm.close();
                    }

                    background: Rectangle
                    {
                        color: "white"
                        border.width: 1
                        border.color: "#961C1C"
                        radius: 0.1  * btnNo.height
                    }

                    contentItem: Text
                    {
                       text: "No"
                       font: btnNo.font
                       opacity: enabled ? 1.0 : 0.3
                       color: btnNo.down ? "gray" : "black"
                       horizontalAlignment: Text.AlignHCenter
                       verticalAlignment: Text.AlignVCenter
                       elide: Text.ElideRight
                    }
                }

                Button
                {
                    id:btnYes
                    width: meterListView.width*0.25
                    height: meterListView.width*0.125
                    text: "Yes"
                    onClicked:
                    {
                        confirm.close()
                        applicationData.invokeRemoveMeter(availableMetersModel[confirm.itemIndex].SerialNo)
                    }

                    background: Rectangle
                    {
                        color: "#961C1C"
                        border.width: 1
                        border.color: "#961C1C"
                        radius: 0.2  * btnYes.height
                    }

                    contentItem: Text
                    {
                       text: "Yes"
                       font: btnYes.font
                       opacity: enabled ? 1.0 : 0.3
                       color: btnYes.down ? "gray" : "white"
                       horizontalAlignment: Text.AlignHCenter
                       verticalAlignment: Text.AlignVCenter
                       elide: Text.ElideRight
                    }
                }
            }
        }
    }

    Connections
    {
        target: applicationData

        function onMeterAction()
        {
            confirm.close()
            availableMetersModel = applicationData.MeterList
            meterListView.enabled = true;
            headerID.backBtn.enabled = true;
            meterAddCommandRect.enabled = true;
            headerID.isInfoVisible = false;
            headerID.progressVisible = false;
            headerID.progressText = "0%";

        }

        function onMeterError(str)
        {
            errorLabel.text = str
            confirm.close()
            availableMetersModel = applicationData.MeterList
            meterListView.enabled = true;
            headerID.backBtn.enabled = true;
            meterAddCommandRect.enabled = true;
            headerID.isInfoVisible = false;
            headerID.progressVisible = false;
            headerID.progressText = "0%";
        }
    }
}


