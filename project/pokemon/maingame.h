#ifndef MAINGAME_H
#define MAINGAME_H

#include <QCloseEvent>
#include <QWidget>
#include <QPainter>

namespace Ui {
class MainGame;
}

class MainGame : public QWidget
{
    Q_OBJECT

public:
    explicit MainGame(QWidget *parent = nullptr);
    ~MainGame();
    QString getLineName();//获取名字输入行信息

signals:
    void closeMainGame();//关闭窗口信号
    void cheakGetPokeSignal();//按钮->获取精灵信号
    void cheakUserListSignal();//按钮->查看用户信号
    void cheakMyBadgeSignal();//按钮->查看我的徽章
    void cheakMyPokeSignal();//按钮->查看我的精灵
    void enterDuelSignal(int which_game);//按钮->进入决斗赛 0
    void enterUpgradeSignal(int which_game);//按钮->进入升级赛 1


protected:
    void closeEvent(QCloseEvent *event);//游戏窗口关闭时触发的函数
    //重写绘图虚函数
    void paintEvent(QPaintEvent *);
private slots:


    void on_buttonGetPoke_clicked();

    void on_buttonUserList_clicked();

    void on_buttonMyBadge_clicked();

    void on_buttonMyPoke_clicked();

    void on_buttonDuel_clicked();

    void on_buttonUpgrade_clicked();

private:
    Ui::MainGame *ui;
};





#endif // MAINGAME_H
