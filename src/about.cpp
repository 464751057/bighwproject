#include "about.h"
#include "ui_about.h"
#include <QPainter>
#include <QLayout>

about::about(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::about)
{
    ui->setupUi(this);
    this->setFixedSize(this->width(),this->height());

}

about::~about()
{
    delete ui;
}

void about::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    painter.setPen({QColorConstants::Svg::red, 20});
    painter.setBrush(QBrush(QColorConstants::Svg::white));
    painter.translate(width()/2,height()/2);
    painter.drawEllipse(-175,-250,350,350);
    painter.setPen({QColorConstants::Svg::red, 40});
    painter.drawLine(0, -150, 0, 40);
    painter.setPen({QColorConstants::Svg::red, 20});
    painter.drawLine(-25,-170, 25, -160);
    painter.drawLine(-25, 50, 25, 60);
    painter.setPen({QColorConstants::Svg::red, 40});
    painter.setBrush(QBrush(QColorConstants::Svg::red));
    painter.drawEllipse(-5,-210,10,10);

}
