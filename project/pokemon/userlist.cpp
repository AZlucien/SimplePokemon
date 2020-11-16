#include "userlist.h"
#include "ui_userlist.h"

UserList::UserList(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UserList)
{
    ui->setupUi(this);
    setWindowTitle("所有用户");

}

UserList::~UserList()
{
    delete ui;
}

void UserList::setStringToTextShow(QString str)
{
    ui->textShow->append(str.mid(2));
}
