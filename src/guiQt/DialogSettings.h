#ifndef DIALOGSETTINGS_H
#define DIALOGSETTINGS_H

#include <QDialog>
#include "configuration.h"
#include "passwordcontainerthread.h"
#include "wizardnewcontainer.h"



namespace Ui {
class DialogSettings;
}

class DialogSettings : public QDialog
{
    Q_OBJECT

public:
    explicit DialogSettings(Configuration* settings, PasswordContainerThread *container,  QWidget *parent = 0);
    ~DialogSettings();

private slots:
    void on_buttonChangePassword_clicked();

    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

    void on_listLanguage_itemSelectionChanged();

    void on_checkEnableCommunicationPort_clicked(bool checked);

private:
    void LoadSettings();
    void SaveSettings();
    uint32_t TextFieldToString(QString string, char** data);


    Ui::DialogSettings *ui;
    PasswordContainerThread* mContainer;
    Configuration* mSettings;
    WizardNewContainer mNewPassword;

};

#endif // DIALOGSETTINGS_H
