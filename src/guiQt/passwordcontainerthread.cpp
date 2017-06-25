#include "passwordcontainerthread.h"
#include "../core/Crypto/hashing/argon.h"

///
/// \brief      Constructor
/// \author     Joachim Danmayr
/// \date       2016-10-29
///
PasswordContainerThread::PasswordContainerThread(QWidget* parent):
    mDialogWait(this,parent),
    mCommand(eCmdNo)
{
    connect(this,SIGNAL(finished()),this,SLOT(ThreadFinished()));
}

///
/// \brief      New container
/// \author     Joachim Danmayr
/// \date       2016-10-29
///
void PasswordContainerThread::New(QWidget* parent,std::string fileName, const char *password, uint32_t passwordSize, uint32_t argon2ItCost, uint32_t argon2MemCost, uint32_t argon2Lanes)
{
    mDialogWait.ShowWaitDialog(parent);
    mFileName = fileName;
    mPassword = password;
    mPasswordSize = passwordSize;
    mArgon2ItCost=argon2ItCost;
    mArgon2MemCost=argon2MemCost;
    mArgon2Lanes=argon2Lanes;
    mCommand = eCmdNew;
    start();
}


///
/// \brief      Open the container
/// \author     Joachim Danmayr
/// \date       2016-10-29
///
void PasswordContainerThread::Open(QWidget* parent,std::string fileName, const char *password, uint32_t passwordSize)
{
    mDialogWait.ShowWaitDialog(parent);
    mFileName = fileName;
    mPassword = password;
    mPasswordSize = passwordSize;
    mCommand = eCmdOpen;
    start();
}

///
/// \brief      Locks the Container
/// \author     Joachim Danmayr
/// \date       2016-10-29
///
void PasswordContainerThread::Lock(void)
{
    ContainerEncrypted::Error_t error = ContainerEncrypted::Lock();
    emit ContainerEvent(eCmdLock,error);
}

///
/// \brief      Unlock the container
/// \author     Joachim Danmayr
/// \date       2016-10-29
///
void PasswordContainerThread::Unlock(QWidget* parent,const char* password, uint32_t passwordSize)
{
    mDialogWait.ShowWaitDialog(parent);
    mPassword = password;
    mPasswordSize = passwordSize;
    mCommand = eCmdUnlock;
    start();
}


///
/// \brief      Change password
/// \author     Joachim Danmayr
/// \date       2016-10-29
///
void PasswordContainerThread::ChangePassword(QWidget* parent,const char *oldPassword, uint32_t oldPasswordSize,const char *newPassword, uint32_t newPasswordSize, uint32_t argon2ItCost, uint32_t argon2MemCost, uint32_t argon2Lanes)
{
    mDialogWait.ShowWaitDialog(parent);
    mOldPassword=oldPassword;
    mOldPasswordSize=oldPasswordSize;
    mNewPassword=newPassword;
    mNewPasswordSize=newPasswordSize;
    mArgon2ItCost=argon2ItCost;
    mArgon2MemCost=argon2MemCost;
    mArgon2Lanes=argon2Lanes;
    mCommand = eCmdChangePwd;
    start();
}

///
/// \brief      Save the container
/// \author     Joachim Danmayr
/// \date       2016-10-29
///
void PasswordContainerThread::Save(QWidget* parent)
{
    mDialogWait.ShowWaitDialog(parent);
    mCommand = eCmdSave;
    start();
}

///
/// \brief      Close the container
/// \author     Joachim Danmayr
/// \date       2016-10-29
///
void PasswordContainerThread::Close(QWidget *parent)
{
    Q_UNUSED(parent);
    mCommand = eCmdClose;
    start();
}

///
/// \brief      Thread has been finished
/// \author     Joachim Danmayr
/// \date       2016-10-29
///
void PasswordContainerThread::ThreadFinished()
{
    mDialogWait.close();
    emit ContainerEvent(mCommand,mError);
}

///
/// \brief      Run method of the thread
/// \author     Joachim Danmayr
/// \date       2016-10-29
///
void PasswordContainerThread::run()
{
    switch(mCommand){
    case eCmdNew:
        mError = ContainerEncrypted::New(mFileName,mPassword,mPasswordSize,mArgon2ItCost,mArgon2MemCost,mArgon2Lanes);
        break;

    case eCmdOpen:
        mError = ContainerEncrypted::Open(mFileName,mPassword,mPasswordSize);
        break;

    case eCmdUnlock:
         mError = ContainerEncrypted::Unlock(mPassword,mPasswordSize);
        break;

    case eCmdSave:
        mError = ContainerEncrypted::Save();
        break;

    case eCmdClose:
        mError = ContainerEncrypted::Close();
        break;

    case eCmdChangePwd:
        mError = ContainerEncrypted::ChangePassword(mOldPassword,mOldPasswordSize,mNewPassword,mNewPasswordSize,mArgon2ItCost,mArgon2MemCost,mArgon2Lanes);
        break;

    case eCmdNo:
        // Nothing to do
        break;
    }
}

///
/// \brief      Interrupt a running thread
/// \author     Joachim Danmayr
/// \date       2016-10-29
///
void PasswordContainerThread::InterruptEachAction(void)
{
    stopArgon2();
}
