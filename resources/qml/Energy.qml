import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.3
import QtQuick.Layouts 1.3

Page
{
    id: energyPage

    Material.theme: Material.Light
    Material.accent: applicationData.Theme.AccentColor

    property date currentDate: new Date()
    property date resetDate: new Date()
    property string resetDateStr: "2000-01-01 00:00:00"
    property var applocale: Qt.locale()

    property string eyselecteddatefrom: Qt.formatDate(datepick.selectedDate, 'dd MMM yyyy').toString()
    property string eyselecteddateto: Qt.formatDate(datepick.selectedDate, 'dd MMM yyyy').toString()
    property int calendarmode: 0 // 0 is for FROM and 1 is for TO
    property int visualizationmode: 0 // 0 is for ENERGY and 1 is for USD/CURRENCY

    property string errorstring: ""

    Component.onCompleted:
    {
        currentDate = new Date()
        dateTextFrom.text = Qt.formatDate(currentDate, 'dd MMM yyyy').toString();
        dateTextTo.text = Qt.formatDate(currentDate, 'dd MMM yyyy').toString();
        eyselecteddatefrom = Qt.formatDate(currentDate, 'dd/MM/yyyy').toString();
        eyselecteddateto = Qt.formatDate(currentDate, 'dd/MM/yyyy').toString();
        parameterList.currentIndex = 0
        periodSelector.currentIndex = 0
        barChart.visible = true
        lineChart.visible = false
        graphSelecter.checked = true
        xAxisLabel.text = "Hours"
        graphSelecter.visible = false
    }

    Rectangle
    {
        id: background
        width: parent.width
        height: parent.height - headerPanel.height
        anchors.top: headerPanel.bottom
        color: applicationData.Theme.BackgroundColor
    }

    Header
    {
        id:headerPanel
        headerTitle : "Energy"
        isMenuButtonVisible: true
        isMeterNameVisible: true
        isSyncDateVisible: true
        isConnectionIndicatorVisible: applicationData.IsAppConnected
    }

    ComboBox
    {
        id: parameterList
        height: headerPanel.height*0.5
        width: energyPage.width*0.9
        font.pointSize: headerPanel.fontSizeSmall
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top:headerPanel.bottom
        Material.accent: applicationData.Theme.AccentColor

        popup: Popup
        {
            id:comboPopup
            y: parameterList.height - 1
            width: parameterList.width
            height:contentItem.implicitHeight
            padding: 1
            font.pointSize: headerPanel.fontSizeSmall

            contentItem: ListView
            {
                id:listView
                implicitHeight: contentHeight
                model: parameterList.popup.visible ? parameterList.delegateModel : null
                ScrollIndicator.vertical: ScrollIndicator { }
                Material.accent: applicationData.Theme.AccentColor
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
                    font.pointSize: headerPanel.fontSizeSmall
                }
            }

            background: Rectangle
            {
                border.width: 1
                border.color: applicationData.Theme.AccentColor
                color: applicationData.Theme.ControlColor
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

        onCurrentIndexChanged:
        {
            loadEnergyData();
        }

        model: ["Import Energy 1", "Export Energy 1", "Import Reactive Energy 1", "Export Reactive Energy 1", "Import Energy 2", "Export Energy 2", "Import Reactive Energy 2", "Export Reactive Energy 2"]
    }

    CustomTab
    {
        id: periodSelector
        tabHeight: headerPanel.height*0.4
        tabWidth: energyPage.width*0.9
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top:parameterList.bottom
        anchors.topMargin: 5
        model: ["Daily", "Weekly", "Monthly", "Annual"]
        accentColor: applicationData.Theme.AccentColor
        accentLowColor: applicationData.Theme.AccentColor
        controlLowColor: applicationData.Theme.ControlColor
        controlColor: applicationData.Theme.ControlColor
        fontColor: applicationData.Theme.FontColor

        onCurrentIndexChanged:
        {
            if(currentIndex === 0)
            {
                lineChart.xIntervalDynamic = 12
                xAxisLabel.text = "Hours"
                loadEnergyData();
            }

            if(currentIndex === 1)
            {
                lineChart.xIntervalDynamic = 7
                xAxisLabel.text = "Day of Week"
                loadEnergyData();
            }

            if(currentIndex === 2)
            {
                lineChart.xIntervalDynamic = 12
                xAxisLabel.text = "Day"
                loadEnergyData();
            }

            if(currentIndex === 3)
            {
                lineChart.xIntervalDynamic = 12
                 xAxisLabel.text = "Month"
                loadEnergyData();
            }
        }
    }

    Rectangle
    {
        id: dateSelectRangeHeader
        width: energyPage.width*0.9
        height: headerPanel.height*0.3
        anchors.top:periodSelector.bottom
        anchors.topMargin: 5
        anchors.horizontalCenter: parent.horizontalCenter
        color: applicationData.Theme.BackgroundColor

        Rectangle
        {
            id: dtrecttop1
            width: dateSelectRangeHeader.width/2
            height: dateSelectRangeHeader.height
            anchors.top: dateSelectRangeHeader.top
            anchors.left: dateSelectRangeHeader.left
            color: applicationData.Theme.BackgroundColor

            Label
            {
                text: "Select Date"
                color: '#1e90ff'
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
            color: applicationData.Theme.BackgroundColor

            Label
            {
                text: "Compare With"
                color: '#ff8c00'
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
            }
        }
    }

    Rectangle
    {
        id: dateSelectRangeContent
        width: energyPage.width*0.9
        height: headerPanel.height*0.35
        anchors.top:dateSelectRangeHeader.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        color: applicationData.Theme.BackgroundColor


        Rectangle
        {
            id: dtrectbottom1
            width: dateSelectRangeContent.width/2
            height: dateSelectRangeContent.height
            anchors.top: dateSelectRangeContent.top
            anchors.left: dateSelectRangeContent.left
            color: applicationData.Theme.BackgroundColor


            Button
            {
                id: calendarFrom
                height: dateSelectRangeContent.height
                width: dateSelectRangeContent.height
                Image
                {
                    height: dateSelectRangeContent.height
                    width: dateSelectRangeContent.height
                    source: "../images/Calendar.png"
                }

                background: Rectangle
                {
                    color: applicationData.Theme.BackgroundColor
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
                color: applicationData.Theme.FontColor
            }
        }

        Rectangle
        {
            id: dtrectbottom2
            width: dateSelectRangeContent.width/2
            height: dateSelectRangeContent.height
            anchors.top: dateSelectRangeContent.top
            anchors.right: dateSelectRangeContent.right
            color: applicationData.Theme.BackgroundColor

            Button
            {
                id: calendarTo
                height: dateSelectRangeContent.height
                width: dateSelectRangeContent.height
                Image
                {
                    height: dateSelectRangeContent.height
                    width: dateSelectRangeContent.height
                    source: "../images/Calendar.png"
                }

                background: Rectangle
                {
                    color: applicationData.Theme.BackgroundColor
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
                color: applicationData.Theme.FontColor
            }
        }
    }

    Rectangle
    {
        id: graphRect
        height: energyPage.width*0.8
        width: energyPage.width*0.9
        anchors.top: dateSelectRangeContent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: 5
        color: applicationData.Theme.BackgroundColor

        CustomBarChart
        {
            id: barChart
            width: parent.width
            height: parent.height
            antialiasing: true
            visible: true
            color1: '#1e90ff'
            color2: '#ff8c00'

            title: applicationData.ChartTitle
            yLabel: applicationData.YLabelEnergy
            xLabel: applicationData.XLabelEnergy
        }

        CustomLineChart
        {
            id: lineChart
            width: parent.width
            height: parent.height
            antialiasing: true
            visible: false
            color1: '#1e90ff'
            color2: '#ff8c00'

            title: applicationData.ChartTitle
            yLabel: applicationData.YLabelEnergy
            xLabel: applicationData.XLabelEnergy
        }

        Label
        {
            id: errorArea
            anchors.fill: parent
            visible: false
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            color: "red"
        }
    }

    Text
    {
        id: xAxisLabel
        anchors.top: graphRect.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        font.pointSize: headerPanel.fontSizeSmall
        text:applicationData.XLabelEnergy
        color: applicationData.Theme.FontColor
    }

    Rectangle
    {
        id: modeRect
        width: energyPage.width*0.9
        height: headerPanel.height/2.5
        anchors.top: xAxisLabel.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: 5
        color: applicationData.Theme.BackgroundColor

        Grid
        {
            anchors.horizontalCenter: parent.horizontalCenter
            rows: 1
            columns: 1

            Rectangle
            {
                height: modeRect.height
                width: modeRect.height*3
                color: applicationData.Theme.BackgroundColor

                CustomSelector
                {
                    id: graphSelecter
                    checked: false
                    selectorSize: 32
                    accentColor: applicationData.Theme.AccentColor
                    fontColor: applicationData.Theme.FontColor
                    backgroundColor: applicationData.Theme.BackgroundColor
                    text: "Bar"

                    onClicked:
                    {
                        var fl = graphSelecter.checked
                        if(fl === true)
                        {
                            barChart.visible = true
                            lineChart.visible = false
                            text = "Bar"
                        }
                        else
                        {
                            barChart.visible = false
                            lineChart.visible = true
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
        color: applicationData.Theme.BackgroundColor

        Label
        {
            id: dailyMaxTouTitle
            text: "Daily Max TOU"
            font.bold: true
            font.pointSize: headerPanel.fontSizeSmall
            color: applicationData.Theme.FontColor
            anchors.left: parent.left
            anchors.top: parent.top
        }

        Label
        {
            id: dailyMaxTouValue
            text: applicationData.DailyMaxTou.toFixed(2) + " kWh"
            color: '#1e90ff'
            font.pointSize: headerPanel.fontSizeSmall
            anchors.left: parent.left
            anchors.bottom: parent.bottom
        }

        Label
        {
            id: totalTouTitle
            text: "Total TOU"
            font.bold: true
            font.pointSize: headerPanel.fontSizeSmall
            color: applicationData.Theme.FontColor
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
        }

        Label
        {
            id: totalTouValue
            text: applicationData.TotalTou.toFixed(2) + " kWh"
            color: '#1e90ff'
            font.pointSize: headerPanel.fontSizeSmall
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom
        }

        Label
        {
            id: hourlyAveTouTitle
            text: "Hourly Avg TOU"
            font.bold: true
            font.pointSize: headerPanel.fontSizeSmall
            color: applicationData.Theme.FontColor
            anchors.right: parent.right
            anchors.top: parent.top
        }

        Label
        {
            id: hourlyAveTouValue
            text: applicationData.HourlyAverageTou.toFixed(2) + " kWh"
            color: '#1e90ff'
            font.pointSize: headerPanel.fontSizeSmall
            anchors.right: parent.right
            anchors.bottom: parent.bottom
        }
    }

    Rectangle
    {
        id: rectTimestamp
        anchors.horizontalCenter: parent.horizontalCenter
        width: energyPage.width*0.9
        height: (energyPage.width*0.9)*0.15
        anchors.top: rectTou.bottom
        color: applicationData.Theme.BackgroundColor

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
                    if(periodSelector.currentIndex === 0)
                    {
                       return applicationData.MinTimestampLP1
                    }
                    else
                    {
                       return applicationData.MinTimestampLP2
                    }
                }

                font.bold: false
                font.pointSize: headerPanel.fontSizeSmall
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
                    if(periodSelector.currentIndex === 0)
                    {
                       return applicationData.MaxTimestampLP1
                    }
                    else
                    {
                       return applicationData.MaxTimestampLP2
                    }
                }

                font.bold: false
                font.pointSize: headerPanel.fontSizeSmall
                color: "darkgray"
            }
        }
    }

    Popup
    {
        id: datepickerpopup
        visible: false
        width: energyPage.width*0.9
        height: energyPage.width
        x: (energyPage.width - datepickerpopup.width)/2
        y: parent.height/3
        padding: 0
        modal: true
        property int itemIndex: 0
        property string itemValue: ""

        contentItem: Rectangle
        {
            anchors.fill: parent
            color: applicationData.Theme.ControlColor

            CustomDatePicker
            {
                id: datepickctl
                height: datepickerpopup.width*0.8
                width: datepickerpopup.width
                fontColor: applicationData.Theme.FontColor
                controlColor: applicationData.Theme.ControlColor
                accentColor: applicationData.Theme.AccentColor
                Component.onCompleted: set(new Date()) // today
                anchors.top: parent.top
                anchors.horizontalCenter: parent.horizontalCenter
                onClicked:
                {
                    currentDate = selectedDate;
                }
            }

            Grid
            {
                id: gridDlg
                rows: 1
                columns: 2
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 5
                anchors.rightMargin: 5
                anchors.right: parent.right
                spacing: 10

                CustomButton
                {
                    id:btnCancel
                    width: energyPage.width*0.25
                    height: energyPage.width*0.125
                    text: "Cancel"
                    isDefault: false
                    accentColor: applicationData.Theme.AccentColor
                    onClicked:
                    {
                         datepickerpopup.close();
                    }
                }

                CustomButton
                {
                    id:btnSave
                    width: energyPage.width*0.25
                    height: energyPage.width*0.125
                    text: "Save"
                    isDefault: true
                    accentColor: applicationData.Theme.AccentColor
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
                        loadEnergyData();
                    }
                }
            }
        }
    }

    function loadEnergyData()
    {
        applicationData.invokeEnergyStatistics(visualizationmode, parameterList.currentIndex, periodSelector.currentIndex,  eyselecteddatefrom, eyselecteddateto);
    }

    Connections
    {
        target: applicationData

        function onEnergyStatisticsRefreshed()
        {
            barChart.points = applicationData.EnergyBarData;
            lineChart.points = applicationData.EnergyLineData;
            xAxisLabel.visible = true
            errorArea.visible = false
            graphSelecter.visible = true

            if(graphSelecter.checked === true)
            {
                barChart.visible = true
                lineChart.visible = false
            }
            else
            {
                barChart.visible = false
                lineChart.visible = true
            }
        }

        function onEnergyError(str)
        {
            console.log(str)
            errorArea.text = str
            errorArea.visible = true
            barChart.visible = false
            lineChart.visible = false
            xAxisLabel.visible = false
            graphSelecter.visible = false
        }
    }


}
