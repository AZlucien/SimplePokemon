#include "battlewidget.h"
#include "ui_battlewidget.h"

BattleWidget::BattleWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BattleWidget)
{
    ui->setupUi(this);
}

BattleWidget::~BattleWidget()
{
    delete ui;
}

bool BattleWidget::fightFuc(PokemonBase *myPoke,PokemonBase *enemyPoke)
{


//    QString name_poke;      //名称
//    pokemonType type_poke;  //类别
//    int isHave = 0;         //是否拥有
//    int level;              //等级
//    int exp;                //经验值
//    int attack;             //攻击力
//    int defense;            //防御力
//    int hp;                 //生命值
//    int attInterval;        //攻击间隔
    ui->textBrowser->append("我方精灵信息:");
    ui->textBrowser->append("名字："+ myPoke->getName());
    switch (myPoke->getType()) {
    case powerPoke:
        ui->textBrowser->append("类别：力量型");
        break;
    case tankPoke:
        ui->textBrowser->append("类别：肉盾型");
        break;
    case defensePoke:
        ui->textBrowser->append("类别：防御型");
        break;
    case quickPoke:
        ui->textBrowser->append("类别：敏捷型");
        break;
    default:
        break;
    }
    ui->textBrowser->append("等级：" + QString::number(myPoke->getLevel()) + " 经验值:" + QString::number(myPoke->getExp()) +
                            " 攻击力:" + QString::number(myPoke->getAttack()) + " 防御力:" + QString::number(myPoke->getDefense()) +
                            " 生命值:" + QString::number(myPoke->getHp()) + " 攻击间隔:" + QString::number(myPoke->getAttInterval()));

    ui->textBrowser->append("敌方精灵信息:");
    ui->textBrowser->append("名字："+ enemyPoke->getName());
    switch (enemyPoke->getType()) {
    case powerPoke:
        ui->textBrowser->append("类别：力量型");
        break;
    case tankPoke:
        ui->textBrowser->append("类别：肉盾型");
        break;
    case defensePoke:
        ui->textBrowser->append("类别：防御型");
        break;
    case quickPoke:
        ui->textBrowser->append("类别：敏捷型");
        break;
    default:
        break;
    }
    ui->textBrowser->append("等级：" + QString::number(enemyPoke->getLevel()) + " 经验值:" + QString::number(enemyPoke->getExp()) +
                            " 攻击力:" + QString::number(enemyPoke->getAttack()) + " 防御力:" + QString::number(enemyPoke->getDefense()) +
                            " 生命值:" + QString::number(enemyPoke->getHp()) + " 攻击间隔:" + QString::number(enemyPoke->getAttInterval()));
    int round,myPokeHp,enemyPokeHp,temHp;
    QTime time;
    int n;


    //我方精灵先手
    for(round = 0,temHp = 0,myPokeHp = myPoke->getHp(),enemyPokeHp = enemyPoke->getHp();round < 100;round++)
    {
        time= QTime::currentTime();
        qsrand(time.msec()+time.second()*1000 + round);
        n = qrand() % 10;    //产生0-9随机数


        switch (n)
        {
        case 0://暴击
            temHp = (myPoke->getAttack() - enemyPoke->getDefense()) * 2;
            enemyPokeHp -= temHp;
            ui->textBrowser->append(myPoke->getName() + "触发暴击，造成了" + QString::number(temHp) + "点伤害！" + enemyPoke->getName() +
                                "还剩下" + QString::number(enemyPokeHp) + "点血量");
            break;
        case 1://技能
            if(myPoke->getType() == powerPoke)//力量型
            {
                temHp = (int)(myPoke->getAttack() * 1.5);
                enemyPokeHp -= temHp;
                ui->textBrowser->append(myPoke->getName() + "发动技能 大力出奇迹，造成了" + QString::number(temHp) + "点伤害！" + enemyPoke->getName() +
                                    "还剩下" + QString::number(enemyPokeHp) + "点血量");
                break;
            }
            else if(myPoke->getType() == tankPoke)//肉盾型
            {
                temHp = myPoke->getAttack() + myPoke->getHp() / 3;
                enemyPokeHp -= temHp;
                ui->textBrowser->append(myPoke->getName() + "发动技能 肉蛋冲击，造成了" + QString::number(temHp) + "点伤害！" + enemyPoke->getName() +
                                    "还剩下" + QString::number(enemyPokeHp) + "点血量");
                break;
            }
            else if(myPoke->getType() == defensePoke)//防御型
            {
                temHp = myPoke->getAttack() + myPoke->getDefense();
                enemyPokeHp -= temHp;
                ui->textBrowser->append(myPoke->getName() + "发动技能 攻防合一，造成了" + QString::number(temHp) + "点伤害！" + enemyPoke->getName() +
                                    "还剩下" + QString::number(enemyPokeHp) + "点血量");
                break;
            }
            else//敏捷型
            {
                temHp = (myPoke->getAttack() - enemyPoke->getDefense()) * 3;
                enemyPokeHp -= temHp;
                ui->textBrowser->append(myPoke->getName() + "发动技能 疯狂攻击，造成了" + QString::number(temHp) + "点伤害！" + enemyPoke->getName() +
                                    "还剩下" + QString::number(enemyPokeHp) + "点血量");
                break;
            }
            break;
        case 2://闪避情况
            temHp = 0;
            enemyPokeHp -= temHp;
            ui->textBrowser->append(myPoke->getName() + "攻击被闪避，造成了" + QString::number(temHp) + "点伤害！" + enemyPoke->getName() +
                                "还剩下" + QString::number(enemyPokeHp) + "点血量");
            break;
        default://正常情况
            temHp = myPoke->getAttack() - enemyPoke->getDefense();
            enemyPokeHp -= temHp;
            ui->textBrowser->append(myPoke->getName() + "造成了" + QString::number(temHp) + "点伤害！" + enemyPoke->getName() +
                                "还剩下" + QString::number(enemyPokeHp) + "点血量");
            break;
        }





        if(enemyPokeHp <= 0)
        {
            ui->textBrowser->append("我方精灵胜利！");
            return true;
        }

        //******************************************************敌方回合*************************************************
        time= QTime::currentTime();
        qsrand(time.msec()+time.second()*1000 + round * 2);
        n = qrand() % 10;    //产生0-9随机数


        switch (n)
        {
        case 0://暴击
            temHp = (enemyPoke->getAttack() - myPoke->getDefense()) * 2;
            myPokeHp -= temHp;
            ui->textBrowser->append(enemyPoke->getName() + "触发暴击，造成了" + QString::number(temHp) + "点伤害！" + myPoke->getName() +
                                "还剩下" + QString::number(myPokeHp) + "点血量");
            break;
        case 1://技能
            if(enemyPoke->getType() == powerPoke)//力量型
            {
                temHp = (int)(myPoke->getAttack() * 1.5);
                myPokeHp -= temHp;
                ui->textBrowser->append(enemyPoke->getName() + "发动技能 大力出奇迹，造成了" + QString::number(temHp) + "点伤害！" + myPoke->getName() +
                                    "还剩下" + QString::number(myPokeHp) + "点血量");
                break;
            }
            else if(enemyPoke->getType() == tankPoke)//肉盾型
            {
                temHp = enemyPoke->getAttack() + enemyPoke->getHp() / 3;
                myPokeHp -= temHp;
                ui->textBrowser->append(enemyPoke->getName() + "发动技能 肉蛋冲击，造成了" + QString::number(temHp) + "点伤害！" + myPoke->getName() +
                                    "还剩下" + QString::number(myPokeHp) + "点血量");
                break;
            }
            else if(enemyPoke->getType() == defensePoke)//防御型
            {
                temHp = enemyPoke->getAttack() + enemyPoke->getDefense();
                myPokeHp -= temHp;
                ui->textBrowser->append(enemyPoke->getName() + "发动技能 攻防合一，造成了" + QString::number(temHp) + "点伤害！" + myPoke->getName() +
                                    "还剩下" + QString::number(myPokeHp) + "点血量");
                break;
            }
            else//敏捷型
            {
                temHp = (enemyPoke->getAttack() - myPoke->getDefense()) * 3;
                myPokeHp -= temHp;
                ui->textBrowser->append(enemyPoke->getName() + "发动技能 疯狂攻击，造成了" + QString::number(temHp) + "点伤害！" + myPoke->getName() +
                                    "还剩下" + QString::number(myPokeHp) + "点血量");
                break;
            }
            break;
        case 2://闪避情况
            temHp = 0;
            myPokeHp -= temHp;
            ui->textBrowser->append(enemyPoke->getName() + "攻击被闪避，造成了" + QString::number(temHp) + "点伤害！" + myPoke->getName() +
                                "还剩下" + QString::number(myPokeHp) + "点血量");
            break;
        default://正常情况
            temHp = enemyPoke->getAttack() - myPoke->getDefense();
            myPokeHp -= temHp;
            ui->textBrowser->append(enemyPoke->getName() + "造成了" + QString::number(temHp) + "点伤害！" + myPoke->getName() +
                                "还剩下" + QString::number(myPokeHp) + "点血量");
            break;
        }

        if(myPokeHp <= 0)
        {
            ui->textBrowser->append("敌方精灵胜利！");
            return false;
        }
    }
    qDebug() << round;
}

void BattleWidget::setText(QString str)
{
    ui->textBrowser->append(str);
}
