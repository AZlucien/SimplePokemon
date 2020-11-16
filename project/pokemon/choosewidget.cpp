#include "choosewidget.h"
#include "ui_choosewidget.h"

ChooseWidget::ChooseWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChooseWidget)
{
    ui->setupUi(this);
    setWindowTitle("选择出战精灵");
}

ChooseWidget::~ChooseWidget()
{
    delete ui;
}

void ChooseWidget::setTitle(int which_game)
{
    if(which_game == 0)//决斗赛
        ui->labelTitle->setText("决斗赛");
    else
        ui->labelTitle->setText("升级赛");
}

 void ChooseWidget::setCombox(int isHavePower,int isHaveTank,int isHaveDefense,int isHaveQuick)
 {
    qDebug() << isHavePower << isHaveTank << isHaveDefense << isHaveQuick;
    if(isHavePower == 1)
        ui->boxMyPoke->addItem("0 力量型 Dali");
    if(isHaveTank == 1)
        ui->boxMyPoke->addItem("1 肉盾型 Mango");
    if(isHaveDefense == 1)
        ui->boxMyPoke->addItem("2 防御型 Tip");
    if(isHaveQuick == 1)
        ui->boxMyPoke->addItem("3 敏捷型 Biubiu");
 }

void ChooseWidget::on_buttonDecide_clicked()
{
    int enemyNum = ui->boxEnemyPoke->currentIndex();
    QString str = ui->boxMyPoke->currentText();
    int myNum = str[0].toLatin1() - '0';
    emit decideFightPoke(myNum,enemyNum);
    for(int i = 0;i < 4;i++)
        ui->boxMyPoke->clear();
    this->hide();
}
