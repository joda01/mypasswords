/**
 * @file	PrintPasswords.cpp
 *
 * Printing support
 *
 * @author	Joachim Danmayr
 * @date	2016-07-26
 *
 */


#include "PrintPasswords.h"
#include <QPrinter>
#include <QPrintDialog>
#include <QPainter>
#include <QTextDocument>
#include <QPrintPreviewDialog>



PrintPasswords::PrintPasswords()
{

}

/**
 * Prints the password list
 *
 * @param [in]  parent   Parent dialog
 */
void PrintPasswords::StartPrinting(Container* container, QWidget* parent)
{
    QString textToPrint;
    container->OrderBy('GRP',0,false);

    QString groupOld;
    for(int n=0;n<container->GetEntryNumber(false);n++){
        Container* entry;
        container->GetEntry(n,&entry,false);
        char* group;
        char* title;
        char* username;
        char* password;
        uint32_t groupSize = entry->GetEntry('GRP',&group);
        uint32_t titleSize = entry->GetEntry('TIT',&title);
        uint32_t usernameSize = entry->GetEntry('USR',&username);
        uint32_t passwordSize = entry->GetEntry('PWD',&password);
        QString groupStr = QString::fromUtf8(group,groupSize);

        if(groupStr!=groupOld){
            textToPrint.append("<table width=\"100%\" bgcolor=\"#e9e9e9\">");
            textToPrint.append(QString("<tr><td>%1</th></td>").arg(groupStr));
            textToPrint.append("</table>");
            textToPrint.append("<p>");
        }
        else{
            textToPrint.append("<hr>");
        }
        textToPrint.append("<table>");
        if(titleSize>0)
            textToPrint.append(QString("<tr><td width=\"200\"><b>Title:</b></td><td>%1</td></tr>").arg(QString::fromUtf8(title,titleSize)));
        if(usernameSize>0)
            textToPrint.append(QString("<tr><td width=\"200\"><i>Username:</i></td><td>%1</td></tr>").arg(QString::fromUtf8(username,usernameSize)));
        if(passwordSize>0)
            textToPrint.append(QString("<tr><td width=\"200\"><i>Password:</i></td><td>%1</td></tr>").arg(QString::fromUtf8(password,passwordSize)));
        textToPrint.append("</table>");


        groupOld = groupStr;
    }

    mTextDocument.setHtml(textToPrint);

    QPrinter printer(QPrinter::HighResolution);
    printer.setFullPage(true);
    QPrintPreviewDialog preview(&printer, parent);
    preview.setWindowFlags ( Qt::Window );
    QObject::connect(&preview, SIGNAL(paintRequested(QPrinter*)),SLOT(printPreview(QPrinter*)));
    preview.exec();
}


void PrintPasswords::printPreview(QPrinter * printer)
{
    printer->setFullPage(true);
    mTextDocument.print(printer);
}
