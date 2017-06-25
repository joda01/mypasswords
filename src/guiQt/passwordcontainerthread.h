#ifndef PASSWORDCONTAINERTHREAD_H
#define PASSWORDCONTAINERTHREAD_H

#include <QThread>
#include "dialogwait.h"
#include "../core/Container/containerencrypted.h"


///
/// \class      PasswordContainerThread
/// \author     Joachim Danmayr
/// \date       2016-10-29
///
/// \brief      Thread which contains a password container object
///
class PasswordContainerThread : public QThread, public ContainerEncrypted
{
    Q_OBJECT

public:
    ///////////////////////////////////////////////////////////
    /// Container events
    enum Event_t {eCmdNo,eCmdNew, eCmdOpen,eCmdUnlock, eCmdLock, eCmdSave, eCmdClose, eCmdChangePwd};

    ///////////////////////////////////////////////////////////
    /// Methods
    PasswordContainerThread(QWidget *parent);
    void New(QWidget* parent,std::string fileName, const char *password, uint32_t passwordSize, uint32_t argon2ItCost, uint32_t argon2MemCost, uint32_t argon2Lanes);
    void Open(QWidget* parent,std::string fileName, const char* password, uint32_t passwordSize);
    void ChangePassword(QWidget* parent,const char *oldPassword, uint32_t oldPasswordSize,const char *newPassword, uint32_t newPasswordSize, uint32_t argon2ItCost, uint32_t argon2MemCost, uint32_t argon2Lanes);
    void Save(QWidget* parent);
    void Close(QWidget* parent);
    void Lock(void);
    void Unlock(QWidget* parent,const char* password, uint32_t passwordSize);
    //bool IsOpen(){return mPasswordContainer.IsOpen();}
   // QString GetFileName(){return mPasswordContainer.GetFileName();}
    void InterruptEachAction(void);

signals:
    void ContainerEvent(int event, int error);

public slots:
    void ThreadFinished();

private:
    ///////////////////////////////////////////////////////////
    /// Enums

    ///////////////////////////////////////////////////////////
    /// Methods
    virtual void run() override;
    Event_t mCommand = eCmdNo;
    ContainerEncrypted::Error_t mError=ContainerEncrypted::eNoError;

    std::string mFileName;
    const char* mPassword;
    uint32_t mPasswordSize;

    const char *mOldPassword;
    uint32_t    mOldPasswordSize;
    const char *mNewPassword;
    uint32_t    mNewPasswordSize;
    uint32_t    mArgon2ItCost;
    uint32_t    mArgon2MemCost;
    uint32_t    mArgon2Lanes;
    DialogWait  mDialogWait;
    QWidget*    mParent;
};

#endif // PASSWORDCONTAINERTHREAD_H
