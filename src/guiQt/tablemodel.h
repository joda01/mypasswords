#ifndef TABLE_MODEL_H_
#define TABLE_MODEL_H_

#include <QAbstractTableModel>
#include <QColor>
#include "../core/Container/Container.h"

class TableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    enum Column_t {eColTitle=0, eColUsername, eColPassword, eColURL, eColNotes, eColTags};
    TableModel(QObject *parent = 0);
    void UpdateView();

private:
    QColor COLOR_RED=QColor(255,51,51);
    QColor COLOR_LIGHT_GRAY=QColor(178,102,255);

    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    virtual Container::PasswordEntry::PasswordElement* GetPasswordElement(int32_t idx) const=0;
    virtual int GetNumberOfElements(void) const =0;
    virtual void UpdateFilter(void)=0;

    static const int COLUMN_NUMBER = 6;
    static const int ICON_SIZE = 64;
};

#endif
