///
/// \file       dialogentry.cpp
/// \author     Joachim Danmayr
/// \date       2016-09-21
///
/// \brief      Password entry dialog file
///

#include "dialogpasswordenty.h"
#include "DialogPasswordGenerator.h"
#include "ui_dialogpasswordenty.h"
#include "../core/Crypto/Secure.h"
#include "../core/Crypto/PasswordGenerator.h"
#include "DialogPasswordGenerator.h"
#include <QDesktopServices>
#include <QDebug>
#include <QBuffer>
#include <QFileDialog>
#include <QFile>
#include <QMessageBox>

///
/// \brief      Constructor
/// \author     Joachim Danmayr
/// \date       2016-09-21
///
/// \param[in]  parent  Pointer to the parent widget
///
DialogPasswordEnty::DialogPasswordEnty(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogPasswordEnty),
    mEntry(0),
    mTableModel(0),
    mOkPressed(false)
{
    ui->setupUi(this);
}


///
/// \brief      Destructor
/// \author     Joachim Danmayr
/// \date       2016-09-21
///
DialogPasswordEnty::~DialogPasswordEnty()
{
    delete ui;
}

///
/// \brief      Shows the dialog and returns true if it has been closed with the OK button
/// \author     Joachim Danmayr
/// \date       2016-09-21
///
/// \param[in]  entry           Pointer to the password entry which should be edited
/// \param[in]  selectedGroupId Selected group id. This is used to make default group entry if a
///                             new element shoudl be created
///
bool DialogPasswordEnty::ShowDialog(Container* container, Container::PasswordEntry *entry, QString selectedGroupId)
{
    if(0==entry){
        qDebug("DialogEntry:ShowDialog:PasswordEntry* entry = 0");
        return false;
    }
    LoadGroups(container);
    // If there is no password element create one
    if(0==entry->GetNumberOfPasswordElement()){
        entry->AddPasswordElement(new Container::PasswordEntry::PasswordElement());
        entry->GetPasswordElement(0)->SetDateCreated();
        // Sets the actual selecetd group as group ID
        if(selectedGroupId.size()>0){
            ui->textGroup->setCurrentIndex(ui->textGroup->findData(selectedGroupId));
            char* data;int length = TextFieldToString(ui->textGroup->currentData().toString(),&data);
            entry->SetGroupId(data,length);
            crypto::SecureClearArray(data,length);
            delete [] data;
        }
    }
    mEntry = entry;
    Container::PasswordEntry::PasswordElement* element = entry->GetPasswordElement(0);

    mTableModel = new ModelPasswordElements(entry,ui->tableHistory);
    ui->tableHistory->setModel(mTableModel);


    LoadDataToTextField(element);

    exec();
    ClearTextFieldContent();
    return mOkPressed;
}


///
/// \brief      Ok button has been pressed
///             The textfield entries will be stored to the container file
///             and deleted then
/// \author     Joachim Danmayr
/// \date       2016-09-21
///
void DialogPasswordEnty::on_buttonBox_accepted()
{

    char* data;
    uint32_t length = 0;

    Container::PasswordEntry::PasswordElement* element = mEntry->GetPasswordElement(0);

    length = TextFieldToString(ui->textTitle->text(),&data);
    element->SetTitle(data,length);
    crypto::SecureClearArray(data,length);
    delete [] data;

    length = TextFieldToString(ui->textUsername->text(),&data);
    element->SetUsername(data,length);
    crypto::SecureClearArray(data,length);
    delete [] data;

    length = TextFieldToString(ui->textPassword->text(),&data);
    element->SetPassword(data,length);
    crypto::SecureClearArray(data,length);
    delete [] data;

    length = TextFieldToString(ui->textUrl->text(),&data);
    element->SetURL(data,length);
    crypto::SecureClearArray(data,length);
    delete [] data;

    length = TextFieldToString(ui->textNotes->toPlainText(),&data);
    element->SetNotes(data,length);
    crypto::SecureClearArray(data,length);
    delete [] data;

    length = TextFieldToString(ui->textTags->text(),&data);
    element->SetTags(data,length);
    crypto::SecureClearArray(data,length);
    delete [] data;

    if(ui->checkExpire->isChecked()){
        int32_t year =ui->dateExpire->dateTime().date().year();
        int32_t month=ui->dateExpire->dateTime().date().month();
        int32_t day=ui->dateExpire->dateTime().date().day();
        int32_t hour=ui->dateExpire->dateTime().time().hour();
        int32_t minute=ui->dateExpire->dateTime().time().minute();
        int32_t second=ui->dateExpire->dateTime().time().second();
        element->SetDateExpired(year,month,day,hour,minute,second);
    }else{
        element->ClearDateExpired();
    }


    length = TextFieldToString(ui->textGroup->currentData().toString(),&data);
    mEntry->SetGroupId(data,length);
    crypto::SecureClearArray(data,length);
    delete [] data;


    //// Save icon
    QPixmap pixmap = ui->buttonAddIcon->icon().pixmap(ICON_SIZE);
    QByteArray bArray; QBuffer buffer(&bArray);
    buffer.open(QIODevice::WriteOnly);
    pixmap.save(&buffer, "PNG");
    element->SetIcon((char*)bArray.constData(),bArray.size());

    mOkPressed = true;

}

///
/// \brief      Load all groups to the combo box
/// \author     Joachim Danmayr
/// \date       2016-09-21
///
void DialogPasswordEnty::LoadGroups(Container* container)
{
    int size = container->GetNumberOfGroupEntry();
    for(int n=0;n<size;n++){
        Container::GroupEntry* group = container->GetGroup(n);
        uint32_t sizeName,sizeId;
        char* name = group->GetName(sizeName);
        char* id = group->GetGroupId(sizeId);
        if(sizeId>0){
            ui->textGroup->addItem(StringToTextField(name,sizeName),StringToTextField(id,sizeId));
        }
    }
}


///
/// \brief      The content of the Password element will be loaded to the text field
/// \author     Joachim Danmayr
/// \date       2016-09-21
///
void DialogPasswordEnty::LoadDataToTextField(Container::PasswordEntry::PasswordElement* element)
{
    char* data;
    uint32_t size;
    data = element->GetTitle(size);
    ui->textTitle->setText(StringToTextField(data,size));

    data = element->GetUsername(size);
    ui->textUsername->setText(StringToTextField(data,size));

    data = element->GetPassword(size);
    ui->textPassword->setText(StringToTextField(data,size));
    element->ClosePassword();

    data = element->GetURL(size);
    ui->textUrl->setText(StringToTextField(data,size));

    data = element->GetNotes(size);
    ui->textNotes->setPlainText(StringToTextField(data,size));

    data = element->GetTags(size);
    ui->textTags->setText(StringToTextField(data,size));


    data = element->GetDateCreated(size);
    QString dateTimString = QString::fromUtf8(data,size);
    QDateTime dateTime = QDateTime::fromString(dateTimString,"yyyy-MM-ddTHH:mm:ss");
    ui->dateCreated->setDateTime(dateTime);

    data = element->GetDateModified(size);
    dateTimString = QString::fromUtf8(data,size);
    dateTime = QDateTime::fromString(dateTimString,"yyyy-MM-ddTHH:mm:ss");
    ui->dateModified->setDateTime(dateTime);

    data = element->GetDateExpire(size);
    if(size>0){
        ui->dateExpire->setEnabled(true);
        ui->checkExpire->setChecked(true);
        dateTimString = QString::fromUtf8(data,size);
        dateTime = QDateTime::fromString(dateTimString,"yyyy-MM-ddTHH:mm:ss");
        ui->dateExpire->setDateTime(dateTime);
    }


    data = mEntry->GetUID(size);
    ui->labeluuid->setText(StringToTextField(data,size));

    data = mEntry->GetGroupId(size);
    ui->textGroup->setCurrentIndex(ui->textGroup->findData(StringToTextField(data,size)));

    //// Load Icon
    data = element->GetIcon(size);
    if(size>0){
        QByteArray iconBuffer = QByteArray::fromRawData(data,size);
        QPixmap p; p.loadFromData(iconBuffer,"PNG");
        QIcon icon(p);
        ui->buttonAddIcon->setIcon(icon);
    }
}


void DialogPasswordEnty::ClearTextFieldContent(void)
{
    QString zero;
    zero.fill('*',ui->textPassword->text().size());
    ui->textPassword->setText(zero);
}

///
/// \brief      Cancle button pressed
/// \author     Joachim Danmayr
/// \date       2016-09-21
///
void DialogPasswordEnty::on_buttonBox_rejected()
{
    mOkPressed = false;
}

///
/// \brief      Converts a QString to a char array
/// \author     Joachim Danmayr
/// \date       2016-09-21
///
/// \param[in]  string  QString element which should be converted
/// \param[out] data    Pointer to the char array where data shouls be stored
/// \return     Number of elements in the data array
///
uint32_t DialogPasswordEnty::TextFieldToString(QString string, char** data)
{
    QByteArray text = string.toUtf8();
    *data = new char[text.size()+1];
    strcpy(*data, text.data());
    return text.size();
}

///
/// \brief      Converts a char array to a QString
/// \author     Joachim Danmayr
/// \date       2016-09-21
///
/// \param[in]  data    Pointer to the data array which should be converted
/// \param[in]  length  Number of data bytes in the array
/// \return     Converted string
///
QString DialogPasswordEnty::StringToTextField(const char* data, uint32_t length)
{

    return  QString::fromUtf8(data,length);
}

///
/// \brief      Show password button toogled
/// \author     Joachim Danmayr
/// \date       2016-09-21
///
void DialogPasswordEnty::on_buttonPwdShow_toggled(bool checked)
{
    if(false==checked){
        ui->textPassword->setEchoMode(QLineEdit::Password);
    }else
    {
        ui->textPassword->setEchoMode(QLineEdit::Normal);
    }
}


///
/// \brief      Text has been changed in password field
/// \author     Joachim Danmayr
/// \date       2016-09-21
///
/// \param[in]  text    Text in the password field
///
void DialogPasswordEnty::on_textPassword_textChanged(const QString &text)
{
    Q_UNUSED(text);
    float entropy = sec::PasswordGenerator::CheckQuality(text.toStdString());
    ui->progressBar->setValue(entropy);
}

///
/// \brief      Open URL clicked
/// \author     Joachim Danmayr
/// \date       2016-09-21
///
void DialogPasswordEnty::on_buttonOpenUrl_clicked()
{
    QDesktopServices::openUrl(QUrl(ui->textUrl->text()));
}


///
/// \brief      URL editing finished
///             Add http:// before the URL if it does not
///             start with https:// or httpt://
/// \author     Joachim Danmayr
/// \date       2016-09-21
///
void DialogPasswordEnty::on_textUrl_editingFinished(void)
{
    if(false==ui->textUrl->text().startsWith("http://")){
        if(false==ui->textUrl->text().startsWith("https://")){
        ui->textUrl->setText(ui->textUrl->text().insert(0,"https://"));
        }
    }
}

///
/// \brief      Add icon button has been pressed
/// \author     Joachim Danmayr
/// \date       2016-09-21
///
void DialogPasswordEnty::on_buttonAddIcon_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("Open icon"), "", tr("Image (*.png)"));
    QFile icon(fileName);
    if(icon.size()>ICON_MAX_FILE_SIZE){
        QString warningMessage = QString(tr("File size of icon should not be bigger than %1 bytes")).arg(ICON_MAX_FILE_SIZE);
        QMessageBox::warning(this,tr("Icon too big!"),warningMessage);
    }
    else{
        ui->buttonAddIcon->setIcon(QIcon(fileName));
    }
}

///
/// \brief      Open password creator buttin pressed
/// \author     Joachim Danmayr
/// \date       2016-09-21
///
void DialogPasswordEnty::on_buttonGenerator_clicked()
{
    DialogPasswordGenerator* dialog = new DialogPasswordGenerator(this);
    QString password = dialog->Show();
    delete dialog;
    if(password.isEmpty()==false){
        ui->textPassword->setText(password);
    }
}

///
/// \brief      Archive password clicked
///             The first password element will be added as a new
///             element to the archive
/// \author     Joachim Danmayr
/// \date       2016-09-23
///
void DialogPasswordEnty::on_buttonArchive_clicked()
{
    Container::PasswordEntry::PasswordElement* archive = new Container::PasswordEntry::PasswordElement();
    (*archive) = (*mEntry->GetPasswordElement(0));
    mEntry->AddPasswordElement(archive);
    mTableModel->UpdateView();
}

///
/// \brief      Remove from archive clicked
///             All selected archieved password elements will be
///             removed. The first one can not be removed. This is
///             the actuel element
/// \author     Joachim Danmayr
/// \date       2016-09-23
///
void DialogPasswordEnty::on_buttonRemoveFromArchive_clicked()
{
    QModelIndexList selecetRows= ui->tableHistory->selectionModel()->selectedRows();
    if(selecetRows.size()>0){
        for(int n=selecetRows.size()-1;n>=0; n--){
            // The first element is the actual one this element should not be removed
            if(0!=selecetRows.at(n).row()){
                mEntry->RemovePasswordElement(selecetRows.at(n).row());
            }
        }
         mTableModel->UpdateView();
    }
}

///
/// \brief      Restore button clicked
///             The selected archieved password will be restored
/// \author     Joachim Danmayr
/// \date       2016-09-23
///
void DialogPasswordEnty::on_buttonRestore_clicked()
{
    QModelIndexList selecetRows= ui->tableHistory->selectionModel()->selectedRows();
    if(selecetRows.size()>0){
        Container::PasswordEntry::PasswordElement* restoreElement = mEntry->GetPasswordElement(selecetRows.at(0).row());
        LoadDataToTextField(restoreElement);
    }
}

///
/// \brief      Tag editing has been finished
/// \author     Joachim Danmayr
/// \date       2016-09-23
///
void DialogPasswordEnty::on_textTags_editingFinished()
{
    ui->textTags->setText(ui->textTags->text().replace(" ",";"));
}

///
/// \brief      Tag editing has been finished
/// \author     Joachim Danmayr
/// \date       2016-09-23
///
void DialogPasswordEnty::on_textTags_textChanged(const QString &arg1)
{
    QString text = arg1;
    text.replace(" ",";");
    ui->textTags->setText(text);
}

///
/// \brief      Time expired checkbox clicked
/// \author     Joachim Danmayr
/// \date       2016-09-23
///
void DialogPasswordEnty::on_checkExpire_clicked(bool checked)
{
    ui->dateExpire->setEnabled(checked);
}
