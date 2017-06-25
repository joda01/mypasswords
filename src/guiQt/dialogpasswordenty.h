///
/// \file       dialogpasswordentry.h
/// \author     Joachim Danmayr
/// \date       2016-09-21
///
/// \brief      Password entry dialog file
///

#ifndef DIALOGPASSWORDENTY_H
#define DIALOGPASSWORDENTY_H

#include <QDialog>
#include "../core/Container/Container.h"
#include "modelpasswordelements.h"

namespace Ui {
class DialogPasswordEnty;
}

///
/// \class      DialogPasswordEnty
/// \author     Joachim Danmayr
/// \date       2016-09-21
///
/// \brief      Password entry dialog class
///
class DialogPasswordEnty : public QDialog
{
    Q_OBJECT

public:
    explicit DialogPasswordEnty(QWidget *parent = 0);
    bool ShowDialog(Container *container, Container::PasswordEntry *entry, QString selectedGroupId=QString(""));

    ~DialogPasswordEnty();

private slots:
    void on_buttonBox_accepted();

    void on_buttonPwdShow_toggled(bool checked);

    void on_textPassword_textChanged(const QString &arg1);

    void on_buttonBox_rejected();

    void on_buttonOpenUrl_clicked();

    void on_textUrl_editingFinished();

    void on_buttonAddIcon_clicked();

    void on_buttonGenerator_clicked();

    void on_buttonArchive_clicked();

    void on_buttonRemoveFromArchive_clicked();

    void on_buttonRestore_clicked();


    void on_textTags_editingFinished();

    void on_textTags_textChanged(const QString &arg1);

    void on_checkExpire_clicked(bool checked);

private:
    static const int PROGRESSBAR_MINIMUM = 128;
    static const int ICON_SIZE = 64;                    ///< Icon size
    static const int ICON_MAX_FILE_SIZE = 1048576;      ///< Maximum file size of the icon

    void LoadGroups(Container *container);
    void LoadDataToTextField(Container::PasswordEntry::PasswordElement* element);
    uint32_t TextFieldToString(QString string, char **data);
    QString StringToTextField(const char* data, uint32_t length);
    void ClearTextFieldContent(void);

    Ui::DialogPasswordEnty *ui;
    Container::PasswordEntry* mEntry;
    ModelPasswordElements*    mTableModel;
    bool mOkPressed;
};

#endif // DIALOGPASSWORDENTY_H
