import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.3
import QtQuick.Layouts 1.3

Page
{
    id: tracePage

    Material.theme: Material.Light
    Material.accent: applicationData.Theme.AccentColor

    Rectangle
    {
        id: background
        width: parent.width
        height: parent.height - headerPanel.height
        anchors.top: headerPanel.bottom
        color: applicationData.Theme.BackgroundColor
    }

    Label
    {
        id: fontlabel
        text: ""
    }

    Header
    {
        id:headerPanel
        headerTitle: "Trace"
        isMenuButtonVisible: true
        isMeterNameVisible: true
        isSyncDateVisible: true
        isConnectionIndicatorVisible: applicationData.IsAppConnected
    }

    ScrollView
    {
        id: traceScrollView
        width: parent.width
        height: parent.height - headerPanel.height
        anchors.top: headerPanel.bottom
        ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
        ScrollBar.vertical.policy: ScrollBar.AlwaysOn
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

        TextArea
        {
            id: trace
            readOnly: true
            anchors.fill: parent
            font.pointSize: fontlabel.font.pointSize -  2.5
            color: applicationData.Theme.FontColor
            text: applicationData.TraceString

            onTextChanged:
            {
                traceScrollView.ScrollBar.postition = 1
            }

            background: Rectangle
            {
                color: applicationData.Theme.BackgroundColor
            }
        }
    }
}
