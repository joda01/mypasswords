#include "dialogcomment.h"
#include "ui_dialogcomment.h"
#include <QMessageBox>
#include <QIcon>
#include "../version.h"

DialogComment::DialogComment(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogComment)
{
    ui->setupUi(this);

    ui->comboBox->addItem(tr("--- please choose ---"));
    ui->comboBox->addItem(QIcon(":icons_black/icons_black/bug1_black.png"),tr("Bug report"),tr("bug"));
    ui->comboBox->addItem(QIcon(":icons_black/icons_black/thumb_up1_black.png"),tr("Like"),tr("like"));
    ui->comboBox->addItem(QIcon(":icons_black/icons_black/thumbs_down1_black.png"),tr("Dislike"),tr("dislike"));
    ui->comboBox->addItem(QIcon(":icons_black/icons_black/star1_black.png"),tr("Feature request"),tr("feature_request"));
    ui->comboBox->addItem(QIcon(":icons_black/icons_black/question_black.png"),tr("Support request"),tr("support"));
    ui->comboBox->addItem(QIcon(":icons_black/icons_black/view_details1_black.png"),tr("Something else"),tr("other"));
}

DialogComment::~DialogComment()
{
    delete ui;
}

///
/// \brief      Send comment button clicked
/// \author     Joachim Danmayr
/// \date       2016-11-20
///
void DialogComment::on_buttonSend_clicked()
{
    if(ui->comboBox->currentIndex()==0){
        QMessageBox::warning(this,tr("Feedback..."),tr("Please select a category!"));
        if(ui->textComment->toPlainText().length() < 10){
            QMessageBox::warning(this,tr("Feedback..."),tr("Please enter a comment with at least 10 signs!"));
        }
    }
    else
    {
        QString textToSend("<p>Version:");
        textToSend.append(VER_PRODUCTVERSION_STR).append("</p>");
        textToSend.append("<p>Name: ").append(ui->textName->text()).append("</p>");
        textToSend.append("<p>E-Mail: ").append(ui->textMail->text()).append("</p>");
        textToSend.append("<p>Category: ").append(ui->comboBox->currentData().toString()).append("</p>");
        textToSend.append("<p>Text: ").append(ui->textComment->toPlainText()).append("</p>");

        Smtp smtp("smtp.gmx.net", "mail@mail.com","password");
        connect(&smtp,SIGNAL(successQuit()),SLOT(on_sendSuccessful()));
        connect(&smtp,SIGNAL(errorCloseAll()),SLOT(on_sendError()));

        smtp.send("from@","to@","feedback: PassDB",textToSend);
    }
}

///
/// \brief      Send successful
/// \author     Joachim Danmayr
/// \date       2016-11-20
///
void DialogComment::on_sendSuccessful()
{
    ui->buttonSend->setEnabled(false);
    ui->textComment->setEnabled(false);
    ui->textMail->setEnabled(false);
    ui->textName->setEnabled(false);
    ui->textSubject->setEnabled(false);
    ui->comboBox->setEnabled(false);
    QMessageBox::information(this,tr("Feedback..."),tr("Thank you for your feedback"));
}

///
/// \brief      Send error
/// \author     Joachim Danmayr
/// \date       2016-11-20
///
void DialogComment::on_sendError()
{
    QMessageBox::warning(this,tr("Feedback..."),tr("Sorry, could not send!\nPlease try again!"));
}

///
/// \brief      Close dialog
/// \author     Joachim Danmayr
/// \date       2016-11-20
///
void DialogComment::on_buttonClose_clicked()
{
    close();
}
