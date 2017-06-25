#include "dialogabout.h"
#include "ui_dialogabout.h"
#include "../version.h"



DialogAbout::DialogAbout(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogAbout)
{
    ui->setupUi(this);
    QString versionText = QString("<html><head/><body><p align=\"center\">Version %1, %2</p></body></html>").arg(VER_PRODUCTVERSION_STR).arg(VER_BUILD_DATE_STR);
    ui->labelVersion->setText(versionText);
}

DialogAbout::~DialogAbout()
{
    delete ui;
}
