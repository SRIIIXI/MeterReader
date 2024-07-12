#include "Help.hpp"

Help::~Help()
{

}

Help::Help()
{

}

void Help::LoadHelpStrings(uint32_t index, QList<QString> &hlist)
{
    hlist.clear();

    switch(index)
    {
        case 0:
        {
            hlist.append("1. Tap on a meter icon makes it active");
            hlist.append("2. A connected meter is active");
            hlist.append("3. Scan QR code, tap on the image once it is centred");
            hlist.append("4. Scan QR code, cancel or save to use the scanned test");
            hlist.append("5. All operations requires a meter to be selected/connected");
            hlist.append("6. Check the top information bar for last communication");
            hlist.append("7. Keep alive is used to fetch all needed information");
            break;
        }

        case 1:
        {
            hlist.append("1. Select Add meter from Menu or");
            hlist.append("2. Tap on Add icon at the bottom in meter list screen");
            hlist.append("3. Fill the pairing code or launch the QR code scanner");
            hlist.append("4. Click on the Add Meter button");
            hlist.append("5. Enter Bluetooth PIN");
            hlist.append("6. If all inputs are correct, meter will be searched and added");
            break;
        }

        case 2:
        {
            hlist.append("1. Go to the meter list screen");
            hlist.append("2. Click Connect Button for meter of your choice");
            hlist.append("3. Fill the pairing code or launch the QR code scanner");
            hlist.append("4. Click on the Add Meter button");
            hlist.append("5. Enter Bluetooth PIN");
            hlist.append("6. If all inputs are correct, meter will be connected");
            break;
        }

        case 3:
        {
            hlist.append("1. First connect to the meter of choice");
            hlist.append("2. Once connected, go to token list screen");
            hlist.append("3. Click on the add icon at the bottom");
            hlist.append("4. Fill the token or launch the QR code scanner");
            hlist.append("5. Save the token");
            break;
        }

        case 4:
        {
            hlist.append("1. First connect to the meter of choice");
            hlist.append("2. Once connected, go to token list screen");
            hlist.append("3. Click on the Apply button");
            hlist.append("4. CSA will indicate success, failure, token value etc.");
            break;
        }

        default:
            break;
    }
}

