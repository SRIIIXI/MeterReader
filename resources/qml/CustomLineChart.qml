import QtQuick 2.0

Item
{
    id: root

    property string title:  'title'
    property string yLabel: 'yLabel'
    property string xLabel: 'xLabel'

    property variant points: []
    property string  color1: '#961C1C'
    property string  color2: '#008080'

    property double factor: Math.min(width, height)

    property double yInterval:  1
    property double xIntervalDynamic:  1
    property double yMaximum:  10
    property double yMinimum:   0
    function toYPixels(y)
    {
        return -plot.height / (yMaximum - yMinimum) * (y - yMinimum) + plot.height
    }

    property double xInterval:  1
    property double xMaximum:  10
    property double xMinimum:   0
    function toXPixels(x)
    {
        return plot.width  / (xMaximum - xMinimum) * (x - xMinimum)
    }

    onPointsChanged:
    { // auto scale
        var xMinimum = 0, xMaximum = 0, yMinimum = 0, yMaximum = 0

        for(var i = 0; i < points.length; i++)
        {
            if(points[i].y1 > yMaximum)  yMaximum = points[i].y1
            if(points[i].y1 < yMinimum)  yMinimum = points[i].y1
            if(points[i].x1 > xMaximum)  xMaximum = points[i].x1
            if(points[i].x1 < xMinimum)  xMinimum = points[i].x1
        }

        for(var j = 0; j < points.length; j++)
        {
            if(points[j].y2 > yMaximum)  yMaximum = points[j].y2
            if(points[j].y2 < yMinimum)  yMinimum = points[j].y2
            if(points[j].x2 > xMaximum)  xMaximum = points[j].x2
            if(points[j].x2 < xMinimum)  xMinimum = points[j].x2
        }

        var yLog10     = Math.log(yMaximum - yMinimum) / Math.LN10
        root.yInterval = Math.pow(10, Math.floor(yLog10)) / 2
        root.yMaximum  = Math.ceil( yMaximum / yInterval) * yInterval
        root.yMinimum  = Math.floor(yMinimum / yInterval) * yInterval

        var xLog10     = Math.log(xMaximum - xMinimum) / Math.LN10
        root.xInterval =  Math.round(xMaximum / points.length);
        root.xMaximum  = Math.ceil( xMaximum / xInterval) * xInterval
        root.xMinimum  = Math.floor(xMinimum / xInterval) * xInterval

        canvas.requestPaint()
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
        width: parent.width - 35
        height: parent.height - 40
        anchors.top: parent.top
        anchors.right: parent.right

        Rectangle
        {
            anchors.fill: parent
            color: "transparent"

            Rectangle
            {
                width: parent.width
                height: 1
                border.color:
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
                anchors.bottom: parent.bottom
            }

            Rectangle
            {
                width: 1
                height: parent.height
                border.color:
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
                anchors.left: parent.left
            }
        }

        Repeater
        {
            model: Math.floor((yMaximum - yMinimum) / yInterval) + 1

            delegate: Rectangle
            {
                property double value: index * yInterval + yMinimum
                y: toYPixels(value)
                width: plot.width;
                height: 0
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

                Text
                {
                    text: parseFloat(parent.value.toPrecision(9)).toString()
                    anchors
                    {
                        right: parent.left;
                        verticalCenter: parent.verticalCenter;
                        margins: 0.01 * factor
                    }
                    font.pixelSize: fontsizer.fontSizeSmall
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
            }
        }

        Repeater
        {
            model: Math.floor((xMaximum - xMinimum) / xInterval) + 1

            delegate: Rectangle
            {
                property double value: index * xInterval + xMinimum
                x: toXPixels(value)
                width: 0
                height: plot.height;
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
                Text
                {
                    text: parseFloat(parent.value.toPrecision(9)).toString()
                    rotation: -90
                    anchors
                    {
                        top: parent.bottom;
                        horizontalCenter: parent.horizontalCenter;
                        margins: 0.01 * factor
                    }
                    font.pixelSize: fontsizer.fontSizeSmall
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
            }
        }

        Canvas
        {
            id: canvas
            anchors.fill: parent

            onPaint:
            {
                var context = getContext("2d")
                context.clearRect(0, 0, width, height)

                context.strokeStyle = color1
                context.lineWidth   = 0.005 * factor
                context.beginPath()
                for(var i = 0; i < points.length; i++)
                {
                    context.lineTo(toXPixels(points[i].x1), toYPixels(points[i].y1))
                }
                context.stroke()

                context.strokeStyle = color2
                context.lineWidth   = 0.005 * factor
                context.beginPath()
                for(var j = 0; j < points.length; j++)
                {
                    context.lineTo(toXPixels(points[j].x2), toYPixels(points[j].y2))
                }
                context.stroke()
            }
        }
    }
}
