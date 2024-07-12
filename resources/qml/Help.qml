import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.3
import QtQuick.Layouts 1.3

Page
{
    id: helpPage

    Material.theme: Material.Light
    Material.accent: applicationData.Theme.AccentColor

    property int helpIndex: 0
    property variant helpStringModel :  []

    Component.onCompleted:
    {
        helpStringModel = applicationData.HelpStrings;
        topicList.currentIndex = helpIndex;
        loadHelpContentsByIndex(helpIndex);
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
        headerTitle: "Help"
        isMenuButtonVisible: true
        isMeterNameVisible: false
        isSyncDateVisible: false
        isConnectionIndicatorVisible: false
    }

    ComboBox
    {
        id: topicList
        height: headerPanel.height*0.5
        width: helpPage.width*0.9
        font.pointSize: headerPanel.fontSizeSmall
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top:headerPanel.bottom
        Material.accent: applicationData.Theme.AccentColor

        popup: Popup
        {
            id:comboPopup
            y: topicList.height - 1
            width: topicList.width
            height:contentItem.implicitHeight
            padding: 1
            font.pointSize: headerPanel.fontSizeSmall

            contentItem: ListView
            {
                id:listView
                implicitHeight: contentHeight
                model: topicList.popup.visible ? topicList.delegateModel : null
                ScrollIndicator.vertical: ScrollIndicator { }
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

                delegate: Text
                {
                    font.pointSize: headerPanel.fontSizeSmall
                }
            }

            background: Rectangle
            {
                border.width: 1
                border.color: applicationData.Theme.AccentColor
                color: applicationData.Theme.ControlColor
            }
        }

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

        onCurrentIndexChanged:
        {
            helpIndex = currentIndex;
            loadHelpContentsByIndex(helpIndex);
        }

        model: ["General", "Add a Meter", "Connect to a Meter", "Add a Token", "Transfer a Token"]
    }

    TextArea
    {
        id: helpText
        readOnly: true
        anchors.top: topicList.bottom
        width: helpPage.width*0.9
        anchors.horizontalCenter: parent.horizontalCenter
        font.pointSize: headerPanel.fontSizeSmall
        color: applicationData.Theme.FontColor

        background: Rectangle
        {
            color: applicationData.Theme.BackgroundColor
        }
    }

    function loadHelpContentsByIndex(idx)
    {
        helpText.clear();

        applicationData.invokeLoadHelpStrings(idx);
        helpStringModel = applicationData.HelpStrings;

        for(var x = 0; x < helpStringModel.length; x++)
        {
            var str = helpStringModel[x];
            helpText.append(str+"\n");
        }
    }
}

