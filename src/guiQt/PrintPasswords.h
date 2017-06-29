/**
 * @file	PrintPasswords.h
 *
 * Printing support
 *
 * @author	Joachim Danmayr
 * @date	2016-07-26
 *
 */


#ifndef PRINTPASSWORDS_H
#define PRINTPASSWORDS_H

#include <QTextDocument>
#include <QObject>
#include <QWidget>
#include <QPrinter>
#include "../core/Container/Container.h"



class PrintPasswords : public QObject
{
    Q_OBJECT

public:
    PrintPasswords();
    void StartPrinting(Container *container, QWidget *parent);
private slots:
    void printPreview(QPrinter *);

private:
    QTextDocument   mTextDocument;
};

#endif // PRINTPASSWORDS_H
