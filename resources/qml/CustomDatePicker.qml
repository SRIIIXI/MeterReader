import QtQuick 2.0
import QtQuick.Controls.Material 2.3

Rectangle
{
    id: root
    property int pickerHeight: 500
    property int pickerWidth: 500
    property date selectedDate: new Date()
    property string fontColor: "black"
    property string controlColor: "white"
    property string accentColor: "blue"
    property int selectedYear: selectedDate.getFullYear()

    height: pickerHeight
    width: pickerWidth

    function set(date)
    {
        selectedDate = new Date(date)
        dateView.positionViewAtIndex((selectedDate.getFullYear()) * 12 + selectedDate.getMonth(), ListView.Center)
    }
    signal clicked(date date);

    Rectangle
    {
        id: yearSelecter
        width: parent.width
        height: parent.height*0.125
        color: controlColor
        anchors.horizontalCenter: parent.horizontalCenter

        Grid
        {
            height: yearSelecter.height*0.5
            width: parent.width
            rows: 1
            columns: 3
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter

            Label
            {
                text: "<<"
                width: parent.width/3
                color: accentColor
                horizontalAlignment: Text.AlignHCenter
                font
                {
                    pixelSize: 0.5 * yearSelecter.height*0.9
                    bold: true
                }

                MouseArea
                {
                    anchors.fill: parent
                    onClicked:
                    {
                        selectedYear--;
                        selectedDate.setFullYear(selectedYear);
                        set(selectedDate);
                    }
                }
            }

            Label
            {
                text: selectedYear
                width: parent.width/3
                color: fontColor
                horizontalAlignment: Text.AlignHCenter
                font
                {
                    pixelSize: 0.5 * yearSelecter.height*0.9
                }
            }

            Label
            {
                text: ">>"
                width: parent.width/3
                color: accentColor
                horizontalAlignment: Text.AlignHCenter
                font
                {
                    pixelSize: 0.5 * yearSelecter.height*0.9
                    bold: true
                }

                MouseArea
                {
                    anchors.fill: parent
                    onClicked:
                    {
                        selectedYear++;;
                        selectedDate.setFullYear(selectedYear);
                        set(selectedDate);
                    }
                }
            }
        }
    }

    ListView
    {
        id: dateView
        property color bordercolor: fontColor
        property color fillcolor: fontColor

        width: parent.width
        height: parent.height - parent.height*0.125
        anchors.top: yearSelecter.bottom
        snapMode:    ListView.SnapOneItem
        orientation: Qt.Horizontal
        clip:        true
        model: 3000 * 12

        delegate: Item
        {
            property int year:      Math.floor(index / 12)
            property int month:     index % 12
            property int firstDay:  new Date(year, month, 1).getDay()

            width: root.width;
            height: root.height

            Column
            {
                Item
                {
                    width: root.width;
                    height: root.height - grid.height

                    Rectangle
                    {
                        color: controlColor
                        anchors.fill: parent

                        Text
                        {
                            anchors.centerIn: parent
                            text: ['January', 'February', 'March', 'April', 'May', 'June', 'July', 'August', 'September', 'October', 'November', 'December'][month] + ' ' + year
                            font
                            {
                                pixelSize: 0.5 * grid.cellHeight
                            }

                            color: fontColor

                            onTextChanged:
                            {
                                if(month === 1 && year > selectedYear)
                                {
                                    selectedYear = year
                                }

                                if(month === 12 &&  selectedYear > year)
                                {
                                    selectedYear = year
                                }
                            }
                        }
                    }
                }

                Grid
                {
                    id: grid
                    width: root.width;
                    height: 0.875 * root.height
                    property real cellWidth:  width  / columns
                    property real cellHeight: height / rows
                    columns: 7
                    rows:    7

                    Repeater
                    {
                        model: grid.columns * grid.rows

                        delegate: Rectangle
                        {
                            property int day:  index - 7
                            property int date: day - firstDay + 1

                            width: grid.cellWidth;
                            height: grid.cellHeight
                            border.width: 1
                            border.color: new Date(year, month, date).toDateString() == selectedDate.toDateString()  &&  text.text  &&  day >= 0
                                          ?
                                          accentColor
                                          : 'transparent'
                            opacity: !mouseArea.pressed? 1: 0.3
                            color: controlColor

                            Text
                            {
                                id: text

                                anchors.centerIn: parent
                                font.pixelSize: 0.5 * parent.height
                                font.bold:      new Date(year, month, date).toDateString() == new Date().toDateString()
                                text:
                                {
                                    if(day < 0)
                                    {
                                        ['S', 'M', 'T', 'W', 'T', 'F', 'S'][index]
                                    }
                                    else
                                    {
                                        if(new Date(year, month, date).getMonth() === month)  date
                                        else
                                            ''
                                    }
                                }

                                color: fontColor
                            }

                            MouseArea
                            {
                                id: mouseArea

                                anchors.fill: parent
                                enabled:    text.text  &&  day >= 0

                                onClicked:
                                {
                                    selectedDate = new Date(year, month, date)
                                    root.clicked(selectedDate)
                                    selectedYear = selectedDate.getFullYear()
                                }
                            }
                        }
                    }
                }
            }
        }

        Component.onCompleted: set(new Date())
    }
}
