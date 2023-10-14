#include "Help.hpp"

Help::~Help()
{

}

Help::Help()
{

}

Help::Help(QString iTitle, QString iDescription)
{
    Title_ = iTitle;
    Description_ = iDescription;
}

Help::Help(const Help& other)
{
    Description_ = other.Description_;
    Title_ = other.Title_;
}

Help& Help::operator = (const Help& other)
{
    Description_ = other.Description_;
    Title_ = other.Title_;
    return *this;
}

void Help::LoadHelpStrings(QList<Help> &hlist)
{
    hlist.clear();

    hlist.append(Help("General", " 1. Tap on a meter icon makes it active<br>\
                                   2. A connected meter is active<br>\
                                   3. Scan QR code, tap on the image once it is centred<br>\
                                   4. Scan QR code, cancel or save to use the scanned test<br>\
                                   5. All operations requires a meter to be selected/connected<br>\
                                   6. Check the top information bar for last communication<br>\
                                   7. Keep alive is used to fetch all needed information"));


    hlist.append(Help("Add a Meter", " 1. Select Add meter from Menu or<br>\
                                       2. Tap on Add icon at the bottom in meter list screen<br>\
                                       3. Fill the pairing code or launch the QR code scanner<br>\
                                       4. Click on the Add Meter button<br>\
                                       5. Enter Bluetooth PIN<br>\
                                       6. If all inputs are correct, meter will be searched and added"));

    hlist.append(Help("Connect to a Meter", " 1. Go to the meter list screen<br>\
                                              2. Click Connect Button for meter of your choice<br>\
                                              3. Fill the pairing code or launch the QR code scanner<br>\
                                              4. Click on the Add Meter button<br>\
                                              5. Enter Bluetooth PIN<br>\
                                              6. If all inputs are correct, meter will be connected"));

    hlist.append(Help("Adding Token", " 1. First connect to the meter of choice<br>\
                                        2. Once connected, go to token list screen<br>\
                                        3. Click on the add icon at the bottom<br>\
                                        4. Fill the token or launch the QR code scanner<br>\
                                        5. Save the token"));

    hlist.append(Help("Adding Tariff", " 1. First connect to the meter of choice<br>\
                                         2. Once connected, go to tariff list screen<br>\
                                         3. Click on the add icon at the bottom<br>\
                                         4. Fill in the tariff details<br>\
                                         5. Save the tariff"));

    hlist.append(Help("Transferring Token", " 1. First connect to the meter of choice<br>\
                                              2. Once connected, go to token list screen<br>\
                                              3. Click on the Apply button<br>\
                                              4. CSA will indicate success, failure, token value etc."));
}
