///
/// \file       modelpasswordentry.h
/// \author     Joachim Danmayr
/// \date       2016-09-21
///
/// \brief      Table model for password entries
///             Will always return the first password element of
///             an entry
///


#ifndef MODELPASSWORDENTRY_H
#define MODELPASSWORDENTRY_H

#include "tablemodel.h"

///
/// \class      ModelPasswordEntry
/// \author     Joachim Danmayr
/// \date       2016-09-23
///
/// \brief      Table model for password entries
///             Will always return the first password element of
///             an entry
///
class ModelPasswordEntry : public TableModel
{
public:
    ModelPasswordEntry(Container* passwordContainer,QObject *parent = 0);

private:
    virtual Container::PasswordEntry::PasswordElement* GetPasswordElement(int32_t idx) const override;
    virtual int GetNumberOfElements(void) const override;
    virtual void UpdateFilter(void) override;
    Container* mPasswordContainer;
};

#endif // MODELPASSWORDENTRY_H
