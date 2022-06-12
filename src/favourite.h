#ifndef FAVOURITE_H
#define FAVOURITE_H
#include <QMainWindow>
#include <QDialog>
#include <QTableView>
#include <unordered_set>
#include <QLineEdit>
#include <QPushButton>
#include <QSortFilterProxyModel>
#include <QComboBox>
#include <QLabel>

class FavModel;
class FavItem;

class favourite : public QWidget
{
    Q_OBJECT
    QTableView *fav_view = nullptr;

    public:
    FavModel *fav_model = nullptr;

public:
    explicit favourite(QWidget *parent = nullptr);
    ~favourite();

private slots:
    void info(const QModelIndex & ind);

signals:
    void send_data(FavItem &res);

};

enum class fav_fields
{
    rank,
    name,
    COUNT
};

const char *enum_to_string_cart (fav_fields field);

class FavItem
{
public:
    std::array<QVariant, (static_cast<int> (fav_fields::COUNT))> data;
    friend bool operator==(const FavItem &a, const FavItem &b) {
        return(a.data[0] == b.data[0] && a.data[1] == b.data[1]);
    }
};

class FavModel : public QAbstractTableModel
{
Q_OBJECT

public:
    std::vector<FavItem> fav_data;

int rowCount(const QModelIndex &/*parent*/) const override
{
    return fav_data.size ();
}

int columnCount(const QModelIndex &/*parent*/) const override
{
    return static_cast<int> (fav_fields::COUNT);
}

void add_data(const FavItem &data)
{
    beginResetModel ();
    fav_data.push_back(data);
    endResetModel ();
    emit layoutChanged ();
}

void delete_rows (const std::unordered_set<size_t> &selected_rows)
{

    beginResetModel();
    size_t j = 0;
    for(size_t i = 0; i < fav_data.size(); ++i)
        if(selected_rows.find(i) == selected_rows.end()) // not found
            fav_data[j++] = fav_data[i];
    fav_data.resize(j);
    endResetModel();
    emit layoutChanged();    
}


QVariant data (const QModelIndex &index, int role = Qt::DisplayRole) const override;
QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
};
#endif // FAVOURITE_H
