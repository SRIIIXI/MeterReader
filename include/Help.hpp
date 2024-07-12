#ifndef ENTITY_HELP
#define ENTITY_HELP

#include <QObject>

class Help
{
public:
    ~Help();
    Help();
    static void LoadHelpStrings(uint32_t index, QList<QString> &hlist);
};

#endif
