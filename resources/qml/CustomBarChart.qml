import QtQuick 2.0

Item
{
    id: root

 // public
    property string title:  'title'
    property string yLabel: 'yLabel'
    property string xLabel: 'xLabel'

    property variant points: []//{x: 'Zero', y: 60, color: 'red'}, {x: 'One', y: 40, color: 'blue' }]
    property string color1 : "#961C1C"
    property string color2 : "#008080"

 // private
    property double factor: Math.min(width, height)

    property double yInterval:  1
    property double yMaximum:  10 // set by onPointsChanged
    property double yMinimum:   0
    function toYPixels(y){return plot.height / (yMaximum - yMinimum) * (y - yMinimum)}

    property int    xMaximum:   0 // string length

    onPointsChanged:
    { // auto scale vertically
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
            bottom: parent.bottom;
            topMargin: 50
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

        Rectangle
        {
            anchors.fill: parent
            color:"transparent"
            border.width: 1
            border.color:
            {
                if(points.length < 1)
                {
                    return "transparent"
                }

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

        anchors
        {
            fill: parent;
            topMargin: 0.05 * factor;
            bottomMargin: (0.015 * xMaximum + 0.05) * factor;
            leftMargin: 0.15 * factor;
            rightMargin: 0.05 * factor
        }

        Repeater
        { // y axis tick marks and labels
            model: Math.floor((yMaximum - yMinimum) / yInterval) + 1 // number of tick marks

            delegate: Rectangle
            {
                property double value: index * yInterval + yMinimum
                y: -toYPixels(value) + plot.height
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
                    text: parent.value
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
        { // data
            model: points

            delegate: Item
            { // column
                width: plot.width / points.length;
                height: plot.height
                x: width * index

                Rectangle
                { // bar
                    id: bar1
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
                { // bar
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
                    // x values (rotated -90 degrees)
                    text: modelData.x
                    x:   (parent.width - height) / 2
                    y:    parent.height + width + 0.5 * height
                    rotation: -90
                    transformOrigin: Item.TopLeft
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
    }
}
