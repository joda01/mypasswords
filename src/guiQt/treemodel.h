///
/// \file       treemodel.h
/// \author     Joachim Danmayr
/// \date       2016-09-21
///
/// \brief      Contains the treemodel and treeitem class
///             This classes can be used to show the groups
///             of the password container in a treee view
///

#ifndef TREEMODEL_H
#define TREEMODEL_H

#include <QAbstractItemModel>
#include "../core/Container/Container.h"

///
/// \class      TreeItem
/// \author     Joachim Danmayr
/// \date       2016-09-23
///
/// \brief      Tree item class. This is an entry
///             of the tree view
///
class TreeItem
{
public:
    explicit TreeItem(Container::GroupEntry *data, TreeItem *parentItem = 0);
    ~TreeItem();
    void appendChild(TreeItem *child);
    void SetParent(TreeItem *parent);
    TreeItem *child(int row);
    int childCount() const;
    int columnCount() const;
    QVariant data(int column) const;
    QIcon GetIcon(void) const;
    QString GetGroupId(void);
    QString GetParentId(void);
    Container::GroupEntry* GetData(void);
    int row() const;
    TreeItem *parentItem();
    void Clear(void);

private:
    static const int COLUMN_NUMBERS = 1;

    QList<TreeItem*> m_childItems;
    Container::GroupEntry* m_itemData;
    TreeItem *m_parentItem;
};


///
/// \class      TreeModel
/// \author     Joachim Danmayr
/// \date       2016-09-23
///
/// \brief      Model which contains tree items
///
class TreeModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit TreeModel(Container *container, QObject *parent = 0);
    ~TreeModel();
    QVariant data(const QModelIndex &index, int role) const override;
    TreeItem* itemFromIndex(const QModelIndex &index) const ;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QVariant headerData(int section, Qt::Orientation orientation,int role = Qt::DisplayRole) const override;
    QModelIndex index(int row, int column,const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    void UpdateView(void);

private:
    enum Column_t {eColGroupName=0};
    void setupModelData(Container *container, TreeItem *rootItem);
    TreeItem *rootItem;
    Container* mContainer;
};

#endif // TREEMODEL_H
