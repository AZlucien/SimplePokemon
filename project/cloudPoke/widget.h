#ifndef WIDGET_H
#define WIDGET_H
#define MAXCONNECT 200

//攻击力
#define START_NORMALPOKE_ATTACK 30
#define START_POWERPOKE_ATTACK 38
#define ADD_NORMALPOKE_ATTACK 5
#define ADD_POWERPOKE_ATTACK 8
//防御力
#define START_NORMALPOKE_DEFENSE 0
#define START_DEFENSEPOKE_DEFENSE 10
#define ADD_NORMALPOKE_DEFENSE 3
#define ADD_DEFENSEPOKE_DEFENSE 5
//血量
#define START_NORMALPOKE_HP 100
#define START_TANKPOKE_HP 120
#define ADD_NORMALPOKE_HP 10
#define ADD_TANKPOKE_HP 15
//敏捷
#define START_NORMALPOKE_ATTINTERVAL 50
#define START_QUICKPOKE_ATTINTERVAL 50
#define ADD_NORMALPOKE_ATTINTERVAL -1
#define ADD_QUICKPOKE_ATTINTERVAL -2





#include <QWidget>
#include <QTcpServer> //监听socket
#include <QTcpSocket> //通信socket
#include <QSqlDatabase>
#include <QMessageBox>
#include <QSqlError>
#include <QDebug>
#include <QSqlQuery>
#include <QVariantList>
#include <QString>
#include <QTime>





QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    //储存所有连接过的用户
    struct clients_check
    {
        QTcpSocket *client_socket;
        QString namestring = "NULL";
        bool isSignIn = false;

    }clients_checks[MAXCONNECT];

    int currentClient = 0;

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    void newconnection();//新建立连接
    void newClientChangeToTrue(QTcpSocket *client_tcp);//登录后改变目标用户数组的登录状态

    void readmessage();//读取信息
    void sendMessageToClient(QString str,QTcpSocket *client_tcp);//发送信息
    void dealMessage(QString str,QTcpSocket *client_tcp);//处理信息
    void distributeSignUp(QString name);//注册时分配精灵
    QString getNameFromTcp(QTcpSocket *client_tcp);//从tcp中找到用户名
    float getRateFromDatabase(QString name);//从数据库返回胜率
    //处理信息细分
    //注册信息 0
    void dealSignUp(QString str,QTcpSocket *client_tcp);
    //登录信息 1
    void dealSignIn(QString str,QTcpSocket *client_tcp);
    //断开信息 2
    void currentClientChangeToFlase(QTcpSocket *client_tcp);//注销后改变目标用户数组的登录状态
    //申领精灵 3
    void dealGetPoke(QTcpSocket *client_tcp);
    //查看用户 4
    void dealGetUser(QTcpSocket *client_tcp);
    //查看徽章 5
    void dealGetBadge(QTcpSocket *client_tcp);
    //查看精灵 6
    void dealCheakPoke(QString str,QTcpSocket *client_tcp);
    //查看出战精灵 7
    void dealCheakFightPoke(QTcpSocket *client_tcp);
    //处理战斗后结果 8
    void dealFightResult(QString str,QTcpSocket *client_tcp);

signals:
    void clientSignIn(QTcpSocket *client_tcp);
 private slots:
    void on_buttonSend_clicked();

private:
    Ui::Widget *ui;
    //网络通信
    QTcpServer *tcpServer;//监听socket
    QTcpSocket *tcpSocket;//通信socket
    //QList<QTcpSocket *> clients_check;//客户序列
    //信号与标志
    bool clientJoinUp_check = false;                    //客户端加
    bool loginSuccessFlag_check = false;                //登陆成功标志


};
#endif // WIDGET_H
