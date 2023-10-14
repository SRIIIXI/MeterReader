import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.3
import QtQuick.Layouts 1.3

Page
{
    id: homePage
    objectName: "homePage"
    visible: true

    Material.theme: Material.Light
    Material.accent: "#961C1C"

    property bool meterStatus: false
    property string lastSyncTime: applicationData.AppSyncTime
    property string meterName: applicationData.CurrentMeter.SerialNo

    Component.onCompleted:
    {
        progress1.value = (((applicationData.GrossCredit - applicationData.AvailableCredit)/applicationData.GrossCredit)).toFixed(2);
    }

    Header
    {
        id                  : headerID
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
        headerTitle         : applicationData.CurrentMeter.SerialNo + " Connected"
        meterName           : "last sync: "+ applicationData.AppSyncTime
        isOptionsBtnVisible: true
        isTitleVisible:
        {
            if(applicationData.IsDemoMode === true)
            {
                return true
            }
            else
            {
                applicationData.IsAppConnected
            }
        }
        isInfoVisible:
        {
            if(applicationData.IsDemoMode === true)
            {
                return true
            }
            else
            {
                applicationData.IsAppConnected
            }
        }
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

    Rectangle
    {
        id: graphRect
        width: homePage.width*0.9
        height: (homePage.height - (headerID.height + quickArea.height + rectEnergy.height + rectAlarms.height + rectAlarms.height*0.6))
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: quickArea.top
        anchors.bottomMargin: rectAlarms.height*0.2
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

        radius: 5

        Rectangle
        {
            id: rectselector
            width: graphRect.width
            height: graphRect.height*0.2
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.topMargin: 10
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
                id: selector
                height: parent.height
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                Material.accent: "#961C1C"
                text: applicationData.CreditCurrency;
                font.bold: true

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

        CircularProgressBar
        {
            id: progress1
            lineWidth: (graphRect.height*0.6)*0.1
            value: (((applicationData.GrossCredit - applicationData.AvailableCredit)/applicationData.GrossCredit)).toFixed(2)
            size: graphRect.height*0.7
            secondaryColor: "#e0e0e0"
            primaryColor: "#961C1C"
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: rectselector.bottom

            Text
            {
                id: mainText
                text: "Remaining\n" + applicationData.AvailableCredit.toFixed(2) + " " + applicationData.CreditCurrency
                anchors.centerIn: parent
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
        }

        Rectangle
        {
            id:divider
            width:2
            height: parent.height
            color:"transparent"
            radius: 2
            anchors.top: parent.top
            anchors.topMargin: 0
            anchors.horizontalCenter: parent.horizontalCenter
        }

        Text
        {
            id: lblRemaining
            text: "Total"
            anchors.right: divider.right
            anchors.rightMargin: 10
            anchors.top: progress1.bottom
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
            id: txtRemaining
            text: applicationData.GrossCredit.toFixed(2) + " " + applicationData.CreditCurrency
            anchors.left: divider.left
            anchors.leftMargin:  10
            anchors.top: progress1.bottom
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

    Rectangle
    {
        id:quickArea
        width: homePage.width * 0.9
        height: homePage.height* 0.165
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: rectEnergy.top
        anchors.bottomMargin: rectAlarms.height*0.2
        anchors.topMargin: 20
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
        }        radius: 5

        Text
        {
            id: quickStatus
            anchors.right: verticalLine.right
            anchors.rightMargin: 10
            anchors.top: parent.top
            anchors.topMargin: 10
            text: qsTr("Quick Status")
            font.bold: true
            color: "#961C1C"
        }

        Text
        {
            id: prepared
            anchors.left: verticalLine.left
            anchors.leftMargin:  10
            anchors.top: parent.top
            anchors.topMargin: 10
            text:
            {
                if(applicationData.PaymentType === 1)
                {
                    return "Post Paid (" + applicationData.CreditCurrency + ")"
                }
                else
                {
                    return "Pre Paid (" + applicationData.CreditCurrency + ")"
                }
            }

            font.bold: true
            color: "#961C1C"
        }

        Rectangle
        {
            id:horizontalLine
            width: parent.width
            height: 2
            color:"#961C1C"
            radius: 0
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top:parent.top
            anchors.topMargin: quickArea.height*0.3
        }

        Rectangle
        {
            id:verticalLine
            width:2
            height: parent.height - quickArea.height*0.3 - horizontalLine.height
            color:"#961C1C"
            radius: 2
            anchors.top: horizontalLine.bottom
            anchors.topMargin: 0
            anchors.horizontalCenter: parent.horizontalCenter
        }

        Text
        {
            id: eneryConsumvalue
            anchors.right: verticalLine.right
            anchors.rightMargin: 10
            anchors.top: horizontalLine.bottom
            anchors.topMargin: 10
            text:  applicationData.InstantaneousPower.toFixed(2) +" kWh"
            font.bold: true
            color: "#961C1C"
            font.pointSize: headerID.fontSizeLarge
        }

        Text
        {
            id: eneryConsum
            anchors.right: verticalLine.right
            anchors.rightMargin: 10
            anchors.top: eneryConsumvalue.bottom
            anchors.topMargin: 5
            text: qsTr("Active Energy Consumed")
            width: parent.width*0.4
            wrapMode: "Wrap"
            font.bold: true
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
            id: activeEnergyvalue
            anchors.left: verticalLine.left
            anchors.leftMargin:  10
            anchors.top: horizontalLine.bottom
            anchors.topMargin: 10
            text: applicationData.DeliveredEnergy.toFixed(2) +" kW"
            font.bold: true
            color: "#961C1C"
            font.pointSize: headerID.fontSizeLarge
        }

        Text
        {
            id: activeEnergy
            anchors.left: verticalLine.left
            anchors.leftMargin:  10
            anchors.topMargin: 5
            anchors.top: activeEnergyvalue.bottom
            text: qsTr("Instantaneous Active Power")
            wrapMode: "Wrap"
            width: parent.width*0.4
            font.bold: true
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
    }

    Rectangle
    {
        id: rectEnergy
        width:homePage.width * 0.4
        height:homePage.height * 0.125
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
        }        radius: 2
        anchors.left: quickArea.left
        anchors.bottom: rectAlarms.top
        anchors.bottomMargin: rectAlarms.height*0.1

        Text
        {
            id: energyID
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
            anchors.topMargin: 5
            text: qsTr("Energy")
            font.bold: true
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

        Image
        {
            id: imgEnergyID
            source: "../images/EnergyRed.png"
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom:  parent.bottom
            anchors.bottomMargin:  10
            height: parent.height*0.6
            width: parent.height*0.6

            MouseArea
            {
                anchors.fill: parent
                onClicked:
                {
                    applicationData.invokeChangePage(1)
                }
            }
        }
    }

    Rectangle
    {
        id: rectTariff
        width:homePage.width * 0.4
        height:homePage.height * 0.125
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
        }        radius: 2
        anchors.right: quickArea.right
        anchors.bottom: rectTokens.top
        anchors.bottomMargin: rectTokens.height*0.1

        Text
        {
            id: tariff
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
            anchors.topMargin: 5
            text: qsTr("Tariff")
            font.bold: true
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

        Image
        {
            id: imgTariff
            source: "../images/TariffRed.png"
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom:  parent.bottom
            anchors.bottomMargin:  10
            height: parent.height*0.6
            width: parent.height*0.6
            MouseArea
            {
                anchors.fill: parent
                onClicked:
                {
                    applicationData.invokeChangePage(17)
                }
            }
        }
    }

    Rectangle
    {
        id: rectAlarms
        width:homePage.width * 0.4
        height:homePage.height * 0.125
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

        anchors.left: quickArea.left
        anchors.bottom: buffer.top
        radius: 2
        anchors.topMargin: 10

        Text
        {
            id: alarmsID
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
            anchors.topMargin: 5
            text: qsTr("Alarms")
            font.bold: true
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

        Image
        {
            id: imgAlarmId
            source: "../images/AlarmRed.png"
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom:  parent.bottom
            anchors.bottomMargin:  10
            height: parent.height*0.6
            width: parent.height*0.6

            MouseArea
            {
                anchors.fill: parent
                onClicked:
                {
                    applicationData.invokeChangePage(3)
                }
            }
        }
    }

    Rectangle
    {
        id: rectTokens
        width:homePage.width * 0.4
        height:homePage.height * 0.125
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

        radius: 2
        anchors.right: quickArea.right
        anchors.bottom: buffer.top
        anchors.topMargin: 10

        Text
        {
            id: tokensID
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
            anchors.topMargin: 5
            text: qsTr("Tokens")
            font.bold: true
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

        Image
        {
            id: imgTokenID
            source: "../images/TokenRed.png"
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom:  parent.bottom
            anchors.bottomMargin:  10
            height: parent.height*0.6
            width: parent.height*0.6

            MouseArea
            {
                anchors.fill: parent
                onClicked:
                {
                    applicationData.invokeClearToken()
                    applicationData.invokeChangePage(5)
                }
            }
        }
    }

    Rectangle
    {
        id: buffer
        anchors.bottom: parent.bottom
        width: parent.width
        height: headerID.height*0.15

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

    Connections
    {
        target: applicationData
        function onDashboardRefreshed()
        {
            headerID.backBtn.enabled = true;
            selector1.enabled = true;
        }

        function onDashboardError(str)
        {
        }
    }
}




