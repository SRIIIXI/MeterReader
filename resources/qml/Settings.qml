import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.3
import QtQuick.Layouts 1.3

Page
{
    id: settingsPage

    Material.theme: Material.Light
    Material.accent: applicationData.Theme.AccentColor

    Component.onCompleted:
    {
        if(applicationData.IsDemoMode === true)
        {
            demoSelector.checked = true;
        }
        else
        {
            demoSelector.checked = false;
        }

        if(applicationData.IsTraceEnabled === true)
        {
            traceSelector.checked = true;
        }
        else
        {
            traceSelector.checked = false;
        }

        if(applicationData.IsTraceEnabled === true)
        {
            dumpSelector.enabled = true;
        }
        else
        {
            dumpSelector.enabled = false;
        }

        if(applicationData.IsDumpRxTxEnabled === true)
        {
            dumpSelector.checked = true;
        }
        else
        {
            dumpSelector.checked = false;
        }
    }

    Rectangle
    {
        id: background
        width: parent.width
        height: parent.height - headerID.height
        anchors.top: headerID.bottom
        color: applicationData.Theme.BackgroundColor
    }

    Header
    {
        id:headerID
        headerTitle: "Settings"
        isMenuButtonVisible: true
        isMeterNameVisible: false
        isSyncDateVisible: false
        isConnectionIndicatorVisible: false
    }

    Rectangle
    {
        id: line0
        width: settingsPage.width*0.9
        height: 2
        border.width: 1
        anchors.top: headerID.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        border.color: applicationData.Theme.BackgroundColor
    }

    CustomSelector
    {
        id: demoSelector
        anchors.top:  line0.bottom
        anchors.topMargin: headerID.height*0.05
        anchors.left: line0.left
        selectorSize: 32
        text: "Demo Mode"
        accentColor: applicationData.Theme.AccentColor
        fontColor: applicationData.Theme.FontColor
        backgroundColor: applicationData.Theme.BackgroundColor

        onClicked:
        {
            applicationData.IsDemoMode = demoSelector.checked
        }
    }

    Rectangle
    {
        id: line2
        width: settingsPage.width*0.9
        height: 2
        border.width: 1
        anchors.top: demoSelector.bottom
        anchors.topMargin: headerID.height*0.05
        anchors.horizontalCenter: parent.horizontalCenter
        border.color: applicationData.Theme.AccentColor
    }

    CustomSelector
    {
        id: traceSelector
        anchors.top:  line2.bottom
        anchors.topMargin: headerID.height*0.05
        anchors.left: line0.left
        selectorSize: 32
        text: "Trace"
        accentColor: applicationData.Theme.AccentColor
        fontColor: applicationData.Theme.FontColor
        backgroundColor: applicationData.Theme.BackgroundColor

        onClicked:
        {
            applicationData.IsTraceEnabled = traceSelector.checked

            if(applicationData.IsTraceEnabled === true)
            {
                dumpSelector.enabled = true;
            }
            else
            {
                dumpSelector.enabled = false;
            }
        }
    }

    Rectangle
    {
        id: line3
        width: settingsPage.width*0.9
        height: 2
        border.width: 1
        anchors.top: traceSelector.bottom
        anchors.topMargin: headerID.height*0.05
        anchors.horizontalCenter: parent.horizontalCenter
        border.color: applicationData.Theme.AccentColor
    }

    CustomSelector
    {
        id: dumpSelector
        anchors.top:  line3.bottom
        anchors.topMargin: headerID.height*0.05
        anchors.left: line0.left
        selectorSize: 32
        text: "Dump Rx/Tx"
        accentColor: applicationData.Theme.AccentColor
        fontColor: applicationData.Theme.FontColor
        backgroundColor: applicationData.Theme.BackgroundColor

        onClicked:
        {
            applicationData.IsDumpRxTxEnabled = dumpSelector.checked
        }
    }

    Label
    {
        id: swVersion
        text: "Version " + applicationData.AppVersion
        color: "darkgrey"
        font.pointSize: headerID.fontSizeSmall
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottomMargin: 10
    }
}

