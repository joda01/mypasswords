/**
 * @file	PBKDF2.h
 *
 * Calculates a HASH with PBKDF2 standard
 *
 * @author	Joachim Danmayr
 * @date	206-07-25
 *
 */


#ifndef PBKDF2_H
#define PBKDF2_H


#include <iostream>
#include <string>
#include <cmath>

using namespace std;

typedef unsigned long UL;

class PBKDF2
{
public:
    PBKDF2();
    std::string Generate(string password, string salt, int32_t iterations, int32_t dkLen);

private:
    std::string HMAC(string Salt, string Password);
    UL f(UL B, UL C, UL D, int t);
    void HexToLong(string input, unsigned long* output, int outSize);

   template< typename T > std::string hexify(T i);


};

#endif // PBKDF2_H
