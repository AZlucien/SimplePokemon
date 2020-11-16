#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    //开始运行服务器

    tcpServer = NULL;
    tcpSocket = NULL;


    //监听套接字,指定父对象自动回收空间
    tcpServer = new QTcpServer(this);

    tcpServer->listen(QHostAddress::Any,8888);

    //建立新连接触发槽函数
    connect(tcpServer,&QTcpServer::newConnection,
            this,&Widget::newconnection);



    //*****************************************************数据库*******************************************************
    //数据库连接
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("140.143.142.112");
    db.setPort(3306);
    db.setDatabaseName("pokemon");
    db.setUserName("pokemon");
    db.setPassword("poke1161");



    bool ok = db.open();
    if (ok)
    {
        QMessageBox::information(this, "infor", "数据库登录成功");
        qDebug() << "数据库登录成功";
    }
    else
    {
        QMessageBox::information(this, "infor", "数据库登录失败");
        qDebug()<<"数据库登录失败"<<db.lastError().text();
    }


    connect(this,&Widget::clientSignIn,this,&Widget::newClientChangeToTrue);

}

Widget::~Widget()
{
    delete ui;
}

void Widget::newClientChangeToTrue(QTcpSocket *client_tcp)
{
    for(int i = 0;i < currentClient;i++)
    {
        if(client_tcp == clients_checks[i].client_socket)
        {
            clients_checks[i].isSignIn = true;
            ui->textRead->append("用户" + clients_checks[i].namestring + "登入成功！");
            //???????????????登录成功后还要初始化一些数据
            break;
        }
    }

}

void Widget::currentClientChangeToFlase(QTcpSocket *client_tcp)
{
    for(int i = 0;i < currentClient;i++)
    {
        if(client_tcp == clients_checks[i].client_socket)
        {
            clients_checks[i].isSignIn = false;
            ui->textRead->append("用户" + clients_checks[i].namestring + "注销成功！");
            //???????????????注销成功后还要初始化一些数据
            break;
        }
    }
}



void Widget::on_buttonSend_clicked()
{
    if(tcpSocket == NULL)
    {
        ui->textRead->append("没有客户连接，请连接后重试！");
        return;
    }

    //获取编辑器内容
    QString str = ui->textSend->toPlainText();
    //给对方发送数据，使用tcpSocket套接字
    tcpSocket->write(str.toUtf8().data());
    ui->textSend->setText("");
    ui->textRead->append("服务器发送了：" + str);
}
//新用户连接
void Widget::newconnection()
{

    qDebug() << "新客户端登陆:";
    QTcpSocket *newClient;

    //得到新进来的socket，用于标识刚刚连接的新用户
    newClient = tcpServer->nextPendingConnection();
    //连接标志
    clientJoinUp_check = true;

    //在客户端列表最后添加新的socket
    clients_checks[currentClient].client_socket = newClient;
    currentClient++;
    tcpSocket = newClient;

    //在对话框显示谁和我连接
    //获取对方IP和端口
    QString ip = tcpSocket->peerAddress().toString();
    qint16 port = tcpSocket->peerPort();
    QString temp = QString("[IP:%1:Port%2]:成功连接").arg(ip).arg(port);

    ui->textRead->append(temp);

    //接收消息
    connect(tcpSocket,&QTcpSocket::readyRead,this,&Widget::readmessage);

}

void Widget::readmessage()
{
    //寻找发出者并新建套接字
    QTcpSocket *client = qobject_cast<QTcpSocket *>(sender());
    QString str;
    //从通信套接字中取出内容
    str = client->readAll();
    //添加到显示面板
    ui->textRead->append("收到消息:" + str);

    dealMessage(str,client);
}

void Widget::sendMessageToClient(QString str,QTcpSocket *client_tcp)
{
    client_tcp->write(str.toUtf8().data());
}



void Widget::dealMessage(QString str,QTcpSocket *client_tcp)
{
    switch (str[0].toLatin1() - '0')
    {
    case 0://注册
        dealSignUp(str,client_tcp);
        break;
    case 1://登录
        dealSignIn(str,client_tcp);
        break;
    case 2://断开连接
        currentClientChangeToFlase(client_tcp);
        break;
    case 3://申领精灵
        dealGetPoke(client_tcp);
        break;
    case 4://查看用户
        dealGetUser(client_tcp);
        break;
    case 5://查看徽章
        dealGetBadge(client_tcp);
        break;
    case 6://查看精灵
        dealCheakPoke(str,client_tcp);
        break;
    case 7://查看出战精灵列表
        dealCheakFightPoke(client_tcp);
        break;
    case 8://战斗信息
        break;

    }
}

void Widget::dealSignUp(QString str,QTcpSocket *client_tcp)
{
    //注册的字符格式是:0 name pass
    //以空格来分割字符串
    QString namestring = str.section(' ',1,1);
    QString passwordstring = str.section(' ',2,2);


    //检查用户名是否已经存在
    QString temname = NULL;
    QSqlQuery query;
    query.exec("use pokemon");
    query.exec( QString("select * from users where name = '%1';").arg(namestring));
    while(query.next())
    {
        temname = query.value("name").toString();
    }
    //如果不存在，即注册成功
    if(temname == NULL)
    {
        query.exec( QString("insert into users(name, password,allcount,wincount,"
                            "havePowerPoke,haveTankPoke,haveDefensePoke,haveQuickPoke) "
                            "values('%1','%2',%3,%4,%5,%6,%7,%8);")
                    .arg(namestring).arg(passwordstring).arg(0).arg(0).arg(0).arg(0).arg(0).arg(0));
        ui->textRead->append("用户：" + namestring + "注册成功！");
        //分配精灵
        QTime time;
        time= QTime::currentTime();
        qsrand(time.msec()+time.second()*1000);
        int n = qrand() % 4;    //产生4以内的随机数
        if(n != 0)//分配力量型精灵
        {
                query.exec(QString("update users set havePowerPoke = 1,namePowerPoke = 'Dali'"
                                   ",levelPowerPoke = %1,expPowerPoke = %2,attackPowerPoke = %3"
                                   ",defensePowerPoke = %4,hpPowerPoke = %5,attIntervalPowerPoke = %6"
                                   " where name = '%7';")
                           .arg(1).arg(0).arg(START_POWERPOKE_ATTACK).arg(START_NORMALPOKE_DEFENSE)
                           .arg(START_NORMALPOKE_HP).arg(START_NORMALPOKE_ATTINTERVAL)
                           .arg(namestring));
        }
        if(n != 1)//分配肉盾型精灵
        {
                query.exec(QString("update users set haveTankPoke = 1,nameTankPoke = 'Mango'"
                                   ",levelTankPoke = %1,expTankPoke = %2,attackTankPoke = %3"
                                   ",defenseTankPoke = %4,hpTankPoke = %5,attIntervalTankPoke = %6"
                                   " where name = '%7';")
                           .arg(1).arg(0).arg(START_NORMALPOKE_ATTACK).arg(START_NORMALPOKE_DEFENSE)
                           .arg(START_TANKPOKE_HP).arg(START_NORMALPOKE_ATTINTERVAL)
                           .arg(namestring));
        }
        if(n != 2)//分配防御型精灵
        {
                query.exec(QString("update users set haveDefensePoke = 1,nameDefensePoke = 'Tip'"
                                   ",levelDefensePoke = %1,expDefensePoke = %2,attackDefensePoke = %3"
                                   ",defenseDefensePoke = %4,hpDefensePoke = %5,attIntervalDefensePoke = %6"
                                   " where name = '%7';")
                           .arg(1).arg(0).arg(START_NORMALPOKE_ATTACK).arg(START_DEFENSEPOKE_DEFENSE)
                           .arg(START_NORMALPOKE_HP).arg(START_NORMALPOKE_ATTINTERVAL)
                           .arg(namestring));
        }
        if(n != 3)//分配敏捷型精灵
        {
                query.exec(QString("update users set haveQuickPoke = 1,nameQuickPoke = 'Biubiu'"
                                   ",levelQuickPoke = %1,expQuickPoke = %2,attackQuickPoke = %3"
                                   ",defenseQuickPoke = %4,hpQuickPoke = %5,attIntervalQuickPoke = %6"
                                   " where name = '%7';")
                           .arg(1).arg(0).arg(START_NORMALPOKE_ATTACK).arg(START_NORMALPOKE_DEFENSE)
                           .arg(START_NORMALPOKE_HP).arg(START_QUICKPOKE_ATTINTERVAL)
                           .arg(namestring));
        }
        sendMessageToClient("01",client_tcp);

    }//如果存在,即注册失败
    else
    {
        ui->textRead->append("用户：" + namestring + "注册失败");
        sendMessageToClient("00",client_tcp);
    }

}


void Widget::dealSignIn(QString str, QTcpSocket *client_tcp)
{
    //注册的字符格式是:1 name pass
    //以空格来分割字符串
    QString namestring = str.section(' ',1,1);
    QString passwordstring = str.section(' ',2,2);


    //检查用户名是否已经存在
    QString temname = NULL;
    QString tempass = NULL;
    QSqlQuery query;
    query.exec("use pokemon");
    query.exec( QString("select * from users where name = '%1';").arg(namestring));
    while(query.next())
    {
        temname = query.value("name").toString();
        tempass = query.value("password").toString();
    }
    //如果不存在,则登录失败 代号10
    if(temname == NULL)
    {
        ui->textRead->append("用户" + namestring + "试图登录，但是用户不存在");
        sendMessageToClient("10",client_tcp);
        return;
    }//如果存在
     //如果密码不正确 代号11
    else if(passwordstring != tempass)
    {
        ui->textRead->append("用户" + namestring + "试图登录，但是密码错误");
        sendMessageToClient("11",client_tcp);
        return;
    }//如果密码正确 代号12
    else
    {
        ui->textRead->append("用户" + namestring + "登录成功");
        emit clientSignIn(client_tcp);
        sendMessageToClient("12",client_tcp);
        clients_checks[currentClient - 1].namestring = namestring;
        qDebug() << clients_checks[currentClient - 1].namestring;
        return;
    }
}

void Widget::dealGetPoke(QTcpSocket *client_tcp)
{
    QString namestring = getNameFromTcp(client_tcp);
    QSqlQuery query;
    int havePowerPoke,haveTankPoke,haveDefensePoke,haveQuickPoke;
    query.exec("use pokemon");
    query.exec( QString("select * from users where name = '%1';").arg(namestring));
    while(query.next())
    {
        havePowerPoke = query.value("havePowerPoke").toInt();
        haveTankPoke = query.value("haveTankPoke").toInt();
        haveDefensePoke = query.value("haveDefensePoke").toInt();
        haveQuickPoke = query.value("haveQuickPoke").toInt();
    }
    qDebug() << havePowerPoke << haveTankPoke << haveDefensePoke << haveQuickPoke;
    if(havePowerPoke + haveTankPoke + haveDefensePoke + haveQuickPoke == 0)//没有精灵
    {
        QTime time;
        time= QTime::currentTime();
        qsrand(time.msec()+time.second()*1000);
        int n = qrand() % 4 + 1;    //产生1,2,3,4随机数
        if(n == 1)//分配力量型精灵
        {
                query.exec(QString("update users set havePowerPoke = 1,namePowerPoke = 'Dali'"
                                   ",levelPowerPoke = %1,expPowerPoke = %2,attackPowerPoke = %3"
                                   ",defensePowerPoke = %4,hpPowerPoke = %5,attIntervalPowerPoke = %6"
                                   " where name = '%7';")
                           .arg(1).arg(0).arg(START_POWERPOKE_ATTACK).arg(START_NORMALPOKE_DEFENSE)
                           .arg(START_NORMALPOKE_HP).arg(START_NORMALPOKE_ATTINTERVAL)
                           .arg(namestring));
                sendMessageToClient("31",client_tcp);
        }
        else if(n == 2)//分配肉盾型精灵
        {
                query.exec(QString("update users set haveTankPoke = 1,nameTankPoke = 'Mango'"
                                   ",levelTankPoke = %1,expTankPoke = %2,attackTankPoke = %3"
                                   ",defenseTankPoke = %4,hpTankPoke = %5,attIntervalTankPoke = %6"
                                   " where name = '%7';")
                           .arg(1).arg(0).arg(START_NORMALPOKE_ATTACK).arg(START_NORMALPOKE_DEFENSE)
                           .arg(START_TANKPOKE_HP).arg(START_NORMALPOKE_ATTINTERVAL)
                           .arg(namestring));
                sendMessageToClient("32",client_tcp);
        }
        else if(n == 3)//分配防御型精灵
        {
                query.exec(QString("update users set haveDefensePoke = 1,nameDefensePoke = 'Tip'"
                                   ",levelDefensePoke = %1,expDefensePoke = %2,attackDefensePoke = %3"
                                   ",defenseDefensePoke = %4,hpDefensePoke = %5,attIntervalDefensePoke = %6"
                                   " where name = '%7';")
                           .arg(1).arg(0).arg(START_NORMALPOKE_ATTACK).arg(START_DEFENSEPOKE_DEFENSE)
                           .arg(START_NORMALPOKE_HP).arg(START_NORMALPOKE_ATTINTERVAL)
                           .arg(namestring));
                sendMessageToClient("33",client_tcp);
        }
        else if(n == 4)//分配敏捷型精灵
        {
                query.exec(QString("update users set haveQuickPoke = 1,nameQuickPoke = 'Biubiu'"
                                   ",levelQuickPoke = %1,expQuickPoke = %2,attackQuickPoke = %3"
                                   ",defenseQuickPoke = %4,hpQuickPoke = %5,attIntervalQuickPoke = %6"
                                   " where name = '%7';")
                           .arg(1).arg(0).arg(START_NORMALPOKE_ATTACK).arg(START_NORMALPOKE_DEFENSE)
                           .arg(START_NORMALPOKE_HP).arg(START_QUICKPOKE_ATTINTERVAL)
                           .arg(namestring));
                sendMessageToClient("34",client_tcp);
        }

    }
    else
    {
        sendMessageToClient("30",client_tcp);
    }
}

QString Widget::getNameFromTcp(QTcpSocket *client_tcp)
{
    for(int i = 0;i < MAXCONNECT;i++)
    {
        if(client_tcp == clients_checks[i].client_socket)
        {
            return clients_checks[i].namestring;
        }
    }
    return NULL;
}

void Widget::dealGetUser(QTcpSocket *client_tcp)
{
    QString str;
    QString ratestr;
    for(int i = 0;i < currentClient;i++)
    {

        str = "4 user:" + clients_checks[i].namestring + " is ";
        if(clients_checks[i].isSignIn == false)
        {
            str += "offline ";
        }
        else
            str += "online ";
        str += "rate:" + QString::number(getRateFromDatabase(clients_checks[i].namestring) * 100,'f',1) + ";";
        sendMessageToClient(str,client_tcp);
    }
}

float Widget::getRateFromDatabase(QString name)
{
    QSqlQuery query;
    int wincount = 0;
    int allcount = 0;
    float rate = 0;
    query.exec("use pokemon");
    query.exec( QString("select * from users where name = '%1';").arg(name));
    while(query.next())
    {
        allcount = query.value("allcount").toInt();
        wincount = query.value("wincount").toInt();
        qDebug() << allcount << wincount;
    }
    if(wincount == 0 || allcount == 0)
        return 0;
    else
    {
        rate = (float)wincount / (float)allcount;
        qDebug() << rate;
        return rate;
    }

}

void Widget::dealGetBadge(QTcpSocket *client_tcp)
{
    int havePowerPoke,haveTankPoke,haveDefensePoke,haveQuickPoke;
    int levelPowerPoke,levelTankPoke,levelDefensePoke,levelQuickPoke;
    int x=0,y=0;
    QString str = NULL;
    QSqlQuery query;
    query.exec("use pokemon");
    query.exec( QString("select * from users where name = '%1';").arg(getNameFromTcp(client_tcp)));
    while(query.next())
    {
        havePowerPoke = query.value("havePowerPoke").toInt();
        haveTankPoke = query.value("haveTankPoke").toInt();
        haveDefensePoke = query.value("haveDefensePoke").toInt();
        haveQuickPoke = query.value("haveQuickPoke").toInt();

        levelPowerPoke = query.value("levelPowerPoke").toInt();
        levelTankPoke = query.value("levelTankPoke").toInt();
        levelDefensePoke = query.value("levelDefensePoke").toInt();
        levelQuickPoke = query.value("levelQuickPoke").toInt();
    }
    x = havePowerPoke + haveTankPoke + haveDefensePoke + haveQuickPoke;
    if(x == 0)//x只有1,2,3,4这四种情况
        x++;
    x--;//改变为x只有0,1,2,3这四种情况
    if(levelPowerPoke == 15)
        y++;
    if(levelTankPoke == 15)
        y++;
    if(levelDefensePoke == 15)
        y++;
    if(levelQuickPoke == 15)
        y++;
    if(y == 0)
        y++;
    y--;//同上 改变为y只有0,1,2,3这四种情况
    //返回5XY中   宠物个数徽章:无铜银金X=0,1,2,3 宠物等级徽章:无铜银金Y=0,1,2,3
    str = "5" + QString::number(x) + QString::number(y);
    sendMessageToClient(str,client_tcp);
}

void Widget::dealCheakPoke(QString str,QTcpSocket *client_tcp)
{
    QString namestring = str.section(' ',1,1);
    qDebug() << namestring;
    if(namestring == NULL)
        namestring = getNameFromTcp(client_tcp);

    int havePowerPoke,haveTankPoke,haveDefensePoke,haveQuickPoke;
    QString namePowerPoke,nameTankPoke,nameDefensePoke,nameQuickPoke;
    int levelPowerPoke,levelTankPoke,levelDefensePoke,levelQuickPoke;
    int expPowerPoke,expTankPoke,expDefensePoke,expQuickPoke;
    int attackPowerPoke,attackTankPoke,attackDefensePoke,attackQuickPoke;
    int defensePowerPoke,defenseTankPoke,defenseDefensePoke,defenseQuickPoke;
    int hpPowerPoke,hpTankPoke,hpDefensePoke,hpQuickPoke;
    int attIntervalPowerPoke,attIntervalTankPoke,attIntervalDefensePoke,attIntervalQuickPoke;

    QString str1 = NULL;
    QString str2 = NULL;
    QString str3 = NULL;
    QString str4 = NULL;
    QSqlQuery query;
    query.exec("use pokemon");
    query.exec( QString("select * from users where name = '%1';").arg(namestring));
    while(query.next())
    {
        havePowerPoke = query.value("havePowerPoke").toInt();
        haveTankPoke = query.value("haveTankPoke").toInt();
        haveDefensePoke = query.value("haveDefensePoke").toInt();
        haveQuickPoke = query.value("haveQuickPoke").toInt();

        namePowerPoke = query.value("namePowerPoke").toString();
        nameTankPoke = query.value("nameTankPoke").toString();
        nameDefensePoke = query.value("nameDefensePoke").toString();
        nameQuickPoke = query.value("nameQuickPoke").toString();

        levelPowerPoke = query.value("levelPowerPoke").toInt();
        levelTankPoke = query.value("levelTankPoke").toInt();
        levelDefensePoke = query.value("levelDefensePoke").toInt();
        levelQuickPoke = query.value("levelQuickPoke").toInt();

        expPowerPoke = query.value("expPowerPoke").toInt();
        expTankPoke = query.value("expTankPoke").toInt();
        expDefensePoke = query.value("expDefensePoke").toInt();
        expQuickPoke = query.value("expQuickPoke").toInt();

        attackPowerPoke = query.value("attackPowerPoke").toInt();
        attackTankPoke = query.value("attackTankPoke").toInt();
        attackDefensePoke = query.value("attackDefensePoke").toInt();
        attackQuickPoke = query.value("attackQuickPoke").toInt();

        defensePowerPoke = query.value("defensePowerPoke").toInt();
        defenseTankPoke = query.value("defenseTankPoke").toInt();
        defenseDefensePoke = query.value("defenseDefensePoke").toInt();
        defenseQuickPoke = query.value("defenseQuickPoke").toInt();

        hpPowerPoke = query.value("hpPowerPoke").toInt();
        hpTankPoke = query.value("hpTankPoke").toInt();
        hpDefensePoke = query.value("hpDefensePoke").toInt();
        hpQuickPoke = query.value("hpQuickPoke").toInt();

        attIntervalPowerPoke = query.value("attIntervalPowerPoke").toInt();
        attIntervalTankPoke = query.value("attIntervalTankPoke").toInt();
        attIntervalDefensePoke = query.value("attIntervalDefensePoke").toInt();
        attIntervalQuickPoke = query.value("attIntervalQuickPoke").toInt();
    }
    //回信格式：
    //6 which have name level exp attack defense hp attInterval
    //6 int int QString int int int int int int
    //which中
    //0代表power  1代表tank  2代表defense  3代表quick
    str1 =  "6 0 "
            + QString::number(havePowerPoke) + " "
            + namePowerPoke + " "
            + QString::number(levelPowerPoke) + " "
            + QString::number(expPowerPoke) + " "
            + QString::number(attackPowerPoke) + " "
            + QString::number(defensePowerPoke) + " "
            + QString::number(hpPowerPoke) + " "
            + QString::number(attIntervalPowerPoke);
    str2 =  ";6 1 "
            + QString::number(haveTankPoke) + " "
            + nameTankPoke + " "
            + QString::number(levelTankPoke) + " "
            + QString::number(expTankPoke) + " "
            + QString::number(attackTankPoke) + " "
            + QString::number(defenseTankPoke) + " "
            + QString::number(hpTankPoke) + " "
            + QString::number(attIntervalTankPoke);
    str3 =  ";6 2 "
            + QString::number(haveDefensePoke) + " "
            + nameDefensePoke + " "
            + QString::number(levelDefensePoke) + " "
            + QString::number(expDefensePoke) + " "
            + QString::number(attackDefensePoke) + " "
            + QString::number(defenseDefensePoke) + " "
            + QString::number(hpDefensePoke) + " "
            + QString::number(attIntervalDefensePoke);
    str4 =  ";6 3 "
            + QString::number(haveQuickPoke) + " "
            + nameQuickPoke + " "
            + QString::number(levelQuickPoke) + " "
            + QString::number(expQuickPoke) + " "
            + QString::number(attackQuickPoke) + " "
            + QString::number(defenseQuickPoke) + " "
            + QString::number(hpQuickPoke) + " "
            + QString::number(attIntervalQuickPoke);
    sendMessageToClient(str1 + str2 + str3 + str4,client_tcp);

}


void Widget::dealCheakFightPoke(QTcpSocket *client_tcp)
{
    QString namestring = getNameFromTcp(client_tcp);
    int havePowerPoke,haveTankPoke,haveDefensePoke,haveQuickPoke;
    QString namePowerPoke,nameTankPoke,nameDefensePoke,nameQuickPoke;
    int levelPowerPoke,levelTankPoke,levelDefensePoke,levelQuickPoke;
    int expPowerPoke,expTankPoke,expDefensePoke,expQuickPoke;
    int attackPowerPoke,attackTankPoke,attackDefensePoke,attackQuickPoke;
    int defensePowerPoke,defenseTankPoke,defenseDefensePoke,defenseQuickPoke;
    int hpPowerPoke,hpTankPoke,hpDefensePoke,hpQuickPoke;
    int attIntervalPowerPoke,attIntervalTankPoke,attIntervalDefensePoke,attIntervalQuickPoke;

    QString str1 = NULL;
    QString str2 = NULL;
    QString str3 = NULL;
    QString str4 = NULL;
    QSqlQuery query;
    query.exec("use pokemon");
    query.exec( QString("select * from users where name = '%1';").arg(namestring));
    while(query.next())
    {
        havePowerPoke = query.value("havePowerPoke").toInt();
        haveTankPoke = query.value("haveTankPoke").toInt();
        haveDefensePoke = query.value("haveDefensePoke").toInt();
        haveQuickPoke = query.value("haveQuickPoke").toInt();

        namePowerPoke = query.value("namePowerPoke").toString();
        nameTankPoke = query.value("nameTankPoke").toString();
        nameDefensePoke = query.value("nameDefensePoke").toString();
        nameQuickPoke = query.value("nameQuickPoke").toString();

        levelPowerPoke = query.value("levelPowerPoke").toInt();
        levelTankPoke = query.value("levelTankPoke").toInt();
        levelDefensePoke = query.value("levelDefensePoke").toInt();
        levelQuickPoke = query.value("levelQuickPoke").toInt();

        expPowerPoke = query.value("expPowerPoke").toInt();
        expTankPoke = query.value("expTankPoke").toInt();
        expDefensePoke = query.value("expDefensePoke").toInt();
        expQuickPoke = query.value("expQuickPoke").toInt();

        attackPowerPoke = query.value("attackPowerPoke").toInt();
        attackTankPoke = query.value("attackTankPoke").toInt();
        attackDefensePoke = query.value("attackDefensePoke").toInt();
        attackQuickPoke = query.value("attackQuickPoke").toInt();

        defensePowerPoke = query.value("defensePowerPoke").toInt();
        defenseTankPoke = query.value("defenseTankPoke").toInt();
        defenseDefensePoke = query.value("defenseDefensePoke").toInt();
        defenseQuickPoke = query.value("defenseQuickPoke").toInt();

        hpPowerPoke = query.value("hpPowerPoke").toInt();
        hpTankPoke = query.value("hpTankPoke").toInt();
        hpDefensePoke = query.value("hpDefensePoke").toInt();
        hpQuickPoke = query.value("hpQuickPoke").toInt();

        attIntervalPowerPoke = query.value("attIntervalPowerPoke").toInt();
        attIntervalTankPoke = query.value("attIntervalTankPoke").toInt();
        attIntervalDefensePoke = query.value("attIntervalDefensePoke").toInt();
        attIntervalQuickPoke = query.value("attIntervalQuickPoke").toInt();
    }
    //回信格式：
    //7 which have name level exp attack defense hp attInterval
    //7 int int QString int int int int int int
    //which中
    //0代表power  1代表tank  2代表defense  3代表quick
    str1 =  "7 0 "
            + QString::number(havePowerPoke) + " "
            + namePowerPoke + " "
            + QString::number(levelPowerPoke) + " "
            + QString::number(expPowerPoke) + " "
            + QString::number(attackPowerPoke) + " "
            + QString::number(defensePowerPoke) + " "
            + QString::number(hpPowerPoke) + " "
            + QString::number(attIntervalPowerPoke);
    str2 =  ";7 1 "
            + QString::number(haveTankPoke) + " "
            + nameTankPoke + " "
            + QString::number(levelTankPoke) + " "
            + QString::number(expTankPoke) + " "
            + QString::number(attackTankPoke) + " "
            + QString::number(defenseTankPoke) + " "
            + QString::number(hpTankPoke) + " "
            + QString::number(attIntervalTankPoke);
    str3 =  ";7 2 "
            + QString::number(haveDefensePoke) + " "
            + nameDefensePoke + " "
            + QString::number(levelDefensePoke) + " "
            + QString::number(expDefensePoke) + " "
            + QString::number(attackDefensePoke) + " "
            + QString::number(defenseDefensePoke) + " "
            + QString::number(hpDefensePoke) + " "
            + QString::number(attIntervalDefensePoke);
    str4 =  ";7 3 "
            + QString::number(haveQuickPoke) + " "
            + nameQuickPoke + " "
            + QString::number(levelQuickPoke) + " "
            + QString::number(expQuickPoke) + " "
            + QString::number(attackQuickPoke) + " "
            + QString::number(defenseQuickPoke) + " "
            + QString::number(hpQuickPoke) + " "
            + QString::number(attIntervalQuickPoke);
    sendMessageToClient(str1 + str2 + str3 + str4,client_tcp);
}


void Widget::dealFightResult(QString str,QTcpSocket *client_tcp)
{

    //送信格式
    //胜利8 isWin whichgame isLevel isGet myNum enemyNum
    //失败8 isWin whichgame 0 isLose myNum enemyNum
    int isWin = str.section(' ',1,1).toInt();
    int whichgame = str.section(' ',2,2).toInt();
    int isLevel = 0;
    int isGet = 0;
    int enemyNum;
    int isLose = 0;
    int myNum;
    QString myPokeStr;

    myNum = str.section(' ',5,5).toInt();
    enemyNum = str.section(' ',6,6).toInt();
    switch (myNum) {
    case 0:
        myPokeStr = "PowerPoke";
        break;
    case 1:
        myPokeStr = "TankPoke";
        break;
    case 2:
        myPokeStr = "DefensePoke";
        break;
    case 3:
        myPokeStr = "QuickPoke";
    }

    QString namestring = getNameFromTcp(client_tcp);
    QSqlQuery query;
    query.exec("use pokemon");
    query.exec( QString("select * from users where name = '%1';").arg(namestring));
    int currentPowerLevel,currentTankLevel,currentDefenseLevel,currentQuickLevel;
    while(query.next())
    {
        currentPowerLevel = query.value("levelPowerPoke").toInt();
        currentTankLevel = query.value("levelTankPoke").toInt();
        currentDefenseLevel = query.value("levelDefensePoke").toInt();
        currentQuickLevel = query.value("levelQuickPoke").toInt();
    }





    if(isWin)//胜利
    {
        isLevel = str.section(' ',3,3).toInt();
        if(isLevel)//升级
        {
            if(myNum == 0)
            {
                if(currentPowerLevel < 15)
                {
                    query.exec(QString("update users set level%1 = %2,exp%3 = %4;")
                            .arg(myPokeStr).arg(currentPowerLevel + 1).arg(myPokeStr).arg(0));
                }
                else
                {
                    query.exec(QString("update users set level%1 = %2,exp%3 = %4;")
                            .arg(myPokeStr).arg(currentPowerLevel).arg(myPokeStr).arg(0));
                }
            }
            else if(myNum == 1)
            {
                if(currentTankLevel < 15)
                {
                    query.exec(QString("update users set level%1 = %2,exp%3 = %4;")
                            .arg(myPokeStr).arg(currentTankLevel + 1).arg(myPokeStr).arg(0));
                }
                else
                {
                    query.exec(QString("update users set level%1 = %2,exp%3 = %4;")
                            .arg(myPokeStr).arg(currentTankLevel).arg(myPokeStr).arg(0));
                }
            }
            else if(myNum == 2)
            {
                if(currentDefenseLevel < 15)
                {
                    query.exec(QString("update users set level%1 = %2,exp%3 = %4;")
                            .arg(myPokeStr).arg(currentDefenseLevel + 1).arg(myPokeStr).arg(0));
                }
                else
                {
                    query.exec(QString("update users set level%1 = %2,exp%3 = %4;")
                            .arg(myPokeStr).arg(currentDefenseLevel).arg(myPokeStr).arg(0));
                }
            }
            else
            {
                if(currentQuickLevel < 15)
                {
                    query.exec(QString("update users set level%1 = %2,exp%3 = %4;")
                            .arg(myPokeStr).arg(currentQuickLevel + 1).arg(myPokeStr).arg(0));
                }
                else
                {
                    query.exec(QString("update users set level%1 = %2,exp%3 = %4;")
                            .arg(myPokeStr).arg(currentQuickLevel).arg(myPokeStr).arg(0));
                }
            }

        }
        else//获取经验
        {
                query.exec(QString("update users set exp%1 = %2;")
                    .arg(myPokeStr).arg(50));
        }


        if(whichgame == 0)//决斗赛
        {
            isGet = str.section(' ',4,4).toInt();
            if(isGet == 1)//能够领取
            {


                if(enemyNum == 0)//分配力量型精灵
                {
                        query.exec(QString("update users set havePowerPoke = 1,namePowerPoke = 'Dali'"
                                           ",levelPowerPoke = %1,expPowerPoke = %2,attackPowerPoke = %3"
                                           ",defensePowerPoke = %4,hpPowerPoke = %5,attIntervalPowerPoke = %6"
                                           " where name = '%7';")
                                   .arg(1).arg(0).arg(START_POWERPOKE_ATTACK).arg(START_NORMALPOKE_DEFENSE)
                                   .arg(START_NORMALPOKE_HP).arg(START_NORMALPOKE_ATTINTERVAL)
                                   .arg(namestring));
                }
                else if(enemyNum == 1)//分配肉盾型精灵
                {
                        query.exec(QString("update users set haveTankPoke = 1,nameTankPoke = 'Mango'"
                                           ",levelTankPoke = %1,expTankPoke = %2,attackTankPoke = %3"
                                           ",defenseTankPoke = %4,hpTankPoke = %5,attIntervalTankPoke = %6"
                                           " where name = '%7';")
                                   .arg(1).arg(0).arg(START_NORMALPOKE_ATTACK).arg(START_NORMALPOKE_DEFENSE)
                                   .arg(START_TANKPOKE_HP).arg(START_NORMALPOKE_ATTINTERVAL)
                                   .arg(namestring));
                }
                else if(enemyNum == 2)//分配防御型精灵
                {
                        query.exec(QString("update users set haveDefensePoke = 1,nameDefensePoke = 'Tip'"
                                           ",levelDefensePoke = %1,expDefensePoke = %2,attackDefensePoke = %3"
                                           ",defenseDefensePoke = %4,hpDefensePoke = %5,attIntervalDefensePoke = %6"
                                           " where name = '%7';")
                                   .arg(1).arg(0).arg(START_NORMALPOKE_ATTACK).arg(START_DEFENSEPOKE_DEFENSE)
                                   .arg(START_NORMALPOKE_HP).arg(START_NORMALPOKE_ATTINTERVAL)
                                   .arg(namestring));
                }
                else if(enemyNum == 5)//分配敏捷型精灵
                {
                        query.exec(QString("update users set haveQuickPoke = 1,nameQuickPoke = 'Biubiu'"
                                           ",levelQuickPoke = %1,expQuickPoke = %2,attackQuickPoke = %3"
                                           ",defenseQuickPoke = %4,hpQuickPoke = %5,attIntervalQuickPoke = %6"
                                           " where name = '%7';")
                                   .arg(1).arg(0).arg(START_NORMALPOKE_ATTACK).arg(START_NORMALPOKE_DEFENSE)
                                   .arg(START_NORMALPOKE_HP).arg(START_QUICKPOKE_ATTINTERVAL)
                                   .arg(namestring));
                }

            }
        }
    }
    else//失败
    {
        isLose = str.section(' ',4,4).toInt();
        if(isLose)//失去精灵
            query.exec(QString("update users set have%1 = 0;").arg(myPokeStr));
    }


}
