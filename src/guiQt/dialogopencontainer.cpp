///
/// \file       dialogopencontainer.cpp
/// \author     Joachim Danmayr
/// \date       2016-07-22
///
/// \brief      Open container Dialog
///

#include "dialogopencontainer.h"
#include "ui_dialogopencontainer.h"
#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>
#include <QStringList>
#include "../core/Crypto/Secure.h"

///
/// \brief      Constructor
/// \author     Joachim Danmayr
/// \date       2016-07-22
///
/// \param[in] settings             Pointer to the settings class
/// \param[in] passwordContainer    Pointer to the password container
/// \param[in] parent               Pointer to the parent window
///
DialogOpenContainer::DialogOpenContainer(Configuration* settings, PasswordContainerThread *passwordContainer, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogOpenContainer),
    mType(eOpen),
    mSettings(settings),
    mPasswordContainer(passwordContainer),
    mOpenOk(false)
{
    mPasswordContainerFileName.clear();
    ui->setupUi(this);

    QObject::connect(passwordContainer,SIGNAL(ContainerEvent(int,int)),this,SLOT(on_PasswordThreadFinished(int,int)));
}


///
/// \brief      Destructor
/// \author     Joachim Danmayr
/// \date       2016-07-22
///
DialogOpenContainer::~DialogOpenContainer()
{
    delete ui;
}

///
/// \brief      Shows the open container dialog
/// \author     Joachim Danmayr
/// \date       2016-07-22
///
/// \return     Path of the opened container else 0
///             if open was not successful
///
QString DialogOpenContainer::ShowDialog()
{
    mType = eOpen;
    LoadFileHistoryToComboBox();
    if(ui->textFileName->count()<=0){
        on_buttonLoad_clicked();
    }

    ui->textPassword->setFocus();
    exec();

    return mPasswordContainerFileName;
}


///
/// \brief      Shows the an unlock dialog
/// \author     Joachim Danmayr
/// \date       2016-07-22
///
/// \return     true if unlock was succesful
///
bool DialogOpenContainer::ShowUnlockDialog()
{
    mType = eUnlock;
    ui->textFileName->setEnabled(false);
    ui->buttonLoad->setEnabled(false);
    ui->textFileName->setEditable(true);
    ui->textFileName->setCurrentText(QString::fromStdString(mPasswordContainer->GetFileName()));
    ui->textPassword->setFocus();
    exec();
    return mOpenOk;
}


///
/// \brief      OK button pressed
/// \author     Joachim Danmayr
/// \date       2016-07-22
///
void DialogOpenContainer::accept()
{
    setWindowOpacity(0);
    mPassword = ui->textPassword->text().toStdString();
    mPasswordPointer = mPassword.data();
    uint32_t passwordSize =  mPassword.size();

    if(eOpen == mType){
        SaveFileHistoryFromComboBox();
        mPasswordContainerFileName = ui->textFileName->currentData().toString();
        mPasswordContainer->Open(this,mPasswordContainerFileName.toStdString().data(),mPasswordPointer,passwordSize);
    }
    else if(eUnlock == mType){
        mPasswordContainer->Unlock(this,mPasswordPointer,passwordSize);
    }
}

///
/// \brief      Password thread has been finsihed
/// \author     Joachim Danmayr
/// \date       2016-07-22
///
void DialogOpenContainer::on_PasswordThreadFinished(int event, int error)
{
    if(PasswordContainerThread::eCmdOpen == event || PasswordContainerThread::eCmdUnlock == event){
        crypto::SecureClearString(&mPassword);
        ui->textPassword->clear();

        if(ContainerEncrypted::Error_t::eNoError == error){
            mOpenOk = true;
            close();
        }
        else{
            mOpenOk = false;
            setWindowOpacity(1);
            mPasswordContainerFileName.clear();
            QMessageBox::warning(this,tr("Can not open"),tr("Wrong password or container file!"));
        }
    }
}


///
/// \brief      Loads the file history of the last loaded database files
/// \author     Joachim Danmayr
/// \date       2016-07-22
///
void DialogOpenContainer::LoadFileHistoryToComboBox()
{
    // Load file history
    QStringList fileHistory = mSettings->GetFileHistory();
    for(int n=0;n<fileHistory.size();n++){
        ui->textFileName->insertItem(n,fileHistory.at(n).right(50),fileHistory.at(n));
    }
    ui->textFileName->setCurrentIndex(mSettings->GetFileHistoryLastOpenIdx());

}

///
/// \brief      Saves the file history from combo box
///             to settings
/// \author     Joachim Danmayr
/// \date       2016-07-22
///
void DialogOpenContainer::SaveFileHistoryFromComboBox()
{
    mSettings->SetFileHistoryLastOpenIdx(ui->textFileName->currentIndex());
    QStringList comboEntries;
    for(int n=0;n< ui->textFileName->count();n++){
        comboEntries.insert(n,ui->textFileName->itemData(n).toString());
    }
    mSettings->SetFileHistory(comboEntries);
}

///
/// \brief      Load file button has been pressed
///             to settings
/// \author     Joachim Danmayr
/// \date       2016-07-22
///
void DialogOpenContainer::on_buttonLoad_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("Open password container"), "", tr("Container Files (*.jpcx)"));
    int index = ui->textFileName->findText(fileName);
    if(index<0){
        this->ui->textFileName->insertItem(0,fileName.right(50),fileName);
        this->ui->textFileName->setCurrentIndex(0);
    }else{
        this->ui->textFileName->setCurrentIndex(index);
    }
}

///
/// \brief      Cancle clicked
/// \author     Joachim Danmayr
/// \date       2016-11-27
///
void DialogOpenContainer::on_buttonBox_rejected()
{
    close();
}
