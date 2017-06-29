///
/// \file       modelpasswordelements.h
/// \author     Joachim Danmayr
/// \date       2016-09-23
///
/// \brief      Table model for password ELEMENTS
///             Will return all password elements from
///             a password entry
///

#ifndef MODELPASSWORDELEMENTS_H
#define MODELPASSWORDELEMENTS_H


#include "tablemodel.h"

///
/// \class      ModelPasswordElements
/// \author     Joachim Danmayr
/// \date       2016-09-23
///
/// \brief      Table model for password ELEMENTS
///             Will return all password elements from
///             a password entry
///
class ModelPasswordElements : public TableModel
{
public:
    ModelPasswordElements(Container::PasswordEntry* passwordContainer,QObject *parent = 0);

private:
    virtual Container::PasswordEntry::PasswordElement* GetPasswordElement(int32_t idx) const override;
    virtual int GetNumberOfElements(void) const override;
    virtual void UpdateFilter(void) override;

    Container::PasswordEntry* mPasswordEntries;
};

#endif // MODELPASSWORDELEMENTS_H
