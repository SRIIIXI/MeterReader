import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.3
import QtQuick.Layouts 1.3

Page
{
    id: powerQualityPage

    Material.theme: Material.Light
    Material.accent: "#961C1C"

    property variant selectedComboItems: [true,true,false,false,false,false]
    property string pqselecteddate: Qt.formatDate(currentDate, 'dd MMM yyyy').toString();
    property date currentDate: new Date()

    property string ylabel: "Volts"
    property string xlabel: "Months"
    property string errorstring: ""

    Component.onCompleted:
    {
        dateText.text = Qt.formatDate(currentDate, 'dd MMM yyyy').toString();
        pqselecteddate = Qt.formatDate(currentDate, 'dd MMM yyyy').toString();
        parameterList.currentIndex = 0
        periodSelector.currentIndex = 3
        xAxisLabel.visible = false;
    }

    Rectangle
    {
        id: background
        width: parent.width
        height: parent.height - headerID.height
        anchors.top: headerID.bottom
        color:
        {
            if(applicationData.IsDarkTheme === true)
            {
                return "black";
            }
            else
            {
                return "white";
            }
        }
    }

    Header
    {
        id:headerID
        backBtn.icon.source :
        if(applicationData.IsDarkTheme === true)
        {
            return "../images/MenuWhite.png";
        }
        else
        {
            return "../images/MenuBlack.png";
        }

        backBtn.action      : openMenuAction
        headerTitle         : applicationData.CurrentMeter.SerialNo
        meterName           : "last sync: "+ applicationData.CurrentMeter.LastSyncStr
        isOptionsBtnVisible: true
        optionsBtn.icon.source:
        if(applicationData.IsDarkTheme === true)
        {
            return "../images/RefreshWhite.png";
        }
        else
        {
            return "../images/RefreshBlack.png";
        }
        isTitleVisible: applicationData.IsAppConnected

        onOptionsBtnClicked:
        {
            loadStatistics();
        }
    }

    ComboBox
    {
        id: parameterList
        height: headerID.height*0.5
        width: powerQualityPage.width*0.9
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top:headerID.bottom

        displayText: "Select"
        font.pointSize: headerID.fontSizeSmall

        model: ListModel
        {
            ListElement { name: "Average Voltage"; selected: true}
            ListElement { name: "Average Current"; selected: true}
            ListElement { name: "Average Power Factor Total"; selected: false}
            ListElement { name: "Average MCU Temperature"; selected: false}
            ListElement { name: "Instantenous Net Frequency"; selected: false}
            ListElement { name: "Average THD Voltage"; selected: false}
        }

        // ComboBox closes the popup when its items (anything AbstractButton derivative) are
        //  activated. Wrapping the delegate into a plain Item prevents that.
        delegate: Item
        {
            width: parent.width
            height: checkDelegate.height

            function toggle()
            {
                checkDelegate.toggle()
                console.log(checkDelegate.data)
            }

            Material.accent: "#961C1C"
            Material.theme:
            {
                if(applicationData.IsDarkTheme === true)
                {
                    return Material.Dark;
                }
                else
                {
                    return Material.Light;
                }
            }

            CheckDelegate
            {
                id: checkDelegate
                anchors.fill: parent
                text: model.name
                highlighted: parameterList.highlightedIndex === index
                checked: model.selected
                font.pointSize: headerID.fontSizeSmall

                onCheckedChanged:
                {
                    model.selected = checked
                    selectedComboItems[index] = checked
                }

                Material.accent: "#961C1C"
                Material.theme:
                {
                    if(applicationData.IsDarkTheme === true)
                    {
                        return Material.Dark;
                    }
                    else
                    {
                        return Material.Light;
                    }
                }
            }
        }



        // override space key handling to toggle items when the popup is visible
        Keys.onSpacePressed:
        {
            if (parameterList.popup.visible)
            {
                var currentItem = parameterList.popup.contentItem.currentItem
                if (currentItem)
                {
                    currentItem.toggle()
                    event.accepted = true
                }
            }
        }

        Keys.onReleased:
        {
            if (comboBox.popup.visible)
                event.accepted = (event.key === Qt.Key_Space)
        }

        Material.theme:
        {
            if(applicationData.IsDarkTheme === true)
            {
                return Material.Dark;
            }
            else
            {
                return Material.Light;
            }
        }
    }

    TabBar
    {
        id: periodSelector
        width: powerQualityPage.width*0.9
        height: headerID.height/3
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top:parameterList.bottom
        Material.accent: "#961C1C"

        Material.theme:
        {
            if(applicationData.IsDarkTheme === true)
            {
                return Material.Dark;
            }
            else
            {
                return Material.Light;
            }
        }

        TabButton
        {
            text: qsTr("Daily")
            onClicked:
            {
                xlabel = "Hours"
            }
        }

        TabButton
        {
            text: qsTr("Weekly")
            onClicked:
            {
                xlabel = "Day of Week"
            }
        }

        TabButton
        {
            text: qsTr("Monthly")
            onClicked:
            {
                xlabel = "Day"
            }
        }

        TabButton
        {
            text: qsTr("Annual")
            onClicked:
            {
                xlabel = "Month"
            }
        }
    }

    Rectangle
    {
        id: dateSelectRangeHeader
        width: powerQualityPage.width*0.9
        height: headerID.height*0.3
        anchors.top:periodSelector.bottom
        anchors.horizontalCenter: parent.horizontalCenter

        color:
        {
            if(applicationData.IsDarkTheme === true)
            {
                return "black";
            }
            else
            {
                return "white";
            }
        }

        Rectangle
        {
            id: dtrecttop1
            width: dateSelectRangeHeader.width/2
            height: dateSelectRangeHeader.height
            anchors.top: dateSelectRangeHeader.top
            anchors.left: dateSelectRangeHeader.left

            color:
            {
                if(applicationData.IsDarkTheme === true)
                {
                    return "black";
                }
                else
                {
                    return "white";
                }
            }

            Label
            {
                text: "Select Date"
                font.pointSize: headerID.fontSizeSmall
                color: "darkcyan"
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
            }
        }
    }

    Rectangle
    {
        id: dateSelectRangeContent
        width: powerQualityPage.width*0.9
        height: headerID.height*0.35
        anchors.top:dateSelectRangeHeader.bottom
        anchors.horizontalCenter: parent.horizontalCenter

        color:
        {
            if(applicationData.IsDarkTheme === true)
            {
                return "black";
            }
            else
            {
                return "white";
            }
        }

        Rectangle
        {
            id: dtrectbottom1
            width: dateSelectRangeContent.width/2
            height: dateSelectRangeContent.height
            anchors.top: dateSelectRangeContent.top
            anchors.left: dateSelectRangeContent.left

            color:
            {
                if(applicationData.IsDarkTheme === true)
                {
                    return "black";
                }
                else
                {
                    return "white";
                }
            }

            Button
            {
                id: calendarFrom
                height: dateSelectRangeContent.height
                width: dateSelectRangeContent.height
                Image
                {
                    height: dateSelectRangeContent.height
                    width: dateSelectRangeContent.height
                    source:
                    if(applicationData.IsDarkTheme === true)
                    {
                        return "../images/CalendarWhite.png";
                    }
                    else
                    {
                        return "../images/CalendarBlack.png";
                    }
                }

                background: Rectangle
                {
                    color:
                    {
                        if(applicationData.IsDarkTheme === true)
                        {
                            return "black";
                        }
                        else
                        {
                            return "white";
                        }
                    }
                }

                onClicked:
                {
                    datepickerpopup.open()
                }
            }

            Label
            {
                id: dateText
                text: ""
                font.pointSize: headerID.fontSizeSmall
                anchors.left: calendarFrom.right
                anchors.leftMargin: calendarFrom.width*0.5
                anchors.bottom: calendarFrom.bottom
                Material.accent: "#961C1C"
                Material.theme:
                {
                    if(applicationData.IsDarkTheme === true)
                    {
                        return Material.Dark;
                    }
                    else
                    {
                        return Material.Light;
                    }
                }
            }
        }
    }

    Rectangle
    {
        id: graphRect
        height: powerQualityPage.width*0.85
        width: powerQualityPage.width
        anchors.top:dateSelectRangeContent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: 5

        color:
        {
            if(applicationData.IsDarkTheme === true)
            {
                return "black";
            }
            else
            {
                return "white";
            }
        }

        CustomBarChart
        {
            id: barChart
            width: parent.width
            height: parent.height
            antialiasing: true
            visible: true
            color1: '#961C1C'
            color2: '#008080'

            title: applicationData.ChartTitlePowerQuality
            yLabel: applicationData.YLabelBarPowerQuality
            xLabel: applicationData.XLabelBarPowerQuality
            points: applicationData.PowerQualityBarData
        }

        CustomLineChart
        {
            id: splineChart
            width: parent.width
            height: parent.height
            antialiasing: true
            visible: false
            color1: '#961C1C'
            color2: '#008080'

            title: applicationData.ChartTitlePowerQuality
            yLabel: applicationData.YLabelLinePowerQuality
            xLabel: applicationData.XLabelLinePowerQuality
            points: applicationData.PowerQualityLineData
        }
    }

    Text
    {
        id: xAxisLabel
        anchors.top: graphRect.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        font.pointSize: headerID.fontSizeSmall
        text:
        {
            if(graphSelecter.checked === true)
            {
                return applicationData.XLabelBarPowerQuality
            }
            else
            {
                return applicationData.XLabelLinePowerQuality
            }
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

    Rectangle
    {
        id: modeRect
        width: powerQualityPage.width*0.9
        height: headerID.height/2.5
        anchors.top:xAxisLabel.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: 5
        color:
        {
            if(applicationData.IsDarkTheme === true)
            {
                return "black";
            }
            else
            {
                return "white";
            }
        }

        Switch
        {
            id: graphSelecter
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            Material.accent: "#961C1C"
            checked: false
            text: "Bar"

            Material.theme:
            {
                if(applicationData.IsDarkTheme === true)
                {
                    return Material.Dark;
                }
                else
                {
                    return Material.Light;
                }
            }

            onClicked:
            {
                var infoType = !checked;
                console.log(infoType);

                if(infoType === true)
                {
                    text = "Bar"
                }
                else
                {
                    text = "Line"
                }
            }
        }
    }

    Rectangle
    {
        id: rectStat1
        anchors.horizontalCenter: parent.horizontalCenter
        width: powerQualityPage.width*0.9
        height: (powerQualityPage.width*0.9)*0.1
        anchors.top: modeRect.bottom
        anchors.topMargin: 5

        color:
        {
            if(applicationData.IsDarkTheme === true)
            {
                return "black";
            }
            else
            {
                return "white";
            }
        }


        Column
        {
            id: colLeft1
            anchors.left: parent.left
            height: rectStat1.height

            Label
            {
                id: maxTitle1
                text: "Max"
                font.pointSize: headerID.fontSizeSmall
                font.bold: true

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

            Label
            {
                id: maxValue1
                text: applicationData.PQMax1.toFixed(2) + " " + applicationData.PQ1Unit
                font.bold: true
                color: "darkcyan"
                font.pointSize: headerID.fontSizeSmall
            }

            Label
            {
                id: maxTS1
                text: ""
                font.bold: false
                font.pointSize: headerID.fontSizeTiny

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

        Column
        {
            id: colRight1
            anchors.right: parent.right
            height: rectStat1.height

            Label
            {
                id: minTitle1
                text: "Min"
                font.bold: true
                font.pointSize: headerID.fontSizeSmall

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

            Label
            {
                id: minValue1
                text: applicationData.PQMin1.toFixed(2) + " " + applicationData.PQ1Unit
                font.bold: true
                color: "darkcyan"
                font.pointSize: headerID.fontSizeSmall
            }

            Label
            {
                id: minTS1
                text: ""
                font.bold: false
                font.pointSize: headerID.fontSizeTiny

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

    Rectangle
    {
        id: rectStat2
        anchors.horizontalCenter: parent.horizontalCenter
        width: powerQualityPage.width*0.9
        height: (powerQualityPage.width*0.9)*0.1
        anchors.top: rectStat1.bottom

        color:
        {
            if(applicationData.IsDarkTheme === true)
            {
                return "black";
            }
            else
            {
                return "white";
            }
        }

        Column
        {
            id: colLeft2
            anchors.left: parent.left
            height: rectStat1.height

            Label
            {
                id: maxTitle2
                text: "Max"
                font.pointSize: headerID.fontSizeSmall
                font.bold: true

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

            Label
            {
                id: maxValue2
                text: applicationData.PQMax2.toFixed(2) + " " + applicationData.PQ2Unit
                font.bold: true
                color: "darkcyan"
                font.pointSize: headerID.fontSizeSmall
            }

            Label
            {
                id: maxTS2
                text: ""
                font.bold: false
                font.pointSize: headerID.fontSizeTiny

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

        Column
        {
            id: colRight2
            anchors.right: parent.right
            height: rectStat2.height

            Label
            {
                id: minTitle2
                text: "Min"
                font.bold: true
                font.pointSize: headerID.fontSizeSmall

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

            Label
            {
                id: minValue2
                text: applicationData.PQMin2.toFixed(2) + " " + applicationData.PQ2Unit
                font.bold: true
                color: "darkcyan"
                font.pointSize: headerID.fontSizeSmall
            }

            Label
            {
                id: minTS2
                text: ""
                font.bold: false
                font.pointSize: headerID.fontSizeTiny

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

    Rectangle
    {
        id: rectTimestamp
        anchors.horizontalCenter: parent.horizontalCenter
        width: powerQualityPage.width*0.9
        height: (powerQualityPage.width*0.9)*0.1
        anchors.top: rectStat2.bottom

        color:
        {
            if(applicationData.IsDarkTheme === true)
            {
                return "black";
            }
            else
            {
                return "white";
            }
        }

        Column
        {
            id: colLeftts
            anchors.left: parent.left
            height: rectTimestamp.height

            Label
            {
                id: tsstart
                text: applicationData.MinTimestampPQ
                font.bold: false
                font.pointSize: headerID.fontSizeSmall
                color: "darkgray"
            }
        }

        Column
        {
            id: colRightts
            anchors.right: parent.right
            height: rectTimestamp.height

            Label
            {
                id: tsend
                text: applicationData.MaxTimestampPQ
                font.bold: false
                font.pointSize: headerID.fontSizeSmall
                color: "darkgray"
            }
        }
    }

    function loadStatistics()
    {
        applicationData.invokeLoadPowerQualityStatistics(selectedComboItems, periodSelector.currentIndex,  pqselecteddate);
        xaxislable.text = xlabel;
    }

    Popup
    {
        id: datepickerpopup
        visible: false
        width: powerQualityPage.width*0.9
        height: powerQualityPage.width*0.9
        x: (powerQualityPage.width - datepickerpopup.width)/2
        y: parent.height/3
        padding: 0
        modal: true
        property int itemIndex: 0
        property string itemValue: ""

        contentItem: Rectangle
        {
            anchors.fill: parent
            color:
            {
                if(applicationData.IsDarkTheme === true)
                {
                    return "#1C2833";
                }
                else
                {
                    return "whitesmoke";
                }
            }

            DatePicker
            {
                id: datepickctl
                height: datepickerpopup.width*0.8
                width: datepickerpopup.width
                Component.onCompleted: set(new Date()) // today
                anchors.top: parent.top
                anchors.horizontalCenter: parent.horizontalCenter
                onClicked:
                {
                    console.log('onClicked', Qt.formatDate(selectedDate, 'M/d/yyyy'))
                }
            }

            Grid
            {
                id: gridDlg
                rows: 1
                columns: 2
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 10
                anchors.rightMargin: 10
                anchors.right: parent.right
                spacing: 10
                Button
                {
                    id:btnCancel
                    width: powerQualityPage.width*0.25
                    height: powerQualityPage.width*0.125
                    text: "Cancel"
                    onClicked:
                    {
                        datepickerpopup.close();
                        pqselecteddate = Qt.formatDate(currentDate, 'dd/MM/yyyy').toString();
                        dateText.text = Qt.formatDate(currentDate, 'dd MMM yyyy').toString();
                    }

                    background: Rectangle
                    {
                        color: "white"
                        border.width: 1
                        border.color: "#961C1C"
                        radius: 0.1  * btnCancel.height
                    }

                    contentItem: Text
                    {
                       text: "Cancel"
                       font: btnCancel.font
                       opacity: enabled ? 1.0 : 0.3
                       color: btnCancel.down ? "gray" : "black"
                       horizontalAlignment: Text.AlignHCenter
                       verticalAlignment: Text.AlignVCenter
                       elide: Text.ElideRight
                    }
                }

                Button
                {
                    id:btnSave
                    width: powerQualityPage.width*0.25
                    height: powerQualityPage.width*0.125
                    text: "Save"
                    onClicked:
                    {
                        pqselecteddate = Qt.formatDate(datepickctl.selectedDate, 'dd/MM/yyyy').toString();
                        dateText.text = Qt.formatDate(datepickctl.selectedDate, 'dd MMM yyyy').toString();
                        datepickerpopup.close()
                    }

                    background: Rectangle
                    {
                        color: "#961C1C"
                        border.width: 1
                        border.color: "#961C1C"
                        radius: 0.2  * btnSave.height
                    }


                    contentItem: Text
                    {
                       text: "Save"
                       font: btnSave.font
                       opacity: enabled ? 1.0 : 0.3
                       color: btnSave.down ? "gray" : "white"
                       horizontalAlignment: Text.AlignHCenter
                       verticalAlignment: Text.AlignVCenter
                       elide: Text.ElideRight
                    }
                }
            }
        }
    }

    Connections
    {
        target: applicationData

        function onPowerQualityStatisticsRefreshed()
        {
            xaxislable.visible = true;
            headerID.refreshBtn.enabled = true;
            headerID.backBtn.enabled = true;
            headerID.optionsBtn.enabled = true;
            headerID.isProgressVisible = false;
            headerID.optionsBtn.enabled = true;
        }
    }
}
