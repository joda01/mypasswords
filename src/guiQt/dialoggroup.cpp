///
/// \file       dialoggroup.cpp
/// \author     Joachim Danmayr
/// \date       2016-09-21
///
/// \brief      Password entry dialog file
///

#include <QBuffer>
#include <QFileDialog>

#include "dialoggroup.h"
#include "ui_dialoggroup.h"
#include "../core/Crypto/Secure.h"
#include "../core/Crypto/uuidgenerator.h"


///
/// \brief      Constructor
/// \author     Joachim Danmayr
/// \date       2016-09-21
///
/// \param[in]  parent  Pointer to the parent widget
///
DialogGroup::DialogGroup(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogGroup),
    mEntry(0),
    mOkPressed(false)
{
    ui->setupUi(this);
}

///
/// \brief      Destructor
/// \author     Joachim Danmayr
/// \date       2016-09-21
///
DialogGroup::~DialogGroup()
{
    delete ui;
}

///
/// \brief      Shows the dialog and returns true if it has been closed with the OK button
/// \author     Joachim Danmayr
/// \date       2016-09-21
///
/// \param[in]  entry           Pointer to the group entry which should be edited
/// \param[in]  selectedGroupId Selected group id. This is used to make as default parent group
///
bool DialogGroup::ShowDialog(Container* container, Container::GroupEntry *entry, QString selectedGroupId)
{
    if(0==entry){
        qDebug("DialogGrup:ShowDialog:GroupEntry* entry = 0");
        return false;
    }
    mEntry = entry;
    uint32_t size;
    char* data = entry->GetGroupId(size);
    QString myGroupId = StringToTextField(data,size);

    LoadGroups(container,myGroupId);
    LoadDataToTextField(entry);
    if(selectedGroupId.size()>0){
        ui->textGroup->setCurrentIndex(ui->textGroup->findData(selectedGroupId));
    }

    exec();
    return mOkPressed;
}

///
/// \brief      Ok button has been pressed
///             The textfield entries will be stored to the container file
///             and deleted then
/// \author     Joachim Danmayr
/// \date       2016-09-21
///
void DialogGroup::on_buttonBox_accepted()
{
    char* data;
    uint32_t length = 0;

    length = TextFieldToString(ui->textName->text(),&data);
    mEntry->SetName(data,length);
    crypto::SecureClearArray(data,length);
    delete [] data;

    length = TextFieldToString(ui->textGroup->currentData().toString(),&data);
    mEntry->SetParentId(data,length);
    crypto::SecureClearArray(data,length);
    delete [] data;

    //// Save icon
    QPixmap pixmap = ui->buttonAddIcon->icon().pixmap(ICON_SIZE);
    QByteArray bArray; QBuffer buffer(&bArray);
    buffer.open(QIODevice::WriteOnly);
    pixmap.save(&buffer, "PNG");
    mEntry->SetIcon((char*)bArray.constData(),bArray.size());

    mOkPressed = true;

}

///
/// \brief      The content of the Password element will be loaded to the text field
/// \author     Joachim Danmayr
/// \date       2016-09-21
///
void DialogGroup::LoadDataToTextField(Container::GroupEntry *element)
{
    char* data;
    uint32_t size;
    data = element->GetName(size);
    ui->textName->setText(StringToTextField(data,size));

    data = mEntry->GetParentId(size);
    ui->textGroup->setCurrentIndex(ui->textGroup->findData(StringToTextField(data,size)));

    data = element->GetGroupId(size);
    ui->labelUID->setText(StringToTextField(data,size));

    //// Load Icon
    data = element->GetIcon(size);
    if(size>0){
        QByteArray iconBuffer = QByteArray::fromRawData(data,size);
        QPixmap p; p.loadFromData(iconBuffer,"PNG");
        QIcon icon(p);
        ui->buttonAddIcon->setIcon(icon);
    }
}

///
/// \brief      Load all groups to the combo box
/// \author     Joachim Danmayr
/// \date       2016-09-21
///
void DialogGroup::LoadGroups(Container* container, QString& myGroupId)
{
    int size = container->GetNumberOfGroupEntry();
    ui->textGroup->addItem(tr("<root>"),"");
    for(int n=0;n<size;n++){
        Container::GroupEntry* group = container->GetGroup(n);
        uint32_t sizeName,sizeId;
        char* name = group->GetName(sizeName);
        char* id = group->GetGroupId(sizeId);
        QString loadedId = StringToTextField(id,sizeId);
        if(sizeId>0 && loadedId!=myGroupId){
            ui->textGroup->addItem(StringToTextField(name,sizeName),loadedId);
        }
    }
}



///
/// \brief      Converts a QString to a char array
/// \author     Joachim Danmayr
/// \date       2016-09-21
///
/// \param[in]  string  QString element which should be converted
/// \param[out] data    Pointer to the char array where data shouls be stored
/// \return     Number of elements in the data array
///
uint32_t DialogGroup::TextFieldToString(QString string, char** data)
{
    QByteArray text = string.toUtf8();
    *data = new char[text.size()];
    strcpy(*data, text.data());
    return text.size();
}

///
/// \brief      Converts a char array to a QString
/// \author     Joachim Danmayr
/// \date       2016-09-21
///
/// \param[in]  data    Pointer to the data array which should be converted
/// \param[in]  length  Number of data bytes in the array
/// \return     Converted string
///
QString DialogGroup::StringToTextField(const char* data, uint32_t length)
{
    return  QString::fromUtf8(data,length);
}


///
/// \brief      Add Icon has been pressed
/// \author     Joachim Danmayr
/// \date       2016-09-24
///
void DialogGroup::on_buttonAddIcon_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("Open icon"), "", tr("Image (*.png)"));
    ui->buttonAddIcon->setIcon(QIcon(fileName));
}
