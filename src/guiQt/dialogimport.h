///
/// \file       dialogimport.cpp
/// \author     Joachim Danmayr
/// \date       2016-11-12
///
/// \brief      Import from a CSV file
///

#ifndef DIALOGIMPORT_H
#define DIALOGIMPORT_H

#include "configuration.h"
#include "passwordcontainerthread.h"

#include <QDialog>

namespace Ui {
class DialogImport;
}



///
/// \class      DialogImport
/// \author     Joachim Danmayr
/// \date       2016-12-10
///
/// \brief      Import from a CSV file
///
class DialogImport : public QDialog
{
    Q_OBJECT

public:
    explicit DialogImport(Configuration *settings, PasswordContainerThread *container, QWidget *parent = 0);
    ~DialogImport();

private:
    void StartImport();

    Ui::DialogImport *ui;
    QStringList mFields;
    PasswordContainerThread *mContainer;


};

#endif // DIALOGIMPORT_H
