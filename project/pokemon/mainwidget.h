#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include "choosewidget.h"
#include "battlewidget.h"
#include "badgewidget.h"
#include "userlist.h"
#include "maingame.h"
#include "pokelist.h"
#include "poke.h"
#include <QWidget>
#include <QTcpSocket>
#include <QDebug>
#include <QMessageBox>
#include <QHostAddress>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWidget; }
QT_END_NAMESPACE

class MainWidget : public QWidget
{
    Q_OBJECT
public:
    PokemonBase myPoke[4];
    PokemonBase enemyPoke;
    int whichgame = 1;              //决定的比赛类型 0->决斗赛 1->升级赛

public:
    MainGame gameWidget;            //登录成功显示的大厅页面
    UserList userWidget;            //查看用户的页面
    BadgeWidget badgeWidget;        //查看徽章的页面
    PokeList pokeWidget;            //查看精灵的页面
    ChooseWidget chooseWidget;      //选择出征的页面
    BattleWidget battleWidget;      //战斗的页面
public:
    MainWidget(QWidget *parent = nullptr);
    ~MainWidget();
    //**********************************************服务器通信******************************************************//
    void connectToServer();                   //连接服务器
    void sendMessageToServer(QString message);//向服务器发送信息
    void receiveMessageFromServer();          //从服务器收到信息的函数
    void dealMessage(QString str);            //处理服务器传来的信息
    //处理返回信息细分
    void dealSignUpBack(QString str);         //注册信息返回处理 0
    void dealSignInBack(QString str);         //登录信息返回处理 1
    void dealGetPokeBack(QString str);        //申领信息返回处理 3
    void dealGetUserListBack(QString str);    //查看用户信息返回处理 4
    void dealGetMyBadgeBack(QString str);     //查看徽章信息返回处理 5
    void dealCheakMyPokeBack(QString str);    //查看精灵信息返回处理 6
    void dealCheakFightPokeBack(QString str); //查看征战精灵信息返回处理 7


    //处理发送信息细分
    void enterGameWidget();                   //登陆成功进入游戏界面
    void closeMainGameDeal();                 //游戏界面关闭注销账号 2
    void getPokeDeal();                       //游戏界面领取精灵处理 3
    void getUserList();                       //游戏界面查看用户列表 4
    void getMyBadge();                        //游戏界面查看我的徽章 5
    void getMyPoke();                         //游戏界面查看我的精灵 6



    void enterChooseWidget(int which_game);   //进入选择页面
    void enterBattleWidget(int myNum,int enemyNum);//进入战斗页面
    void dealWinOrLose(bool isWin,int myNum);            //处理战斗胜利或失败
signals:
    void signInSuccess();                     //登录成功信号
    void toUserList(QString str);             //给用户页面信号
    void toBattlePoke(PokemonBase *,PokemonBase *);//给战斗界面两只精灵
private slots:
    void on_checkShowPass_stateChanged(int arg1);

    void on_buttonSignup_clicked();

    void on_buttonLogin_clicked();

private:
    Ui::MainWidget *ui;
    QTcpSocket *tcpsocket = NULL;
};
#endif // MAINWIDGET_H
