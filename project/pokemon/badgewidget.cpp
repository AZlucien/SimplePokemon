#include "badgewidget.h"
#include "ui_badgewidget.h"

BadgeWidget::BadgeWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BadgeWidget)
{
    ui->setupUi(this);
    setWindowTitle("我的徽章");
}

BadgeWidget::~BadgeWidget()
{
    delete ui;
}

void BadgeWidget::setNumBadge(int num)
{
    switch (num)
    {
    case 0:
        ui->labelImageNum->setText("无");
        break;
    case 1:
        ui->labelImageNum->setPixmap(QPixmap(":/images/images/copper.jpg"));
        break;
    case 2:
        ui->labelImageNum->setPixmap(QPixmap(":/images/images/sliver.jpg"));
        break;
    case 3:
        ui->labelImageNum->setPixmap(QPixmap(":/images/images/gold.jpg"));
        break;
    default:
        ui->labelImageNum->setText("出错！");

    }
    ui->labelImageNum->setScaledContents(true);

}

void BadgeWidget::setLevBadge(int num)
{
    switch (num)
    {
    case 0:
        ui->labelImageLev->setText("无");
        break;
    case 1:
        ui->labelImageLev->setPixmap(QPixmap(":/images/images/copper.jpg"));
        break;
    case 2:
        ui->labelImageLev->setPixmap(QPixmap(":/images/images/sliver.jpg"));
        break;
    case 3:
        ui->labelImageLev->setPixmap(QPixmap(":/images/images/gold.jpg"));
        break;
    default:
        ui->labelImageLev->setText("出错！");
    }
    ui->labelImageLev->setScaledContents(true);
}
