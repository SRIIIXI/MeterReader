#include "Controller.hpp"
#include "Helpers.h"

//New User creation interpage variables
QString newUName, newConsId, newPassCode1, newPassCode2;
//New User creation interpage variables

//Reset passcode interpage variables
QString resetConsId, resetPassCode1, resetPassCode2;
//Reset passcode interpage variables

void Controller::invokeLogin(QString pin)
{
    bool found = false;

    isTraceEnabled_ = false;
    emit IsTraceEnabledChanged();

    foreach (auto user, userList_)
    {
        if(user.PassCode1_ == pin)
        {
            found = true;
            currentUser_ = user;
            break;
        }
    }

    if(found)
    {
        emit userLoginSucceeded();
        emit CurrentUserChanged();
    }
    else
    {
        emit userLoginFailed("Passcode is incorrect. Please retry.");
    }
}

void Controller::invokeChangePassword(QString consumerid, QString pass)
{
    bool found = true;
    UserInfo u;
    foreach (auto user, userList_)
    {
        if(user.ConsumerId_ == consumerid)
        {
            found = true;
            u = user;
            break;
        }
    }

    userinfoEntityPtr->update("consumer_id", u.ConsumerId_, "passcode1", pass);
    userinfoEntityPtr->update("consumer_id", u.ConsumerId_, "passcode2", pass);
    currentUser_ = u;

    emit CurrentUserChanged();

    userList_.clear();
    userinfoEntityPtr->allUserInfo(userList_);

    emit pageAction(9);
}

void Controller::invokeCreateUserStage1(QString name, QString userid)
{
    bool errorset =  false;

    if(name.length() > 16 || name.length() < 4)
    {
        errorset = true;
    }

    for(int x = 0; x < name.length(); x++)
    {
        char ch = name.at(x).toLatin1();

        if(isalnum(ch) == 0)
        {
            if(ch != ' ')
            {
                errorset = true;
                break;
            }
        }
    }

    if(errorset)
    {
        emit userNameError();
        return;
    }

    newUName = name;
    newConsId = userid;

    emit pageAction(11);
}

void Controller::invokeCreateUserStage2(QString passcode)
{
    newPassCode1 = passcode;
    emit pageAction(12);
}

void Controller::invokeCreateUserStage3(QString passcode)
{
    newPassCode2 = passcode;

    if(newPassCode1 != newPassCode2)
    {
        emit passCodeMismatch();
        return;
    }

    UserInfo u1;
    u1.ConsumerId_ = newConsId;
    u1.IsFirstLogin_ = true;
    u1.PassCode1_ = newPassCode1;
    u1.PassCode2_ = newPassCode2;
    u1.UserId_ = newUName;

    userinfoEntityPtr->createUserInfo(u1);

    userList_.clear();
    userinfoEntityPtr->allUserInfo(userList_);
    
    currentUser_ = u1;
    emit CurrentUserChanged();

    emit pageAction(9);
}

void Controller::invokeResetPasscodeStage1(QString userid)
{
    resetConsId = userid;
    emit pageAction(15);
}

void Controller::invokeResetPasscodeStage2(QString passcode)
{
    resetPassCode1 = passcode;
    emit pageAction(16);
}

void Controller::invokeResetPasscodeStage3(QString passcode)
{
    resetPassCode2 = passcode;

    if(resetPassCode1 != resetPassCode2)
    {
        emit passCodeMismatch();
        return;
    }

    foreach (auto user, userList_)
    {
        if(user.ConsumerId_ == resetConsId)
        {
            break;
        }
    }

    userinfoEntityPtr->update("consumer_id", resetConsId, "passcode1", resetPassCode1);
    userinfoEntityPtr->update("consumer_id", resetConsId, "passcode2", resetPassCode2);

    userList_.clear();
    userinfoEntityPtr->allUserInfo(userList_);

    if(userList_.count() > 0)
    {
        currentUser_ = userList_.at(0);
    }

    emit CurrentUserChanged();

    emit pageAction(9);
}

void Controller::setCurrentUser(UserInfo iuser)
{
    currentUser_ = iuser;
    emit CurrentUserChanged();
}

UserInfo Controller::getCurrentUser()
{
    return currentUser_;
}



