#include "Controller.hpp"
#include "Help.hpp"
#include "Logger.hpp"

void Controller::invokeChangePage(int ipage)
{
    emit pageAction(ipage);
}

void Controller::invokeThemeUpdate()
{
    if(app->palette().window().color().value() > app->palette().windowText().color().value())
    {
        isDarkTheme_ = false;
    }
    else
    {
        isDarkTheme_ = true;
    }

    emit IsDarkThemeChanged();
}

void Controller::invokeSaveCurrentPage(QString page)
{
    lastPage_ = page;
}

QString Controller::invokeRetreiveLastPage()
{
    return lastPage_;
}

void Controller::invokeLoadHelpStrings(uint32_t index)
{
    helpStringList_.clear();

    Help::LoadHelpStrings(index, helpStringList_);

    emit HelpStringsChanged();
}

bool Controller::getIsDemoMode()
{
    return isDemoMode_;
}

void Controller::setIsDemoMode(bool isdemomode)
{
    deliveredEnergy_ = 20;
    instantaneousPower_ = 2;
    usedCredit_ = 100;
    grossCredit_ =  200;
    creditCurrency_ = "kWh";

    isDemoMode_ = isdemomode;
    emit IsDemoModeChanged();

    LoadAllData();
}

bool Controller::getShowIndicator()
{
    if(lastPage_ == "AddToken.qml"
        || lastPage_ == "AddMeter.qml"
        || lastPage_ == "AddTariff.qml"
        || lastPage_ == "AvailableTokens.qml"
        || lastPage_ == "AvailableMeters.qml"
        || lastPage_ == "AvailableTariff.qml"
        || lastPage_ == "Dashboard.qml"
        || lastPage_ == "Energy.qml"
        || lastPage_ == "PowerQuality.qml"
        || lastPage_ == "Alarms.qml"
        || lastPage_ == "Trace.qml")
    {
        return true;
    }

    return false;
}

QString Controller::getAppVersion()
{
    return appVerion_;
}

bool Controller::getIsDarkTheme()
{
    return isDarkTheme_;
}

void Controller::setIsDarkTheme(bool isdarktheme)
{
    isDarkTheme_ = isdarktheme;

    if(isDarkTheme_)
    {
        theme_.EnableDarkMode();
    }
    else
    {
        theme_.EnableLightMode();
    }

    emit IsDarkThemeChanged();
    emit ThemeChanged();
}

QList<QString> Controller::getHelpStrings()
{
    return helpStringList_;
}

void Controller::setIsFirstRun(bool isfirstrun)
{
    is_first_run_ = isfirstrun;
    emit IsFirstRunChanged();
}

bool Controller::getIsFirstRun()
{
    if(userList_.count() < 1)
    {
        is_first_run_ = true;
    }
    else
    {
        is_first_run_ = false;
    }

    return is_first_run_;
}

CsaTheme Controller::getTheme()
{
    return theme_;
}

void Controller::Trace(QString str)
{
    Logger::GetInstance()->write(str.toStdString(), LogInfo);
    str = QDateTime::currentDateTime().toString("yy-MM-dd hh:mm:ss ") + str + "\n";
    if(traceString_.length() > 1024*1024)
    {
        traceString_.clear();
    }

    traceString_ = str + traceString_;
    emit TraceStringChanged();
}

void Controller::ClearTrace()
{
    traceString_.clear();
    emit TraceStringChanged();
}

void Controller::setIsTraceEnabled(bool istraceenabled)
{
    isTraceEnabled_ = istraceenabled;
    emit IsTraceEnabledChanged();
}

bool Controller::getIsTraceEnabled()
{
    return isTraceEnabled_;
}

QString Controller::getTraceString()
{
    return traceString_;
}

void Controller::setIsDumpRxTxEnabled(bool isdumprxtxenabled)
{
    isDumpRxTxEnabled_ = isdumprxtxenabled;
    emit IsDumpRxTxEnabledChanged();
}

bool Controller::getIsDumpRxTxEnabled()
{
    return isDumpRxTxEnabled_;
}
