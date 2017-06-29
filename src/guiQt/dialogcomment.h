#ifndef DIALOGCOMMENT_H
#define DIALOGCOMMENT_H

#include <QDialog>
#include "../web/smtp.h"


namespace Ui {
class DialogComment;
}

class DialogComment : public QDialog
{
    Q_OBJECT

public:
    explicit DialogComment(QWidget *parent = 0);
    ~DialogComment();

private slots:
    void on_buttonSend_clicked();
    void on_sendSuccessful();
    void on_sendError();

    void on_buttonClose_clicked();

private:
    Ui::DialogComment *ui;
};

#endif // DIALOGCOMMENT_H
