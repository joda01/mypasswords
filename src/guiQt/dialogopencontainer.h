///
/// \file       dialogopencontainer.h
/// \author     Joachim Danmayr
/// \date       2016-07-22
///
/// \brief      Open container Dialog
///


#ifndef DIALOGOPENDATABASE_H
#define DIALOGOPENDATABASE_H

#include <QDialog>
#include "configuration.h"

#include "passwordcontainerthread.h"

namespace Ui {
class DialogOpenContainer;
}


///
/// \class      DialogOpenContainer
/// \author     Joachim Danmayr
/// \date       2016-09-18
///
/// \brief      Open container dialog
///
class DialogOpenContainer : public QDialog
{
    Q_OBJECT

public:
    ///////////////////////////////////////////////////////////////
    /// Methods
    explicit DialogOpenContainer(Configuration* settings, PasswordContainerThread* passwordContainer,QWidget *parent = 0);
    ~DialogOpenContainer();
    QString ShowDialog();
    bool ShowUnlockDialog();

private slots:
    void on_buttonLoad_clicked();
    void on_PasswordThreadFinished(int event, int error);

    void on_buttonBox_rejected();

private:
    enum DialogType_t {eOpen, eUnlock};

    ///////////////////////////////////////////////////////////////
    /// Methods
    virtual void accept();
    void LoadFileHistoryToComboBox();
    void SaveFileHistoryFromComboBox();

    ///////////////////////////////////////////////////////////////
    /// Members
    Ui::DialogOpenContainer *ui;
    DialogType_t mType;
    Configuration* mSettings;
    PasswordContainerThread *mPasswordContainer;
    QString mPasswordContainerFileName;
    bool mOpenOk;
    std::string mPassword;
    const char* mPasswordPointer;
};

#endif // DIALOGOPENDATABASE_H
