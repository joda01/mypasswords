///
/// \file       FileContainer.cpp
/// \author     Joachim Danmayr
/// \date       2016-09-17
/// \brief      File container which contains
///             settings, passwords and groups
///


#ifndef FILECONTAINER_H
#define FILECONTAINER_H

#include "../Message/repeatedelement.h"
#include "GroupEntry.h"

namespace container {

///
/// \class      FileContainer
/// \author     Joachim Danmayr
/// \date       2016-09-17
/// \brief      File container class
///
class FileContainer : public msg::Nested
{
    enum FiledNr_t{eGroupEntry=0, ePasswordElement=1};
public:
    FileContainer();
    void AddGroupEntry(GroupEntry* entry);
    virtual void Serialize(std::ostream& stream) override;
    virtual void Deserialize(std::istream& stream) override;
    virtual void Clear(void) override;

private:
    msg::RepeatedElement<GroupEntry*> mGroupEntries;
};
}

#endif // FILECONTAINER_H
