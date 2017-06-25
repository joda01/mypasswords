///
/// \file       Elementary.cpp
/// \author     Joachim Danmayr
/// \date       2016-09-18
///
/// \brief
///

#include <cstring>
#include <iostream>
#include "elementary.h"
#include "../Crypto/Secure.h"

namespace msg {

///
/// Constructor
///
/// \param[in] id   Field ID
///
Elementary::Elementary(uint32_t id):
    Type(id),
    mDataSize(0),
    mData(0)
{

}

///
/// Copy assigenment
///
/// \param[in] obj   Object to copy
///
Elementary& Elementary::operator=(const Elementary &obj)
{
    SetId(obj.mID);
    SetData(obj.mData,obj.mDataSize);

    return *this;
}

///
/// Destructor
///
Elementary::~Elementary()
{
    Clear();
}

///
/// Sets data
///
/// \param[in] data      Pointer to the data array
/// \param[in] size      Number of data bytes
///
void Elementary::SetData(const char* data, uint32_t size)
{
    if(size > GetMaxAllowedSize()){
        throw "Elementary:SetData:File size too big";
        return;
    }

    Clear();
    mDataSize = size;
    mData = new char[size];
    std::memcpy(mData,data,size);
}

///
/// Serialize data to the outstream
/// A elementry element has got following struct
/// ID       | size   | data ...
/// ---------+--------+-------------------
/// 4bytes   | 4byte  | dataSize bytes
///
/// \param[in,out]  stream      Reference to the stream object
///
void Elementary::Serialize(std::ostream& stream)
{
    uint32_t id = GetId();
    stream.write ((char *)&id, KEY_FIELD_SIZE);
    stream.write ((char *)&mDataSize, SIZE_FIELD_SIZE);
    stream.write(mData,mDataSize);
}

///
/// Deserialize data from an input stream
///
/// \param[in,out]  stream      Reference to the stream object
///
void Elementary::Deserialize(std::istream& stream)
{
    uint32_t idRead=0;
    uint32_t idMy = GetId();
    stream.read ((char *)&idRead, KEY_FIELD_SIZE);
    if(idMy == idRead){
        // My Element found
        Clear();
        stream.read ((char *)&mDataSize, SIZE_FIELD_SIZE);
        mData = new char[mDataSize];
        stream.read(mData,mDataSize);
#ifdef DEBUG_MODE
        std::cout << "My elementary with ID: "<<idMy << " Size: " << mDataSize << std::endl;
        //uint32_t n=0;
        //while(n<mDataSize){std::cout<< std::hex << (int)mData[n];n++; if(n>20){std::cout << "..."; break;}}std::cout << std::endl;
#endif
    }
    else{
        // Not my Element rewind
        stream.seekg(stream.tellg()-(std::istream::pos_type)KEY_FIELD_SIZE);
    }
}

///
/// Clear the array
///
void Elementary::Clear(void)
{
    if(mData!=0){
        crypto::SecureClearArray(mData,mDataSize);
        delete mData;
        mDataSize=0;
        mData=0;
    }
}

///
/// Returns the number of data bytes
///
/// \return Number of data bytes
///
uint32_t Elementary::GetSize()
{
    return mDataSize;
}
}
