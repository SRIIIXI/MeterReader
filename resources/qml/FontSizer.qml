import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Basic 2.3
import QtQuick.Layouts 1.15

Item
{
    id: root

    property double fontSizeNormal: 1
    property double fontSizeSmall: 1
    property double fontSizeTiny: 1
    property double fontSizeBig: 1
    property double fontSizeLarge: 1

    Component.onCompleted:
    {
        fontSizeNormal = fontlabel.font.pointSize
        fontSizeSmall = fontlabel.font.pointSize * 0.85
        fontSizeTiny = fontlabel.font.pointSize * 0.50
        fontSizeBig = fontlabel.font.pointSize * 1.25
        fontSizeLarge = fontlabel.font.pointSize * 1.75
    }

    Label
    {
        id: fontlabel
        text: ""
    }
}
