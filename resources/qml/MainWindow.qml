import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.3
import QtQuick.Layouts 1.3

ApplicationWindow
{
    id: mainView

    Material.theme: Material.Light
    Material.accent: applicationData.Theme.AccentColor

    objectName: "mainViewID"
    visible: true

    minimumHeight: 680
    minimumWidth: 320

    property int iconHeight : width*0.075
    property int iconWidth : width*0.075
    property bool isfirst: applicationData.IsFirstRun
    property string oldPage: ""
    property bool isClosing: false

    onClosing:
    {
        close.accepted = isClosing;
    }

    Component.onCompleted:
    {
        stackView.push("SplashScreen.qml")
    }

    Action
    {
        id: navigateMenuAction
        icon.name: stackView.depth > 1 ? "back" : "drawer"
        onTriggered:
        {
            if (stackView.depth > 1)
            {
                stackView.pop()
            }
            else
            {
                drawer.open()
            }
        }
    }

    Action
    {
        id: openMenuAction
        icon.name: stackView.depth > 1 ? "back" : "drawer"
        onTriggered:
        {
            drawer.open()
        }
    }

    Action
    {
        id: navigateBackAction
        icon.name: stackView.depth
        onTriggered:
        {
            applicationData.invokeSaveCurrentPage("Dashboard.qml")
            stackView.pop()
            stackView.push("Dashboard.qml")
        }
    }

    Action
    {
        id: resetPassCode
        icon.name: stackView.depth
        onTriggered:
        {
            applicationData.invokeSaveCurrentPage("ResetPasscodeStage1.qml")
            stackView.pop()
            stackView.push("ResetPasscodeStage1.qml")
        }
    }

    Action
    {
        id: navigateAddMeter
        icon.name: stackView.depth
        onTriggered:
        {
            applicationData.invokeSaveCurrentPage("AddMeter.qml")
            stackView.pop()
            stackView.push("AddMeter.qml")
        }
    }

    Action
    {
        id: navigateAvailableMeters
        icon.name: stackView.depth
        onTriggered:
        {
            applicationData.invokeSaveCurrentPage("AvailableMeters.qml")
            stackView.pop()
            stackView.push("AvailableMeters.qml")
        }
    }

    Action
    {
        id: navigateAvailableTokens
        icon.name: stackView.depth
        onTriggered:
        {
            applicationData.invokeSaveCurrentPage("AvailableTokens.qml")
            stackView.pop()
            stackView.push("AvailableTokens.qml")
        }
    }

    Action
    {
        id: navigateAddToken
        icon.name: stackView.depth
        onTriggered:
        {
            applicationData.invokeSaveCurrentPage("AddToken.qml")
            stackView.pop()
            stackView.push("AddToken.qml")
        }
    }

    Action
    {
        id: navigatAlarmSettingsAction
        icon.name: stackView.depth
        onTriggered:
        {
            applicationData.invokeSaveCurrentPage("AlarmSettings.qml")
            stackView.pop()
            stackView.push("AlarmSettings.qml")
        }
    }

    Action
    {
        id: navigatActiveAlarms
        icon.name: stackView.depth
        onTriggered:
        {
            applicationData.invokeSaveCurrentPage("Alarms.qml")
            stackView.pop()
            stackView.push("Alarms.qml")
        }
    }

    Action
    {
        id: navigateLogin
        icon.name: stackView.depth
        onTriggered:
        {
            applicationData.invokeSaveCurrentPage("Login.qml")
            stackView.pop()
            stackView.push("Login.qml")
        }
    }

    Action
    {
        id: navigateEnergy
        icon.name: stackView.depth
        onTriggered:
        {
            applicationData.invokeSaveCurrentPage("Energy.qml")
            stackView.pop()
            stackView.push("Energy.qml")
        }
    }

    Drawer
    {
        id: drawer
        width: mainView.width * 0.75
        height: mainView.height

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

        Column
        {
            anchors.fill: parent

            ItemDelegate
            {
                text: applicationData.CurrentUser.UserId
                width: parent.width

                icon
                {
                    source: "../images/User.png"
                    width: iconWidth
                    height: iconHeight
                }
            }

            ItemDelegate
            {
                text: qsTr("Home")
                width: parent.width

                onClicked:
                {                   
                    drawer.close()
                    applicationData.invokeSaveCurrentPage("Dashboard.qml")
                    stackView.pop()
                    stackView.push("Dashboard.qml")
                }

                icon
                {
                    source: "../images/Home.png"
                    width: iconWidth
                    height: iconHeight
                }

                enabled:
                {
                    if(applicationData.CurrentMeterSerialNo === "")
                    {
                        return false
                    }

                    return true
                }
            }

            ItemDelegate
            {
                text: qsTr("My Meters")
                width: parent.width
                onClicked:
                {
                    applicationData.invokeSaveCurrentPage("AvailableMeters.qml")
                    stackView.pop()
                    stackView.push("AvailableMeters.qml")
                    drawer.close()
                }

                icon
                {
                    source: "../images/Meter.png"
                    width: iconWidth
                    height: iconHeight
                }
            }

            ItemDelegate
            {
                text: qsTr("Tokens")
                width: parent.width

                onClicked:
                {
                    drawer.close()
                    applicationData.invokeSaveCurrentPage("AvailableTokens.qml")
                    stackView.pop()
                    stackView.push("AvailableTokens.qml")
                }

                icon
                {
                    source: "../images/Token.png"
                    width: iconWidth
                    height: iconHeight
                }

                enabled:
                {
                    if(applicationData.CurrentMeterSerialNo === "")
                    {
                        return false
                    }

                    return true
                }
            }

            ItemDelegate
            {
                text: qsTr("Alarms")
                width: parent.width

                onClicked:
                {
                    drawer.close()
                    applicationData.invokeSaveCurrentPage("Alarms.qml")
                    stackView.pop()
                    stackView.push("Alarms.qml")
                }

                icon
                {
                    source: "../images/Alarm.png"
                    width: iconWidth
                    height: iconHeight
                }

                enabled:
                {
                    if(applicationData.CurrentMeterSerialNo === "")
                    {
                        return false
                    }

                    return true
                }
            }

            ItemDelegate
            {
                text: qsTr("Energy")
                width: parent.width

                onClicked:
                {
                    drawer.close()
                    applicationData.invokeSaveCurrentPage("Energy.qml")
                    stackView.pop()
                    stackView.push("Energy.qml")
                }

                icon
                {
                    source: "../images/Energy.png"
                    width: iconWidth
                    height: iconHeight
                }

                enabled:
                {
                    if(applicationData.CurrentMeterSerialNo === "")
                    {
                        return false
                    }

                    return true
                }
            }

            ItemDelegate
            {
                text: qsTr("Settings")
                width: parent.width
                onClicked:
                {
                    applicationData.invokeSaveCurrentPage("Settings.qml")
                    stackView.pop()
                    stackView.push("Settings.qml")
                    drawer.close()
                }

                icon
                {
                    source: "../images/Settings.png"
                    width: iconWidth
                    height: iconHeight
                }
            }

            ItemDelegate
            {
                text: qsTr("Contact Us")
                width: parent.width
                onClicked:
                {
                    applicationData.invokeSaveCurrentPage("Contact.qml")
                    stackView.pop()
                    stackView.push("Contact.qml")
                    drawer.close()
                }

                icon
                {
                    source: "../images/ContactUs.png"
                    width: iconWidth
                    height: iconHeight
                }
            }

            ItemDelegate
            {
                text: qsTr("Help")
                width: parent.width
                onClicked:
                {
                    applicationData.invokeSaveCurrentPage("Help.qml")
                    stackView.pop()
                    stackView.push("Help.qml")
                    drawer.close()
                }

                icon
                {
                    source: "../images/Help.png"
                    width: iconWidth
                    height: iconHeight
                }
            }

            ItemDelegate
            {
                text: qsTr("Trace")
                width: parent.width
                onClicked:
                {
                    applicationData.invokeSaveCurrentPage("Trace.qml")
                    stackView.pop()
                    stackView.push("Trace.qml")
                    drawer.close()
                }

                icon
                {
                    source: "../images/Trace.png"
                    width: iconWidth
                    height: iconHeight
                }

                visible: applicationData.IsTraceEnabled
            }

            ItemDelegate
            {
                text: qsTr("Exit")
                width: parent.width
                onClicked:
                {
                    isClosing = true;
                    drawer.close();
                    applicationData.invokeExit();
                }

                icon
                {
                    source: "../images/Exit.png"
                    width: iconWidth
                    height: iconHeight
                }
            }
        }
    }

    StackView
    {
        id: stackView
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.left: parent.left
    }

    Connections
    {
        target: applicationData

        function onInitializationComplete()
        {
            if(isfirst === true)
            {
                stackView.pop()
                stackView.push("CreateUserStage1.qml")
            }
            else
            {
                stackView.pop()
                stackView.push("Login.qml")
            }
        }

        function onOperationCancelled()
        {
            stackView.pop()
            applicationData.invokeSaveCurrentPage("Dashboard.qml")
            stackView.push("Dashboard.qml")
        }

        function onTokenAdded()
        {
            stackView.pop();
            applicationData.invokeSaveCurrentPage("AvailableTokens.qml")
            stackView.push("AvailableTokens.qml");
        }

        function onMeterAdded()
        {
            stackView.pop();
            applicationData.invokeSaveCurrentPage("AvailableMeters.qml")
            stackView.push("AvailableMeters.qml");
        }

        function onUserLoginSucceeded()
        {
            stackView.pop();
            applicationData.invokeSaveCurrentPage("AvailableMeters.qml")
            stackView.push("AvailableMeters.qml");
        }

        function onAlarmSettingsChanged()
        {
            stackView.pop();
            applicationData.invokeSaveCurrentPage("Alarms.qml")
            stackView.push("Alarms.qml");
        }

        function onPageAction(pg)
        {
            if(pg === 0)
            {
                stackView.pop();
                oldPage = applicationData.invokeRetreiveLastPage()
                applicationData.invokeSaveCurrentPage(oldPage)
                stackView.push(oldPage);
            }

            if(pg === 1)
            {
                stackView.pop();
                applicationData.invokeSaveCurrentPage("Energy.qml")
                stackView.push("Energy.qml");
            }

            if(pg === 3)
            {
                stackView.pop();
                applicationData.invokeSaveCurrentPage("Alarms.qml")
                stackView.push("Alarms.qml");
            }

            if(pg === 4)
            {
                stackView.pop();
                applicationData.invokeSaveCurrentPage("AvailableMeters.qml")
                stackView.push("AvailableMeters.qml");
            }

            if(pg === 5)
            {
                stackView.pop();
                applicationData.invokeSaveCurrentPage("AvailableTokens.qml")
                stackView.push("AvailableTokens.qml");
            }

            if(pg === 6)
            {
                stackView.pop();
                applicationData.invokeSaveCurrentPage("QRCodeScanner.qml")
                stackView.push("QRCodeScanner.qml");
            }

            if(pg === 7)
            {
                stackView.pop();
                applicationData.invokeSaveCurrentPage("AddMeter.qml")
                stackView.push("AddMeter.qml");
            }

            if(pg === 8)
            {
                stackView.pop();
                applicationData.invokeSaveCurrentPage("AddToken.qml")
                stackView.push("AddToken.qml");
            }

            if(pg === 9)
            {
                stackView.pop();
                applicationData.invokeSaveCurrentPage("Login.qml")
                stackView.push("Login.qml");
            }

            if(pg === 10)
            {
                stackView.pop();
                applicationData.invokeSaveCurrentPage("CreateUserStage1.qml")
                stackView.push("CreateUserStage1.qml");
            }

            if(pg === 11)
            {
                stackView.pop();
                applicationData.invokeSaveCurrentPage("CreateUserStage2.qml")
                stackView.push("CreateUserStage2.qml");
            }

            if(pg === 12)
            {
                stackView.pop();
                applicationData.invokeSaveCurrentPage("CreateUserStage3.qml")
                stackView.push("CreateUserStage3.qml");
            }

            if(pg === 13)
            {
                stackView.pop();

                applicationData.invokeSaveCurrentPage("Dashboard.qml")
                stackView.push("Dashboard.qml")
            }

            if(pg === 14)
            {
                stackView.pop();
                applicationData.invokeSaveCurrentPage("ResetPasscodeStage1.qml")
                stackView.push("ResetPasscodeStage1.qml");
            }

            if(pg === 15)
            {
                stackView.pop();
                applicationData.invokeSaveCurrentPage("ResetPasscodeStage2.qml")
                stackView.push("ResetPasscodeStage2.qml");
            }

            if(pg === 16)
            {
                stackView.pop();
                applicationData.invokeSaveCurrentPage("ResetPasscodeStage3.qml")
                stackView.push("ResetPasscodeStage3.qml");
            }

            if(pg === 19)
            {
                stackView.pop();
                applicationData.invokeSaveCurrentPage("Help.qml")
                stackView.push("Help.qml");
            }
        }
    }
}
