#include "dialogwait.h"
#include "ui_dialogwait.h"
#include <QMovie>
#include "passwordcontainerthread.h"

///
/// \brief      Constructor
/// \author     Joachim Danmayr
/// \date       2016-10-29
///
DialogWait::DialogWait(PasswordContainerThread *passwordContainer, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogWait),
    mPasswordContainer(passwordContainer)
{
    ui->setupUi(this);
    setWindowFlags( Qt::SplashScreen );

   // QLabel *lbl = new QLabel;
    QMovie *movie = new QMovie(":/Loading_icon.gif");
    ui->label->setMovie(movie);
    movie->start();
}

///
/// \brief      Destructor
/// \author     Joachim Danmayr
/// \date       2016-10-29
///
DialogWait::~DialogWait()
{
    delete ui;
}

///
/// \brief      Shows the wait dialog and resize it to its parent
/// \author     Joachim Danmayr
/// \date       2016-10-29
///
void DialogWait::ShowWaitDialog(QWidget *parent)
{
    ui->buttonCancle->setEnabled(true);
    int width = parent->size().width();
    int height = parent->size().height();

    setMaximumSize(width,height);
    setMinimumSize(width,height);
    setBaseSize(width,height);
    resize(width,height);
    setGeometry(parent->geometry());
    show();
}

///
/// \brief      Cancle button pressed
/// \author     Joachim Danmayr
/// \date       2016-10-29
///
void DialogWait::on_buttonCancle_clicked()
{
    ui->buttonCancle->setEnabled(false);
    mPasswordContainer->InterruptEachAction();
}
