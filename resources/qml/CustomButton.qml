import QtQuick 2.0

Rectangle
{
    id: root
    property string text: 'text'
    property int buttonWidth: 75
    property int buttonHeight: 25
    property string accentColor: "blue"
    property bool isDefault: true
    property int textAlignment: Text.ElideNone
    signal clicked();

    width: buttonWidth
    height: buttonHeight
    border.color: accentColor
    border.width: 1
    radius:       0.125  * root.height
    opacity:      enabled  &&  !mouseArea.pressed? 1: 0.3

    color:
    {
        if(isDefault === true)
        {
            return accentColor
        }
        else
        {
            return "white"
        }
    }

    Text
    {
        text: root.text
        anchors.centerIn: parent
        elide: textAlignment
        color:
        {
            if(isDefault === true)
            {
                return "white"
            }
            else
            {
                return accentColor
            }
        }
    }

    MouseArea
    {
        id: mouseArea
        anchors.fill: parent
        onClicked:  root.clicked()
    }
}
