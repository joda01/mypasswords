///
/// \file       nested.h
/// \author     Joachim Danmayr
/// \date       2016-09-18
///
/// \brief
///

#ifndef NESTED_H
#define NESTED_H

#include <map>
#include "type.h"


namespace msg {

///
/// \class      Nested
/// \author     Joachim Danmayr
/// \date       2016-09-18
///
/// \brief      Class for a nested type.
///             A nested type is a set of Types.
///             These types could be nested types again or
///             elementary types.
///
class Nested : public Type
{
public:
    Nested(uint32_t id);
    Nested& operator=(Nested&);
    virtual ~Nested();
    virtual void Serialize(std::ostream&) override;
    virtual void Deserialize(std::istream&stream) override;
    virtual void Clear(void) override;
    virtual uint32_t GetSize(void) override;
protected:
    void SetElement(int key, Serializable* data){mData[key] = data;}
    Serializable* GetElement(uint32_t key){return mData[key];}

    std::map<uint32_t,Serializable*> mData;
};
}
#endif // TYPE_H
