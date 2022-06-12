#include "view.h"
#include "mainwindow.h"
#include <QDialog>
#include <QGridLayout>
#include <fstream>
#include <sstream>

view::view(QWidget *parent) :
    QDialog(parent)
{

    QGridLayout *l = new QGridLayout(this);

    info1 = new QLabel;
    info2 = new QLabel;
    info3 = new QLabel;
    info4 = new QLabel;
    info5 = new QLabel;
    info6 = new QLabel;
    info7 = new QLabel;
    add = new QPushButton;


    info1->setStyleSheet(QString("font-size: %1px; font-weight: bold").arg(30));
    info1->setAlignment(Qt::AlignCenter);
    info2->setStyleSheet(QString("font-size: %1px").arg(25));
    info3->setStyleSheet(QString("font-size: %1px").arg(25));
    info4->setStyleSheet(QString("font-size: %1px").arg(20));
    info5->setStyleSheet(QString("font-size: %1px").arg(20));
    info6->setStyleSheet(QString("font-size: %1px").arg(20));
    info7->setStyleSheet(QString("font-size: %1px").arg(20));
    add->setStyleSheet(QString("font-size: %1px").arg(25));

    connect(add, SIGNAL(clicked()), this, SLOT(change()));


    QHBoxLayout *box = new QHBoxLayout();
    box->addWidget(info2);
    box->addWidget(info3);
    box->addStretch();
    l->addWidget(info1);
    l->addWidget(info4);
    l->addWidget(info5);
    l->addWidget(info6);
    l->addWidget(info7);
    l->addLayout(box, 5, 0);
    l->addWidget(add);

}

void view::change()
{
    if(check)
    {
        add->setStyleSheet("background-color: rgb(255,255,255);");
        add->setText("Add to Favourite");
        check = false;
        emit send_delete(a);
    }
    else
    {
        add->setStyleSheet("background-color: rgb(255,255,0);");
        add->setText("Delete from Favourite");
        check = true;
        emit send_add(a);
    }
}

void view::recieveData(Comp& air, bool& flag)
{
    a = air;
    info1->setText((air.data[0].toString().toStdString() + '&' + air.data[2].toString().toStdString()).c_str());
    std::string s = air.data[1].toString().toStdString();   
    info2->setText(("<a href=\"" + s + "\">Profile url</a>").c_str());
    info2->setTextFormat(Qt::RichText);
    info2->setTextInteractionFlags(Qt::TextBrowserInteraction);
    info2->setOpenExternalLinks(true);   
    s = air.data[3].toString().toStdString();
    info3->setText(("<a href=\"" + s + "\">Url</a>").c_str());
    info3->setTextFormat(Qt::RichText);
    info3->setTextInteractionFlags(Qt::TextBrowserInteraction);
    info3->setOpenExternalLinks(true);
    info4->setText(("State: " + air.data[4].toString().toStdString()).c_str());
    info5->setText(("Revenur: " + air.data[5].toString().toStdString()).c_str());
    info6->setText(("Sphere of work: " + air.data[6].toString().toStdString()).c_str());
    info7->setText(("Amount of Workers: " + air.data[7].toString().toStdString()).c_str());
    check = flag;
    if(check)
    {
        add->setStyleSheet("background-color: rgb(255,255,0);");
        add->setText("Delete from Favourite");
    }
    else
    {
        add->setStyleSheet("background-color: rgb(255,255,255);");
        add->setText("Add to Favourite");
    }
}

view::~view()
{
}
