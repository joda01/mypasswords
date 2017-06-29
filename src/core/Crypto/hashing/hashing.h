#ifndef HASHING_H
#define HASHING_H

#include <cstdint>

namespace crypto {
///
/// \class      Hashing
/// \author     Joachim Danmayr
/// \date       2016-10-27
///
/// \brief      Interface class for hashing functions
///
class Hashing{
public:
    enum HashSize_t {hash224 = 28,hash256 = 32,hash384 = 48,hash512 = 64};
    enum Error_t{eHashOk, eHashingError};
    Hashing(){}
    virtual Error_t Generate(const char* dataIn, uint32_t dataSize, char* hashOut,HashSize_t hashSize)=0;
};
}

#endif // HASHING_H
