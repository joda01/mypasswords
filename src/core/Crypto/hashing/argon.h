///
/// \file       Argon.h
/// \author     Joachim Danmayr
/// \date       2016-10-27
///
/// \brief      Argon2 password hashing
///

#ifndef ARGON_H
#define ARGON_H

#include "hashing.h"
#include "argon/argon2.h"

namespace crypto {


///
/// \class      Argon2
/// \author     Joachim Danmayr
/// \date       2016-10-27
///
/// \brief      Argon2 password hashing class
///
class Argon2 : public Hashing
{
public:
    Argon2(char *salt, uint32_t saltLen, int32_t iterationCost, int32_t memoryCost, int32_t lanes);
    virtual Error_t Generate(const char* dataIn, uint32_t dataSize, char* hashOut,HashSize_t hashSize) override;
    void StopCalculation();
private:
    Argon2_Context mContext;
};
}

#endif // ARGON2_H
