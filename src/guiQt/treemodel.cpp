///
/// \file       treemodel.cpp
/// \author     Joachim Danmayr
/// \date       2016-09-21
///
/// \brief      Contains the treemodel and treeitem class
///             This classes can be used to show the groups
///             of the password container in a treee view
///

#include "treemodel.h"
#include <QStringList>
#include <QIcon>
#include <QDebug>
#include <QMap>
#include "../core/Container/Container.h"

///
/// \brief      Constructor
/// \author     Joachim Danmayr
/// \date       2016-09-21
///
/// \param[in]  data    Pointer to the group entry which is
///                     part of the Tree item
/// \param[in]  parent  Parent item
///
TreeItem::TreeItem(Container::GroupEntry *data, TreeItem *parent)
{
    m_parentItem = parent;
    m_itemData = data;
}

///
/// \brief      Destructor
/// \author     Joachim Danmayr
/// \date       2016-09-21
///
TreeItem::~TreeItem()
{
    Clear();
    qDeleteAll(m_childItems);
}

void TreeItem::SetParent(TreeItem *parent){m_parentItem = parent;}
void TreeItem::appendChild(TreeItem *item){m_childItems.append(item);}
TreeItem *TreeItem::child(int row){return m_childItems.value(row);}
TreeItem *TreeItem::parentItem(){return m_parentItem;}
int TreeItem::childCount() const{return m_childItems.count();}
int TreeItem::columnCount() const{return COLUMN_NUMBERS;}
Container::GroupEntry* TreeItem::GetData(void){return m_itemData;}

///
/// \brief      Returns data at the given column
///             This item has only one column which contains
///             the name of the group
/// \author     Joachim Danmayr
/// \date       2016-09-21
///
/// \param[in]  column  Column
/// \return     Group name
///
QVariant TreeItem::data(int column) const
{
    Q_UNUSED(column);
    uint32_t size;
    char* data = m_itemData->GetName(size);
    QString ret  = QString::fromUtf8(data,size);
    return ret;
}

///
/// \brief      Returns the group ID of this item
/// \author     Joachim Danmayr
/// \date       2016-09-21
///
/// \return     Returns the group ID of this item
///
QString TreeItem::GetGroupId(void)
{
    uint32_t size;
    char* data = m_itemData->GetGroupId(size);
    QString ret  = QString::fromLatin1(data,size);
    return ret;
}

///
/// \brief      Returns the group ID of parent item of this item
///             stored in the container
/// \author     Joachim Danmayr
/// \date       2016-09-21
///
/// \return     Parent item group if
///
QString TreeItem::GetParentId(void)
{
    uint32_t size;
    char* data = m_itemData->GetParentId(size);
    QString ret  = QString::fromLatin1(data,size);
    return ret;
}

///
/// \brief      Returns the icon of this group
/// \author     Joachim Danmayr
/// \date       2016-09-21
///
/// \return     Icon of this group
///
QIcon TreeItem::GetIcon(void) const
{
    uint32_t size=0;
    char* data = m_itemData->GetIcon(size);
    QByteArray iconBuffer = QByteArray::fromRawData(data,size);
    QPixmap p; p.loadFromData(iconBuffer,"PNG");
    //p= p.scaled(ICON_SIZE,ICON_SIZE);
    QIcon icon(p);
    return icon;
}

///
/// \brief      Returns the row number
/// \author     Joachim Danmayr
/// \date       2016-09-21
///
/// \return     row number
///
int TreeItem::row() const
{
    if (m_parentItem){
        return m_parentItem->m_childItems.indexOf(const_cast<TreeItem*>(this));
    }
    return 0;
}

///
/// \brief      Clear this item
/// \author     Joachim Danmayr
/// \date       2016-09-21
///
 void TreeItem::Clear(void)
 {
     m_childItems.clear();
     if(0!=m_parentItem){
        m_parentItem->Clear();
     }
 }

///
/// \brief      Constructor
/// \author     Joachim Danmayr
/// \date       2016-09-21
///
/// \param[in]  container   Pointer to the password container
/// \param[in]  parent      Parent object
///
TreeModel::TreeModel(Container* container, QObject *parent)
    : QAbstractItemModel(parent),
      mContainer(container)
{
    Container::GroupEntry* root = new Container::GroupEntry();
    root->SetName((const char*)"Root",4);
    rootItem = new TreeItem(root,0);
    setupModelData(container, rootItem);
}

///
/// \brief      Destructor
/// \author     Joachim Danmayr
/// \date       2016-09-21
///
TreeModel::~TreeModel()
{
    delete rootItem;
}

///
/// \brief      Update the tree table view
/// \author     Joachim Danmayr
/// \date       2016-09-21
///
void TreeModel::UpdateView(void)
{
    setupModelData(mContainer, rootItem);
}

///
/// \brief      Number of columns of this tree table entry
/// \author     Joachim Danmayr
/// \date       2016-09-21
///
/// \param[in]  parent   Index of the parent item
///
int TreeModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid()){
        return static_cast<TreeItem*>(parent.internalPointer())->columnCount();
    }
    else{
       // return rootItem->columnCount();
        return 1;
    }
}

///
/// \brief      Returns the data of the tree table
///             at given index
/// \author     Joachim Danmayr
/// \date       2016-09-21
///
/// \param[in]  index   Index of the item data should be returned
/// \param[in]  role    User role
/// \return     Data at given index
///
QVariant TreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()){
        return QVariant();
    }

    if(role == Qt::DecorationRole)
    {
        switch(index.column()){
        case eColGroupName:{
            TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
            return item->GetIcon();
        }

        default:
            return QVariant();
        }
    }
    else if (role == Qt::DisplayRole){
        TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
        return item->data(index.column());
    }
    else{
        return QVariant();
    }
}

///
/// \brief      Returns the tree item at given index
/// \author     Joachim Danmayr
/// \date       2016-09-21
///
/// \param[in]  index   Index of the item data should be returned
/// \return     TreeItem at given index
///
TreeItem* TreeModel::itemFromIndex(const QModelIndex &index) const
{
    TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
    return item;
}

///
/// \brief      Returns flags of an item
/// \author     Joachim Danmayr
/// \date       2016-09-21
///
/// \param[in]  index   Index of the item data should be returned
/// \return     Flags
///
Qt::ItemFlags TreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    return QAbstractItemModel::flags(index);
}

///
/// \brief      Returns header data
/// \author     Joachim Danmayr
/// \date       2016-09-21
///
/// \return     Header data
///
QVariant TreeModel::headerData(int section, Qt::Orientation orientation,int role) const
{
    Q_UNUSED(section);
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole){
        return tr("Categories");
    }

    return QVariant();
}

///
/// \brief      Index of an item
/// \author     Joachim Danmayr
/// \date       2016-09-21
///
QModelIndex TreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    TreeItem *parentItem;

    if (!parent.isValid()){
        parentItem = rootItem;
    }
    else{
        parentItem = static_cast<TreeItem*>(parent.internalPointer());
    }

    TreeItem *childItem = parentItem->child(row);
    if (childItem){
        return createIndex(row, column, childItem);
    }
    else{
        return QModelIndex();
    }
}

///
/// \brief      Returns parent item
/// \author     Joachim Danmayr
/// \date       2016-09-21
///
QModelIndex TreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    TreeItem *childItem = static_cast<TreeItem*>(index.internalPointer());
    TreeItem *parentItem = childItem->parentItem();

    if (parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

///
/// \brief      Number of rows
/// \author     Joachim Danmayr
/// \date       2016-09-21
///
int TreeModel::rowCount(const QModelIndex &parent) const
{
    TreeItem *parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<TreeItem*>(parent.internalPointer());

    return parentItem->childCount();
}

///
/// \brief      Create the tree item view model
/// \author     Joachim Danmayr
/// \date       2016-09-21
///
/// \param[in]  container   Pointer to the container which contains
///                         the groups which should be shown in the tree view
/// \param[in]  rootItem    Pointer to the root item
///
void TreeModel::setupModelData(Container* container, TreeItem *rootItem)
{
    beginResetModel();
    endResetModel();
    rootItem->Clear();
    QMap<QString,TreeItem*> items;
    items.insert("",rootItem);

    // Add all items ti the map
    for(uint32_t n=0;n<container->GetNumberOfGroupEntry();n++){
        Container::GroupEntry* columnData = container->GetGroup(n);
        TreeItem* newItem = new TreeItem(columnData);
        QString parentIdString = newItem->GetGroupId();
        items.insert(parentIdString,newItem);
    }

    // Link items to a tree
    QMap<QString,TreeItem*>::iterator i;
    for (i = items.begin(); i != items.end(); ++i){
        TreeItem* actItem = i.value();
        QString parentIdString = actItem->GetParentId();
        if(items.contains(parentIdString)){
            if(items[parentIdString]!=actItem){
                items[parentIdString]->appendChild(actItem);
                actItem->SetParent(items[parentIdString]);
            }
        }
    }
}
