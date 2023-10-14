import QtQuick 2.0

Item
{
    id: root

 // public
    property string title:  'title'
    property string yLabel: 'yLabel'
    property string xLabel: 'xLabel'

    property variant points: []//{x: 0, y: 0}, {x: 1, y: 2}]
    property string  color1: '#961C1C'
    property string  color2: '#008080'

 // private
    property double factor: Math.min(width, height)

    property double yInterval:  1 // set by onPointsChanged
    property double xIntervalDynamic:  1 // set by onPointsChanged
    property double yMaximum:  10
    property double yMinimum:   0
    function toYPixels(y)
    {
        return -plot.height / (yMaximum - yMinimum) * (y - yMinimum) + plot.height
    }

    property double xInterval:  1 // set by onPointsChanged
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

        var yLog10     = Math.log(yMaximum - yMinimum) / Math.LN10 //  take log, convert to integer, and then raise 10 to this power
        root.yInterval = Math.pow(10, Math.floor(yLog10)) / 2 // distance between ticks
        root.yMaximum  = Math.ceil( yMaximum / yInterval) * yInterval
        root.yMinimum  = Math.floor(yMinimum / yInterval) * yInterval

        var xLog10     = Math.log(xMaximum - xMinimum) / Math.LN10 //  take log, convert to integer, and then raise 10 to this power
        //root.xInterval = Math.pow(10, Math.floor(xLog10)) // distance between ticks
        root.xInterval =  Math.round(xMaximum / points.length);
        root.xMaximum  = Math.ceil( xMaximum / xInterval) * xInterval
        root.xMinimum  = Math.floor(xMinimum / xInterval) * xInterval

        canvas.requestPaint()
    }

    width: 500;
    height: 500 // default size

    Text
    { // title
        text: title
        anchors.horizontalCenter: parent.horizontalCenter
        font.pixelSize: 0.03 * factor
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

    Text
    { // y label
        text: yLabel
        font.pixelSize: 0.03 * factor
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
    { // plot
        id: plot
        anchors
        {
            fill: parent;
            topMargin: 0.05 * factor;
            bottomMargin: 0.1 * factor;
            leftMargin: 0.15 * factor;
            rightMargin: 0.05 * factor
        }

        Repeater
        {
            // y axis tick marks and labels
            model: Math.floor((yMaximum - yMinimum) / yInterval) + 1 // number of tick marks

            delegate: Rectangle
            {
                property double value: index * yInterval + yMinimum
                y: toYPixels(value)
                width: plot.width;
                height: 1
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
                    font.pixelSize: 0.03 * factor
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
            // x axis tick marks and labels
            model: Math.floor((xMaximum - xMinimum) / xInterval) + 1 // number of tick marks

            delegate: Rectangle
            {
                property double value: index * xInterval + xMinimum
                x: toXPixels(value)
                width: 1;
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
                    font.pixelSize: 0.03 * factor
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
        { // points
            id: canvas
            anchors.fill: parent

            onPaint:
            {
                var context = getContext("2d")
                context.clearRect(0, 0, width, height) // new points data (animation)

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
