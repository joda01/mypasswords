#ifndef ENCRYPTION_H
#define ENCRYPTION_H

#include <cstdint>

class Encryption
{
public:
    Encryption(){}
    virtual void Encrypt(char const* in, char* result, uint32_t n)=0;
    virtual void Decrypt(char const* in, char* result, uint32_t n)=0;

};

#endif // ENCRYPTION_H
