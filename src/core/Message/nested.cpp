///
/// \file       nested.cpp
/// \author     Joachim Danmayr
/// \date       2016-09-18
///
/// \brief
///

#include <iostream>
#include <cstdarg>
#include "nested.h"

namespace msg {

///
/// Constructor
///
/// \param[in] id   Field ID
///
Nested::Nested(uint32_t id):
    Type(id)
{
}

///
/// Copy constructor
///
/// \param[in] copy   Element which should copied
///
Nested& Nested::operator= (Nested& copy)
{
    SetId(copy.mID);
    for (auto& kv : mData) {
        kv.second = copy.mData.at(kv.first);
    }
    return *this;
}

///
/// Destructor
///
Nested::~Nested()
{
}

///
/// \brief     Serialize all elements.
///
/// \param[in] stream  Reference to the stream object
///
void Nested::Serialize(std::ostream& stream)
{
    uint32_t id = GetId();
    uint32_t size = GetSize();
    stream.write ((char *)&id, KEY_FIELD_SIZE);
    stream.write ((char *)&size, SIZE_FIELD_SIZE);

    for (auto& kv : mData) {
        kv.second->Serialize(stream);
    }
}

///
/// \brief     Deserialize data from an input stream.
///
/// \param[in] stream  Reference to the stream object
///
void Nested::Deserialize(std::istream& stream)
{
    uint32_t idRead=0;
    uint32_t idMy = GetId();
    stream.read ((char *)&idRead, KEY_FIELD_SIZE);
    if(idMy == idRead){
        uint32_t mySize = 0;
        stream.read ((char *)&mySize, SIZE_FIELD_SIZE);
#ifdef DEBUG_MODE
        std::cout << "Nested with ID: " << idRead <<" size: " << mySize<< std::endl;
#endif
        uint32_t posStart  = stream.tellg();
        uint32_t posAkt    = stream.tellg();
        uint32_t posAktOld = stream.tellg();
        while((posAkt-posStart)<mySize && (posAkt > 0)){
            for (auto& kv : mData) {
                kv.second->Deserialize(stream);
            }
            posAkt = stream.tellg();
            if((posAkt-posAktOld)<=0){
                // No more bytes could be read
                break;
            }
            posAktOld = posAkt;
        }
    }
}


///
/// \brief     Clear all elements from this nested message
///
void Nested::Clear(void)
{
    for (auto& kv : mData) {
        kv.second->Clear();
    }
}

///
/// \brief Returns the number of data bytes this nested messag
///        is long
///
/// \return Size of this nested message
///
uint32_t Nested::GetSize(void)
{
    uint32_t size=0;
    for (auto& kv : mData) {
        size+=kv.second->GetHeaderSize() + kv.second->GetSize();
        if(size>GetMaxAllowedSize()){
            throw "Nested:GetSize:File size too big";
            return 0;
        }
    }
    return size;
}
}
