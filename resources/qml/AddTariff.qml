import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.3
import QtQuick.Layouts 1.3

Page
{
    id: addTariffPage

    Material.theme: Material.Light
    Material.accent: "#961C1C"

    property bool meterStatus: false
    property string lastSyncTime: applicationData.CurrentMeter.LastSyncStr
    property string meterName: applicationData.CurrentMeter.SerialNo

    Keys.onReleased:
    {
        if(Qt.platform.os === "android")
        {
            if (event.key === Qt.Key_Back)
            {
                event.accepted = true
                applicationData.pageAction(17)
            }
        }
    }

    Component.onCompleted:
    {
        txtImport1Edit.text = "00.00"
        txtExport1Edit.text = "00.00"
        txtImport2Edit.text = "00.00"
        txtExport2Edit.text = "00.00"
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

    Header
    {
        id:headerID
        headerTitle: "Add New Tariff"
        meterName  : applicationData.CurrentMeter.SerialNo
        syncDate   : applicationData.CurrentMeter.LastSyncStr
        isOptionsBtnVisible:  false
        isMeterNameVisible: true
        isSyncDateVisible: true
        backBtn.action      : navigateAvailableTariffs
    }

    Label
    {
        id: tariffHeader1
        font.bold: true
        text: "Tariff 1"
        width: addTariffPage.width*0.9
        anchors.top: headerID.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: headerID.height*0.05
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
    }


    Rectangle
    {
        id: tariff1Rect
        width: addTariffPage.width*0.9
        height: addTariffPage.width*0.1
        anchors.top: tariffHeader1.bottom
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

        Label
        {
            id: lblImport1
            text: "Import (USD)"
            font.pointSize: headerID.fontSizeSmall
            anchors.left: tariff1Rect.left
            anchors.bottom: tariff1Rect.bottom
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
        }

        TextField
        {
            id: txtImport1Edit
            font.pointSize: headerID.fontSizeSmall
            focus: true
            height: addTariffPage.width*0.1
            inputMethodHints: Qt.ImhDigitsOnly
            anchors.left: lblImport1.right
            anchors.bottom: tariff1Rect.bottom
            anchors.leftMargin: parent.width*0.05
            width: parent.width*0.15
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

            onTextChanged:
            {
                errorLabel.text = ""
            }
        }

        Label
        {
            id: lblExport1
            text: "Export (USD)"
            font.pointSize: headerID.fontSizeSmall
            anchors.right: txtExport1Edit.left
            anchors.bottom: tariff1Rect.bottom
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
        }

        TextField
        {
            id: txtExport1Edit
            font.pointSize: headerID.fontSizeSmall
            focus: true
            height: addTariffPage.width*0.1
            inputMethodHints: Qt.ImhDigitsOnly
            anchors.right: tariff1Rect.right
            anchors.bottom: tariff1Rect.bottom
            anchors.leftMargin: parent.width*0.05
            width: parent.width*0.15
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

            onTextChanged:
            {
                errorLabel.text = ""
            }
        }
    }

    Rectangle
    {
        id: line1
        width: addTariffPage.width*0.9
        height: 2
        border.width: 1
        anchors.top: tariff1Rect.bottom
        anchors.topMargin: headerID.height*0.05
        anchors.horizontalCenter: parent.horizontalCenter
        border.color:
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


    Label
    {
        id: tariffHeader2
        font.bold: true
        text: "Tariff 2"
        width: addTariffPage.width*0.9
        anchors.top: line1.bottom
        anchors.topMargin: headerID.height*0.05
        anchors.horizontalCenter: parent.horizontalCenter
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
    }

    Rectangle
    {
        id: tariff2Rect
        width: addTariffPage.width*0.9
        height: addTariffPage.width*0.1
        anchors.top: tariffHeader2.bottom
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

        Label
        {
            id: lblImport2
            text: "Import (USD)"
            font.pointSize: headerID.fontSizeSmall
            anchors.left: tariff2Rect.left
            anchors.bottom: tariff2Rect.bottom
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
        }

        TextField
        {
            id: txtImport2Edit
            font.pointSize: headerID.fontSizeSmall
            focus: true
            height: addTariffPage.width*0.1
            inputMethodHints: Qt.ImhDigitsOnly
            anchors.left: lblImport2.right
            anchors.bottom: tariff2Rect.bottom
            anchors.leftMargin: parent.width*0.05
            width: parent.width*0.15
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

            onTextChanged:
            {
                errorLabel.text = ""
            }
        }

        Label
        {
            id: lblExport2
            text: "Export (USD)"
            font.pointSize: headerID.fontSizeSmall
            anchors.right: txtExport2Edit.left
            anchors.bottom: tariff2Rect.bottom
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
        }

        TextField
        {
            id: txtExport2Edit
            font.pointSize: headerID.fontSizeSmall
            focus: true
            height: addTariffPage.width*0.1
            inputMethodHints: Qt.ImhDigitsOnly
            anchors.right: tariff2Rect.right
            anchors.bottom: tariff2Rect.bottom
            anchors.leftMargin: parent.width*0.05
            width: parent.width*0.15
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

            onTextChanged:
            {
                errorLabel.text = ""
            }
        }
    }

    Rectangle
    {
        id: line2
        width: addTariffPage.width*0.9
        height: 2
        border.width: 1
        anchors.top: tariff2Rect.bottom
        anchors.topMargin: headerID.height*0.05
        anchors.horizontalCenter: parent.horizontalCenter
        border.color:
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


    DatePicker
    {
        id: datepicker
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: line2.bottom
        width: addTariffPage.width * 0.9
        height: addTariffPage.width * 0.7
        anchors.topMargin: headerID.height*0.05
        Component.onCompleted: set(new Date()) // today
        onClicked:
        {
            errorLabel.text = ""
            console.log('onClicked', Qt.formatDate(selectedDate, 'dd/MM/yyyy').toString())
        }
    }

    Label
    {
        id: errorLabel
        width: addTariffPage.width * 0.9
        height: addTariffPage.width * 0.05
        anchors.top: datepicker.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        font.bold: true
        text: "Error"
        color: "red"
        wrapMode: "Wrap"
    }

    Grid
    {
        id: grid
        rows: 1
        columns: 2
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 5
        anchors.rightMargin: 10
        anchors.right: parent.right
        spacing: 10

        Button
        {
            id:btnAdd
            height: addTariffPage.width*0.15
            width: (addTariffPage.width*0.2)*2
            text: "Save"
            onClicked:
            {
                applicationData.invokeAddTariff(txtImport1Edit.text,  txtExport1Edit.text, txtImport2Edit.text, txtExport2Edit.text, Qt.formatDate(datepicker.selectedDate, 'dd/MM/yyyy').toString())
            }

            background: Rectangle
            {
                color: "#961C1C"
                border.width: 1
                border.color: "#961C1C"
                radius: 0.2  * btnAdd.height
            }

            contentItem: Text
            {
               text: "Save"
               font: btnAdd.font
               opacity: enabled ? 1.0 : 0.3
               color: btnAdd.down ? "gray" : "white"
               horizontalAlignment: Text.AlignHCenter
               verticalAlignment: Text.AlignVCenter
               elide: Text.ElideRight
            }
        }
    }

    Connections
    {
        target: applicationData

        function onTariffError(str)
        {
            console.log(str)
            errorLabel.text = str
            errorLabel.visible = true
        }
    }
}

