#include "tablemodel.h"
#include <QSize>
#include <QDebug>
#include <QPixmap>
#include <QIcon>

TableModel::TableModel(QObject *parent) :
    QAbstractTableModel(parent)
{
}

/**
***************************************************************************
* Gibt die Anzahl an Zeilen zurueck
*
* \author Joachim Danmayr
* \date   2012-10-13
*
***************************************************************************
*/
int TableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return GetNumberOfElements();
}

/**
***************************************************************************
* Gibt die Anzahl an Spalten zurueck
*
* \author Joachim Danmayr
* \date   2012-10-13
*
***************************************************************************
*/
int TableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return COLUMN_NUMBER;
}


/**
 ***************************************************************************
 * Gibt die Daten an der angegebenen Stelle zurueck
 *
 * \author Joachim Danmayr
 * \date   2012-10-13
 *
 ***************************************************************************
 */
QVariant TableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
    {
        return QVariant();
    }
    Container::PasswordEntry::PasswordElement* element = GetPasswordElement(index.row());
    if(element==0){
        return QVariant();
    }
    if (role == Qt::DisplayRole)
    {
        char* data;
        uint32_t size=0;
        QString ret;
        switch(index.column()){
        case eColTitle:
            data = element->GetTitle(size);
            ret  = QString::fromUtf8(data,size);
            return ret;

        case eColUsername:
            data = element->GetUsername(size);
            ret  = QString::fromUtf8(data,size);
            return ret;

        case eColPassword:
            //data = element->GetPassword(size);
            //ret  = QString::fromUtf8(data,size);
            return "*****";

        case eColURL:
            data = element->GetURL(size);
            ret  = QString::fromUtf8(data,size);
            return ret;


        case eColNotes:
            data = element->GetNotes(size);
            ret  = QString::fromUtf8(data,size);
            return ret;

        case eColTags:
            data = element->GetTags(size);
            ret  = QString::fromUtf8(data,size);
            return ret;
        }

        return QVariant();

    }
    else if(role ==Qt::DecorationRole)
    {// Rolle fuer Icons
        char* data=0;
        uint32_t size =0;

        switch(index.column()){
        case eColTitle:{
            data = element->GetIcon(size);
            QByteArray iconBuffer = QByteArray::fromRawData(data,size);
            QPixmap p; p.loadFromData(iconBuffer,"PNG");
            //p= p.scaled(ICON_SIZE,ICON_SIZE);
            QIcon icon(p);
            return icon;
        }

        default:
            return QVariant();
        }

    }
    else if (role == Qt::BackgroundColorRole) {
       /* if(element->IsExpired()){
            return QVariant(COLOR_RED);
        }else{
            return QVariant();
        }*/
        return QVariant();
    }
    else if(role == Qt::TextColorRole){
        if(element->IsExpired()){
            return QVariant(COLOR_LIGHT_GRAY);
        }else{
            return QVariant();
        }
    }
    else
    {
        return QVariant();
    }
}


/**
 ***************************************************************************
 * Gibt den Header zurueck
 *
 * \author Joachim Danmayr
 * \date   2012-10-13
 *
 ***************************************************************************
 */
QVariant TableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    // Groesse bestimmen
    if(role == Qt::SizeHintRole && orientation == Qt::Horizontal)
    {
        // Groesse
        return QVariant();
    }

    if (role != Qt::DisplayRole)
    {
        return QVariant();
    }

    if (orientation == Qt::Horizontal)
    {
        switch(section){
        case eColTitle:
            return QString(tr("Title"));

        case eColUsername:
            return QString(tr("Username"));

        case eColPassword:
            return QString(tr("Password"));

        case eColURL:
            return QString(tr("Url"));

        case eColNotes:
            return QString(tr("Notes"));

        case eColTags:
            return QString(tr("Tags"));

        }
    }
    return QVariant();
}



/**
 ***************************************************************************
 * Updates the view
 *
 * \author Joachim Danmayr
 * \date   2016-07-14
 *
 ***************************************************************************
 */
void TableModel::UpdateView()
{
    UpdateFilter();
    beginResetModel();
    endResetModel();


    if(GetNumberOfElements()>0){
        beginInsertRows(QModelIndex(), 0, GetNumberOfElements()-1);
        endInsertRows();
    }
}



/**
 ***************************************************************************
 * Flags auslesen
 *
 * \author Joachim Danmayr
 * \date   2012-10-13
 *
 ***************************************************************************
 */
Qt::ItemFlags TableModel::flags(const QModelIndex &index) const
{

    if (!index.isValid())
        return Qt::ItemIsEnabled;

    return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
}

