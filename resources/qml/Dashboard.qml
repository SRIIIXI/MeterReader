import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.3
import QtQuick.Layouts 1.3

Page
{
    id: homePage

    Material.theme: Material.Light
    Material.accent: applicationData.Theme.AccentColor

    Component.onCompleted:
    {
        creditBalanceGraph.value = applicationData.RemainingCreditPercent.toFixed(2);
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
        id : headerPanel
        headerTitle: "Home"
        isMenuButtonVisible: true
        isMeterNameVisible: true
        isSyncDateVisible: true
        isConnectionIndicatorVisible: applicationData.IsAppConnected
    }

    Rectangle
    {
        id: graphRect
        width: homePage.width*0.9
        height: (homePage.height - (headerPanel.height + quickArea.height + rectEnergy.height + rectAlarms.height + rectAlarms.height*0.6 + 10))
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: quickArea.top
        anchors.bottomMargin: rectAlarms.height*0.2
        anchors.topMargin: 0
        radius: 5
        color: applicationData.Theme.ControlColor

        Text
        {
            id: txtAccountCurrency
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.topMargin: 10
            anchors.top: graphRect.top
            Material.accent: applicationData.Theme.AccentColor
            text: applicationData.CreditCurrency;
            font.bold: true
            color: applicationData.Theme.FontColor
        }

        CircularProgressBar
        {
            id: creditBalanceGraph
            lineWidth: (graphRect.height*0.6)*0.1
            value: applicationData.RemainingCreditPercent.toFixed(2)
            size: graphRect.height*0.7
            secondaryColor: applicationData.Theme.AccentColorLow
            primaryColor: applicationData.Theme.AccentColor
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
        }

        Text
        {
            id: mainText
            text: "Remaining\n" + applicationData.RemainingCreditStr + " " + applicationData.CreditCurrency
            anchors.centerIn: parent
            font.pointSize: headerPanel.fontSizeSmall
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            color: applicationData.Theme.FontColor
        }

        Rectangle
        {
            id:divider
            width:2
            height: parent.height
            color:"transparent"
            radius: 2
            anchors.bottom: parent.bottom
            anchors.topMargin: 0
            anchors.horizontalCenter: parent.horizontalCenter
        }

        Text
        {
            id: lblTotal
            text: "Total"
            anchors.right: divider.right
            anchors.rightMargin: 10
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 10
            color: applicationData.Theme.FontColor
        }

        Text
        {
            id: txtTotal
            text: applicationData.GrossCreditStr + " " + applicationData.CreditCurrency
            anchors.left: divider.left
            anchors.leftMargin:  10
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 10
            color: applicationData.Theme.FontColor
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
        anchors.topMargin: 10
        color: applicationData.Theme.ControlColor
        radius: 5

        Text
        {
            id: quickStatus
            anchors.right: verticalLine.right
            anchors.rightMargin: 10
            anchors.top: parent.top
            anchors.topMargin: 10
            text: qsTr("Quick Status")
            font.bold: true
            color:applicationData.Theme.FontColor
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
            color: applicationData.Theme.FontColor
        }

        Rectangle
        {
            id:horizontalLine
            width: parent.width
            height: 2
            color:applicationData.Theme.AccentColor
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
            color: applicationData.Theme.AccentColor
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
            text:  applicationData.DeliveredEnergyStr + " kWh"
            font.bold: true
            color: applicationData.Theme.FontColor
            font.pointSize: headerPanel.fontSizeLarge
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
            horizontalAlignment: Text.AlignRight
            font.pointSize: headerPanel.fontSizeSmall
            color: applicationData.Theme.FontColor
        }

        Text
        {
            id: activeEnergyvalue
            anchors.left: verticalLine.left
            anchors.leftMargin:  10
            anchors.top: horizontalLine.bottom
            anchors.topMargin: 10
            text: applicationData.InstantaneousPowerStr + " kW"
            font.bold: true
            color: applicationData.Theme.FontColor
            font.pointSize: headerPanel.fontSizeLarge
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
            font.pointSize: headerPanel.fontSizeSmall
            color: applicationData.Theme.FontColor
        }
    }

    Rectangle
    {
        id: rectEnergy
        width:homePage.width * 0.4
        height:homePage.height * 0.125
        color: applicationData.Theme.ControlColor
        radius: 2
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
            color: applicationData.Theme.FontColor
        }

        Image
        {
            id: imgEnergyID
            source: "../images/Energy.png"
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
        color: applicationData.Theme.ControlColor
        radius: 2
        anchors.right: quickArea.right
        anchors.bottom: rectTokens.top
        anchors.bottomMargin: rectTokens.height*0.1

        Text
        {
            id: tariff
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
            anchors.topMargin: 5
            text: qsTr("Help")
            font.bold: true
            color: applicationData.Theme.FontColor
        }

        Image
        {
            id: imgTariff
            source: "../images/Help.png"
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
                    applicationData.invokeChangePage(19)
                }
            }
        }
    }

    Rectangle
    {
        id: rectAlarms
        width:homePage.width * 0.4
        height:homePage.height * 0.125
        color: applicationData.Theme.ControlColor
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
            color: applicationData.Theme.FontColor
        }

        Image
        {
            id: imgAlarmId
            source: "../images/Alarm.png"
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom:  parent.bottom
            anchors.bottomMargin:  5
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
        color: applicationData.Theme.ControlColor
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
            color: applicationData.Theme.FontColor
        }

        Image
        {
            id: imgTokenID
            source: "../images/Token.png"
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
        height: 5
        color: applicationData.Theme.BackgroundColor
    }

    Connections
    {
        target: applicationData

        function onUpdateDashboard()
        {
            creditBalanceGraph.value = applicationData.RemainingCreditPercent.toFixed(2);
            creditBalanceGraph.valueChange();
        }
    }
}




