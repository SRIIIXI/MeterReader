import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.3
import QtQuick.Layouts 1.3

Page
{
    id: contactPage

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

    Header
    {
        id:headerPanel
        headerTitle: "Contact Us"
        isMenuButtonVisible: true
        isMeterNameVisible: false
        isSyncDateVisible: false
        isConnectionIndicatorVisible: false
    }

    Rectangle
    {
        id:rectHomeAreaID
         width: contactPage.width*0.8
         height: contactPage.height
         anchors.top:headerPanel.bottom
         anchors.topMargin: parent.width*0.15
         anchors.horizontalCenter: parent.horizontalCenter
         color: applicationData.Theme.BackgroundColor

        Label
        {
            id: custLabel1
            width: parent.width
            wrapMode: Label.Wrap
            text: "EDMI Utility team will support to ensure you with the best energy supply. For a precise help:"
            color: applicationData.Theme.FontColor
        }

        Label
        {
            id: meterQueryLabel
            anchors.top: custLabel1.bottom
            anchors.topMargin: parent.width*0.15
            width: parent.width
            font.bold: true
            color: applicationData.Theme.AccentColor
            text: "Meter related query"
        }

        Image
        {
            id: phone1
            anchors.top: meterQueryLabel.bottom
            source: "../images/Phone.png"
            height: (parent.width*0.15)/2
            width: (parent.width*0.15)/2
        }

        Label
        {
            id: phoneLbl1
            text: "+971 55 222 333 11"
            anchors.left: phone1.right
            anchors.top: meterQueryLabel.bottom
            anchors.leftMargin: (parent.width*0.15)/2
            color: applicationData.Theme.FontColor
        }

        Image
        {
            id: email1
            anchors.top: phone1.bottom
            source: "../images/EMail.png"
            height: (parent.width*0.15)/2
            width: (parent.width*0.15)/2
        }

        Label
        {
            text: "meteringservice@edmi-meters.com"
            anchors.left: email1.right
            anchors.top: phone1.bottom
            anchors.leftMargin: (parent.width*0.15)/2
            color: applicationData.Theme.FontColor
        }


        Label
        {
            id: billingQueryLabel
            anchors.top: email1.bottom
            anchors.topMargin: parent.width*0.15
            width: parent.width
            font.bold: true
            color: applicationData.Theme.AccentColor
            text: "Billing related query"
        }

        Image
        {
            id: phone2
            anchors.top: billingQueryLabel.bottom
            source: "../images/Phone.png"
            height: (parent.width*0.15)/2
            width: (parent.width*0.15)/2
        }

        Label
        {
            text: "+971 55 222 333 22"
            anchors.left: phone2.right
            anchors.top: billingQueryLabel.bottom
            anchors.leftMargin: (parent.width*0.15)/2
            color: applicationData.Theme.FontColor
        }

        Image
        {
            id: email2
            source: "../images/EMail.png"
            anchors.top: phone2.bottom
            height: (parent.width*0.15)/2
            width: (parent.width*0.15)/2
        }

        Label
        {
            text: "billingservice@edmi-meters.com"
            anchors.left: email2.right
            anchors.top: phone2.bottom
            anchors.leftMargin: (parent.width*0.15)/2
            color: applicationData.Theme.FontColor
        }

        Label
        {
            id: custLabel2
            width: parent.width
            wrapMode: Label.Wrap
            anchors.top: email2.bottom
            anchors.topMargin: parent.width*0.1
            text: "Thank you for trusting EDMI Utility"
            color: applicationData.Theme.FontColor
        }
    }

    Rectangle
    {
        id: contactDummyRect
        width: contactPage.width
        height: contactPage.width*0.1
        radius: 0
        color: applicationData.Theme.BackgroundColor

        anchors
        {
            bottom:parent.bottom
            horizontalCenter: parent.horizontalCenter
        }
    }
}
