#ifndef VIEW_H
#define VIEW_H
#include "mainwindow.h"
#include <QDialog>
#include <QPushButton>
#include <QLabel>


class view : public QDialog
{
    Q_OBJECT
    QLabel *info1;
    QLabel *info2;
    QLabel *info3;
    QLabel *info4;
    QLabel *info5;
    QLabel *info6;
    QLabel *info7;
    QPushButton *add;

public:
    explicit view(QWidget *parent = nullptr);
    ~view();
    bool check = false;
    Comp a;

signals:
    void update_cart_sum();
    void send_delete(Comp& air);
    void send_add(Comp& air);

private slots:
    void recieveData(Comp& air, bool& flag);
    void change();

};

#endif // VIEW_H
