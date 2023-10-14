import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.3
import QtQuick.Layouts 1.3

Page
{
    id: availableTokensPage

    Material.theme: Material.Light
    Material.accent: "#961C1C"

    property variant availableTokensModel :  []

    Header
    {
        id:headerID
        headerTitle: "Available Tokens"
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
        availableTokensModel = applicationData.TokenList
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
        id: tokenListView
        width: parent.width*0.90
        anchors.top:headerID.bottom
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

            property int tokenStatus : availableTokensModel[index].IsApplied
            property int tokenInValid : availableTokensModel[index].IsInValid

            Column
            {
                anchors.verticalCenter: parent.verticalCenter
                anchors.margins: 20
                anchors.left: availableTokensItemID.left

                Rectangle
                {
                    id: rectData
                    height: availableTokensItemID.height
                    width: availableTokensItemID.height*1.5

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
                        id: tokenNolabel
                        text: "Token No."
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
                        id: tokenNoValue
                        anchors.top: tokenNolabel.bottom
                        text: availableTokensModel[index].TokenId
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
                        id: tokenDateLabel
                        anchors.top: tokenNoValue.bottom
                        font.bold: true
                        text:
                        {
                            if(availableTokensModel[index].IsApplied === true)
                                return "Applied On"
                            else
                                return " "
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
                        id: tokenDateValue
                        anchors.top: tokenDateLabel.bottom
                        text:
                        {
                            if(availableTokensModel[index].IsApplied === true)
                                return availableTokensModel[index].AppliedDateStr
                            else
                                return " "
                        }

                        color:
                        {
                            if(availableTokensModel[index].IsApplied === true)
                            {
                                if(availableTokensModel[index].TokenStatusText === "OK")
                                {
                                    return "green"
                                }
                                else
                                {
                                    return "red"
                                }
                            }
                            else
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

                        font.pointSize: headerID.fontSizeSmall
                    }

                    Text
                    {
                        id: energyValue
                        anchors.top: tokenDateValue.bottom
                        visible: tokenStatus

                        text:
                        {
                            if(availableTokensModel[index].IsApplied === true)
                            {
                                return applicationData.CreditCurrency + " " + availableTokensModel[index].TokenValue.toFixed(2)
                            }
                            else
                            {
                                return applicationData.CreditCurrency + " ";
                            }
                        }

                        font.bold: true

                        color:
                        {
                            if(availableTokensModel[index].IsApplied === true )
                            {
                                if(availableTokensModel[index].TokenStatusText === "OK")
                                {
                                    return "green"
                                }
                                else
                                {
                                    return "red"
                                }
                            }
                            else
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

                        font.pointSize: headerID.fontSizeSmall
                    }

                    Text
                    {
                        id: tokenStateDescription
                        anchors.top: energyValue.bottom
                        text: availableTokensModel[index].TokenStatusText
                        font.pointSize: headerID.fontSizeSmall

                        color:
                        {
                            if(availableTokensModel[index].IsApplied === true )
                            {
                                if(availableTokensModel[index].TokenStatusText === "OK")
                                {
                                    return "green"
                                }
                                else
                                {
                                    return "red"
                                }
                            }
                            else
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
            }

            Column
            {
                anchors.verticalCenter: parent.verticalCenter
                anchors.margins: 0
                anchors.right: availableTokensItemID.right
                anchors.rightMargin: 10

                Rectangle
                {
                    id: rectButtons
                    height: availableTokensItemID.height
                    width: (availableTokensItemID.height*0.35)*3

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
                        width: (availableTokensItemID.height*0.35)*3
                        height: availableTokensItemID.height*0.4
                        enabled: !availableTokensModel[index].IsApplied
                        text: "Remove"
                        onClicked:
                        {
                            confirm.itemIndex = index;
                            confirm.itemValue = availableTokensModel[index].TokenId;
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
                        id: btnApply
                        anchors.right: rectButtons.right
                        anchors.bottom: rectButtons.bottom
                        anchors.bottomMargin: rectButtons.height*0.1
                        width: (availableTokensItemID.height*0.35)*3
                        height: availableTokensItemID.height*0.4
                        enabled: !availableTokensModel[index].IsApplied
                        text: "Apply"
                        onClicked:
                        {
                            headerID.isInfoVisible = true;
                            headerID.progressVisible = true;
                            headerID.progressText = "0%";
                            tokenListView.enabled = false;
                            headerID.backBtn.enabled = false;
                            tokenAddCommandRect.enabled = false;
                            applicationData.invokeApplyToken(availableTokensModel[index].TokenId)
                        }

                        background: Rectangle
                        {
                            color: "#961C1C"
                            border.width: 1
                            border.color: "#961C1C"
                            radius: 0.2  * btnApply.height
                        }

                        contentItem: Text
                        {
                           text: btnApply.text
                           font: btnApply.font
                           opacity: enabled ? 1.0 : 0.3
                           color: btnApply.down ? "gray" : "white"
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
        width: availableTokensPage.width * 0.9
        height: availableTokensPage.width * 0.05
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

        ToolButton
        {
            id: name
            action: navigateAddToken
            height: tokenAddCommandRect.height*0.75
            width: tokenAddCommandRect.height*0.75
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
            icon.height: tokenAddCommandRect.height*0.5
            icon.width: tokenAddCommandRect.height*0.5
            anchors.horizontalCenter:tokenAddCommandRect.horizontalCenter

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
            text: "Add a new Token"

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
                text: "Do you want to remove the token " + availableTokensModel[confirm.itemIndex].TokenId + " ?"
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
                    height: tokenListView.width*0.125
                    width: tokenListView.width*0.25
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
                    height: tokenListView.width*0.125
                    width: tokenListView.width*0.25
                    text: "Yes"
                    onClicked:
                    {
                        confirm.close()
                        applicationData.invokeRemoveToken(availableTokensModel[confirm.itemIndex].TokenId)
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

        function onTokenAction()
        {
            tokenListView.enabled = true;
            headerID.backBtn.enabled = true;
            tokenAddCommandRect.enabled = true;
            availableTokensModel = applicationData.TokenList
        }

        function onTokenTransferState(str, fl)
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
        }
    }
}

