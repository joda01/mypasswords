///
/// \file       argon.cpp
/// \author     Joachim Danmayr
/// \date       2016-10-27
///
/// \brief      Argon2 password hashing clas
///



#include "argon.h"

namespace crypto {


///
/// Constructor
///
/// \param[in] salt          Pointer to the salt
/// \param[in] saltLen       Number of data bytes the salt has got
/// \param[in] iterationCost Iteration cost
/// \param[in] memoryCost    Memory cost [kByte]
/// \param[in] lanes         Number of lanes
///
Argon2::Argon2(char* salt, uint32_t saltLen, int32_t iterationCost, int32_t memoryCost, int32_t lanes)
{
    mContext.salt=(uint8_t*)salt;
    mContext.saltlen=saltLen;
    mContext.secret=NULL;
    mContext.secretlen=0;
    mContext.ad=NULL;
    mContext.adlen=0;
    mContext.t_cost=iterationCost;
    mContext.m_cost=memoryCost;
    mContext.lanes=lanes;
    mContext.threads=1;
    mContext.version=19;
    mContext.allocate_cbk=NULL;
    mContext.free_cbk=NULL;
}



Argon2::Error_t Argon2::Generate(const char* dataIn, uint32_t dataSize, char* hashOut,HashSize_t hashSize)
{
    mContext.out=(uint8_t*)hashOut;
    mContext.outlen=hashSize;
    mContext.pwd=(uint8_t*)dataIn;
    mContext.pwdlen=dataSize;
    int error = argon2i_ctx(&mContext);
    if(0==error){
        return eHashOk;
    }else{
        return eHashingError;
    }
}

void Argon2::StopCalculation()
{
    stopArgon2();
}


}
