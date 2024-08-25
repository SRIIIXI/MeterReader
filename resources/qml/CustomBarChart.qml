import QtQuick 2.0

Item
{
    id: root

    property string title:  'title'
    property string yLabel: 'yLabel'
    property string xLabel: 'xLabel'

    property variant points: []
    property string color1 : "#961C1C"
    property string color2 : "#008080"

    property double factor: Math.min(width, height)

    property double yInterval:  1
    property double yMaximum:  10
    property double yMinimum:   0
    function toYPixels(y) { return plot.height / (yMaximum - yMinimum) * (y - yMinimum) }
    property int    xMaximum:   0

    onPointsChanged:
    {
        if(!points)  return
        var xMaximum = 0, yMinimum = 0, yMaximum = 0

        for(var i = 0; i < points.length; i++)
        {
            if(points[i].y1 > yMaximum)          yMaximum = points[i].y1
            if(points[i].y1 < yMinimum)          yMinimum = points[i].y1
            if(points[i].x.length > xMaximum)   xMaximum = points[i].x.length
        }

        for(var j = 0; j < points.length; j++)
        {
            if(points[j].y2 > yMaximum)          yMaximum = points[j].y2
            if(points[j].y2 < yMinimum)          yMinimum = points[j].y2
            if(points[j].x.length > xMaximum)   xMaximum = points[j].x.length
        }

        var yLog10     = Math.log(yMaximum - yMinimum) / Math.LN10 // take log, convert to integer, and then raise 10 to this power
        root.yInterval = Math.pow(10, Math.floor(yLog10)) / (yLog10 % 1 < 0.7? 4: 2) // distance between ticks
        root.yMaximum  = Math.ceil( yMaximum / yInterval) * yInterval
        root.yMinimum  = Math.floor(yMinimum / yInterval) * yInterval

        root.xMaximum  = xMaximum
    }

    width: 500
    height: 500

    FontSizer
    {
        id: fontsizer
    }

    Text
    {
        text: title
        anchors.horizontalCenter: parent.horizontalCenter
        color: applicationData.Theme.FontColor
        anchors.bottom: parent.bottom
    }

    Text
    {
        text: yLabel
        font.pixelSize: fontsizer.fontSizeSmall
        y: 0.5 * (2 * plot.y + plot.height + width)
        rotation: -90
        transformOrigin: Item.TopLeft
        color: applicationData.Theme.FontColor
    }

    /*
    Text
    { // x label
        text: xLabel
        font.pixelSize: 0.03 * factor
        anchors
        {
            topMargin: 50
            bottom: parent.bottom;
            horizontalCenter: plot.horizontalCenter
        }
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
    */

    Item
    {
        id: plot
        Rectangle
        {
            anchors.fill: parent
            color:"transparent"

            Rectangle
            {
                height: 1
                width: parent.width
                anchors.bottom: parent.bottom
                border.width: 1
                border.color:
                {
                    if(points.length < 1)
                    {
                        return "transparent"
                    }
                    else
                    {
                        return applicationData.Theme.FontColor
                    }
                }
            }

            Rectangle
            {
                height: parent.height
                width: 1
                anchors.left: parent.left
                border.width: 1
                border.color:
                {
                    if(points.length < 1)
                    {
                        return "transparent"
                    }
                    else
                    {
                        return applicationData.Theme.FontColor
                    }
                }
            }
        }

        anchors
        {
            fill: parent;
            topMargin: 0
            bottomMargin: (0.05 * xMaximum + 0.05) * factor;
            leftMargin: 0.15 * factor;
            rightMargin: 0.05 * factor
        }

        Repeater
        {
            model: Math.floor((yMaximum - yMinimum) / yInterval) + 1

            delegate: Rectangle
            {
                property double value: index * yInterval + yMinimum
                y: -toYPixels(value) + plot.height
                width: plot.width*0.95;
                height: 1
                color: "transparent"

                Text
                {
                    text: parent.value
                    anchors
                    {
                        right: parent.left;
                        verticalCenter: parent.verticalCenter;
                        margins: 0.01 * factor
                    }
                    font.pixelSize: fontsizer.fontSizeSmall
                    color: applicationData.Theme.FontColor
                }
            }
        }

        Repeater
        {
            model: points

            delegate: Item
            {
                width: plot.width / points.length;
                height: plot.height
                x: width * index

                Rectangle
                {
                    id: bar1
                    anchors.leftMargin: 5
                    anchors
                    {
                        left: parent.left
                        bottom: modelData.y1 > 0? parent.bottom: undefined;
                        bottomMargin: toYPixels(0)
                        top:    modelData.y1 < 0? parent.top:    undefined;
                        topMargin: plot.height - toYPixels(0)
                    }
                    width:  0.35 * parent.width;
                    height: toYPixels(Math.abs(modelData.y1) + yMinimum)
                    color: color1
                }

                Rectangle
                {
                    id: bar2
                    anchors
                    {
                        left: bar1.right
                        bottom: modelData.y2 > 0? parent.bottom: undefined;
                        bottomMargin: toYPixels(0)
                        top:    modelData.y2 < 0? parent.top:    undefined;
                        topMargin: plot.height - toYPixels(0)
                    }
                    width:  0.35 * parent.width;
                    height: toYPixels(Math.abs(modelData.y2) + yMinimum)
                    color: color2
                }

                Text
                {
                    text: modelData.x
                    x:   (parent.width - height) / 2
                    y:    parent.height + width + 0.5 * height
                    rotation: -90
                    transformOrigin: Item.TopLeft
                    font.pixelSize: fontsizer.fontSizeSmall
                    color: applicationData.Theme.FontColor
                }
            }
        }
    }
}
