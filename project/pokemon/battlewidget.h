#ifndef BATTLEWIDGET_H
#define BATTLEWIDGET_H

#include <QTime>
#include <QWidget>
#include <QDebug>
#include "poke.h"
namespace Ui {
class BattleWidget;
}

class BattleWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BattleWidget(QWidget *parent = nullptr);
    ~BattleWidget();
    bool fightFuc(PokemonBase *,PokemonBase *);
    void setText(QString str);
private:
    Ui::BattleWidget *ui;
};

#endif // BATTLEWIDGET_H
