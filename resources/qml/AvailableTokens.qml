import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.3
import QtQuick.Layouts 1.3

Page
{
    id: availableTokensPage

    Material.theme: Material.Light
    Material.accent: applicationData.Theme.AccentColor

    property variant availableTokensModel :  []
    property variant availableTokenHistoriesModel :  []
    property string selectedToken:  " "

    Component.onCompleted:
    {
        tokenListView.visible = true
        tokenHistoryListView.visible = false
        tokenListSelector.currentIndex = 0
        availableTokensModel = applicationData.TokenList
        availableTokenHistoriesModel = applicationData.TokenHistoryList
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
        headerTitle: "Available Tokens"
        isMenuButtonVisible: true
        isMeterNameVisible: true
        isSyncDateVisible: true
        isConnectionIndicatorVisible: applicationData.IsAppConnected
    }

    CustomTab
    {
        id: tokenListSelector
        tabHeight: headerPanel.height*0.4
        tabWidth: availableTokensPage.width*0.9
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top:headerPanel.bottom
        model: ["New", "Transferred"]
        accentColor: applicationData.Theme.AccentColor
        accentLowColor: applicationData.Theme.AccentColor
        controlLowColor: applicationData.Theme.ControlColor
        controlColor: applicationData.Theme.ControlColor
        fontColor: applicationData.Theme.FontColor

        onCurrentIndexChanged:
        {
            if(currentIndex === 0)
            {
                tokenListView.visible = true
                tokenHistoryListView.visible = false
            }

            if(currentIndex === 1)
            {
                tokenListView.visible = false
                tokenHistoryListView.visible = true
            }
        }
    }

    ListView
    {
        id: tokenListView
        width: parent.width*0.90
        anchors.top:tokenListSelector.bottom
        anchors.topMargin: 10
        anchors.bottom: errorLabel.top
        anchors.horizontalCenter: parent.horizontalCenter
        visible: true
        spacing: 10
        clip: true
        model: availableTokensModel
        delegate: tokenListDelegate
    }

    Component
    {
        id: tokenListDelegate

        Rectangle
        {
            id: availableTokensItemID
            width: tokenListView.width
            height: availableTokensPage.width*0.3
            radius: 5
            color: applicationData.Theme.ControlColor

            property int tokenStatus : availableTokensModel[index].IsTokenNew

            Column
            {
                id: imagecol
                anchors.left: availableTokensItemID.left
                height: parent.height
                width: availableTokensItemID.height*0.45

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
                       icon.source: "../images/TokenItem.png"
                       icon.color: "transparent"
                       icon.height: availableTokensItemID.height*0.33
                       icon.width: availableTokensItemID.height*0.33

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
                anchors.rightMargin: 10
                anchors.leftMargin: 5
                anchors.left: imagecol.right

                Rectangle
                {
                    id: rectData
                    height: availableTokensItemID.height
                    width: availableTokensItemID.height*1.5
                    color: applicationData.Theme.ControlColor

                    Text
                    {
                        id: tokenNolabel
                        text: "Token No."
                        font.pointSize: headerPanel.fontSizeSmall
                        font.bold: true
                        anchors.top: rectData.top
                        anchors.topMargin: rectData.height*0.1
                        color: applicationData.Theme.FontColor
                    }

                    Text
                    {
                        id: tokenNoValue
                        anchors.top: tokenNolabel.bottom
                        text: availableTokensModel[index].TokenId
                        font.pointSize: headerPanel.fontSizeSmall
                        color: applicationData.Theme.FontColor
                    }

                    Text
                    {
                        id: tokenStatusLabel
                        anchors.top: tokenNoValue.bottom
                        font.bold: true
                        text: availableTokensModel[index].TokenStatusStr
                        font.pointSize: headerPanel.fontSizeSmall
                        color: applicationData.Theme.FontColor
                    }

                    Text
                    {
                        id: tokenValue
                        anchors.top: tokenStatusLabel.bottom
                        text: availableTokensModel[index].TokenValue.toFixed(2)+ " " + applicationData.CreditCurrency
                        font.bold: true
                        font.pointSize: headerPanel.fontSizeSmall
                        color: applicationData.Theme.FontColor
                    }
                }
            }

            Column
            {
                id: commandcol
                anchors.verticalCenter: parent.verticalCenter
                anchors.margins: 0
                anchors.right: availableTokensItemID.right
                anchors.rightMargin: 10

                Rectangle
                {
                    id: rectButtons
                    height: availableTokensItemID.height
                    width: (availableTokensItemID.height*0.35)*3
                    color: applicationData.Theme.ControlColor

                    CustomButton
                    {
                        id: btnRemove
                        anchors.right: rectButtons.right
                        anchors.top: rectButtons.top
                        anchors.topMargin: rectButtons.height*0.1
                        width: (availableTokensItemID.height*0.35)*3
                        height: availableTokensItemID.height*0.35
                        isDefault: false
                        text: "Remove"
                        accentColor: applicationData.Theme.AccentColor
                        onClicked:
                        {
                            confirm.itemIndex = index;
                            headerPanel.isMenuButtonVisible = false
                            selectedToken = availableTokensModel[index].TokenId;
                            confirm.itemValue = availableTokensModel[index].TokenId;
                            confirm.open();
                        }
                    }

                    CustomButton
                    {
                        id: btnApply
                        anchors.right: rectButtons.right
                        anchors.bottom: rectButtons.bottom
                        anchors.bottomMargin: rectButtons.height*0.1
                        width: (availableTokensItemID.height*0.35)*3
                        height: availableTokensItemID.height*0.35
                        isDefault: true
                        text: "Apply"
                        accentColor: applicationData.Theme.AccentColor
                        onClicked:
                        {
                            errorLabel.text = ""
                            tokenListView.enabled = false;
                            headerPanel.isMenuButtonVisible = false;
                            tokenAddCommandRect.enabled = false;
                            addTokenBtn.enabled = false
                            addTokenBtn.visible = false
                            addLbl.visible = false
                            applicationData.invokeApplyToken(availableTokensModel[index].TokenId)
                        }
                    }
                }
            }
        }
    }

    ListView
    {
        id: tokenHistoryListView
        width: parent.width*0.90
        anchors.top:tokenListSelector.bottom
        anchors.topMargin: 10
        anchors.bottom: errorLabel.top
        anchors.horizontalCenter: parent.horizontalCenter
        visible: true
        spacing: 10
        clip: true
        model: availableTokenHistoriesModel
        delegate: tokenHistoryListDelegate
    }

    Component
    {
        id: tokenHistoryListDelegate

        Rectangle
        {
            id: availableTokenHistoriesItemID
            width: tokenHistoryListView.width
            height: availableTokensPage.width*0.3
            radius: 5
            color: applicationData.Theme.ControlColor

            Column
            {
                id: imagecol
                anchors.left: availableTokenHistoriesItemID.left
                height: parent.height
                width: availableTokenHistoriesItemID.height*0.45

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
                       icon.source: "../images/TokenItem.png"
                       icon.color: "transparent"
                       icon.height: availableTokenHistoriesItemID.height*0.33
                       icon.width: availableTokenHistoriesItemID.height*0.33

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
                anchors.rightMargin: 10
                anchors.leftMargin: 5
                anchors.left: imagecol.right

                Rectangle
                {
                    id: rectData
                    height: availableTokenHistoriesItemID.height
                    width: availableTokenHistoriesItemID.height*1.5
                    color: applicationData.Theme.ControlColor

                    Text
                    {
                        id: tokenNolabel
                        text: "Token No."
                        font.pointSize: headerPanel.fontSizeSmall
                        font.bold: true
                        anchors.top: rectData.top
                        anchors.topMargin: rectData.height*0.1
                        color: applicationData.Theme.FontColor
                    }

                    Text
                    {
                        id: tokenNoValue
                        anchors.top: tokenNolabel.bottom
                        text: availableTokenHistoriesModel[index].TokenId
                        font.pointSize: headerPanel.fontSizeSmall
                        color: applicationData.Theme.FontColor
                    }

                    Text
                    {
                        id: tokenDateLabel
                        anchors.top: tokenNoValue.bottom
                        font.bold: true
                        text: availableTokenHistoriesModel[index].AppliedDateStr
                        font.pointSize: headerPanel.fontSizeSmall
                        color: applicationData.Theme.FontColor
                    }

                    Text
                    {
                        id: tokenValue
                        anchors.top: tokenDateLabel.bottom
                        text: availableTokenHistoriesModel[index].TokenValue.toFixed(2)+ " " + applicationData.CreditCurrency
                        font.bold: true
                        font.pointSize: headerPanel.fontSizeSmall
                        color: applicationData.Theme.FontColor
                    }
                }
            }
        }
    }


    Label
    {
        id: errorLabel
        anchors.bottom: tokenAddCommandRect.top
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
        id: tokenAddCommandRect
        width: availableTokensPage.width
        height: availableTokensPage.width*0.2
        radius: 5
        color: applicationData.Theme.NavigationBarColor

        anchors
        {
            bottom:parent.bottom
            horizontalCenter: parent.horizontalCenter
        }

        ToolButton
        {
            id: addTokenBtn
            action: navigateAddToken
            height: tokenAddCommandRect.height*0.75
            width: tokenAddCommandRect.height*0.75
            icon.source: "../images/Add.png"
            icon.color: "transparent"
            icon.height: tokenAddCommandRect.height*0.5
            icon.width: tokenAddCommandRect.height*0.5
            anchors.horizontalCenter:tokenAddCommandRect.horizontalCenter

            background: Rectangle
            {
                color: applicationData.Theme.NavigationBarColor
            }
        }

        Label
        {
            id: addLbl
            text: "Add a new Token"
            color: applicationData.Theme.FontColor
            height: tokenAddCommandRect.height*0.5
            elide: Label.ElideRight
            horizontalAlignment: Qt.AlignHCenter
            verticalAlignment: Qt.AlignVCenter
            anchors.bottom: tokenAddCommandRect.bottom
            anchors.horizontalCenter:tokenAddCommandRect.horizontalCenter
        }
    }

    Popup
    {
        id: confirm
        visible: false
        width: availableTokensPage.width*0.9
        height: (availableTokensPage.width*0.9)/1.5
        x: (availableTokensPage.width - confirm.width)/2
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
                text: "Do you want to remove the token " + selectedToken + " ?"
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
                    height: tokenListView.width*0.125
                    width: tokenListView.width*0.25
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
                    height: tokenListView.width*0.125
                    width: tokenListView.width*0.25
                    text: "Yes"
                    isDefault: true
                    accentColor: applicationData.Theme.AccentColor
                    onClicked:
                    {
                        confirm.close()
                        applicationData.invokeRemoveToken(availableTokensModel[confirm.itemIndex].TokenId)
                    }
                }
            }
        }
    }

    Connections
    {
        target: applicationData

        function onTokenAction()
        {
            availableTokensModel = applicationData.TokenList
            availableTokenHistoriesModel = applicationData.TokenHistoryList
            tokenListView.enabled = true;
            tokenAddCommandRect.enabled = true;
            headerPanel.isMenuButtonVisible = true
            addTokenBtn.enabled = true
            addTokenBtn.visible = true
            addLbl.visible = true
        }

        function onTokenTransferState(fl, str)
        {
            errorLabel.text = str

            if(fl === true)
            {
                errorLabel.color = "green"
            }
            else
            {
                errorLabel.color = "red"
            }

            tokenListView.enabled = true;
            tokenAddCommandRect.enabled = true;
            headerPanel.isMenuButtonVisible = true
            addTokenBtn.enabled = true
            addTokenBtn.visible = true
            addLbl.visible = true
        }
    }
}

