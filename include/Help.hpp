#ifndef ENTITY_HELP
#define ENTITY_HELP

#include <QObject>
#include <QtSql>

class Help
{
public:
    Q_GADGET

public:
    QString Title_;
public:
    QString Description_;

public:
    Q_PROPERTY(QString Title MEMBER Title_)
    Q_PROPERTY(QString Description MEMBER Description_)

public:
    ~Help();
    Help();
    Help(QString iTitle, QString iDescription);
    Help(const Help& other);
    Help& operator = (const Help& other);

    static void LoadHelpStrings(QList<Help> &hlist);
};

#endif
