#include "mainwindow.h"
#include "view.h"
#include "about.h"
#include <QGridLayout>
#include <QMenuBar>
#include <QFileDialog>
#include <fstream>
#include <unordered_set>
#include <sstream>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QWidget *central_widget = new QWidget (this);
    comp_model = new CompModel();
    fav_window = new favourite;
    filter_model = new FilterModel(this, this);
    setCentralWidget(central_widget);
    comp_view = new QTableView(central_widget);

    QGridLayout *layout = new QGridLayout(central_widget);

    open_fav = new QPushButton();
    state_filter = new QLineEdit();
    indus_filter = new QLineEdit();
    search_ln = new QLineEdit();
    state_lb = new QLabel();
    indus_lb  = new QLabel();
    search_lb  = new QLabel();
    apply_filter = new QPushButton();
    search = new QPushButton();

    comp_view->setModel(filter_model);
    filter_model->setSourceModel(comp_model);
    filter_model->setSortCaseSensitivity(Qt::CaseInsensitive);

    comp_view->setColumnHidden(1, true);
    comp_view->setColumnHidden(3, true);
    comp_view->setColumnHidden(4, true);
    comp_view->setColumnHidden(5, true);
    comp_view->setColumnHidden(6, true);
    comp_view->setColumnHidden(7, true);

    open_fav->setText("Open Favourite");
    open_fav->setStyleSheet(QString("font-size: %1px").arg(25));
    state_lb->setText("Filter State");
    indus_lb->setText("Filter Industry");
    search_lb->setText("Search");
    apply_filter->setText("Apply Filters");
    search->setText("Search");

    connect(comp_view, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(get_info(QModelIndex)));
    connect(open_fav, SIGNAL(clicked()), this, SLOT(open_favourite()));
    connect(fav_window, SIGNAL(send_data(FavItem&)), this, SLOT(get_info_fav(FavItem&)));
    connect(apply_filter, SIGNAL(clicked()), this, SLOT(filter_data()));
    connect(search, SIGNAL(clicked()), this, SLOT(search_func()));

    QHBoxLayout *box = new QHBoxLayout();
    QVBoxLayout *box_ser = new QVBoxLayout();
    QVBoxLayout *box_st = new QVBoxLayout();
    QVBoxLayout *box_ind = new QVBoxLayout();
    QHBoxLayout *box_btn = new QHBoxLayout();
    box_ser->addWidget(search_lb);
    box_ser->addWidget(search_ln);
    box_st->addWidget(state_lb);
    box_st->addWidget(state_filter);
    box_ind->addWidget(indus_lb);
    box_ind->addWidget(indus_filter);
    box->addLayout(box_ser);
    box->addLayout(box_st);
    box->addLayout(box_ind);
    box_btn->addWidget(search);
    box_btn->addWidget(open_fav);
    box_btn->addWidget(apply_filter);
    layout->addLayout(box_btn, 0, 0);
    layout->addLayout(box, 1, 0);
    layout->addWidget(comp_view);

    auto file_menu = menuBar()->addMenu ("File");
    QAction *open_action = new QAction ("Open");
    connect (open_action, &QAction::triggered, this, &MainWindow::load_data_function);
    file_menu->addAction (open_action);

    QAction *saveas_action = new QAction ("Save As");
    connect (saveas_action, &QAction::triggered, this, &MainWindow::save_data_function);
    file_menu->addAction  (saveas_action);

    QAction *about = new QAction ("About");
    connect (about, &QAction::triggered, this, &MainWindow::about_func);
    menuBar()->addAction(about);

}

void MainWindow::search_func()
{
    QString s = search_ln->text();
    if(s.isNull()) //if there is no text it won't go throw all data
        return;
    for(Comp &item : comp_model->comp_data)
        if(item.data[2] == s)
        {
            bool flag = false;
            for(unsigned int i = 0; i < fav_window->fav_model->fav_data.size();i++)
            {
                if(fav_window->fav_model->fav_data[i].data[0] == item.data[0] && fav_window->fav_model->fav_data[i].data[1] == item.data[2])
                {
                    flag = true;
                    break;
                }
            }
            view v;
            connect(this, SIGNAL(sendData(Comp&, bool&)), &v, SLOT(recieveData(Comp&, bool&)));
            connect(&v, SIGNAL(send_delete(Comp&)), this, SLOT(receive_delete(Comp&)));
            connect(&v, SIGNAL(send_add(Comp&)), this, SLOT(receive_add(Comp&)));
            emit sendData(item, flag);
            v.exec();
            break;
        }
}

void MainWindow::about_func()
{
    about about_window;
    about_window.exec();
}

void MainWindow::receive_add(Comp& air)
{
    FavItem res;
    res.data[0] = air.data[0];
    res.data[1] = air.data[2];
    fav_window->fav_model->add_data(res);
}

void MainWindow::receive_delete(Comp& air)
{
    int row = -1;
    for(unsigned int i = 0; i < fav_window->fav_model->fav_data.size();i++)
    {
        if(fav_window->fav_model->fav_data[i].data[0] == air.data[0] && fav_window->fav_model->fav_data[i].data[1] == air.data[2])
        {
            row = i;
            break;
        }
    }
    if(row != -1)
    {
        std::unordered_set<size_t> rows_to_delete = {size_t(row)};
        fav_window->fav_model->delete_rows(rows_to_delete);
    }
}

void MainWindow::get_info_fav(FavItem& res)
{
    Comp data;
    for(auto &item : comp_model->comp_data) {
        if(res.data[0] == item.data[0] && res.data[1] == item.data[2]) {
            data = item;
            break;
        }
    }
    view v;
    connect(this, SIGNAL(sendData(Comp&, bool&)), &v, SLOT(recieveData(Comp&, bool&)));
    connect(&v, SIGNAL(send_delete(Comp&)), this, SLOT(receive_delete(Comp&)));
    connect(&v, SIGNAL(send_add(Comp&)), this, SLOT(receive_add(Comp&)));
    bool flag = true;
    emit sendData(data, flag);
    v.exec();
}

void MainWindow::open_favourite()
{
    fav_window->show();
}

void MainWindow::get_info(const QModelIndex & ind)
{
    Comp res;
    int row = ind.row();
    QModelIndex index;
    std::string s;
    for(int i = 0; i < 8; i++) {
        index = comp_view->model()->index(row,i,QModelIndex());
        s = index.data().toString().toStdString();
        if(s.size() == 0)
            res.data[i] = QString("No Info");
        else
            res.data[i] = QString(s.c_str());
    }
    bool flag = false;
    for(unsigned int i = 0; i < fav_window->fav_model->fav_data.size();i++)
    {
        if(fav_window->fav_model->fav_data[i].data[0] == res.data[0] && fav_window->fav_model->fav_data[i].data[1] == res.data[2])
        {
            flag = true;
            break;
        }
    }
    view v;
    connect(this, SIGNAL(sendData(Comp&, bool&)), &v, SLOT(recieveData(Comp&, bool&)));
    connect(&v, SIGNAL(send_delete(Comp&)), this, SLOT(receive_delete(Comp&)));
    connect(&v, SIGNAL(send_add(Comp&)), this, SLOT(receive_add(Comp&)));
    emit sendData(res, flag);
    v.exec();
}

void MainWindow::filter_data()
{
  filter_model->invalidate();
}

void MainWindow::load_data_function ()
{
    auto dir = QFileDialog::getOpenFileName (this, "Open File", QDir::currentPath ());
    if (!dir.size ())
        return;
    auto data = load_data(dir);
    comp_model->set_data(data);
}


void MainWindow::save_data_function ()
{
  auto dir = QFileDialog::getSaveFileName (this, "Save File As", QDir::currentPath ());
  if (!dir.size ())
    return;
  save_data (dir);
}

std::vector<Comp> MainWindow::load_data (const QString &dir)
{
    std::fstream file;
    std::vector<Comp> result;
    file.open(dir.toStdString ());
    std::string temp1, temp2;
    int j = 0;
    while(std::getline(file, temp1))
    {
        Comp res;
        int i = 0, k = 0;
        std::stringstream str(temp1);
        std::string temp3 = "";
        bool flag = false;
        while(std::getline(str, temp2, ','))
        {
            if(temp2[0]=='"' || flag == true)
            {
                temp3 += temp2;
                temp3 += ',';
                flag = true;
                if(temp2[temp2.size()-1] == '"')
                {
                    flag = false;
                    temp3.erase(temp3.begin());
                    temp3.erase(temp3.end()-1);
                    temp3.erase(temp3.end()-1);
                    if(i<9 && i != 6)
                        res.data[k++] = QString(temp3.c_str());
                    temp3="";
                    ++i;
                }
            }
            else
            {
                if(i<9 && i != 6)
                    res.data[k++] = QString(temp2.c_str());
                ++i;
            }
        }       
        if(j) {
            res.data[0] = std::strtod(res.data[0].toString().toStdString().c_str(), NULL);
            res.data[7] = std::strtod(res.data[7].toString().toStdString().c_str(), NULL);
            result.push_back(res);
        }
        j++;
    }
    file.close();
    return result;
}

void MainWindow::save_data (const QString &dir)
{
    std::ofstream file (dir.toStdString ());
    int total_columns = static_cast<int> (comp_fields::COUNT);
    for (auto &item : comp_model->comp_data)
    {
        for (int i_column = 0; i_column < total_columns; ++i_column)
        {
            bool flag = false;
            std::string s = item.data[i_column].toString().toStdString();
            if(s.find(',') != std::string::npos)
            {
                file << "\"";
                flag = true;
            }
            file << item.data[i_column].toString().toStdString();
            if(flag)
                file << "\"";
            if (i_column < total_columns - 1)
                file << ",";
            else
                file << "\n";
        }
    }
}


MainWindow::~MainWindow()
{
}

FilterModel::FilterModel (QObject *parent, const MainWindow *main_window) : QSortFilterProxyModel (parent), main_window (main_window)
{}

QVariant CompModel::data (const QModelIndex &index, int role) const
{
   if (role == Qt::DisplayRole || role == Qt::EditRole)
   {
       const Comp &current_comp = comp_data[index.row ()];
        return current_comp.data[index.column()];
   }
   return {};
}

const char *enum_to_string (comp_fields field)
{
    switch (field)
    {
        case comp_fields::rank      : return "Rank";
        case comp_fields::profile   : return "Profile link";
        case comp_fields::name      : return "Name of Company";
        case comp_fields::url       : return "Url";
        case comp_fields::state     : return "State";
        case comp_fields::revenue   : return "Revenue";
        case comp_fields::industry  : return "Industry";
        case comp_fields::workers   : return "Workers";
        case comp_fields::COUNT     : return "";
    }
    return {};
}

bool FilterModel::filterAcceptsRow (int source_row, const QModelIndex &/*source_parent*/) const
{
  auto state = main_window->state_filter->text();
  auto indus = main_window->indus_filter->text();
  const Comp& comp = main_window->comp_model->comp_data[source_row];
  return ((!state.size() || comp.data[(int)comp_fields::state].toString () == state) &&
          (!indus.size() || comp.data[(int)comp_fields::industry].toString () == indus));
}

QVariant CompModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        comp_fields field = (comp_fields)section;
        return enum_to_string (field);
    }
    return {};
}
