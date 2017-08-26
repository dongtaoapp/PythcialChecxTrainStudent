#include "net_client.h"
#include "define.h"
#include <QHostAddress>
#include <QHostInfo>
#include <QMessageBox>
#include <QtCore>
#include <QSettings>
#include "Windows.h"
net_client::net_client(QObject *parent) :
    QObject(parent)
{
    /***********查看教师机是否在线*************/
   udpsocket=new QUdpSocket;
   udpsocket->bind(QHostAddress::AnyIPv4, 999999, QUdpSocket::ReuseAddressHint);
   QHostAddress mcast_addr("224.0.0.17");
  udpsocket->joinMulticastGroup(mcast_addr);//这句是关键，加入组播地址

  connect(udpsocket,SIGNAL(readyRead()),this,SLOT(onUdpSocket()));



/*****************************/
  net_client_socket=new QTcpSocket();
  connect(net_client_socket,SIGNAL(readyRead()),this,SLOT(recv_msg()));
  connect(net_client_socket,SIGNAL(connected()),this,SLOT(onconnected()));
  connect(net_client_socket,SIGNAL(disconnected()),this,SLOT(ondisconnected()));
  connect(net_client_socket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(error(QAbstractSocket::SocketError)));
}

net_client::~net_client()
{

   net_client_socket->deleteLater();

   udpsocket->deleteLater();

    this->deleteLater();
   qDebug()<<__FUNCTION__;

}

void net_client::onUdpSocket()
{
    qDebug()<<__FUNCTION__;

    QSettings *readconfig=new QSettings(APPPATH,QSettings::IniFormat);
    readconfig->value("Client/ServerIP").toString();
    connect_to_server(readconfig->value("Client/ServerIP").toString(),666666);
    delete readconfig;
}


void net_client::connect_to_server(QString ip, qint16 port)
{
    QHostAddress address(ip);
    net_client_socket->connectToHost(address,port);
}

void net_client::onconnected()
{
    qDebug()<<__FUNCTION__;
    emit signal_socket_state(net_client_state::CONNECTED);
}
void net_client::ondisconnected()
{
    qDebug()<<__FUNCTION__;
    emit signal_socket_state(net_client_state::UNCONNECTED);
}

void net_client::send_msg(QByteArray &msg)
{
  net_client_socket->write(msg);
}
/*
 * 接收信息
 */
void net_client::recv_msg()
{
  QByteArray recvbytes=net_client_socket->readAll();

  if(recvbytes.isEmpty())
  {
      return;
  }
  manager_msg(recvbytes);
}
/**json格式***[type: cmd: data:]***type: flash  talk exam train auscultar closePC string play*****/

/***********解析教师机的json字符串 并发出相关的信号**********/

QByteArray net_client::TellServerDeskID()
{
    QString data;
    QString DeskID;
    QSettings *readconfig=new QSettings(APPPATH,QSettings::IniFormat);
    DeskID=readconfig->value("System_Param/deskID").toString();
    delete readconfig;
    foreach(QHostAddress address,QHostInfo::fromName(QHostInfo::localHostName()).addresses())
    {
         if(address.protocol() == QAbstractSocket::IPv4Protocol)
         data=DeskID+QString(":")+ address.toString();
    }
    jobject.insert("Type",QString("string"));
    jobject.insert("cmd",QString("DeskID"));
    jobject.insert("data",data);
    jdocument.setObject(jobject);
    QByteArray str=jdocument.toJson(QJsonDocument::Compact);
    return str;

}
void net_client::manager_msg(QByteArray &recv)
{
  if(recv.isEmpty())
  {
      return ;
  }
    QJsonParseError simp_json_error;

    QJsonDocument jdocument =QJsonDocument::fromJson(recv,&simp_json_error);

    if(simp_json_error.error==QJsonParseError::NoError)
    {
        QJsonObject object=jdocument.object();
        if(object.isEmpty())
        {
            return;
        }
         QString type=object.value("Type").toString();
         bool cmd=object.value("cmd").toBool();
         QString data=object.value("data").toString();
         if(type==QString("flash"))
         {
            emit signal_switch_flash(data);
            qDebug()<<__FUNCTION__<<data;
            return;
         }
         if(type==QString("talk"))
         {
            emit signal_control_talk(cmd);
            return;
          }
           else if (type==QString("exam"))
          {
             bool ok;
             int examid= data.toInt(&ok);
             emit signal_exam(cmd,examid);
             qDebug()<<cmd<<examid;
             return;
          }
         else if(type==QString("train"))
         {
             bool ok;
             int trainid=data.toInt(&ok);
             emit signal_start_train(cmd,trainid);
             qDebug()<<cmd<<trainid;
             return;
          }
         else if(type==QString("auscultar"))
         {
            emit signal_auscultar(cmd);
             return;
         }
         else if(type==QString("closePC"))
         {        
            shutdown();
             return;
         }
         else if(type==QString("string"))
         {
            emit signal_string(data);

            if(data=="CLIENTCONNECTEDSUCCESS")
            {
                qDebug()<<__FUNCTION__<<"data"<<data;
               net_client_socket->write(TellServerDeskID());
               QMessageBox::information(NULL,QStringLiteral("连接提示"),QStringLiteral("连接教师机成功"),QMessageBox::Yes);
               emit CONNECTEDSUCCESS();


               return;
            }
         }
    }
    else
    {
        qDebug()<<simp_json_error.errorString();
    }
}
/***********连接错误信息***********/
void net_client::error(QAbstractSocket::SocketError errorstring)
{
#if 0
   if(errorstring==QAbstractSocket::ConnectionRefusedError)
   {
      emit signal_socket_state(net_client_state::CONECTEDREFUSE);
   }
   else if(errorstring==QAbstractSocket::SocketAccessError)
   {
       emit signal_socket_state(net_client_state::SOCKETACCESSERROR);
   }
   else
   {
       emit signal_socket_state(net_client_state::UNKNOWNERROR);
   }
#endif
   QMessageBox::information(NULL,QStringLiteral("连接提示"),QStringLiteral("连接教师机失败，确认教师机是否已经打开"),QMessageBox::Yes);
}

void net_client::shutdown()
{
    qDebug()<<__FUNCTION__;
# if 0
    system("shutdown -s -t 0");
#endif
}
