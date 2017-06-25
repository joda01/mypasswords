///
/// \file       elementary.h
/// \author     Joachim Danmayr
/// \date       2016-09-18
///
/// \brief      Base type for an element in the container
///

#ifndef ELEMENTARY_H
#define ELEMENTARY_H

#include "type.h"

namespace msg {

///
/// \class      Elementary
/// \author     Joachim Danmayr
/// \date       2016-09-18
///
/// \brief      An elementary data entry.
///             Holds data in a char array
///
class Elementary : public Type
{
public:
    Elementary(uint32_t id);
    Elementary& operator=(const Elementary&);
    ~Elementary();
    virtual void Serialize(std::ostream& stream) override;
    virtual void Deserialize(std::istream& stream) override;
    virtual uint32_t GetSize() override;
    virtual void Clear(void) override;
    virtual char* GetData(uint32_t& size){size=mDataSize;return mData;}
    virtual void SetData(const char *data, uint32_t size);
private:
    uint32_t mDataSize;
    char* mData;
};
}
#endif // ELEMENTARY_H
