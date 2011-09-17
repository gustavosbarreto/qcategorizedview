#include <QApplication>
#include <QDebug>

#include "qcategorizedview.h"
#include "qcategorydrawer.h"
#include "qcategorizedsortfilterproxymodel.h"

struct ConfigPaneData: public QSharedData
{
    QString id;
    QString name;
    QString category;
};

class ConfigPane
{
public:
    ConfigPane(): d(new ConfigPaneData) { }
    ConfigPane(const ConfigPane &other): d(other.d) { }

    inline QString &id() const { return d->id; }
    inline QString &name() const { return d->name; }
    inline QString &category() const { return d->category; }

    bool operator==(const ConfigPane &other)
    {
        return d->id == other.id();
    }

private:
    QExplicitlySharedDataPointer<ConfigPaneData> d;
};

class ConfigPaneModel: public QAbstractListModel
{
public:
    ConfigPaneModel(): QAbstractListModel()
    {
        ConfigPane c1;
        c1.id() = "id1";
        c1.name() = "Appearance";
        c1.category() = "Personal";
        m_list.append(c1);

        ConfigPane c2;
        c2.id() = "id2";
        c2.name() = "Screen Saver";
        c2.category() = "Personal";
        m_list.append(c2);

        ConfigPane c3;
        c3.id() = "id1";
        c3.name() = "Dock";
        c3.category() = "Personal";
        m_list.append(c3);

        ConfigPane c4;
        c4.id() = "id1";
        c4.name() = "Keyboard & Mouse";
        c4.category() = "Hardware";
        m_list.append(c4);

        ConfigPane c5;
        c5.id() = "id1";
        c5.name() = "Sound";
        c5.category() = "Hardware";
        m_list.append(c5);

        ConfigPane c6;
        c6.id() = "id1";
        c6.name() = "Sound";
        c6.category() = "System";
        m_list.append(c6);

        ConfigPane c7;
        c7.id() = "id1";
        c7.name() = "Date & Time";
        c7.category() = "System";
        m_list.append(c7);

    }

    ~ConfigPaneModel() { }

    int rowCount(const QModelIndex &parent = QModelIndex()) const
    {
        return m_list.count();
    }

    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole)
    {
    }

    QVariant data(const QModelIndex &index, int role) const
    {
        if (role == Qt::DisplayRole)
            return m_list[index.row()].name();
        if (role == QCategorizedSortFilterProxyModel::CategoryDisplayRole)
            return m_list[index.row()].category();
        if (role == QCategorizedSortFilterProxyModel::CategorySortRole)
            return m_list[index.row()].category();
        if (role == Qt::UserRole)
            return m_list[index.row()].id();
        return QVariant();
    }

private:
    QList<ConfigPane> m_list;
};


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    ConfigPaneModel *model = new ConfigPaneModel();

    QCategorizedView *view = new QCategorizedView;
    view->setCategoryDrawer(new QCategoryDrawer());

    QCategorizedSortFilterProxyModel *proxyModel = new QCategorizedSortFilterProxyModel();
    proxyModel->setCategorizedModel(true);
    proxyModel->setSourceModel(model);

    view->setModel(proxyModel);
    view->resize(200, 400);
    view->show();

    return app.exec();
}
