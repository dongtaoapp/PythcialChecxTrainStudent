#ifndef QMANAGER_H
#define QMANAGER_H

#include <QObject>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

#include "net_client.h"
#include "listen.h"
#include "speak.h"
class QManager : public QObject
{
    Q_OBJECT
public:
    QManager();
    ~QManager();

    void RequestTalk();
public slots:
    void managerListen();

private:
    net_client m_client;
    listen m_listen;
    speak m_speak;
};

#endif // QMANAGER_H
