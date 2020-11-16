#include "subwidget.h"
#include "ui_subwidget.h"

SubWidget::SubWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SubWidget)
{
    ui->setupUi(this);
}

SubWidget::~SubWidget()
{
    delete ui;
}
