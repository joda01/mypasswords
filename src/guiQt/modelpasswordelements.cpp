///
/// \file       modelpasswordelements.cpp
/// \author     Joachim Danmayr
/// \date       2016-09-23
///
/// \brief      Table model for password ELEMENTS
///             Will return all password elements from
///             a password entry
///

#include "modelpasswordelements.h"

///
/// \brief     Constructor
/// \author    Joachim Danmayr
/// \date      2016-09-23
///
/// \param[in] passwordContainer    Pointer to the password element which
///                                 containes the table information
/// \param[in] parent               Pointer to the parent widget
///
ModelPasswordElements::ModelPasswordElements(Container::PasswordEntry* passwordContainer, QObject *parent):
    TableModel(parent),
    mPasswordEntries(passwordContainer)
{

}

///
/// \brief     Returns the password element at the given index
/// \author    Joachim Danmayr
/// \date      2016-09-23
///
/// \param[in] idx  Index of the element which should be returned
/// \return    Passwordelement at index idx
///
Container::PasswordEntry::PasswordElement* ModelPasswordElements::GetPasswordElement(int32_t idx) const
{
    if(idx < GetNumberOfElements())
    {
        return mPasswordEntries->GetPasswordElement(idx);
    }
    return 0;
}

///
/// \brief     Returns the number of containing password elements
/// \author    Joachim Danmayr
/// \date      2016-09-23
///
/// \return    Number of password elements
///
int ModelPasswordElements::GetNumberOfElements(void) const
{
   return mPasswordEntries->GetNumberOfPasswordElement();
}

///
/// \brief     Updates the model filter
/// \author    Joachim Danmayr
/// \date      2016-10-09
///
///
void  ModelPasswordElements::UpdateFilter(void)
{
}
