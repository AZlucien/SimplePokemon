#ifndef USERLIST_H
#define USERLIST_H

#include <QWidget>

namespace Ui {
class UserList;
}

class UserList : public QWidget
{
    Q_OBJECT

public:
    explicit UserList(QWidget *parent = nullptr);
    ~UserList();
    void setStringToTextShow(QString str);
private:
    Ui::UserList *ui;
};

#endif // USERLIST_H
