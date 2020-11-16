#include "maingame.h"
#include "ui_maingame.h"

MainGame::MainGame(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainGame)
{
    ui->setupUi(this);
    setWindowTitle("大厅");



}

MainGame::~MainGame()
{
    delete ui;
}

void MainGame::closeEvent(QCloseEvent *event)
{
     //TODO: 在退出窗口之前，实现希望做的操作
    emit closeMainGame();
}


void MainGame::paintEvent(QPaintEvent *)
{

    //画背景图
    QPainter p(this);
    p.drawPixmap(rect(),QPixmap(":/images/images/backgr.jpg"));



}


void MainGame::on_buttonGetPoke_clicked()
{
    emit cheakGetPokeSignal();
}

void MainGame::on_buttonUserList_clicked()
{
    emit cheakUserListSignal();
}

void MainGame::on_buttonMyBadge_clicked()
{
    emit cheakMyBadgeSignal();
}

void MainGame::on_buttonMyPoke_clicked()
{
    emit cheakMyPokeSignal();
}

void MainGame::on_buttonDuel_clicked()
{

    emit enterDuelSignal(0);
}

void MainGame::on_buttonUpgrade_clicked()
{
    emit enterUpgradeSignal(1);
}

QString MainGame::getLineName()//获取名字输入行信息
{
    return ui->lineName->text();
}
