#ifndef POKE_H
#define POKE_H
#include <QString>



#define MAXEXP 100
#define MAXLEVEL 15
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


//不同类型的宠物小精灵的初始值以及升级值
//初始等级level = 1 每次升一级 最大等级为15
//初始经验exp = 0 每次胜利获得20+等级*4的经验 到达100升级 无论是否溢出都归零
//初始攻击力attack30 普通每次升级增加5攻击力 最大值100 力量型初始值为38 每次升级增加8攻击力 最大值150
//初始防御值0 普通每次升级增加3防御力 最大值42 防御性初始值10 每次升级增加5防御力 最大值80
//初始血量100 普通每次升级增加10血量 最大值240 肉盾型初始值120 每次升级增加15点血量 最大值330
//初始敏捷50 普通每次升级减少1 最小值36 敏捷型初始值50 每次升级减少2 最小值22
//注 攻击间隔（s）即敏捷值除以20


enum pokemonType {powerPoke = 0,tankPoke = 1,defensePoke = 2,quickPoke = 3};

class Poke
{
public:
    Poke();
};

class PokemonBase
{

public:
    PokemonBase();
    //重载构造函数
    //自定义数值构造
    PokemonBase(QString m_name,int m_level,int m_exp,int m_attack,int m_defense,int m_hp,int m_attInterval);
    //初始数值构造
    PokemonBase(QString m_name,pokemonType m_type);
    ~PokemonBase();

public:
    //查看接口
    QString getName();
    pokemonType getType();
    int getHave();
    int getLevel();
    int getAttack();
    int getDefense();
    int getHp();
    int getAttInterval();
    int getExp();

    //设置接口
    void setName(QString newName);
    void setHave(int have);
    void setType(pokemonType newtype);
    void setLevel(int newLevel);
    void setAttack(int newAttack);
    void setDefense(int newDefense);
    void setHp(int newHp);
    void setAttInterval(int newAttInterval);
    void setExp(int newexp);
    //增加接口
    void addLevel();
    void addExp(int addexp);
    //这四类要根据类型决定
    void addAttack();
    void addDefense();
    void addHp();
    void addAttInterval();

    //独特的战斗技能函数
    virtual int fightFight(PokemonBase *){};

    //战斗函数
    bool fightWithPoke(PokemonBase *);
private:
    QString name_poke;      //名称
    pokemonType type_poke;  //类别
    int isHave = 0;         //是否拥有
    int level;              //等级
    int exp;                //经验值
    int attack;             //攻击力
    int defense;            //防御力
    int hp;                 //生命值
    int attInterval;        //攻击间隔
};

class PokemonPower : public PokemonBase
{
    public:
        //力量型特殊攻击 大力出奇迹
        //造成1.5倍攻击力
        PokemonPower();
        PokemonPower(QString m_name,int m_level,int m_exp,int m_attack,int m_defense,int m_hp,int m_attInterval);
        int fightFight(PokemonBase *);
};

class PokemonTank : public PokemonBase
{
    public:
        //肉盾型特殊攻击 肉蛋冲击
        //造成当前攻击力+血量三分之一
        int fightFight(PokemonBase *);
        PokemonTank();
        PokemonTank(QString m_name,int m_level,int m_exp,int m_attack,int m_defense,int m_hp,int m_attInterval);

};

class PokemonDefense : public PokemonBase
{
    public:
        //防御型特殊攻击 攻防合一
        //造成当前攻击力+防御力
        int fightFight(PokemonBase *);
        PokemonDefense();
        PokemonDefense(QString m_name,int m_level,int m_exp,int m_attack,int m_defense,int m_hp,int m_attInterval);

};

class PokemonQuick : public PokemonBase
{
    public:
        //敏捷型特殊攻击 连打
        //打两下 造成两倍伤害
        int fightFight(PokemonBase *);
        PokemonQuick();
        PokemonQuick(QString m_name,int m_level,int m_exp,int m_attack,int m_defense,int m_hp,int m_attInterval);

};
#endif // POKE_H
