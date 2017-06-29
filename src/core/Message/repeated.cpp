///
/// \file       repeated.cpp
/// \author     Joachim Danmayr
/// \date       2016-09-18
///
/// \brief
///


#include "repeated.h"
#include <iostream>
#include <algorithm>

namespace msg {


///
/// Constructor
///
template<class TYPE>
Repeated<TYPE>::Repeated(void)
{

}

///
/// Destructor
///
template<class TYPE>
Repeated<TYPE>::~Repeated()
{
    Clear();
}

///
/// \brief     Serialize all elements.
///
/// \param[in] stream  Reference to the stream object
///
template<class TYPE>
void Repeated<TYPE>::Serialize(std::ostream& stream)
{
    for(TYPE* element:mData){
        element->Serialize(stream);
    }
}

///
/// \brief     Deserialize data from an input stream.
///
/// \param[in] stream  Reference to the stream object
///
template<class TYPE>
void Repeated<TYPE>::Deserialize(std::istream& stream)
{
    uint32_t idRead=0;
    stream.read ((char *)&idRead, KEY_FIELD_SIZE);
    stream.seekg(stream.tellg()-(std::istream::pos_type)KEY_FIELD_SIZE);

    TYPE* newType = new TYPE();
    if(newType->GetId()==idRead){
#ifdef DEBUG_MODE
        std::cout << "My Repeated with ID: " << idRead << std::endl;
#endif
        newType->Deserialize(stream);
        AddElement(newType);
    }
    else{
        delete newType;
    }
}

///
/// \brief  Removes all elements
///
template<class TYPE>
void Repeated<TYPE>::Clear(void)
{
    for(TYPE* element:mData){
        element->Clear();
        //mData.erase(element);
    }
    mData.clear();
}

///
/// \brief  Returns the size of the element including all data
///
/// \return Number of data bytes
///
template<class TYPE>
uint32_t Repeated<TYPE>::GetSize(void)
{
    uint32_t size = 0;
    for(TYPE* element:mData){
        size = size + element->GetSize()+element->GetHeaderSize();
        if(size > GetMaxAllowedSize()){
            throw "Repeated:GetSize:File size too big";
            return 0;
        }
    }
    return size;
}

///
/// \brief     Adds an element
///
/// \param[in] element  Element to add
///
template<class TYPE>
void Repeated<TYPE>::AddElement(TYPE* element)
{
    mData.push_back(element);
}

///
/// \brief     Gets an element
///
/// \param[in] idx  Index of the element which should be returned
///
template<class TYPE>
TYPE* Repeated<TYPE>::GetElement(uint32_t idx)
{
    return mData.at(idx);
}

///
/// \brief     Removes an element
///
/// \param[in] idx  Remove element at the given index
///
template<class TYPE>
void Repeated<TYPE>::EraseElement(uint32_t idx)
{
    mData.erase(mData.begin()+idx);
}

///
/// \brief     Removes an element
///
/// \param[in] element  Pointer of the element, which should be erased
///
template<class TYPE>
void Repeated<TYPE>::EraseElement(TYPE* element)
{
    auto i = std::find(mData.begin(), mData.end(), element);
    mData.erase(i);

}

}
