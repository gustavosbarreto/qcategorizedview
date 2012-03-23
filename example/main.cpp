#include <QApplication>
#include <QDir>
#include <QSettings>
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

static QStringList preferredCategoryOrder;

static bool categorySorter(const ConfigPane &a, const ConfigPane &b)
{
    int aIdx = preferredCategoryOrder.indexOf(a.category());
    int bIdx = preferredCategoryOrder.indexOf(b.category());
    if (aIdx < 0)
        return false;
    return aIdx < bIdx;
}

class ConfigPaneModel: public QAbstractListModel
{
public:
    ConfigPaneModel(): QAbstractListModel()
    {
        if (preferredCategoryOrder.isEmpty())
            preferredCategoryOrder << "Personal" << "Hardware" << "System";

        QDir dir(qApp->applicationDirPath() + "/items/");
        Q_FOREACH (const QFileInfo &fileInfo, dir.entryInfoList(QStringList() << "*.desktop"))
        {
            QSettings desktopFile(fileInfo.filePath(), QSettings::IniFormat);
            desktopFile.beginGroup("Desktop Entry");

            ConfigPane pane;
            pane.id() = desktopFile.value("Icon").toString();
            pane.name() = desktopFile.value("Name").toString();
            pane.category() = desktopFile.value("Category").toString();
            m_list.append(pane);
        }

        qSort(m_list.begin(), m_list.end(), categorySorter);
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
        if (role == Qt::DecorationRole)
        {
            if (!QFile::exists(qApp->applicationDirPath() + "/icons/" + m_list[index.row()].id()))
                qFatal("asdads");

            return QIcon(qApp->applicationDirPath() + "/icons/" + m_list[index.row()].id());
        }
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
    view->setViewMode(QListView::IconMode);
    view->setIconSize(QSize(48, 48));
    view->setGridSize(QSize(140, 74));
    view->setCategoryDrawer(new QCategoryDrawerV3(view));

    QCategorizedSortFilterProxyModel *proxyModel = new QCategorizedSortFilterProxyModel();
    proxyModel->setCategorizedModel(true);
    proxyModel->setSourceModel(model);

    view->setModel(proxyModel);
    view->resize(640, 400);
    view->show();

    return app.exec();
}
