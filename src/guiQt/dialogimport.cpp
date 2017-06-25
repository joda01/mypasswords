///
/// \file       dialogimport.cpp
/// \author     Joachim Danmayr
/// \date       2016-11-12
///
/// \brief      Import from a CSV file
///

#include <QFile>


#include "dialogimport.h"
#include "ui_dialogimport.h"



DialogImport::DialogImport(Configuration* settings, PasswordContainerThread *container,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogImport)
{
    ui->setupUi(this);

    mFields << "Title" << "Username" << "Password" << "URL" << "Notes" << "Group" << "Tags";

    ui->listItems->addItems(mFields);

}

DialogImport::~DialogImport()
{
    delete ui;
}


void DialogImport::StartImport()
{
    QFile file(ui->textFileToImport);
    file.open(QFile::ReadOnly);

    QString line = QString::fromUtf8(file.readLine());
    line.split(";");

    Container::PasswordEntry* entry = new Container::PasswordEntry();
    Container::PasswordEntry::PasswordElement* element = new Container::PasswordEntry::PasswordElement();



    entry->AddPasswordElement(element);
    mContainer->AddPasswordEntry(entry);
}
