#ifndef QTALKBACKMANAGER_H
#define QTALKBACKMANAGER_H

#include <QObject>
#include "speak.h"
#include "listen.h"
class QTalkbackManager :public QObject
{
    Q_OBJECT
public:
    QTalkbackManager();
    ~QTalkbackManager();
private:
    speak *m_speak;
    listen *m_listen;
};

#endif // QTALKBACKMANAGER_H
