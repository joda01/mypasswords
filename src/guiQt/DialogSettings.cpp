#include <QMessageBox>
#include "DialogSettings.h"
#include "ui_DialogSettings.h"
#include "../core/Crypto/Secure.h"



DialogSettings::DialogSettings(Configuration *settings,PasswordContainerThread* container, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSettings),
    mContainer(container),
    mSettings(settings),
    mNewPassword(settings,container,this)

{
    ui->setupUi(this);
    LoadSettings();

}

DialogSettings::~DialogSettings()
{
    delete ui;
}

///
/// \brief      Load settings
/// \author     Joachim Danmayr
/// \date       2016-11-13
///
void DialogSettings::LoadSettings()
{
    // Enable or disable change password button
    ui->buttonChangePassword->setEnabled(mContainer->IsOpen());

    ui->keySequenceOpenUrl->setKeySequence(mSettings->GetKeySequenceOpenUrl());
    ui->keySequenceCopyUsername->setKeySequence(mSettings->GetKeySequenceCopyUsername());
    ui->keySequenceCopyPassword->setKeySequence(mSettings->GetKeySequenceCopyPassword());
    ui->keySequencePerformAutoType->setKeySequence(mSettings->GetKeySequencePerformAutoType());
    ui->keySequenceSave->setKeySequence(mSettings->GetKeySequenceSave());
    ui->keySequenceBrintToFront->setKeySequence(mSettings->GetKeySequenceBringToFront());

    QStringList languages;
    languages.append("en-US (English)");
    //languages.append("es-ES (Español)");
    //languages.append("fr-FR (Français)");
    //languages.append("de-DE (Deutsch)");
    //languages.append("it-IT (Italiano)");
    //languages.append("sv-SV (Svenska)");
    //languages.append("jp-JP (日本の)");
    QString actLang =mSettings->GetLanguage();
    foreach(QString language,languages){
        QString icon(":flags_color/flags_color/flag_") ;
        icon.append(language.split(" ")[0]).append(".png");
        ui->listLanguage->addItem(new QListWidgetItem(QIcon(icon),language));
        if(language.contains(actLang)){
            ui->listLanguage->item(ui->listLanguage->count()-1)->setSelected(true);
        }
    }
    ui->checkMinimizeWhenLocking->setChecked (mSettings->GetMinimizeWhenLocking() );
    ui->checkLockWhenMinimized->setChecked   (mSettings->GetLockWhenMinimized()   );
    ui->checkLockAfterTime->setChecked       (mSettings->GetLockAfterTime()       );
    ui->textInactivityTime->setValue         (mSettings->GetTimeBeforeLocking());
    ui->checkOpenLastDbOnStartUp->setChecked (mSettings->GetOpenLastDbOnStartUp() );
    ui->checkAutoSave->setChecked            (mSettings->GetAutoSave()            );
    ui->checkSecureDesktop->setChecked       (mSettings->GetSecureDesktop()       );
    ui->checkShowSubGroupElements->setChecked(mSettings->GetShowSubGroupElements());
    ui->checkHideFromTaskbar->setChecked(mSettings->GetHideFromTaskbarWhenMinimized());

    ui->textCommunicationAccessToken->setText(mSettings->GetPluginToken());
    ui->textPortNumber->setValue(mSettings->GetPluginPort());
    ui->checkEnableCommunicationPort->setChecked(mSettings->GetBrowserPluginEnable());
    on_checkEnableCommunicationPort_clicked(ui->checkEnableCommunicationPort->isChecked());
}

///
/// \brief      Save settings
/// \author     Joachim Danmayr
/// \date       2016-11-13
///
void DialogSettings::SaveSettings()
{
    mSettings->SetKeySequenceOpenUrl(ui->keySequenceOpenUrl->keySequence());
    mSettings->SetKeySequenceCopyUsername(ui->keySequenceCopyUsername->keySequence());
    mSettings->SetKeySequenceCopyPassword(ui->keySequenceCopyPassword->keySequence());
    mSettings->SetKeySequencePerformAutoType(ui->keySequencePerformAutoType->keySequence());
    mSettings->SetKeySequenceSave(ui->keySequenceSave->keySequence());
    mSettings->SetKeySequenceBringToFront(ui->keySequenceBrintToFront->keySequence());

    mSettings->SetMinimizeWhenLocking (ui->checkMinimizeWhenLocking->isChecked() );
    mSettings->SetLockWhenMinimized   (ui->checkLockWhenMinimized->isChecked()   );
    mSettings->SetLockAfterTime       (ui->checkLockAfterTime->isChecked()       );
    mSettings->SetTimeBeforeLocking   (ui->textInactivityTime->value());
    mSettings->SetOpenLastDbOnStartUp (ui->checkOpenLastDbOnStartUp->isChecked() );
    mSettings->SetAutoSave            (ui->checkAutoSave->isChecked()            );
    mSettings->SetSecureDesktop       (ui->checkSecureDesktop->isChecked()       );
    mSettings->SetShowSubGroupElements(ui->checkShowSubGroupElements->isChecked());
    mSettings->SetHideFromTaskbarWhenMinimized(ui->checkHideFromTaskbar->isChecked());

    mSettings->SetPluginToken(ui->textCommunicationAccessToken->text());
    mSettings->SetPluginPort(ui->textPortNumber->value());
    mSettings->SetBrowserPluginEnable(ui->checkEnableCommunicationPort->isChecked());

    QList<QListWidgetItem*> selectedItems = ui->listLanguage->selectedItems();

    if(selectedItems.size()>0){
        mSettings->SetLanguage(selectedItems[0]->data(Qt::DisplayRole).toString().split(" ")[0]);
    }
}


///
/// \brief      Change password button was clicked
/// \author     Joachim Danmayr
/// \date       2016-10-24
///
void DialogSettings::on_buttonChangePassword_clicked()
{
    mNewPassword.ShowDialog(WizardNewContainer::eNewPassword);
}

///
/// \brief      Ok button pressed
/// \author     Joachim Danmayr
/// \date       2016-10-24
///
void DialogSettings::on_buttonBox_accepted()
{
    SaveSettings();
    close();
}

///
/// \brief      Cancle pressed
/// \author     Joachim Danmayr
/// \date       2016-10-24
///
void DialogSettings::on_buttonBox_rejected()
{
    close();
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
uint32_t DialogSettings::TextFieldToString(QString string, char** data)
{
    QByteArray text = string.toUtf8();
    *data = new char[text.size()+1];
    strcpy(*data, text.data());
    return text.size();
}

///
/// \brief      New language has been selected
/// \author     Joachim Danmayr
/// \date       2016-11-19
///
void DialogSettings::on_listLanguage_itemSelectionChanged()
{
    //QMessageBox::information(this,tr("Language changed"),tr("Please restart to apply kanguage settings!"));
}

///
/// \brief      Enable plugin check box clicked
/// \author     Joachim Danmayr
/// \date       2016-11-19
///
void DialogSettings::on_checkEnableCommunicationPort_clicked(bool checked)
{
    ui->textPortNumber->setEnabled(checked);
    ui->textCommunicationAccessToken->setEnabled(checked);
}
