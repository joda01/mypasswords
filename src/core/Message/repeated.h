///
/// \file       repeated.h
/// \author     Joachim Danmayr
/// \date       2016-09-18
///
/// \brief      A repeated field
///

#ifndef REPEATED_H
#define REPEATED_H

#include <vector>
#include "serializable.h"

namespace msg {

///
/// \class      Repeated
/// \author     Joachim Danmayr
/// \date       2016-09-18
///
/// \brief      Inherent from this class if this field is more
///             often in a container
///
template<class TYPE> class Repeated : public Serializable
{
public:
    Repeated(void);
    ~Repeated();
    virtual void Serialize(std::ostream&stream) override;
    virtual void Deserialize(std::istream&stream) override;
    virtual void Clear(void) override;
    virtual uint32_t GetSize(void) override;
    virtual uint32_t GetHeaderSize(void) override {return 0;}
    void AddElement(TYPE* element);
    TYPE* GetElement(uint32_t idx);
    void EraseElement(uint32_t idx);
    void EraseElement(TYPE* element);
    uint32_t GetNumberOfElements(void){return mData.size();}

private:
    std::vector<TYPE*> mData;

};
}
#include "repeated.cpp"

#endif // REPEATED_H
