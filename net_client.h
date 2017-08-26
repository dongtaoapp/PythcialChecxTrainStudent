#ifndef NET_CLIENT_H
#define NET_CLIENT_H
#include <QTcpSocket>
#include <QUdpSocket>
#include <QObject>
#include <QString>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
class net_client : public QObject
{
    Q_OBJECT
public:
    enum net_client_state{
        CONNECTED,//������
        UNCONNECTED,//δ����
        CONECTEDREFUSE,//���Ӿܾ�
        SOCKETACCESSERROR,//�׽��ֲ���ʧ�ܣ���ΪӦ�ó���ȱ�ٱ����Ȩ�ޡ�
        UNKNOWNERROR//δ֪����
    };
    net_client(QObject *parent=Q_NULLPTR);
    ~net_client();
    QByteArray TellServerDeskID();
public:
   void connect_to_server(QString ip,qint16 port);
public slots:
     void send_msg(QByteArray &msg);

    void recv_msg();
    void manager_msg(QByteArray & recv);
    void error(QAbstractSocket::SocketError errorstring);
    void ondisconnected();
    void onconnected();
    void  shutdown();





    void onUdpSocket();
signals:
    void CONNECTEDSUCCESS();


    void signal_switch_flash(QString);
    void signal_control_talk(bool talk);
    void signal_start_train(bool,int);//��һ���������Ƿ�ѵ�� �ڶ�����ѵ��ID
    void signal_exam(bool,int);//��һ���������Ƿ��� �ڶ��������¿���ID
    void signal_auscultar(bool);
    void signal_string(QString &);
    void signal_error(QString);
    void signal_socket_state(net_client_state);
private:
   QTcpSocket *net_client_socket;
   QJsonArray jarry;
   QJsonDocument jdocument;
   QJsonObject jobject;

   QUdpSocket *udpsocket;
};
#endif // NET_CLIENT_H
