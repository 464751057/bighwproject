#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <favourite.h>
#include <QTableView>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
#include <QDebug>
#include <QSortFilterProxyModel>

class CompModel;
class Comp;
class FilterModel;

class MainWindow : public QMainWindow
{
public:
    Q_OBJECT
    QTableView *comp_view = nullptr;
    CompModel *comp_model = nullptr;
    FilterModel *filter_model = nullptr;
    QPushButton *open_fav;
    QLineEdit *state_filter;
    QLineEdit *indus_filter;
    QLineEdit *search_ln;
    QLabel *state_lb;
    QLabel *indus_lb;
    QLabel *search_lb;
    QPushButton *apply_filter;
    QPushButton *search;


public:
    MainWindow(QWidget *parent = nullptr);
    std::vector<Comp> load_data (const QString &dir);
    void save_data (const QString &dir);
    favourite *fav_window;
    ~MainWindow();

public slots:
    void load_data_function();
    void save_data_function();
    void filter_data();
    void get_info(const QModelIndex &ind);
    void open_favourite();
    void get_info_fav(FavItem& res);
    void receive_delete(Comp& air);
    void receive_add(Comp& air);
    void about_func();
    void search_func();

signals:
    void sendData(Comp& air, bool& flag);

friend class FilterModel;
};

enum class comp_fields
{
    rank,
    profile,
    name,
    url,
    state,
    revenue,
    industry,
    workers,
    COUNT
};

const char *enum_to_string (comp_fields field);

class Comp
{
public:
    std::array<QVariant, (static_cast<int> (comp_fields::COUNT))> data;
};

class FilterModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    FilterModel (QObject *parent, const MainWindow *main_window);
protected:
    virtual bool filterAcceptsRow (int source_row, const QModelIndex &source_parent) const override;
private:
    const MainWindow *main_window = nullptr;
};

class CompModel : public QAbstractTableModel
{
Q_OBJECT

public:
    std::vector<Comp> comp_data;
    int rowCount(const QModelIndex &/*parent*/) const override
    {
        return comp_data.size ();
    }

    int columnCount(const QModelIndex &/*parent*/) const override
    {
        return static_cast<int> (comp_fields::COUNT);
    }

    void set_data (const std::vector<Comp> &data)
    {
      beginResetModel();
      comp_data.insert(comp_data.begin(), data.begin(), data.end());
      endResetModel();
      emit layoutChanged ();
    }
    QVariant data (const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
};

#endif // MAINWINDOW_H
