///
/// \file       ElementaryEncrypted.cpp
/// \author     Joachim Danmayr
/// \date       2016-09-18
///
/// \brief
///

#include <cstring>
#include <iostream>
#include "elementaryencrypted.h"
#include "../Crypto/Secure.h"
#include "../Crypto/RandomNumber.h"

namespace msg {

///
/// Constructor
///
/// \param[in] id   Field ID
///
ElementaryEncrypted::ElementaryEncrypted(uint32_t id, const char *sessionKey):
    Type(id),
    mDataSize(0),
    mData(0),
    mSessionKey(sessionKey),
    mState(eDecrypted)
{

}

///
/// Copy assigenment
///
/// \param[in] obj   Object to copy
///
ElementaryEncrypted& ElementaryEncrypted::operator=(const ElementaryEncrypted &obj)
{
    SetId(obj.mID);
    mState = obj.mState;
    SetData(obj.mData,obj.mDataSize);
    mSessionKey = obj.mSessionKey;
    return *this;
}

///
/// Destructor
///
ElementaryEncrypted::~ElementaryEncrypted()
{
    Clear();
}

///
/// Sets data
///
/// \param[in] data      Pointer to the data array
/// \param[in] size      Number of data bytes
///
void ElementaryEncrypted::SetData(const char* data, uint32_t size)
{
    if(size > GetMaxAllowedSize()){
        throw "ElementaryEncrypted:SetData:File size too big";
        return;
    }
    if(size > MAX_ENCRYPTED_DATA_SIZE){
        throw "ElementaryEncrypted:SetData:File size too big";
        return;
    }

    Clear();
    mDataSize = size;
    mData = new char[size];
    std::memcpy(mData,data,size);
    Encrypt();
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
void ElementaryEncrypted::Serialize(std::ostream& stream)
{
    uint32_t id = GetId();
    stream.write ((char *)&id, KEY_FIELD_SIZE);
    stream.write ((char *)&mDataSize, SIZE_FIELD_SIZE);
    Decrypt();
    stream.write(mData,mDataSize);
    Encrypt();
}

///
/// Deserialize data from an input stream
///
/// \param[in,out]  stream      Reference to the stream object
/// \return true if element has been deserialized
///
void ElementaryEncrypted::Deserialize(std::istream& stream)
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
        Encrypt();
#ifdef DEBUG_MODE
        std::cout << "My ElementaryEncrypted with ID: "<<idMy << " Size: " << mDataSize << std::endl;
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
void ElementaryEncrypted::Clear(void)
{
    mState = eDecrypted;
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
uint32_t ElementaryEncrypted::GetSize()
{
    return mDataSize;
}

void ElementaryEncrypted::Encrypt()
{
    if(0!=mSessionKey){
        if(eDecrypted==mState){
            mState = eEncrypted;
            for(uint32_t n=0;n<mDataSize;n++){
                mData[n] ^=mSessionKey[n];
            }
        }
    }
}

void ElementaryEncrypted::Decrypt()
{
    if(0!=mSessionKey){
        if(eEncrypted==mState){
            mState = eDecrypted;
            for(uint32_t n=0;n<mDataSize;n++){
                mData[n] ^=mSessionKey[n];
            }
        }
    }
}
}
