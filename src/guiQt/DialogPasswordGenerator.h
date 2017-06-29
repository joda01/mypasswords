///
/// \file       dialogpasswordgenerator.h
/// \author     Joachim Danmayr
/// \date       2016-07-24
///
/// \brief      Password generator window
///

#ifndef DIALOGPASSWORDGENERATOR_H
#define DIALOGPASSWORDGENERATOR_H

#include <QDialog>
#include <QString>
#include <QMouseEvent>

namespace Ui {
class DialogPasswordGenerator;
}


///
/// \class      DialogPasswordGenerator
/// \author     Joachim Danmayr
/// \date       2016-07-24
///
/// \brief      Password generator dialog class
///
class DialogPasswordGenerator : public QDialog
{
    Q_OBJECT

public:
    explicit DialogPasswordGenerator(QWidget *parent = 0);
    ~DialogPasswordGenerator();
    QString Show();

private slots:
    void on_buttonGenerate_clicked();

    void on_buttonBox_accepted();

private:
    static const int MAX_PASSWORD_LENGTH = 32;

    void GenerataPassword();
    virtual void mouseMoveEvent (QMouseEvent * event);

    std::string PrepareCharSet();

    Ui::DialogPasswordGenerator *ui;
    QString mGeneratedPassword;
    uint32_t mMouseMove;

};

#endif // DIALOGPASSWORDGENERATOR_H
