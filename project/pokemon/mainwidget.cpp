#include "mainwidget.h"
#include "ui_mainwidget.h"

//本界面负责与服务器通信以及登录注册

MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainWidget)
{
    ui->setupUi(this);

    setWindowTitle("登录");
    //标题图片
    ui->labelTitleImage->setPixmap(QPixmap(":/images/images/title.jpg"));
    //自动适配
    ui->labelTitleImage->setScaledContents(true);

    //连接至服务器
    connectToServer();



    //连接成功
    connect(tcpsocket,&QTcpSocket::connected,
            [=]()
            {
                qDebug() << "和服务器连接成功！";
            }

            );

    //获取消息
    connect(tcpsocket,&QTcpSocket::readyRead,this,&MainWidget::receiveMessageFromServer);

    //连接断开
    connect(tcpsocket,&QTcpSocket::disconnected,
            [=]()
            {
                //断开连接时警告
                QMessageBox::information(this,"不好","与服务器断开连接！");
            }
            );

    //登录成功信号绑定
    connect(this,&MainWidget::signInSuccess,this,&MainWidget::enterGameWidget);
    //游戏窗口关闭信号绑定
    connect(&gameWidget,&MainGame::closeMainGame,this,&MainWidget::closeMainGameDeal);
    //领取宠物信号绑定
    connect(&gameWidget,&MainGame::cheakGetPokeSignal,this,&MainWidget::getPokeDeal);
    //查看用户信号绑定
    connect(&gameWidget,&MainGame::cheakUserListSignal,this,&MainWidget::getUserList);
    //查看徽章信号绑定
    connect(&gameWidget,&MainGame::cheakMyBadgeSignal,this,&MainWidget::getMyBadge);
    //查看精灵信号绑定
    connect(&gameWidget,&MainGame::cheakMyPokeSignal,this,&MainWidget::getMyPoke);
    //进入决斗赛信号绑定 0
    connect(&gameWidget,&MainGame::enterDuelSignal,this,&MainWidget::enterChooseWidget);
    //进入升级赛信号绑定 1
    connect(&gameWidget,&MainGame::enterUpgradeSignal,this,&MainWidget::enterChooseWidget);
    //进入比赛界面信号绑定
    connect(&chooseWidget,&ChooseWidget::decideFightPoke,this,&MainWidget::enterBattleWidget);
}

MainWidget::~MainWidget()
{
    delete ui;
}

void MainWidget::connectToServer()
{
    //初始化对象
    tcpsocket = new QTcpSocket(this);
    //获取ip和端口
    QString ip = ui->lineIP->text();
    qint16 port = ui->linePort->text().toInt();
    //连接服务器
    tcpsocket->connectToHost(QHostAddress(ip),port);

}

void MainWidget::on_checkShowPass_stateChanged(int arg1)
{
    if( ui->checkShowPass->isChecked() )//如果按下则显示密码
        ui->linePassword->setEchoMode(QLineEdit::Normal);
    else
        ui->linePassword->setEchoMode(QLineEdit::Password);
}


//发送信息函数
void MainWidget::sendMessageToServer(QString message)
{
    if(tcpsocket == NULL)
    {
        QMessageBox::information(this,"不好","还未建立通信，无法发送信息！");
        return;
    }

    tcpsocket->write(message.toUtf8().data());
}





//注册向服务器发送信号
void MainWidget::on_buttonSignup_clicked()
{
    //提取框内用户名和密码
    QString namestring = ui->lineName->text();
    QString passwordstring = ui->linePassword->text();

    //检查是否为空串
    if(namestring == NULL||passwordstring == NULL)
    {
        QMessageBox::information(this,"不好","用户名或密码不能为空！");
        return;
    }

    //检查是否有中文
    if(namestring.contains(QRegExp("[\\x4e00-\\x9fa5]+")) ||
       passwordstring.contains(QRegExp("[\\x4e00-\\x9fa5]+")))
    {
       QMessageBox::information(this,"不好","账号或者密码里面不能包含中文！");
       return;
    }

    //检查账号、密码位数是否合格
    if(namestring.length() < 6 || namestring.length() > 12)
    {
        QMessageBox::information(this,"不好","用户名要在6-12个字符间！");
        return;
    }
    if(passwordstring.length() < 4 || passwordstring.length() > 10)
    {
        QMessageBox::information(this,"不好","密码要在4-10个字符间！");
        return;
    }

    sendMessageToServer("0 " + namestring + " " + passwordstring);
}

void MainWidget::receiveMessageFromServer()
{
    QString str;
    //从通信套接字中取出内容
    str = tcpsocket->readAll();
    qDebug() << str;
    dealMessage(str);
}

void MainWidget::dealMessage(QString str)
{
    switch (str[0].toLatin1() - '0')
    {
    case 0:   //注册反馈
        dealSignUpBack(str);;
        break;
    case 1:   //登录反馈
        dealSignInBack(str);
        break;
    case 2:   //断开连接（无需要）
        break;
    case 3:   //申领精灵反馈
        dealGetPokeBack(str);
        break;
    case 4:   //查看用户反馈
        dealGetUserListBack(str);
        break;
    case 5:   //查看徽章反馈
        dealGetMyBadgeBack(str);
        break;
    case 6:   //查看精灵反馈
        dealCheakMyPokeBack(str);
        break;
    case 7:
        dealCheakFightPokeBack(str);
        break;


    }
}

void MainWidget::dealSignUpBack(QString str)
{
    switch (str[1].toLatin1() - '0')
    {
    case 1:        //注册成功
        QMessageBox::information(this,"好啊","注册成功！");
        qDebug() << "注册成功";
        break;
    case 0:        //注册失败
        QMessageBox::information(this,"不好","用户名已存在，注册失败");
        qDebug() << "注册失败";
        break;
    }
}

void MainWidget::dealSignInBack(QString str)
{
    switch (str[1].toLatin1() - '0')
    {
    case 0:         //用户不存在
        QMessageBox::information(this,"不好","用户不存在");
        break;
    case 1:         //密码不正确
        QMessageBox::information(this,"不好","密码不正确");
        break;
    case 2:         //登录成功
        QMessageBox::information(this,"好啊","登录成功！");
        //??????????此处登录成功后信息
        emit signInSuccess();
        break;
    }
}

void MainWidget::on_buttonLogin_clicked()
{
    //提取框内用户名和密码
    QString namestring = ui->lineName->text();
    QString passwordstring = ui->linePassword->text();

    //检查是否为空串
    if(namestring == NULL||passwordstring == NULL)
    {
        QMessageBox::information(this,"不好","用户名或密码不能为空！");
        return;
    }

    //检查是否有中文
    if(namestring.contains(QRegExp("[\\x4e00-\\x9fa5]+")) ||
       passwordstring.contains(QRegExp("[\\x4e00-\\x9fa5]+")))
    {
       QMessageBox::information(this,"不好","账号或者密码里面不能包含中文！");
       return;
    }

    //检查账号、密码位数是否合格
    if(namestring.length() < 6 || namestring.length() > 12)
    {
        QMessageBox::information(this,"不好","用户名要在6-12个字符间！");
        return;
    }
    if(passwordstring.length() < 4 || passwordstring.length() > 10)
    {
        QMessageBox::information(this,"不好","密码要在4-10个字符间！");
        return;
    }

    sendMessageToServer("1 " + namestring + " " + passwordstring);
}


void MainWidget::enterGameWidget()
{
    this->hide();
    gameWidget.show();

}

void MainWidget::closeMainGameDeal()
{
    sendMessageToServer("2");
    tcpsocket->disconnectFromHost();
    tcpsocket->close();
    this->close();
}

void MainWidget::getPokeDeal()
{
    sendMessageToServer("3");
}

void MainWidget::dealGetPokeBack(QString str)
{
    switch (str[1].toLatin1() - '0')
    {
    case 0:         //申领失败
        QMessageBox::information(&this->gameWidget,"不好","精灵数量不为0，领取失败！");
        break;
    case 1:         //申领力量型
        QMessageBox::information(&this->gameWidget,"好啊","力量型领取成功！");
        break;
    case 2:         //申领肉盾型
        QMessageBox::information(&this->gameWidget,"好啊","肉盾型领取成功！");
        break;
    case 3:         //申领防御型
        QMessageBox::information(&this->gameWidget,"好啊","防御型领取成功！");
        break;
    case 4:         //申领敏捷型
        QMessageBox::information(&this->gameWidget,"好啊","敏捷型领取成功！");
        break;
    default:        //未知错误
        QMessageBox::information(&this->gameWidget,"不好","由于未知原因领取失败！");
        break;
    }
}

void MainWidget::getUserList()
{
    sendMessageToServer("4");
}

void MainWidget::dealGetUserListBack(QString str)
{
    userWidget.show();
    //用户列表查看格式
    //4 user:name is Xline rate:float%;
    userWidget.setStringToTextShow(str);
}

void MainWidget::getMyBadge()
{
    sendMessageToServer("5");
}

void MainWidget::dealGetMyBadgeBack(QString str)
{
    //返回5XY中   宠物个数徽章:无铜银金X=0,1,2,3 宠物等级徽章:无铜银金Y=0,1,2,3
    badgeWidget.setNumBadge(str[1].toLatin1() - '0');
    badgeWidget.setLevBadge(str[2].toLatin1() - '0');
    badgeWidget.show();
}

void MainWidget::getMyPoke()
{
    QString str ="6 " + gameWidget.getLineName();
    qDebug() << str;
    sendMessageToServer(str);
}

void MainWidget::dealCheakMyPokeBack(QString str1)
{
    //回信格式：
    //6 which have name level exp attack defense hp attInterval
    //6 int int QString int int int int int int
    //which中
    //0代表power  1代表tank  2代表defense  3代表quick
    //以空格来分割字符串
    int i;
    for(i = 0;i < 4;i++)
    {
        QString str = str1.section(';',i,i);
        int which_poke = str.section(' ',1,1).toInt();
        int have = str.section(' ',2,2).toInt();
        QString name = str.section(' ',3,3);
        int level = str.section(' ',4,4).toInt();
        int exp = str.section(' ',5,5).toInt();
        int attack = str.section(' ',6,6).toInt();
        int defense = str.section(' ',7,7).toInt();
        int hp = str.section(' ',8,8).toInt();
        int attInterval = str.section(' ',9,9).toInt();

        pokeWidget.setLabel(which_poke,have,name,level,exp,attack,defense,hp,attInterval);
    }
    pokeWidget.show();
}

void MainWidget::enterChooseWidget(int which_game)
{
    whichgame = which_game;
    chooseWidget.setTitle(which_game);//！！！！！！！！！！！！！进入选择页面
    sendMessageToServer("7");
}

void MainWidget::dealCheakFightPokeBack(QString str1)
{
    int i;
    for(i = 0;i < 4;i++)
    {
        QString str = str1.section(';',i,i);
        int which_poke = str.section(' ',1,1).toInt();

        myPoke[which_poke].setHave(str.section(' ',2,2).toInt());
        //int have = str.section(' ',2,2).toInt();

        switch (i) {
        case 0:
            myPoke[which_poke].setType(powerPoke);
            break;
        case 1:
            myPoke[which_poke].setType(tankPoke);
            break;
        case 2:
            myPoke[which_poke].setType(defensePoke);
            break;
        case 3:
            myPoke[which_poke].setType(quickPoke);
            break;
        default:
            qDebug() << "分配精灵类型出错！";
        }

        myPoke[which_poke].setName(str.section(' ',3,3));
        //QString name = str.section(' ',3,3);

        myPoke[which_poke].setLevel(str.section(' ',4,4).toInt());
        //int level = str.section(' ',4,4).toInt();

        myPoke[which_poke].setExp(str.section(' ',5,5).toInt());
        //int exp = str.section(' ',5,5).toInt();

        myPoke[which_poke].setAttack(str.section(' ',6,6).toInt());
        //int attack = str.section(' ',6,6).toInt();

        myPoke[which_poke].setDefense(str.section(' ',7,7).toInt());
        //int defense = str.section(' ',7,7).toInt();

        myPoke[which_poke].setHp(str.section(' ',8,8).toInt());
        //int hp = str.section(' ',8,8).toInt();

        myPoke[which_poke].setAttInterval(str.section(' ',9,9).toInt());
        //int attInterval = str.section(' ',9,9).toInt();

    }
    qDebug() << "zheli";
    chooseWidget.setCombox(myPoke[0].getHave(),myPoke[1].getHave(),myPoke[1].getHave(),myPoke[2].getHave());
    chooseWidget.show();
}

void MainWidget::enterBattleWidget(int myNum,int enemyNum)
{
    bool isWin= true;
    enemyPoke.setName("enemy");

    enemyPoke.setExp(0);
    switch(enemyNum)
    {
    case 0://0级力量系
    {
        qDebug() << "敌人是0级力量系";
        //QString m_name,int m_level,int m_exp,int m_attack,int m_defense,int m_hp,int m_attInterval
        enemyPoke.setType(powerPoke);
        enemyPoke.setLevel(0);
        enemyPoke.setAttack(20);
        enemyPoke.setDefense(0);
        enemyPoke.setHp(80);
        enemyPoke.setAttInterval(50);
        break;
    }
    case 1://5级肉盾系
    {
        qDebug() << "敌人是5级肉盾系";
        //PokemonBase *enemyPoke = new PokemonPower("enemy",5,0,35,8,100,50);
        enemyPoke.setType(tankPoke);
        enemyPoke.setLevel(5);
        enemyPoke.setAttack(35);
        enemyPoke.setDefense(8);
        enemyPoke.setHp(100);
        enemyPoke.setAttInterval(50);
        break;
    }
    case 2://10级防御系
    {
        qDebug() << "敌人是10级防御系";
        //PokemonBase *enemyPoke = new PokemonPower("enemy",10,0,50,16,120,50);
        enemyPoke.setType(defensePoke);
        enemyPoke.setLevel(10);
        enemyPoke.setAttack(50);
        enemyPoke.setDefense(16);
        enemyPoke.setHp(120);
        enemyPoke.setAttInterval(50);
        break;
    }
    case 3://15级敏捷系
    {
        qDebug() << "敌人是15级敏捷系";
        //PokemonBase *enemyPoke = new PokemonPower("enemy",15,0,65,24,140,50);
        enemyPoke.setType(quickPoke);
        enemyPoke.setLevel(15);
        enemyPoke.setAttack(65);
        enemyPoke.setDefense(24);
        enemyPoke.setHp(140);
        enemyPoke.setAttInterval(50);
        break;
    }
    default://出错
        qDebug() << "进入战斗页面时获取敌人精灵类型出错";

    }
    isWin = battleWidget.fightFuc(&myPoke[myNum],&enemyPoke);
    battleWidget.show();
    dealWinOrLose(isWin,myNum);
//    if(isWin)
//        QMessageBox::information(this,"好啊","赢了！");
//    else
//        QMessageBox::information(this,"不好","输力！");
}

void MainWidget::dealWinOrLose(bool isWin,int myNum)
{
    int isLevelUp = 0;
    int isGet = 0;
    int isLose = 0;
    QString str;
    //发送信息格式：
    //胜利8 isWin whichgame isLevel isGet enemyNum
    //失败8 isWin whichgame 0 isLose myNum
    if(isWin)//战斗胜利
    {
        if(myPoke[myNum].getExp() >= 50)//能升级
        {
            isLevelUp = 1;
            battleWidget.setText("精灵" + myPoke[myNum].getName() + "升级啦！");
        }
        if(whichgame == 0)//决斗赛
        {
            if((enemyPoke.getType() == powerPoke   && myPoke[0].getHave() == 0) ||
               (enemyPoke.getType() == tankPoke    && myPoke[1].getHave() == 0) ||
               (enemyPoke.getType() == defensePoke && myPoke[2].getHave() == 0) ||
               (enemyPoke.getType() == quickPoke   && myPoke[3].getHave() == 0)
              )
            {
                isGet = 1;
                battleWidget.setText("获得敌方力量型精灵！");
            }
            else
                battleWidget.setText("已经拥有对方精灵，无法获得！");
        }
        str = "8 1 " + QString::number(whichgame) + " " + QString::number(isLevelUp) +
              " " + QString::number(isGet) + " " + enemyPoke.getType();
        sendMessageToServer(str);
    }
    else//战斗失败
    {
        if(whichgame == 0)//决斗赛
        {
            isLose = 1;
            battleWidget.setText("失去当前精灵！");
        }
        str = "8 0 " + QString::number(whichgame) + " 0 " + QString::number(isLose) + " " + myPoke[myNum].getType();
        sendMessageToServer(str);
    }
}
