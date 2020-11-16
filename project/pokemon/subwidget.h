#ifndef SUBWIDGET_H
#define SUBWIDGET_H

#include <QWidget>

namespace Ui {
class SubWidget;
}

class SubWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SubWidget(QWidget *parent = nullptr);
    ~SubWidget();

private:
    Ui::SubWidget *ui;
};

#endif // SUBWIDGET_H
