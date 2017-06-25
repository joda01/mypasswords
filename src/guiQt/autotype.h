///
/// \file       autotype.cpp
/// \author     Joachim Danmayr
/// \date       2016-11-12
///
/// \brief      Generates autotyp commands with hte keyboard
///
#ifndef AUTOTYPE_H
#define AUTOTYPE_H

#include <QVector>
#include "../core/NativeFunctions/NativeFunctions.h"
#include "../core/Container/Container.h"

///
/// \class      AutoType
/// \author     Joachim Danmayr
/// \date       2016-11-12
///
/// \brief      Generates autotyp commands with hte keyboard
///
class AutoType
{
public:
    AutoType(QString command)
    {
        mCommandList.append(this);
    }
    virtual void Execute(Container::PasswordEntry::PasswordElement*elem)=0;
private:
    static QVector<AutoType*> mCommandList;
    Container::PasswordEntry::PasswordElement* mPassElement;
};

class CommandTab : public AutoType
{
public:
    CommandTab():AutoType("{TAB}"){}
    virtual void Execute(Container::PasswordEntry::PasswordElement*elem) override{sendKeyEventTab();}
};

class CommandCtrlV : public AutoType
{
public:
    CommandCtrlV():AutoType("{CTRL+V}"){}
    virtual void Execute(Container::PasswordEntry::PasswordElement*elem) override{sendKeyEventCtrlV();}
};

class CommandUsername : public AutoType
{
public:
    CommandUsername():AutoType("{USERNAME}"){}
    virtual void Execute(Container::PasswordEntry::PasswordElement*elem) override{}
};

class CommandPassword : public AutoType
{
public:
    CommandPassword():AutoType("{PASSWORD}"){}
    virtual void Execute(Container::PasswordEntry::PasswordElement*elem) override{}
};


#endif // AUTOTYPE_H
