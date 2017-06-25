///
/// \file       wizardnewcontainer.h
/// \author     Joachim Danmayr
/// \date       2016-07-22
///
/// \brief      New container Dialog
///

#ifndef WIZARDNEWCONTAINER_H
#define WIZARDNEWCONTAINER_H

#include <QWizard>
#include <QColor>
#include <QThread>
#include <QTimer>
#include "passwordcontainerthread.h"
#include "configuration.h"
#include "../core/Crypto/hashing/argon.h"


namespace Ui {
class WizardNewContainer;
}

///
/// \class      WizardNewContainer
/// \author     Joachim Danmayr
/// \date       2016-09-18
///
/// \brief      New container dialog
///
class WizardNewContainer : public QWizard
{
    Q_OBJECT

    class ArgonThread : public QThread{
    public:
        ArgonThread(WizardNewContainer* parent):mParent(parent),mArgon(0){}
        virtual void run() override;
        void stop();
    private:
        WizardNewContainer* mParent;
        crypto::Argon2* mArgon;
    };

    class ReadFileThread : public QThread{
    public:
        ReadFileThread(QStringList* listToLoad):mPasswordList(listToLoad),mStop(false){}
        void stop(){mStop=true;}
    private:
        virtual void run() override;
        QStringList* mPasswordList;
        bool mStop;
    };

public:
    enum DialogType_t {eNone, eNewContainer, eNewPassword};
    explicit WizardNewContainer(Configuration *settings, PasswordContainerThread *passwordContainer, QWidget *parent = 0);
    ~WizardNewContainer();
    bool ShowDialog(DialogType_t type);

private slots:
    void on_WizardNewContainer_finished(int result);

    void on_buttonSelectFile_clicked();

    void on_WizardNewContainer_currentIdChanged(int id);

    void on_textRepeatedPassword_textChanged(const QString &arg1);

    void on_textPassword_textChanged(const QString &arg1);

    void on_textFileName_textChanged(const QString &arg1);

    void on_wizardPage1_completeChanged();

    void on_buttonTestArgon_clicked();

    void on_timerTimeout();

    void on_spinLanes_valueChanged(int arg1);

    void on_spinMemoryCost_valueChanged(int arg1);

    void on_spinIterationCost_valueChanged(int arg1);

    void on_WizardNewContainer_rejected();

    void on_PasswordThreadFinished(int event, int error);

    void on_WizardNewContainer_accepted();

    void on_textRepeatedPassword_editingFinished();

    void on_LoadPasswordListThreadFinished();

private:
    ///////////////////////////////////////////////////////////////
    /// Constants
    QColor mColorGreen=QColor(140,207,127);
    QColor mColorRed=QColor(255,51,51);
    static const int32_t PROGRESSBAR_TIME_MS = 5000;        ///< Maximum time for the progressbar in ms
    static const int32_t WIZARD_PAGE1 = 0;                  ///< Index of the first wizard page
    static const int32_t WIZARD_PAGE2 = 1;                  ///< Index of the second wizard page
    static const int32_t MINIMUM_PASSWORD_LENGTH = 10;


    ///////////////////////////////////////////////////////////////
    /// Methods
    bool ArePasswordsEqual();
    void CheckPasswordSafety(const QString &arg1);
    void CreateContainer();
    void ChangePassword();
    void CreateContainerFinsihed(int error);
    void ChangePasswordFinished(int error);
    void LoadPasswordListToRam();
    void CleanUp();
    void Restart();

    ///////////////////////////////////////////////////////////////
    /// Members
    Ui::WizardNewContainer *ui;
    Configuration* mSettings;
    PasswordContainerThread *mPasswordContainer;
    ArgonThread mArgonThread;
    ReadFileThread mReadPasswordFileThread;
    QTimer mTimer;
    uint32_t mArgonCounter;
    std::string mPassword;
    std::string mPasswordOld;
    DialogType_t mDialogType;
    QStringList mPasswordList;
    bool mOkPressed;
    bool mPasswordListLoaded;

};

#endif // WIZARDNEWCONTAINER_H
