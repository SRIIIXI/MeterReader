import QtQuick 2.0

ListView
{
    id: root
    property int tabWidth: 500
    property int tabHeight: 100
    property string accentColor: "blue"
    property string accentLowColor: "blue"
    property string controlColor: "grey"
    property string controlLowColor: "grey"
    property string fontColor: "white"

    model:          []
    currentIndex:   0

    width: tabWidth;
    height: tabHeight

    orientation: ListView.Horizontal
    interactive: false
    spacing: 0.1 * height
    clip: true

    delegate: Item
    {
        width: 3 * height;
        height: 0.7 * root.height
        y: root.height - height
        opacity: mouseArea.pressed? 0.3: 1

        Item
        {
            anchors.fill: parent
            clip: true

            Rectangle
            {
                width: parent.width;
                height: 2 * parent.height
                border.width: 0.02 * root.height
                border.color: accentColor
                radius: 0.2 * root.height
                color: currentIndex === index? controlColor: "transparent" //'black'
            }
        }

        Text
        {
            text: modelData
            font.pixelSize: 0.3 * root.height
            anchors.centerIn: parent
            color: fontColor
        }

        Rectangle
        {
            visible: currentIndex === index;
            anchors
            {
                bottom: parent.bottom;
                right: parent.left
            }
            width: root.width;
            height: 0.02 * root.height
            color: accentColor
        }

        Rectangle
        {
            visible: currentIndex === index;
            anchors
            {
                bottom: parent.bottom;
                left:  parent.right
            }
            width: root.width;
            height: 0.02 * root.height
            color: accentColor
        }

        MouseArea
        {
            id: mouseArea
            anchors.fill: parent
            enabled: currentIndex !== index
            onClicked:  currentIndex = index
        }
    }
}
