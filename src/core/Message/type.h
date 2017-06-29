///
/// \file       type.h
/// \author     Joachim Danmayr
/// \date       2016-09-18
///
/// \brief      Base typ for an element in the container
///

#ifndef TYPE_H
#define TYPE_H

#include "serializable.h"

namespace msg {

///
/// \class      Type
/// \author     Joachim Danmayr
/// \date       2016-09-18
///
/// \brief      Base type class
///
class Type : public Serializable
{
public:
    Type(uint32_t id):mID(id){}
    virtual ~Type(){}
    uint32_t GetId(void){return mID;}
    virtual uint32_t GetHeaderSize(void) override {return (KEY_FIELD_SIZE+SIZE_FIELD_SIZE);}

protected:
    void SetId(uint32_t id){mID = id;}
    uint32_t mID;

};
}
#endif // TYPE_H
