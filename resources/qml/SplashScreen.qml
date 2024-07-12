import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.3
import QtQuick.Controls.Material 2.3

Page
{
    id: splashScreen

    Material.theme: Material.Light
    Material.accent: applicationData.Theme.AccentColor

    property double fontSizeNormal: fontlabel.font.pointSize
    property double fontSizeSmall: fontlabel.font.pointSize -  2.5
    property double fontSizeTiny: fontlabel.font.pointSize - 5.0
    property double fontSizeBig: fontlabel.font.pointSize +  2.5
    property double fontSizeLarge: fontlabel.font.pointSize + 5.0


    Component.onCompleted:
    {
        visible = true
        fontSizeNormal: fontlabel.font.pointSize
        fontSizeSmall: fontlabel.font.pointSize -  2.5
        fontSizeTiny: fontlabel.font.pointSize - 5.0
        fontSizeBig: fontlabel.font.pointSize +  2.5
        fontSizeLarge: fontlabel.font.pointSize + 5.0
    }

    Label
    {
        id: fontlabel
        text: ""
    }

    Rectangle
    {
       id: splashRect
       anchors.fill: parent
       color: "white"

       AnimatedImage
       {
           id: animation_splash
           width: splashScreen.width * 0.5
           height:  splashScreen.width * 0.5
           source: "../images/AnimationSplash.gif"
           anchors.horizontalCenter: parent.horizontalCenter
           anchors.verticalCenter: parent.verticalCenter
       }

       Text
       {
           id: appName
           text: "Consumer Smartphone App"
           anchors.horizontalCenter: parent.horizontalCenter
           anchors.top: animation_splash.bottom
           anchors.topMargin: 40
           font.bold: true
           color: applicationData.Theme.AccentColor
       }
    }

    Rectangle
    {
        id:companyName
        anchors.bottom: swVersion.top
        width: splashScreen.width*0.33
        height: (splashScreen.width*0.33)/5
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottomMargin: 10

        Image
        {
            id: companyimg
            anchors.horizontalCenter: parent.horizontalCenter
            source: "../images/MeterReader.png"
            height: companyName.height
            width: companyName.width
        }
    }

    Label
    {
        id: swVersion
        text: "Version " + applicationData.AppVersion
        color: "darkgrey"
        font.pointSize: fontSizeSmall
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottomMargin: 10
    }
}
