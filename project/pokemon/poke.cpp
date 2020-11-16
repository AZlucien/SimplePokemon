#include "poke.h"

Poke::Poke()
{

}
QString PokemonBase::getName()
{
    return this->name_poke;
}
pokemonType PokemonBase::getType()
{
    return this->type_poke;
}
int PokemonBase::getHave()
{
    return this->isHave;
}
int PokemonBase::getLevel()
{
    return this->level;
}
int PokemonBase::getAttack()
{
    return this->attack;
}
int PokemonBase::getDefense()
{
    return this->defense;
}
int PokemonBase::getHp()
{
    return this->hp;
}
int PokemonBase::getAttInterval()
{
    return this->attInterval;
}

int PokemonBase::getExp()
{
    return this->exp;
}


void PokemonBase::setHave(int have)
{
    isHave = have;
}
void PokemonBase::setExp(int newexp)
{
    exp = newexp;
}

void PokemonBase::setHp(int newHp)
{
    hp = newHp;
}

void PokemonBase::setName(QString newName)
{
    name_poke = newName;
}

void PokemonBase::setType(pokemonType newtype)
{
    type_poke = newtype;
}

void PokemonBase::setLevel(int newLevel)
{
    level = newLevel;
}

void PokemonBase::setAttack(int newAttack)
{
    attack = newAttack;
}

void PokemonBase::setDefense(int newDefense)
{
    defense = newDefense;
}

void PokemonBase::setAttInterval(int newAttInterval)
{
    attInterval = newAttInterval;
}



void PokemonBase::addHp()
{
    switch (type_poke)
    {
    case tankPoke : this->hp += ADD_TANKPOKE_HP;
    default: this->hp += ADD_NORMALPOKE_HP;
    }
}
void PokemonBase::addAttack()
{
    switch (type_poke)
    {
    case powerPoke : this->attack += ADD_POWERPOKE_ATTACK;
    default: this->attack += ADD_NORMALPOKE_ATTACK;
    }
}
void PokemonBase::addDefense()
{
    switch (type_poke)
    {
    case defensePoke : this->defense += ADD_DEFENSEPOKE_DEFENSE;
    default: this->defense += ADD_NORMALPOKE_DEFENSE;
    }
}
void PokemonBase::addAttInterval()
{
    switch (type_poke)
    {
    case quickPoke : this->attInterval += ADD_QUICKPOKE_ATTINTERVAL;
    default: this->attInterval += ADD_NORMALPOKE_ATTINTERVAL;
    }
}


void PokemonBase::addLevel()
{
    if(level < MAXLEVEL)
    {
        level++;
        addHp();
        addAttack();
        addDefense();
        addAttInterval();
    }
}

void PokemonBase::addExp(int addexp)
{
    if(exp + addexp >= MAXEXP)
    {
        addLevel();
        exp = 0;
    }
    else
    {
        exp += addexp;
    }
}


PokemonBase::PokemonBase(QString m_name,int m_level,int m_exp,int m_attack,int m_defense,int m_hp,int m_attInterval)
{
    setName(m_name);
    setLevel(m_level);
    setExp(m_exp);
    setAttack(m_attack);
    setDefense(m_defense);
    setHp(m_hp);
    setAttInterval(m_attInterval);

}

PokemonBase::PokemonBase(QString m_name,pokemonType m_type)
{
    setName(m_name);
    setType(m_type);
    setLevel(1);
    setExp(0);
    if(m_type == powerPoke)
        setAttack(START_POWERPOKE_ATTACK);
    else
        setAttack(START_NORMALPOKE_ATTACK);
    if(m_type == defensePoke)
        setDefense(START_DEFENSEPOKE_DEFENSE);
    else
        setDefense(START_NORMALPOKE_DEFENSE);
    if(m_type == tankPoke)
        setHp(START_TANKPOKE_HP);
    else
        setHp(START_NORMALPOKE_HP);
    if(m_type == quickPoke)
        setAttInterval(START_QUICKPOKE_ATTINTERVAL);
    else
        setAttInterval(START_NORMALPOKE_ATTINTERVAL);

}

PokemonBase::PokemonBase()
{

}

PokemonBase::~PokemonBase()
{

}

PokemonPower::PokemonPower()
{

}

PokemonPower::PokemonPower(QString m_name,int m_level,int m_exp,int m_attack,int m_defense,int m_hp,int m_attInterval):
    PokemonBase::PokemonBase(m_name,m_level,m_exp,m_attack,m_defense,m_hp,m_attInterval)
{
    setType(powerPoke);
}

PokemonTank::PokemonTank()
{

}

PokemonTank::PokemonTank(QString m_name,int m_level,int m_exp,int m_attack,int m_defense,int m_hp,int m_attInterval):
    PokemonBase::PokemonBase(m_name,m_level,m_exp,m_attack,m_defense,m_hp,m_attInterval)
{
    setType(tankPoke);
}

PokemonDefense::PokemonDefense()
{

}

PokemonDefense::PokemonDefense(QString m_name,int m_level,int m_exp,int m_attack,int m_defense,int m_hp,int m_attInterval):
    PokemonBase::PokemonBase(m_name,m_level,m_exp,m_attack,m_defense,m_hp,m_attInterval)
{
    setType(defensePoke);
}

PokemonQuick::PokemonQuick()
{

}

PokemonQuick::PokemonQuick(QString m_name,int m_level,int m_exp,int m_attack,int m_defense,int m_hp,int m_attInterval):
    PokemonBase::PokemonBase(m_name,m_level,m_exp,m_attack,m_defense,m_hp,m_attInterval)
{
    setType(quickPoke);
}



int PokemonPower::fightFight(PokemonBase *)
{
    return (this->getAttack() *1.5);
}

int PokemonTank::fightFight(PokemonBase *)
{
    return (this->getAttack() + this->getHp() / 3);
}

int PokemonDefense::fightFight(PokemonBase *)
{
    return (this->getAttack() + this->getDefense());
}

int PokemonQuick::fightFight(PokemonBase *)
{
    return (this->getAttack() * 2);
}
