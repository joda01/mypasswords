///
/// \file       dialoggroup.h
/// \author     Joachim Danmayr
/// \date       2016-09-24
///
/// \brief      Group manager
///

#ifndef DIALOGGROUP_H
#define DIALOGGROUP_H

#include <QDialog>
#include "../core/Container/Container.h"
#include "modelpasswordentry.h"

namespace Ui {
class DialogGroup;
}

///
/// \class      DialogGroup
/// \author     Joachim Danmayr
/// \date       2016-09-21
///
/// \brief      Group entry dialog class
///
class DialogGroup : public QDialog
{
    Q_OBJECT

public:
    explicit DialogGroup(QWidget *parent = 0);
    bool ShowDialog(Container* container, Container::GroupEntry *entry, QString selectedGroupId=QString(""));
    ~DialogGroup();

private slots:


    void on_buttonBox_accepted();

    void on_buttonAddIcon_clicked();

private:
    static const int ICON_SIZE = 64;

    void LoadDataToTextField(Container::GroupEntry* element);
    void LoadGroups(Container* container, QString &myGroupId);
    uint32_t TextFieldToString(QString string, char **data);
    QString StringToTextField(const char* data, uint32_t length);

    Ui::DialogGroup *ui;
    Container::GroupEntry* mEntry;
    bool mOkPressed;
};
#endif // DIALOGGROUP_H
