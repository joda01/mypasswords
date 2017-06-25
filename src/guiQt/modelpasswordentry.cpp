///
/// \file       modelpasswordentry.cpp
/// \author     Joachim Danmayr
/// \date       2016-09-21
///
/// \brief      Table model for password entries
///             Will always return the first password element of
///             an entry
///

#include "modelpasswordentry.h"

///
/// \brief     Constructor
/// \author    Joachim Danmayr
/// \date      2016-09-21
///
/// \param[in] passwordContainer    Pointer to the password container which
///                                 containes the table information
/// \param[in] parent               Pointer to the parent widget
///
ModelPasswordEntry::ModelPasswordEntry(Container *passwordContainer, QObject *parent):
    TableModel(parent),
    mPasswordContainer(passwordContainer)
{

}

///
/// \brief     Returns the first password element from the password
///            entry with the given index
/// \author    Joachim Danmayr
/// \date      2016-09-21
///
/// \param[in] idx  Index of the element which should be returned
/// \return    Passwordelement at index idx
///
Container::PasswordEntry::PasswordElement* ModelPasswordEntry::GetPasswordElement(int32_t idx) const
{
    if(idx < GetNumberOfElements())
    {
        Container::PasswordEntry* entry= mPasswordContainer->GetPasswordEntry(idx);
        if((0!=entry) && (entry->GetNumberOfPasswordElement()>0)){
            return entry->GetPasswordElement(0);
        }
    }
    return 0;
}

///
/// \brief     Returns the number of containing password entries
/// \author    Joachim Danmayr
/// \date      2016-09-21
///
/// \return    Number of password entries
///
int ModelPasswordEntry::GetNumberOfElements(void) const
{
   return mPasswordContainer->GetNumberOfPasswordEntry();
}

///
/// \brief     Updates the model filter
/// \author    Joachim Danmayr
/// \date      2016-10-09
///
///
void  ModelPasswordEntry::UpdateFilter(void)
{
    mPasswordContainer->UpdatePasswordFilter();
}
