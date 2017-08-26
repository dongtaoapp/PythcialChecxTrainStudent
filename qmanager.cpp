#include "qmanager.h"
#include "define.h"
#include <qDebug>
#include <QMessageBox>
#include <QtCore>
#include <QSettings>
QManager::QManager()
{
   QSettings *readconfig=new QSettings(APPPATH,QSettings::IniFormat);
   readconfig->value("Client/ServerIP").toString();
   m_client.connect_to_server(readconfig->value("Client/ServerIP").toString(),666666);
   delete readconfig;
   connect(&m_client,SIGNAL(CONNECTEDSUCCESS()),this,SLOT(managerListen()));
}
QManager::~QManager()
{
    qDebug()<<__FUNCTION__;

}

void QManager::managerListen()
{
    m_listen.funbind(25000);
    m_listen.start_system_audio();
}

void QManager::RequestTalk()
{
    QJsonDocument jdocument;
    QJsonObject jobject;

    QSettings *readconfig=new QSettings(APPPATH,QSettings::IniFormat);

    jobject.insert("Type",QString("string"));
    jobject.insert("cmd",QString("REQUESTTALK"));
    jobject.insert("data",readconfig->value("System_Param/deskID").toString());
    jdocument.setObject(jobject);
    QByteArray sendbyte=jdocument.toJson(QJsonDocument::Compact);
   m_client.send_msg(sendbyte);
   delete readconfig;
}
