#ifndef CHOOSEWIDGET_H
#define CHOOSEWIDGET_H

#include <QWidget>
#include <QDebug>
namespace Ui {
class ChooseWidget;
}

class ChooseWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ChooseWidget(QWidget *parent = nullptr);
    ~ChooseWidget();
    void setTitle(int which_game);
    void setCombox(int isHavePower,int isHaveTank,int isHaveDefense,int isHaveQuick);

signals:
    void decideFightPoke(int,int);
private slots:
    void on_buttonDecide_clicked();


private:
    Ui::ChooseWidget *ui;
};

#endif // CHOOSEWIDGET_H
