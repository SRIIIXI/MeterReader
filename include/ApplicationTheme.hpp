#ifndef APPLICATION_THEME
#define APPLICATION_THEME

#include <QObject>

class CsaTheme
{
public:
    Q_GADGET

public:
    bool IsDarkMode_;
public:
    QString FontColor_;
public:
    QString AccentColor_;
public:
    QString AccentColorLow_;
public:
    QString BackgroundColor_;
public:
    QString ControlColor_;
public:
    Q_PROPERTY(bool IsDarkMode MEMBER IsDarkMode_)
    Q_PROPERTY(QString FontColor MEMBER FontColor_)
    Q_PROPERTY(QString AccentColor MEMBER AccentColor_)
    Q_PROPERTY(QString AccentColorLow MEMBER AccentColorLow_)
    Q_PROPERTY(QString BackgroundColor MEMBER BackgroundColor_)
    Q_PROPERTY(QString ControlColor MEMBER ControlColor_)

public:
    ~CsaTheme()
    {

    }

    CsaTheme()
    {

    }

    CsaTheme(bool iIsDarkMode, QString iFontColor, QString iAccentColor, QString iAccentColorLow, QString iBackgroundColor, QString iControlColor)
    {
        IsDarkMode_ = iIsDarkMode;
        FontColor_ = iFontColor;
        AccentColor_ = iAccentColor;
        AccentColorLow_ = iAccentColorLow;
        BackgroundColor_ = iBackgroundColor;
        ControlColor_ = iControlColor;
    }

    CsaTheme(const CsaTheme& other)
    {
        IsDarkMode_ = other.IsDarkMode_;
        FontColor_ = other.FontColor_;
        AccentColor_ = other.AccentColor_;
        AccentColorLow_ = other.AccentColorLow_;
        BackgroundColor_ = other.BackgroundColor_;
        ControlColor_ = other.ControlColor_;
    }

    CsaTheme& operator = (const CsaTheme& other)
    {
        IsDarkMode_ = other.IsDarkMode_;
        FontColor_ = other.FontColor_;
        AccentColor_ = other.AccentColor_;
        AccentColorLow_ = other.AccentColorLow_;
        BackgroundColor_ = other.BackgroundColor_;
        ControlColor_ = other.ControlColor_;
        return *this;
    }

    void EnableDarkMode()
    {
        IsDarkMode_ = true;
        FontColor_ = "white";
        AccentColor_ = "dodgerblue";
        AccentColorLow_ = "#e0e0e0";
        BackgroundColor_ = "black";
        ControlColor_ = "#1C2833";
    }

    void EnableLightMode()
    {
        IsDarkMode_ = false;
        FontColor_ = "black";
        AccentColor_ = "dodgerblue";
        AccentColorLow_ = "#e0e0e0";
        BackgroundColor_ = "white";
        ControlColor_ = "whitesmoke";
    }
};

#endif
