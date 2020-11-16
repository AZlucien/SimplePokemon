#include "pokelist.h"
#include "ui_pokelist.h"

PokeList::PokeList(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PokeList)
{
    ui->setupUi(this);
    setWindowTitle("我的精灵");

    ui->labelImagePowerPoke->setPixmap(QPixmap(":/images/images/powerPoke.jpg"));
    ui->labelImageTankPoke->setPixmap(QPixmap(":/images/images/tankPoke.jpg"));
    ui->labelImageDefensePoke->setPixmap(QPixmap(":/images/images/defensePoke.jpg"));
    ui->labelImageQuickPoke->setPixmap(QPixmap(":/images/images/quickPoke.jpg"));

    ui->labelImagePowerPoke->setScaledContents(true);
    ui->labelImageTankPoke->setScaledContents(true);
    ui->labelImageDefensePoke->setScaledContents(true);
    ui->labelImageQuickPoke->setScaledContents(true);


}

PokeList::~PokeList()
{
    delete ui;
}

void PokeList::setLabel(
int which_poke, int have,QString name,int level,int exp,int attack,int defense,int hp,int attInterval)
{


    if(which_poke == 0)//power
    {
        if(have == 1)
        {
            ui->labelHavePowerPoke->setText("已拥有");
            ui->labelNamePowerPoke->setText(name);
            ui->labelLevelPowerPoke->setText(QString::number(level));
            ui->labelExpPowerPoke->setText(QString::number(exp));
            ui->labelAttackPowerPoke->setText(QString::number(attack));
            ui->labelDefensePowerPoke->setText(QString::number(defense));
            ui->labelHpPowerPoke->setText(QString::number(hp));
            ui->labelAttIntervalPowerPoke->setText(QString::number(attInterval));
        }
        else
        {
            ui->labelHavePowerPoke->setText("未拥有");
            ui->labelNamePowerPoke->setText("NULL");
            ui->labelLevelPowerPoke->setText("NULL");
            ui->labelExpPowerPoke->setText("NULL");
            ui->labelAttackPowerPoke->setText("NULL");
            ui->labelDefensePowerPoke->setText("NULL");
            ui->labelHpPowerPoke->setText("NULL");
            ui->labelAttIntervalPowerPoke->setText("NULL");
        }
    }
    else if(which_poke == 1)//tank
    {
        if(have == 1)
        {
            ui->labelHaveTankPoke->setText("已拥有");
            ui->labelNameTankPoke->setText(name);
            ui->labelLevelTankPoke->setText(QString::number(level));
            ui->labelExpTankPoke->setText(QString::number(exp));
            ui->labelAttackTankPoke->setText(QString::number(attack));
            ui->labelDefenseTankPoke->setText(QString::number(defense));
            ui->labelHpTankPoke->setText(QString::number(hp));
            ui->labelAttIntervalTankPoke->setText(QString::number(attInterval));
        }
        else
        {
            ui->labelHaveTankPoke->setText("未拥有");
            ui->labelNameTankPoke->setText("NULL");
            ui->labelLevelTankPoke->setText("NULL");
            ui->labelExpTankPoke->setText("NULL");
            ui->labelAttackTankPoke->setText("NULL");
            ui->labelDefenseTankPoke->setText("NULL");
            ui->labelHpTankPoke->setText("NULL");
            ui->labelAttIntervalTankPoke->setText("NULL");
        }
    }
    else if(which_poke == 2)//defense
    {
        if(have == 1)
        {
            ui->labelHaveDefensePoke->setText("已拥有");
            ui->labelNameDefensePoke->setText(name);
            ui->labelLevelDefensePoke->setText(QString::number(level));
            ui->labelExpDefensePoke->setText(QString::number(exp));
            ui->labelAttackDefensePoke->setText(QString::number(attack));
            ui->labelDefenseDefensePoke->setText(QString::number(defense));
            ui->labelHpDefensePoke->setText(QString::number(hp));
            ui->labelAttIntervalDefensePoke->setText(QString::number(attInterval));
        }
        else
        {
            ui->labelHaveDefensePoke->setText("未拥有");
            ui->labelNameDefensePoke->setText("NULL");
            ui->labelLevelDefensePoke->setText("NULL");
            ui->labelExpDefensePoke->setText("NULL");
            ui->labelAttackDefensePoke->setText("NULL");
            ui->labelDefenseDefensePoke->setText("NULL");
            ui->labelHpDefensePoke->setText("NULL");
            ui->labelAttIntervalDefensePoke->setText("NULL");
        }
    }
    else if(which_poke == 3)//quick
    {
        if(have == 1)
        {
            ui->labelHaveQuickPoke->setText("已拥有");
            ui->labelNameQuickPoke->setText(name);
            ui->labelLevelQuickPoke->setText(QString::number(level));
            ui->labelExpQuickPoke->setText(QString::number(exp));
            ui->labelAttackQuickPoke->setText(QString::number(attack));
            ui->labelDefenseQuickPoke->setText(QString::number(defense));
            ui->labelHpQuickPoke->setText(QString::number(hp));
            ui->labelAttIntervalQuickPoke->setText(QString::number(attInterval));
        }
        else
        {
            ui->labelHaveQuickPoke->setText("未拥有");
            ui->labelNameQuickPoke->setText("NULL");
            ui->labelLevelQuickPoke->setText("NULL");
            ui->labelExpQuickPoke->setText("NULL");
            ui->labelAttackQuickPoke->setText("NULL");
            ui->labelDefenseQuickPoke->setText("NULL");
            ui->labelHpQuickPoke->setText("NULL");
            ui->labelAttIntervalQuickPoke->setText("NULL");
        }
    }
}
