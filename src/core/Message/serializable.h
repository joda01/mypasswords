///
/// \file       serializable.h
/// \author     Joachim Danmayr
/// \date       2016-09-18
///
/// \brief
///

#ifndef SERIALIZABLE_H
#define SERIALIZABLE_H

#include <ostream>
#include <istream>

namespace msg {

///
/// \class      Serializable
/// \author     Joachim Danmayr
/// \date       2016-09-18
///
/// \brief      Interface for a serializable element
///
class Serializable
{
public:
    virtual void Serialize(std::ostream&)=0;
    virtual void Deserialize(std::istream&)=0;
    virtual void Clear(void)=0;
    virtual uint32_t GetSize(void)=0;
    virtual uint32_t GetHeaderSize(void)=0;

protected:
    static const uint32_t KEY_FIELD_SIZE = 4;         ///< Number of bytes of the key field
    static const uint32_t SIZE_FIELD_SIZE = 4;        ///< Number of bytes of the size field

    uint32_t GetMaxAllowedSize(void){return MAX_SIZE;}

private:
    static const unsigned int MAX_SIZE =  2147483647;           ///< Maximum allowed data bytes (2^31-1)

};
}
#endif // SERIALIZABLE_H
