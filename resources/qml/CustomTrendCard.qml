import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.3
import QtQuick.Layouts 1.3

Item
{
    id: root
    property bool isDown: false
    property string imageSource: "../images/Bandwidth.png"
    property string trendName: "ABC"
    property string trendValue: "55.5"
    property string trendValueChange: "12.5%"
    property string fontColor: "white"

    FontSizer
    {
        id: fontsize
    }

    Rectangle
    {
        anchors.fill: parent
        color: "transparent"

        Rectangle
        {
            id: imgArea
            height: parent.height
            width: parent.width*0.25
            anchors.left: parent.left
            color: "transparent"

            Image
            {
                id: name
                width: parent.width*0.75
                height: parent.width*0.75
                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter
                source: imageSource
            }
        }

        Rectangle
        {
            id: infoArea
            anchors.left: imgArea.right
            height: parent.height
            width: parent.width -  imgArea.width - trendArea.width
            color: "transparent"

            Rectangle
            {
                width: parent.width
                height: parent.height/2
                anchors.top: parent.top
                color: "transparent"

                Text
                {
                    text: trendName
                    anchors.bottom: parent.bottom
                    anchors.horizontalCenter: parent.horizontalCenter
                    color: fontColor
                    font.pixelSize: fontsize.fontSizeLarge
                }
            }

            Rectangle
            {
                width: parent.width
                height: parent.height/2
                anchors.bottom: parent.bottom
                color: "transparent"

                Text
                {
                    text: trendValue
                    anchors.top: parent.top
                    anchors.horizontalCenter: parent.horizontalCenter
                    color: fontColor
                    font.pixelSize: fontsize.fontSizeNormal
                }
            }
        }

        Rectangle
        {
            id: trendArea
            height: parent.height
            width: parent.width*0.25
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            color: "transparent"

            Rectangle
            {
                width: parent.width
                height: parent.height/2
                anchors.top: parent.top
                color: "transparent"

                Text
                {
                    text: trendValueChange
                    anchors.bottom: parent.bottom
                    anchors.horizontalCenter: parent.horizontalCenter
                    color: fontColor
                    font.pixelSize: fontsize.fontSizeSmall
                }
            }

            Rectangle
            {
                width: parent.width
                height: parent.height/2
                anchors.bottom: parent.bottom
                color: "transparent"

                UpIndicator
                {
                    width: parent.width/2
                    height: parent.height/2
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.horizontalCenter: parent.horizontalCenter

                    visible:
                    {
                        if(isDown === true)
                        {
                            return false
                        }
                        else
                        {
                            return true
                        }
                    }
                }

                DownIndicator
                {
                    width: parent.width/2
                    height: parent.height/2
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.horizontalCenter: parent.horizontalCenter

                    visible:
                    {
                        if(isDown === true)
                        {
                            return true
                        }
                        else
                        {
                            return false
                        }
                    }
                }
            }
        }
    }
}
