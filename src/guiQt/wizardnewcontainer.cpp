#include "wizardnewcontainer.h"
#include "ui_wizardnewcontainer.h"
#include <QMessageBox>
#include <QFile>
#include <QFileDialog>
#include "../core/Crypto/hashing/argon.h"
#include "../core/Crypto/Secure.h"
#include "../core/Crypto/PasswordGenerator.h"

WizardNewContainer::WizardNewContainer(Configuration* settings, PasswordContainerThread *passwordContainer, QWidget *parent) :
    QWizard(parent),
    ui(new Ui::WizardNewContainer),
    mSettings(settings),
    mPasswordContainer(passwordContainer),
    mArgonThread(this),
    mOkPressed(false)
{
    ui->setupUi(this);

    connect(passwordContainer,SIGNAL(ContainerEvent(int,int)),this,SLOT(on_PasswordThreadFinished(int, int)));
    connect(&mTimer,SIGNAL(timeout()),this,SLOT(on_timerTimeout()));
}

WizardNewContainer::~WizardNewContainer()
{
    disconnect(mPasswordContainer,SIGNAL(finished()),this,SLOT(on_PasswordThreadFinished()));
    CleanUp();
    delete ui;
}


/// \brief      Shows the new container dialog
/// \author     Joachim Danmayr
/// \date       2016-07-22
///
/// \return     TRUE if open was successful
///
bool WizardNewContainer::ShowDialog(DialogType_t type)
{
    mDialogType = type;

    if(eNewContainer==type){
        /// New container dialog
        on_buttonSelectFile_clicked();        // Show open file dialog
        if(ui->textFileName->text().size() >0){
            ui->textPassword->setFocus();
            ui->textOldPassword->setVisible(false);
            ui->labelOldPassword->setVisible(false);
            emit ui->wizardPage1->completeChanged();
            show();
            ArePasswordsEqual();
            exec();
        }
    }else if(eNewPassword == type){
        ui->labelFilename->setVisible(false);
        ui->textFileName->setVisible(false);
        ui->buttonSelectFile->setVisible(false);

        // Load argon2 settings
        uint32_t itCost,memCost,lanes;
        mPasswordContainer->GetArgon2Setting(itCost,memCost,lanes);
        ui->spinIterationCost->setValue(itCost);
        ui->spinMemoryCost->setValue(memCost);
        ui->spinLanes->setValue(lanes);
        show();
        ArePasswordsEqual();
        exec();
    }
    return mOkPressed;
}

///
/// \brief      Finished button clicked
/// \date       2016-07-22
///
void WizardNewContainer::on_WizardNewContainer_finished(int result)
{
    Q_UNUSED(result);

}

///
/// \brief      Finished button clicked
/// \date       2016-07-22
///
void WizardNewContainer::on_WizardNewContainer_accepted()
{
    mOkPressed = false;
    switch(mDialogType){
    case eNewContainer:
        CreateContainer();
        break;
    case eNewPassword:
        ChangePassword();
        break;
    }
}

///
/// \brief      Password thread has been finsihed
/// \author     Joachim Danmayr
/// \date       2016-07-22
///
void WizardNewContainer::on_PasswordThreadFinished(int event, int error)
{
    ui->textOldPassword->clear();
    ui->textPassword->clear();
    ui->textRepeatedPassword->clear();
    crypto::SecureClearString(&mPassword);
    crypto::SecureClearString(&mPasswordOld);

    switch(mDialogType){
    case eNewContainer:
        CreateContainerFinsihed(error);
        break;
    case eNewPassword:
        ChangePasswordFinished(error);
        break;
    }
}

///
/// \brief      Create a new container
/// \author     Joachim Danmayr
/// \date       2016-07-22
///
void WizardNewContainer::CreateContainer()
{
    //----------------------------------------------------------------------------------
    // Check if both passwords are equal
    if(true==ArePasswordsEqual()){
        QFile file(ui->textFileName->text());
        file.remove();
        bool createOk = file.open(QIODevice::ReadWrite);
        file.close();

        if(true==createOk){
            mPassword = ui->textPassword->text().toStdString();
            uint32_t passwordSize = mPassword.size();
            mPasswordContainer->New(this,ui->textFileName->text().toStdString(),mPassword.c_str(),passwordSize,ui->spinIterationCost->value(),ui->spinMemoryCost->value(),ui->spinLanes->value());
        }
    }
    else{
        mOkPressed = false;
        QMessageBox::warning(this,tr("Can not create"),tr("Password and repeated password are not equal or empty!"));
    }
}

///
/// \brief      Change password
/// \author     Joachim Danmayr
/// \date       2016-07-22
///
void WizardNewContainer::ChangePassword()
{
    //----------------------------------------------------------------------------------
    // Check if both passwords are equal
    if(true==ArePasswordsEqual()){
        mPassword = ui->textPassword->text().toStdString();
        mPasswordOld = ui->textOldPassword->text().toStdString();
        uint32_t passwordSize = mPassword.size();
        uint32_t passwordOldSize = mPasswordOld.size();
        mPasswordContainer->ChangePassword(this,mPasswordOld.c_str(),passwordOldSize,mPassword.c_str(),passwordSize,ui->spinIterationCost->value(),ui->spinMemoryCost->value(),ui->spinLanes->value());
    }
    else{
        Restart();
        mOkPressed = false;
        QMessageBox::warning(this,tr("Can not change"),tr("Password and repeated password are not equal or empty!"));
    }
}

///
/// \brief      Container has been created
/// \author     Joachim Danmayr
/// \date       2016-07-22
///
void WizardNewContainer::CreateContainerFinsihed(int error)
{
    mOkPressed = false;
    if(ContainerEncrypted::eNoError == error){
        mOkPressed = true;
    }
    if(true == mOkPressed){
        //----------------------------------------------------------------------------------
        // Add new file to file history
        QVariant loaded = mSettings->GetFileHistory();
        QStringList fileHistory = loaded.toStringList();
        QString fileToEnter = ui->textFileName->text();
        if(false == fileHistory.contains(fileToEnter)){
            fileHistory.insert(0,fileToEnter);
            mSettings->SetFileHistory(fileHistory);
            mSettings->SetFileHistoryLastOpenIdx(0);
        }

        //----------------------------------------------------------------------------------
        // Close dialog
        CleanUp();
        QMessageBox::information(this,tr("Create succesful"),tr("New password container created!"));
        close();
    }
    else{
        QMessageBox::warning(this,tr("Can not create"),tr("Error during saving file!"));
    }
}

///
/// \brief      Password has been changed
/// \author     Joachim Danmayr
/// \date       2016-07-22
///
void WizardNewContainer::ChangePasswordFinished(int error)
{
    mOkPressed = false;
    if(ContainerEncrypted::eNoError == error){
        mOkPressed = true;
    }
    if(true == mOkPressed){
        //----------------------------------------------------------------------------------
        // Close dialog
        CleanUp();
        QMessageBox::information(this,tr("Change succesful"),tr("Password changed!"));
        close();
    }
    else{
        Restart();
        QMessageBox::warning(this,tr("Can not change!"),tr("Old password not correct!"));
    }
}

///
/// \brief      Restart wizard
/// \date       2016-07-22
///
void WizardNewContainer::Restart()
{
    emit back();
    show();
    ArePasswordsEqual();
}

///
/// \brief      Cancel button pressed
/// \date       2016-07-22
///
void WizardNewContainer::on_WizardNewContainer_rejected()
{
    CleanUp();
    mOkPressed = false;
}

///
/// \brief      Returns true if password and repeated password are equal
/// \author     Joachim Danmayr
/// \date       2016-07-22
///
bool WizardNewContainer::ArePasswordsEqual()
{
    if((ui->textPassword->text().size() > 0) && (0==ui->textPassword->text().compare(ui->textRepeatedPassword->text()))){
        ui->textRepeatedPassword->setStyleSheet(ui->textPassword->styleSheet());
        button(QWizard::NextButton)->setEnabled(true);
        return true;
    }
    else{
        ui->textRepeatedPassword->setStyleSheet("");
        button(QWizard::NextButton)->setEnabled(false);
        return false;
    }
}


///
/// \brief      Page changed
/// \author     Joachim Danmayr
/// \date       2016-07-22
///
void WizardNewContainer::on_WizardNewContainer_currentIdChanged(int id)
{
    if(WIZARD_PAGE2==id){
        on_buttonTestArgon_clicked();
    }
}

///
/// \brief      Select file clicked
/// \author     Joachim Danmayr
/// \date       2016-07-22
///
void WizardNewContainer::on_buttonSelectFile_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this,tr("Save password container"), "", tr("Container Files (*.jpcx)"));
    ui->textFileName->setText(fileName);
}

///
/// \brief      Filename text changed
/// \author     Joachim Danmayr
/// \date       2016-07-22
///
void WizardNewContainer::on_textFileName_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    ArePasswordsEqual();
}

///
/// \brief      Password was edited
/// \author     Joachim Danmayr
/// \date       2016-07-22
///
void WizardNewContainer::on_textPassword_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    ArePasswordsEqual();
    CheckPasswordQuality(arg1);
}

///
/// \brief      Repeated password was edited
/// \author     Joachim Danmayr
/// \date       2016-07-22
///
void WizardNewContainer::on_textRepeatedPassword_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    ArePasswordsEqual();
}

///
/// \brief      Checks if the password is strong or not
/// \author     Joachim Danmayr
/// \date       2016-07-22
///
void WizardNewContainer::CheckPasswordQuality(const QString &arg1)
{
    QString password = ui->textPassword->text();
    float entropy = sec::PasswordGenerator::CheckQuality(password.toStdString());
    ui->progressQuality->setValue(entropy);

    ui->textPassword->setStyleSheet("");
    int size =arg1.size();

    //if(true == mPasswordList.contains(password)){
    //    ui->labelPasswordSafety->setText(tr("Your password is a common password and not safe!"));
    //    ui->textPassword->setStyleSheet("background:#F55454;");
    //}
   if(size<=0){
        ui->labelPasswordSafety->setText(tr("Please enter a password!"));
        ui->textPassword->setStyleSheet("background:#F55454;");
    }
   else if(entropy<VERY_BAD_PASSWORD){
       ui->labelPasswordSafety->setText(tr("Common password! Do not use this!"));
       ui->textPassword->setStyleSheet("background:#F55454;");
    }
    else if(entropy<BAD_PASSWORD){
        ui->labelPasswordSafety->setText(tr("For a computer it is easy to hack!"));
        ui->textPassword->setStyleSheet("background:#F5AC54;");
    }else{
        ui->labelPasswordSafety->setText(tr("Looks like be a good password!"));
        ui->textPassword->setStyleSheet("background:#54F58A;");
    }
    ArePasswordsEqual();
}

///
/// \brief      Password editing finsihed
/// \author     Joachim Danmayr
/// \date       2016-07-22
///
void WizardNewContainer::on_textRepeatedPassword_editingFinished()
{

}


///
/// \brief      Page1 complete state changed
/// \author     Joachim Danmayr
/// \date       2016-07-22
///
void WizardNewContainer::on_wizardPage1_completeChanged()
{
    ArePasswordsEqual();
}

///
/// \brief      Argon2 test button
/// \author     Joachim Danmayr
/// \date       2016-07-22
///
void WizardNewContainer::on_buttonTestArgon_clicked()
{
    if(true == mArgonThread.isRunning()){
        mArgonThread.stop();
    }
    while(mArgonThread.isRunning()){
    }
    mArgonCounter = 0;
    mTimer.start(1);
    mArgonThread.start();
}

///
/// \brief      1ms timer timeout
/// \author     Joachim Danmayr
/// \date       2016-07-22
///
void WizardNewContainer::on_timerTimeout()
{
    mArgonCounter++;
    QString str = QString(tr("Hashing:")+"%1").arg(mArgonCounter);
    if(mArgonCounter >= PROGRESSBAR_TIME_MS){
        ui->progressBar->setMaximum(mArgonCounter);
    }else{
        ui->progressBar->setMaximum(PROGRESSBAR_TIME_MS);
    }
    ui->progressBar->setValue(mArgonCounter);

    if(mArgonThread.isFinished()){
        mTimer.stop();
    }
}


///
/// \brief      Lanes value changed
/// \author     Joachim Danmayr
/// \date       2016-07-22
///
void WizardNewContainer::on_spinLanes_valueChanged(int arg1)
{
    Q_UNUSED(arg1);
    //on_buttonTestArgon_clicked();
}

///
/// \brief      Memorycost value changed
/// \author     Joachim Danmayr
/// \date       2016-07-22
///
void WizardNewContainer::on_spinMemoryCost_valueChanged(int arg1)
{
    Q_UNUSED(arg1);
    //on_buttonTestArgon_clicked();
}

///
/// \brief      Iterationcost value changed finished
/// \author     Joachim Danmayr
/// \date       2016-07-22
///
void WizardNewContainer::on_spinIterationCost_valueChanged(int arg1)
{
    Q_UNUSED(arg1);
    //on_buttonTestArgon_clicked();
}

///
/// \brief      Runs argon2 calculatation
/// \author     Joachim Danmayr
/// \date       2016-07-22
///
void WizardNewContainer::ArgonThread::run()
{
    char salt[32];
    char dataIn[32];
    char hashOut[32];
    crypto::Argon2 argon(salt,32,mParent->ui->spinIterationCost->value(),mParent->ui->spinMemoryCost->value(),mParent->ui->spinLanes->value());
    mArgon = &argon;
    argon.Generate(dataIn,32,hashOut,crypto::Argon2::hash256);
}


///
/// \brief      Stops key generarion
/// \author     Joachim Danmayr
/// \date       2016-07-22
///
void WizardNewContainer::ArgonThread::stop()
{
    if(0!=mArgon){
        mArgon->StopCalculation();
        mArgon = 0;
    }
}

///
/// \brief      Removes all passwords from RAM and clean all fields
/// \author     Joachim Danmayr
/// \date       2016-07-22
///
void WizardNewContainer::CleanUp()
{
    mDialogType = eNone;
    restart();
    mOkPressed=false;
    ui->textFileName->clear();
    ui->textOldPassword->clear();
    ui->textPassword->clear();
    ui->textRepeatedPassword->clear();
    crypto::SecureClearString(&mPassword);
    ui->textPassword->setStyleSheet("");
    ui->textRepeatedPassword->setStyleSheet("");
}

