///
/// \file       dialogpasswordgenerator.cpp
/// \author     Joachim Danmayr
/// \date       2016-07-24
///
/// \brief      Password generator window
///

#include <QDateTime>
#include "DialogPasswordGenerator.h"
#include "ui_DialogPasswordGenerator.h"
#include "../core/Crypto/PasswordGenerator.h"

///
/// \brief      Constructor
/// \author     Joachim Danmayr
/// \date       2016-09-21
///
/// \param[in]  parent  Pointer to the parent widget
///
DialogPasswordGenerator::DialogPasswordGenerator(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogPasswordGenerator),
    mGeneratedPassword("")
{
    ui->setupUi(this);
}

///
/// \brief      Destructor
/// \author     Joachim Danmayr
/// \date       2016-07-24
///
DialogPasswordGenerator::~DialogPasswordGenerator()
{
    delete ui;
}

///
/// \brief      Shows the create password dialog
/// \author     Joachim Danmayr
/// \date       2016-07-24
///
QString DialogPasswordGenerator::Show()
{
    exec();
    return mGeneratedPassword;
}

///
/// \brief      Generates a new random password
/// \author     Joachim Danmayr
/// \date       2016-07-24
///
void DialogPasswordGenerator::GenerataPassword()
{

    uint32_t currenTimeInms = QDateTime::currentMSecsSinceEpoch() - QDateTime(QDate::currentDate()).toMSecsSinceEpoch();


    uint32_t startvalue = mMouseMove ^currenTimeInms;
    sec::PasswordGenerator generator(ui->textLengthOfPassword->value(),
                                     ui->checkUpperCase->isChecked(),
                                     ui->checkLowerCase->isChecked(),
                                     ui->checkDigits->isChecked(),
                                     ui->checkMinusAndPlus->isChecked(),
                                     ui->checkUnterlind->isChecked(),
                                     ui->checkSpecial->isChecked(),
                                     ui->checkBrackets->isChecked(),
                                     ui->checkSpaces->isChecked());

    int32_t minNrOfDigits = ui->textMinNrOfDigits->value();
    int32_t minNrOfUpperCase = ui->textMinNrOfUpperCase->value();
    int32_t minNrOfLowerCase = ui->textMinNrOfLowerCase->value();

    ui->textGeneratedPassword->setText(QString::fromStdString(generator.GeneratePassword(startvalue,minNrOfDigits,minNrOfUpperCase,minNrOfLowerCase)));
}

///
/// \brief      Mouse move event
/// \author     Joachim Danmayr
/// \date       2016-07-24
///
void DialogPasswordGenerator::mouseMoveEvent (QMouseEvent * event)
{
    mMouseMove+=event->globalX();
}

///
/// \brief      Generates password clicked
/// \author     Joachim Danmayr
/// \date       2016-07-24
///
void DialogPasswordGenerator::on_buttonGenerate_clicked()
{
    GenerataPassword();
}

///
/// \brief      Ok button pressed
/// \author     Joachim Danmayr
/// \date       2016-07-24
///
void DialogPasswordGenerator::on_buttonBox_accepted()
{
    mGeneratedPassword = ui->textGeneratedPassword->text();
}
