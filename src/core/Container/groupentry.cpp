///
/// \file       GroupEntry.cpp
/// \author     Joachim Danmayr
/// \date       2016-09-17
/// \brief      A group element entry
///

#include "GroupEntry.h"

namespace container {


///
/// Constructor
///
/// \author     Joachim Danmayr
/// \date       2016-09-17
///
GroupEntry::GroupEntry(int32_t fieldNr):
    msg::Nested(fieldNr)
{

}

///
/// Destructor
///
/// \author     Joachim Danmayr
/// \date       2016-09-17
///
GroupEntry::~GroupEntry()
{

}

}
