#ifndef BADGEWIDGET_H
#define BADGEWIDGET_H

#include <QWidget>

namespace Ui {
class BadgeWidget;
}

class BadgeWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BadgeWidget(QWidget *parent = nullptr);
    ~BadgeWidget();
    void setNumBadge(int num);
    void setLevBadge(int num);

private:
    Ui::BadgeWidget *ui;
};

#endif // BADGEWIDGET_H
