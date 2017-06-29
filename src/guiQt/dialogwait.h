#ifndef DIALOGWAIT_H
#define DIALOGWAIT_H

#include <QDialog>

class PasswordContainerThread;

namespace Ui {
class DialogWait;
}

///
/// \class      DialogWait
/// \author     Joachim Danmayr
/// \date       2016-10-29
///
/// \brief      Wait dialog
///
class DialogWait : public QDialog
{
    Q_OBJECT

public:
    explicit DialogWait(PasswordContainerThread* passwordContainer, QWidget *parent = 0);
    void ShowWaitDialog(QWidget *parent);
    ~DialogWait();

private slots:
    void on_buttonCancle_clicked();

private:
    Ui::DialogWait *ui;
    PasswordContainerThread* mPasswordContainer;
};

#endif // DIALOGWAIT_H
