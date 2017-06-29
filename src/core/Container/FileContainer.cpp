///
/// \file       FileContainer.cpp
/// \author     Joachim Danmayr
/// \date       2016-09-17
/// \brief      File container which contains
///             settings, passwords and groups
///


#include "FileContainer.h"


namespace container {


///
/// Constructor
///
/// \author     Joachim Danmayr
/// \date       2016-09-17
///
FileContainer::FileContainer():
    Nested(0)
{

}

///
/// Add a group element
///
/// \author     Joachim Danmayr
/// \date       2016-09-17
///
/// \param[in] entry    Pointer to the new group element
///
void FileContainer::AddGroupEntry(GroupEntry* entry)
{
    mGroupEntries.AddElement(entry);
}


///
/// Clear all elements
///
/// \author     Joachim Danmayr
/// \date       2016-09-18
///
void FileContainer::Clear(void)
{
    mGroupEntries.Clear();
}


///
/// Serialize the data to an output stream
///
/// \author     Joachim Danmayr
/// \date       2016-09-17
///
/// \param[in,out] stream   Pointer to the stream where the data should be serialized
///
void FileContainer::Serialize(std::ostream& stream)
{
    Nested::Serialize(stream);
    mGroupEntries.Serialize(stream);

}


///
/// Deserialize the data from an input stream
///
/// \author     Joachim Danmayr
/// \date       2016-09-17
///
/// \param[in,out] stream   Pointer to the stream from where the data should be read
///
void FileContainer::Deserialize(std::istream& stream)
{
    Nested::Deserialize(stream);
}

}
