#ifndef POKELIST_H
#define POKELIST_H

#include <QWidget>

namespace Ui {
class PokeList;
}

class PokeList : public QWidget
{
    Q_OBJECT

public:
    explicit PokeList(QWidget *parent = nullptr);
    ~PokeList();
    void setLabel(int,int,QString,int,int,int,int,int,int);

private:
    Ui::PokeList *ui;
};

#endif // POKELIST_H
