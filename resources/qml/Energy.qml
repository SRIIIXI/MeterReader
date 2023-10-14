import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.3
import QtQuick.Layouts 1.3

Page
{
    id: energyPage

    Material.theme: Material.Light
    Material.accent: "#961C1C"

    property date currentDate: new Date()

    property string eyselecteddatefrom: Qt.formatDate(datepick.selectedDate, 'dd MMM yyyy').toString()
    property string eyselecteddateto: Qt.formatDate(datepick.selectedDate, 'dd MMM yyyy').toString()
    property int calendarmode: 0 // 0 is for FROM and 1 is for TO
    property int visualizationmode: 0 // 0 is for ENERGY and 1 is for USD/CURRENCY

    property string ylabel: "kWh"
    property string xlabel: "Months"
    property string errorstring: ""

    Component.onCompleted:
    {
        dateTextFrom.text = Qt.formatDate(currentDate, 'dd MMM yyyy').toString();
        dateTextTo.text = Qt.formatDate(currentDate, 'dd MMM yyyy').toString();
        eyselecteddatefrom = Qt.formatDate(currentDate, 'dd/MM/yyyy').toString();
        eyselecteddateto = Qt.formatDate(currentDate, 'dd/MM/yyyy').toString();
        parameterList.currentIndex = 0
        periodSelector.currentIndex = 0
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
        isTitleVisible: true

        onOptionsBtnClicked:
        {
            headerID.backBtn.enabled = false;
            headerID.optionsBtn.enabled = false;
            console.log(periodSelector.currentIndex,  eyselecteddatefrom, eyselecteddateto);
            applicationData.invokeFetchEnergyStatistics(visualizationmode, parameterList.currentIndex, periodSelector.currentIndex,  eyselecteddatefrom, eyselecteddateto);
        }
    }

    ComboBox
    {
        id: parameterList
        height: headerID.height*0.5
        width: energyPage.width*0.9
        font.pointSize: headerID.fontSizeSmall
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top:headerID.bottom
        Material.accent: "#961C1C"

        popup: Popup
        {
            id:comboPopup
            y: parameterList.height - 1
            width: parameterList.width
            height:contentItem.implicitHeight
            padding: 1

            contentItem: ListView
            {
                id:listView
                implicitHeight: contentHeight
                model: parameterList.popup.visible ? parameterList.delegateModel : null
                ScrollIndicator.vertical: ScrollIndicator { }
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

                delegate: Text
                {
                    font.pointSize: headerID.fontSizeSmall
                }
            }

            background: Rectangle
            {
                border.width: 1
                border.color:"#961c1c"

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
            }
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

        model: ["Import Energy Tariff 1", "Export Energy Tariff 1", "Import Reactive Energy Tariff 1", "Export Reactive Energy Tariff 1", "Import Energy Tariff 2", "Export Energy Tariff 2", "Import Reactive Energy Tariff 2", "Export Reactive Energy Tariff 2"]
    }

    TabBar
    {
        id: periodSelector
        height: headerID.height*0.4
        width: energyPage.width*0.9
        font.pointSize: headerID.fontSizeSmall
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
                splineChart.xIntervalDynamic = 12
            }
        }

        TabButton
        {
            text: qsTr("Weekly")
            onClicked:
            {
                splineChart.xIntervalDynamic = 7
            }
        }

        TabButton
        {
            text: qsTr("Monthly")
            onClicked:
            {
                splineChart.xIntervalDynamic = 12
            }
        }

        TabButton
        {
            text: qsTr("Annual")
            onClicked:
            {
                splineChart.xIntervalDynamic = 12
            }
        }
    }

    Rectangle
    {
        id: dateSelectRangeHeader
        width: energyPage.width*0.9
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
                color: "darkcyan"
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
            }
        }

        Rectangle
        {
            id: dtrecttop2
            width: dateSelectRangeHeader.width/2
            height: dateSelectRangeHeader.height
            anchors.top: dateSelectRangeHeader.top
            anchors.right: dateSelectRangeHeader.right

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
                text: "Compare With"
                color: "darkred"
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
            }
        }
    }

    Rectangle
    {
        id: dateSelectRangeContent
        width: energyPage.width*0.9
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
                    calendarmode = 0
                    datepickerpopup.open()
                }
            }

            Label
            {
                id: dateTextFrom
                text: ""
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

        Rectangle
        {
            id: dtrectbottom2
            width: dateSelectRangeContent.width/2
            height: dateSelectRangeContent.height
            anchors.top: dateSelectRangeContent.top
            anchors.right: dateSelectRangeContent.right

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
                id: calendarTo
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
                    calendarmode = 1
                    datepickerpopup.open()
                }
            }

            Label
            {
                id: dateTextTo
                text: ""
                anchors.left: calendarTo.right
                anchors.leftMargin: calendarTo.width*0.5
                anchors.bottom: calendarTo.bottom
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
        height: energyPage.width*0.9
        width: energyPage.width*0.9
        anchors.top: dateSelectRangeContent.bottom
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

            title: applicationData.ChartTitleEnergy
            yLabel: applicationData.YLabelBarEnergy
            xLabel: applicationData.XLabelBarEnergy
            points: applicationData.EnergyBarData
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

            title: applicationData.ChartTitleEnergy
            yLabel: applicationData.YLabelLineEnergy
            xLabel: applicationData.XLabelLineEnergy
            points: applicationData.EnergyLineData
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
                return applicationData.XLabelBarEnergy
            }
            else
            {
                return applicationData.XLabelLineEnergy
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
        width: energyPage.width*0.9
        height: headerID.height/2.5
        anchors.top: xAxisLabel.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: 10

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

        Grid
        {
            rows: 1
            columns: 3

            Rectangle
            {
                height: modeRect.height
                width: modeRect.height*3

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
                    Material.accent: "#961C1C"
                    checked: false
                    text: "kWh"

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
                            visualizationmode = 0;
                            text = "kWh"
                        }
                        else
                        {
                            visualizationmode = 1;
                            text = "USD"
                        }
                    }
                }
            }

            Rectangle
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

                height: modeRect.height
                width: modeRect.width - modeRect.height*3 - modeRect.height*2 - (energyPage.width*0.1)/2
            }

            Rectangle
            {
                height: modeRect.height
                width: modeRect.height*3
                anchors.rightMargin: (energyPage.width*0.1)/2

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
                            barChart.visible = true
                            splineChart.visible = false
                            text = "Bar"
                        }
                        else
                        {
                            barChart.visible = false
                            splineChart.visible = true
                            text = "Line"
                        }
                    }
                }
            }
        }
    }

    Rectangle
    {
        id: rectTou
        anchors.horizontalCenter: parent.horizontalCenter
        width: energyPage.width*0.9
        height: (energyPage.width*0.9)*0.125
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
            id: colLeft
            anchors.left: parent.left
            height: rectTou.height

            Label
            {
                id: dailyMaxTouTitle
                text: "Daily Max TOU"
                font.bold: true
                font.pointSize: headerID.fontSizeSmall
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

            Label
            {
                id: dailyMaxTouValue
                text: applicationData.DailyMaxTou.toFixed(2) + " kWh"
                color: "darkcyan"
                font.pointSize: headerID.fontSizeSmall
            }
        }

        Column
        {
            id: colMid
            anchors.horizontalCenter: parent.horizontalCenter
            height: rectTou.height

            Label
            {
                id: totalTouTitle
                text: "Total TOU"
                font.bold: true
                font.pointSize: headerID.fontSizeSmall
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

            Label
            {
                id: totalTouValue
                text: applicationData.TotalTou.toFixed(2) + " kWh"
                color: "darkcyan"
                font.pointSize: headerID.fontSizeSmall
            }
        }

        Column
        {
            id: colRight
            anchors.right: parent.right
            height: rectTou.height

            Label
            {
                id: hourlyAveTouTitle
                text: "Hourly Average TOU"
                font.bold: true
                font.pointSize: headerID.fontSizeSmall
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

            Label
            {
                id: hourlyAveTouValue
                text: applicationData.HourlyAverageTou.toFixed(2) + " kWh"
                color: "darkcyan"
                font.pointSize: headerID.fontSizeSmall
            }
        }
    }

    Rectangle
    {
        id: rectTimestamp
        anchors.horizontalCenter: parent.horizontalCenter
        width: energyPage.width*0.9
        height: (energyPage.width*0.9)*0.15
        anchors.top: rectTou.bottom

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
                text:
                {
                    if(periodSelector.currentIndex == 0)
                    {
                        return applicationData.MinTimestampLP1
                    }
                    else
                    {
                        return applicationData.MinTimestampLP2
                    }
                }

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
                text:
                {
                    if(periodSelector.currentIndex == 0)
                    {
                        return applicationData.MaxTimestampLP1
                    }
                    else
                    {
                        return applicationData.MaxTimestampLP2
                    }
                }

                font.bold: false
                font.pointSize: headerID.fontSizeSmall
                color: "darkgray"
            }
        }
    }

    Popup
    {
        id: datepickerpopup
        visible: false
        width: energyPage.width*0.9
        height: energyPage.width*0.9
        x: (energyPage.width - datepickerpopup.width)/2
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
                    width: energyPage.width*0.25
                    height: energyPage.width*0.125
                    text: "Cancel"
                    onClicked:
                    {
                        if(calendarmode === 0)
                        {
                            eyselecteddatefrom = Qt.formatDate(currentDate, 'dd/MM/yyyy').toString();
                            dateTextFrom.text = Qt.formatDate(currentDate, 'dd MMM yyyy').toString();
                        }
                        else
                        {
                            if(calendarmode === 1)
                            {
                                eyselecteddateto = Qt.formatDate(currentDate, 'dd/MM/yyyy').toString();
                                dateTextTo.text = Qt.formatDate(currentDate, 'dd MMM yyyy').toString();
                            }
                        }

                        datepickerpopup.close();
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
                    width: energyPage.width*0.25
                    height: energyPage.width*0.125
                    text: "Save"
                    onClicked:
                    {
                        if(calendarmode === 0)
                        {
                            eyselecteddatefrom = Qt.formatDate(datepickctl.selectedDate, 'dd/MM/yyyy').toString();
                            dateTextFrom.text = Qt.formatDate(datepickctl.selectedDate, 'dd MMM yyyy').toString();
                        }
                        else
                        {
                            if(calendarmode === 1)
                            {
                                eyselecteddateto = Qt.formatDate(datepickctl.selectedDate, 'dd/MM/yyyy').toString();
                                dateTextTo.text = Qt.formatDate(datepickctl.selectedDate, 'dd MMM yyyy').toString();
                            }
                        }

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

        function onEnergyStatisticsRefreshed()
        {
            headerID.backBtn.enabled = true;
            headerID.optionsBtn.enabled = true;
            barChart.points = applicationData.EnergyBarData;
            splineChart.points = applicationData.EnergyLineData;
        }
    }
}
