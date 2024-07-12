import QtQuick 2.0

Rectangle
{
    id: root
    property bool checked: false
    property  int selectorSize: 100
    property int selectorWidth: selectorSize*5
    property string accentColor: "blue"
    property string fontColor: "black"
    property string backgroundColor: "white"
    property string text: "Test"

    signal clicked(bool checked);

    height: selectorSize
    width: selectorSize*5
    color: "transparent"

    Rectangle
    {
        id: selector_outer
        width: selectorSize*2.5
        height: selectorSize

        border.width: 2
        border.color: checked ? accentColor : "darkgrey"
        radius:       0.5  * height
        color:        checked ? accentColor : 'lightgrey'
        opacity:      enabled  &&  !mouseArea.pressed? 1: 0.3

        Rectangle
        {
            id: selector_inner
            x: checked ? 5 : parent.width - selector_inner.width - 5
            height: parent.height*0.8
            width: parent.height*0.8
            border.width: parent.border.width
            radius:       parent.radius
            border.color: checked ?  accentColor : "darkgrey"
            anchors.verticalCenter: parent.verticalCenter
            color: checked ? backgroundColor : "darkgrey"
        }

        MouseArea
        {
            id: mouseArea

            anchors.fill: parent
            onClicked:
            {
                checked = !checked
                root.clicked(checked)
            }
        }
    }

    Rectangle
    {
        height: selectorSize
        width: root.width - selector_outer.width
        anchors.left: selector_outer.right
        anchors.leftMargin: 10
        color: "transparent"

        Text
        {
            text:  root.text
            elide: Text.ElideRight
            color: checked ? fontColor : "darkgrey"
            font.pixelSize: 0.35 * root.height
            anchors.verticalCenter: parent.verticalCenter
            verticalAlignment: Text.AlignVCenter
            anchors.fill: parent
        }
    }
}
