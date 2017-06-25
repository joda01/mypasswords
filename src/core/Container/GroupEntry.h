///
/// \file       GroupEntry.h
/// \author     Joachim Danmayr
/// \date       2016-09-17
/// \brief      A group element entry
///

#ifndef GROUPENTRY_H
#define GROUPENTRY_H

#include "../Message/nested.h"
#include "../Message/Elementary.h"

namespace container
{

///
/// \class      GroupEntry
/// \author     Joachim Danmayr
/// \date       2016-09-17
/// \brief      Group entry class
///
class GroupEntry : public msg::Nested
{
public:
    enum FiledNr_t{eId=0, eParentId=1, eIcon=2, eName=3};

    GroupEntry(int32_t fieldNr);
    ~GroupEntry();
    char* GetId(int32_t& size){return ((msg::Elementary*)GetMessage(eId))->GetData(size);}
    char* GetParentId(int32_t& size){return ((msg::Elementary*)GetMessage(eParentId))->GetData(size);}
    char* GetIcon(int32_t& size){return ((msg::Elementary*)GetMessage(eIcon))->GetData(size);}
    char* GetName(int32_t& size){return ((msg::Elementary*)GetMessage(eName))->GetData(size);}

    void SetId(const char* data,int32_t size){SetMessage(new msg::Elementary(eId,data,size));}
    void SetParentId(const char* data,int32_t size){SetMessage(new msg::Elementary(eParentId,data,size));}
    void SetIcon(const char* data,int32_t size){SetMessage(new msg::Elementary(eIcon,data,size));}
    void SetName(const char* data,int32_t size){SetMessage(new msg::Elementary(eName,data,size));}
};
}
#endif // GROUPENTRY_H
