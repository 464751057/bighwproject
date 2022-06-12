#include "favourite.h"
#include "mainwindow.h"
#include <QDialog>
#include <QGridLayout>
#include <QMenuBar>
#include <QFileDialog>
#include <fstream>
#include <sstream>
#include <QDebug>

favourite::favourite(QWidget *parent) :
    QWidget(parent)
{
    fav_model = new FavModel();
    fav_view = new QTableView(this);

    QGridLayout *l = new QGridLayout(this);

    fav_view->setModel(fav_model);

    connect(fav_view, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(info(QModelIndex)));

    l->addWidget(fav_view);
}

void favourite::info(const QModelIndex & ind)
{
    FavItem res;
    int row = ind.row();
    QModelIndex index;
    std::string s;
    for(int i = 0; i < 2; i++) {
        index = fav_view->model()->index(row,i,QModelIndex());
        s = index.data().toString().toStdString();
        res.data[i] = QString(s.c_str());
    }
    emit send_data(res);
}

favourite::~favourite()
{
}

QVariant FavModel::data (const QModelIndex &index, int role) const
{
   if (role == Qt::DisplayRole || role == Qt::EditRole)
   {
       const FavItem &current_fav = fav_data[index.row ()];
       return current_fav.data[index.column ()];
   }
   return {};
}


const char *enum_to_string_cart (fav_fields field)
{
    switch (field)
    {
        case fav_fields::rank      : return "Rank";
        case fav_fields::name     : return "Name";
        case fav_fields::COUNT     : return "";
    }
    return {};
}

QVariant FavModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        fav_fields field = (fav_fields)section;
        return enum_to_string_cart(field);
    }
    return {};
}
