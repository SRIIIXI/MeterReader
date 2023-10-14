import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.3
import QtQuick.Layouts 1.3

Page
{
    id: availableTariffsPage

    Material.theme: Material.Light
    Material.accent: "#961C1C"

    property variant availableTariffsModel :  []

    Header
    {
        id:headerID
        headerTitle: "Available Tariffs"
        isOptionsBtnVisible:  false
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
        availableTariffsModel = applicationData.TariffList
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
        id: tariffListView
        width: parent.width*0.90
        anchors.top:headerID.bottom
        anchors.topMargin: 10
        anchors.bottom: tariffAddCommandRect.top
        anchors.horizontalCenter: parent.horizontalCenter
        visible: true
        spacing: 10
        clip: true
        model: availableTariffsModel
        delegate: tariffListDelegate
    }

    Component
    {
        id: tariffListDelegate

        Rectangle
        {
            id: availableTariffsItemID
            width: parent.width
            height: availableTariffsPage.width*0.3
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

            property int tokenStatus : availableTariffsModel[index].IsApplied

            Column
            {
                id:col1
                anchors.verticalCenter: parent.verticalCenter
                anchors.margins: 20
                anchors.left: availableTariffsItemID.left

                Rectangle
                {
                    id: rectData1
                    height: availableTariffsItemID.height
                    width: availableTariffsItemID.height*0.8

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
                        id: tokenNolabel1
                        text: "Rate 1 Import"
                        font.pointSize: headerID.fontSizeSmall
                        font.bold: true
                        anchors.top: rectData1.top
                        anchors.topMargin: rectData1.height*0.1

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
                        id: tokenNolabel2
                        anchors.top: tokenNolabel1.bottom
                        text: "Rate 1 Export"
                        font.pointSize: headerID.fontSizeSmall
                        font.bold: true

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
                        id: tokenNolabel3
                        anchors.top: tokenNolabel2.bottom
                        text: "Rate 2 Import"
                        font.pointSize: headerID.fontSizeSmall
                        font.bold: true

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
                        id: tokenNolabel4
                        anchors.top: tokenNolabel3.bottom
                        text: "Rate 2 Export"
                        font.pointSize: headerID.fontSizeSmall
                        font.bold: true

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
                        id: tokenNolabel5
                        anchors.top: tokenNolabel4.bottom
                        text: "Effective Date"
                        font.pointSize: headerID.fontSizeSmall
                        font.bold: true

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
                }
            }

            Column
            {
                id: col2
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: col1.right
                anchors.leftMargin: availableTariffsItemID.height*0.025

                Rectangle
                {
                    id: rectData2
                    height: availableTariffsItemID.height
                    width: availableTariffsItemID.height*0.8

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
                        id: tokenNoValue1
                        text: availableTariffsModel[index].Import1.toFixed(2)
                        font.pointSize: headerID.fontSizeSmall
                        anchors.top: rectData2.top
                        anchors.topMargin: rectData2.height*0.1

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
                        id: tokenNoValue2
                        anchors.top: tokenNoValue1.bottom
                        text: availableTariffsModel[index].Export1.toFixed(2)
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
                    }

                    Text
                    {
                        id: tokenNoValue3
                        anchors.top: tokenNoValue2.bottom
                        text: availableTariffsModel[index].Import2.toFixed(2)
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
                    }

                    Text
                    {
                        id: tokenNoValue4
                        anchors.top: tokenNoValue3.bottom
                        text: availableTariffsModel[index].Export2.toFixed(2)
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
                    }

                    Text
                    {
                        id: tokenNoValue5
                        anchors.top: tokenNoValue4.bottom
                        text: availableTariffsModel[index].EffectiveDateStr
                        font.pointSize: headerID.fontSizeSmall
                        color: "red"
                    }
                }
            }

            Column
            {
                id: col3
                anchors.verticalCenter: parent.verticalCenter
                anchors.margins: 0
                anchors.right: availableTariffsItemID.right
                anchors.rightMargin: 10

                Rectangle
                {
                    id: rectButtons
                    height: availableTariffsItemID.height
                    width: (availableTariffsItemID.height*0.35)*3

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
                        width: (availableTariffsItemID.height*0.35)*3
                        height: availableTariffsItemID.height*0.4
                        enabled: !availableTariffsModel[index].IsApplied
                        text: "Remove"
                        onClicked:
                        {
                            confirm.itemIndex = index;
                            confirm.itemValue = availableTariffsModel[index].EffectiveDateStr;
                            confirm.open();
                        }

                        background: Rectangle
                        {
                                color: "#961C1C"
                                border.width: 1
                                border.color: "#961C1C"
                                radius: 0.2  * btnRemove.height
                        }

                        contentItem: Text
                        {
                               text: "Remove"
                               font: btnRemove.font
                               opacity: enabled ? 1.0 : 0.3
                               color: btnRemove.down ? "gray" : "white"
                               horizontalAlignment: Text.AlignHCenter
                               verticalAlignment: Text.AlignVCenter
                               elide: Text.ElideRight
                        }
                    }
                }
            }
        }
    }

    Rectangle
    {
        id: tariffAddCommandRect
        width: availableTariffsPage.width
        height: availableTariffsPage.width*0.2
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
            bottom:parent.bottom
            horizontalCenter: parent.horizontalCenter
        }

        //ToolButton
        ToolButton
        {
            id: name
            action: navigateAddTariff
            height: tariffAddCommandRect.height*0.75
            width: tariffAddCommandRect.height*0.75
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
            icon.height: tariffAddCommandRect.height*0.5
            icon.width: tariffAddCommandRect.height*0.5
            anchors.horizontalCenter:tariffAddCommandRect.horizontalCenter

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
            text: "Add a new Tariff"
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
            height: tariffAddCommandRect.height*0.5
            horizontalAlignment: Qt.AlignHCenter
            verticalAlignment: Qt.AlignVCenter
            anchors.bottom: tariffAddCommandRect.bottom
            anchors.horizontalCenter:tariffAddCommandRect.horizontalCenter
        }
    }

    Popup
    {
        id: confirm
        visible: false
        width: availableTariffsPage.width*0.9
        height: (availableTariffsPage.width*0.9)/1.5
        x: (availableTariffsPage.width - confirm.width)/2
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
                text: "Do you want to remove the tariff ending " + availableTariffsModel[confirm.itemIndex].EffectiveDateStr + " ?"
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
                    width: tariffListView.width*0.25
                    height: tariffListView.width*0.125
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
                    width: tariffListView.width*0.25
                    height: tariffListView.width*0.125
                    text: "Yes"
                    onClicked:
                    {
                        confirm.close()
                        applicationData.invokeRemoveTariff(availableTariffsModel[confirm.itemIndex].TariffId)
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

        function onTariffAction()
        {
            availableTariffsModel = applicationData.TariffList
        }
    }
}

