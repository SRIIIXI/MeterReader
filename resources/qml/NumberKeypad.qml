import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.3

Rectangle
{
    Material.theme: Material.Light
    Material.accent: "#961C1C"

    id:numberKeyPadID
    width: parent.width * 0.6
    height: parent.height * 0.5
    signal buttonPressed()
    signal cancelPressed()
    signal enterBtnPressed()
    property bool isTickMarkVisible: false
    property int currentButtonPressed: -1

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
        columns: 3
        columnSpacing: 32
        rowSpacing: 16
        anchors.horizontalCenter: parent.horizontalCenter

        RoundButton
        {
            width: numberKeyPadID.width*0.3
            height: numberKeyPadID.width*0.3
            text: "1"
            font.bold: true

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

            background: Rectangle
            {
                border.width: 1
                radius: width*0.5
                border.color:
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

            onClicked:
            {
                currentButtonPressed = 1
                buttonPressed()
            }
        }

        RoundButton
        {
            width: numberKeyPadID.width*0.3
            height: numberKeyPadID.width*0.3
            text: "2"
            font.bold: true

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

            background: Rectangle
            {
                border.width: 1
                radius: width*0.5
                border.color:
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

            onClicked:
            {
                currentButtonPressed = 2
                buttonPressed()
            }
        }

        RoundButton
        {
            width: numberKeyPadID.width*0.3
            height: numberKeyPadID.width*0.3
            text: "3"
            font.bold: true

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

            background: Rectangle
            {
                border.width: 1
                radius: width*0.5
                border.color:
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

            onClicked:
            {
                currentButtonPressed = 3
                buttonPressed()
            }
        }

        RoundButton
        {
            width: numberKeyPadID.width*0.3
            height: numberKeyPadID.width*0.3
            text: "4"
            font.bold: true

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

            background: Rectangle
            {
                border.width: 1
                radius: width*0.5
                border.color:
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

            onClicked:
            {
                currentButtonPressed = 4
                buttonPressed()
            }
        }

        RoundButton
        {
            width: numberKeyPadID.width*0.3
            height: numberKeyPadID.width*0.3
            text: "5"
            font.bold: true

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

            background: Rectangle
            {
                border.width: 1
                radius: width*0.5
                border.color:
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

            onClicked:
            {
                currentButtonPressed = 5
                buttonPressed()
            }
        }

        RoundButton
        {
            width: numberKeyPadID.width*0.3
            height: numberKeyPadID.width*0.3
            text: "6"
            font.bold: true

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

            background: Rectangle
            {
                border.width: 1
                radius: width*0.5
                border.color:
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

            onClicked:
            {
                currentButtonPressed = 6
                buttonPressed()
            }
        }

        RoundButton
        {
            width: numberKeyPadID.width*0.3
            height: numberKeyPadID.width*0.3
            text: "7"
            font.bold: true

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

            background: Rectangle
            {
                border.width: 1
                radius: width*0.5
                border.color:
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

            onClicked:
            {
                currentButtonPressed = 7
                buttonPressed()
            }
        }

        RoundButton
        {
            width: numberKeyPadID.width*0.3
            height: numberKeyPadID.width*0.3
            text: "8"
            font.bold: true

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

            background: Rectangle
            {
                border.width: 1
                radius: width*0.5
                border.color:
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

            onClicked:
            {
                currentButtonPressed = 8
                buttonPressed()
            }
        }

        RoundButton
        {
            width: numberKeyPadID.width*0.3
            height: numberKeyPadID.width*0.3
            text: "9"
            font.bold: true

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

            background: Rectangle
            {
                border.width: 1
                radius: width*0.5
                border.color:
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

            onClicked:
            {
                currentButtonPressed = 9
                buttonPressed()
            }
        }

        RoundButton
        {
            width: numberKeyPadID.width*0.3
            height: numberKeyPadID.width*0.3
            text: "0"
            font.bold: true

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

            background: Rectangle
            {
                border.width: 1
                radius: width*0.5
                border.color:
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

            onClicked:
            {
                currentButtonPressed = 0
                buttonPressed()
            }
        }

        RoundButton
        {
            width: numberKeyPadID.width*0.3
            height: numberKeyPadID.width*0.3
            icon.source:
            {
                if(applicationData.IsDarkTheme === true)
                {
                    return "../images/BackspaceWhite.png";
                }
                else
                {
                    return "../images/BackspaceBlack.png";
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

            background: Rectangle
            {
                border.width: 1
                radius: width*0.5
                border.color:
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

            onClicked:
            {
                cancelPressed()
            }
        }

        RoundButton
        {
            width: numberKeyPadID.width*0.3
            height: numberKeyPadID.width*0.3
            icon.source:
            {
                if(applicationData.IsDarkTheme === true)
                {
                    return "../images/NextWhite.png";
                }
                else
                {
                    return "../images/NextBlack.png";
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

            visible: isTickMarkVisible

            background: Rectangle
            {
                border.width: 1
                radius: width*0.5
                border.color:
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

            onClicked:
            {
                enterBtnPressed()
            }
        }
    }
}


