import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.3
import QtQuick.Layouts 1.3

Page
{
    id: contactPage

    Material.theme: Material.Light
    Material.accent: "#961C1C"

    Header
    {
        id:headerID
        headerTitle: "Contact Us"
        isInfoVisible: false
        optionsBtn.visible: false
        backBtn.action      : openMenuAction
        backBtn.icon.source :
        if(applicationData.IsDarkTheme === true)
        {
            return "../images/MenuWhite.png";
        }
        else
        {
            return "../images/MenuBlack.png";
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
        id:rectHomeAreaID
         width: contactPage.width*0.8
         height: contactPage.height
         anchors.top:headerID.bottom
         anchors.topMargin: parent.width*0.15
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
            id: custLabel1
            width: parent.width
            wrapMode: Label.Wrap
            text: "EDMI Utility team will support to ensure you with the best energy supply. For a precise help:"
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

        Label
        {
            id: meterQueryLabel
            anchors.top: custLabel1.bottom
            anchors.topMargin: parent.width*0.15
            width: parent.width
            font.bold: true
            color: "#961C1C"
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


        Label
        {
            id: billingQueryLabel
            anchors.top: email1.bottom
            anchors.topMargin: parent.width*0.15
            width: parent.width
            font.bold: true
            color: "#961C1C"
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

        Label
        {
            id: custLabel2
            width: parent.width
            wrapMode: Label.Wrap
            anchors.top: email2.bottom
            anchors.topMargin: parent.width*0.1
            text: "Thank you for trusting EDMI Utility"
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
    }

    Button
    {
        id: btnClose
        text: "Close"
        anchors.bottom: contactDummyRect.top
        anchors.bottomMargin: 10
        height: parent.width*0.15
        width: (parent.width*0.2)*2
        anchors.horizontalCenter: parent.horizontalCenter
        action: navigateBackAction

        background: Rectangle
        {
            color: "#961C1C"
            border.width: 1
            border.color: "#961C1C"
            radius: 0.2  * btnClose.height
        }

        contentItem: Text
        {
           text: "Close"
           font: btnClose.font
           opacity: enabled ? 1.0 : 0.3
           color: btnClose.down ? "gray" : "white"
           horizontalAlignment: Text.AlignHCenter
           verticalAlignment: Text.AlignVCenter
           elide: Text.ElideRight
        }
    }

    Rectangle
    {
        id: contactDummyRect
        width: contactPage.width
        height: contactPage.width*0.1
        radius: 0
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
        anchors
        {
            bottom:parent.bottom
            horizontalCenter: parent.horizontalCenter
        }
    }
}
