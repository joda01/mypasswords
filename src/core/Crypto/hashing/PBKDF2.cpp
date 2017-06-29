///
/// \file       PBKDF2.cpp
/// \author     Joachim Danmayr
/// \date       2016-10-12
///
/// \brief
///


#include "sha256.h"
#include "PBKDF2.h"
#include <cstring>
#include <ostream>
#include <iostream>
#include <iomanip>
#include <sstream>


#define hLenSz 32 // Block size of sha256 in bytes

PBKDF2::PBKDF2()
{
    std::string Salt="salt";
    std::string Password="password";
    int c=1;
    int dkLen=32;

    string sha = sha256(Password);
    std::cout << sha << std::endl;

    std::string hmac = HMAC(Salt, Password);//Key, data
    std::cout << hmac << std::endl;

    std::string key= Generate(Password,Salt,c,dkLen);
    std::cout << std::hex << key;

}

//Password-Based Key Derivation Function 2
std::string PBKDF2::Generate(string Password, string Salt, int c, int dkLen)
{
    //Restrict max 128 bytes of Derivated Key
    if(dkLen > 128)
    {
        dkLen = 128;
    }

    const int BlockSize = hLenSz / sizeof(UL); // 8 bytes

    UL T[32] = {}; // Holds DK in 4 byte chunks
    UL L[BlockSize] = {};
    UL H[BlockSize] = {};

    int l = ceil((float)(dkLen) / (float)hLenSz); // Compute the number of passes needed to get the desired DK length Masterkey length in bits


    for(int i = 0; i < l; i++)
    {
        std::memset(L, 0, sizeof(L));
        string innerSalt = Salt + (char)0+ (char)0+(char)0+(char)(i+1); // Add laut doku

        //Iterate c times
        for(int j = 0; j < c; j++)
        {
            innerSalt = HMAC(innerSalt, Password);
            HexToLong(innerSalt, H, BlockSize);
            //XOR function
            for(int p = 0; p < BlockSize; p++)
            {
                L[p] = L[p] ^ H[p];
            }
        }

        for(int x = 0; x < BlockSize; x++)
        {
            T[BlockSize * i + x] = L[x];
        }
    }

    std::string output = "";
    for(int i = 0; i < dkLen / sizeof(UL); i++) // dkLen / 4 - unsigned long has 4 bytes
    {
        // hexify takes sizeof(T) and converts it to hex bytes
        output += hexify<UL>(T[i]);
    }
    return output;
}

//Hash-based message authentication code
#define HMAC_SIZE  32
std::string PBKDF2::HMAC(string Salt, string Password)
{
    char c;
    string s;
    UL Key[16] = {0};
    UL X[16] = {0};
    UL Y[16] = {0};
    UL ipad = 0x36363636; // 0x36 = 54 = '6'
    UL opad = 0x5c5c5c5c; // 0x5c = 92 = '\'
    int k;
    s = "";

    int32_t length = HMAC_SIZE/4;

    //Process string key into sub-key
    //Hash key in case it is less than 64 bytes
    if(Password.length() > 64)
    {
        std::string tmp = sha256(Password);

        HexToLong(tmp, Key, length); // 5
    }
    else
    {
        for(int i = 0; i < 16; i++)
        {
            for(int j = 0; j < 4; j++)
            {
                if(4 * i + j <= Password.length())
                {
                    k = Password[4 * i + j];
                }
                else
                {
                    k = 0;
                }
                if(k < 0)
                {
                    k = k + 256;
                }
                Key[i] += +k*pow(256, (double)3 - j);
            }
        }
    }

    for(int i = 0; i < 16; i++)
    {
        X[i] = Key[i] ^ ipad;
        Y[i] = Key[i] ^ opad;
    }

    //Turn X-Array into a String
    for(int i = 0; i < 16; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            c = ((X[i] >> 8 * (3 - j)) % 256);
            s += c;
        }
    }

    //Append text to string
    s += Salt;

    std::string tmp = sha256(s);
    HexToLong(tmp, Key, length);//5

    s = "";

    //Convert Y array to a string
    for(int i = 0; i < 16; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            c = ((Y[i] >> 8 * (3 - j)) % 256);
            s += c;
        }
    }

    for(int i = 0; i < length; i++)//5
    {
        for(int j = 0; j < 4; j++)
        {
            c = ((Key[i] >> 8 * (3 - j)) % 256);
            s += c;
        }
    }

    //Hash final aggregated string
    return sha256(s);
}

UL PBKDF2::f(UL B, UL C, UL D, int t)
{
    if(t < 20)
    {
        return ((B & C) ^ ((~B) & D));
    }
    if((t > 19) & (t < 40))
    {
        return (B ^ C ^ D);
    }
    if((t > 39) & (t < 60))
    {
        return ((B & C) ^ (B & D) ^ (C & D));
    }
    if(t > 59)
    {
        return (B ^ C ^ D);
    }
}

template< typename T >std::string PBKDF2::hexify(T i)
{
    std::stringbuf buf;
    std::ostream os(&buf);

    os << std::setfill('0') << std::setw(sizeof(T) * 2) << std::hex << i;

    return buf.str().c_str();
}

void PBKDF2::HexToLong(string input, unsigned long* output, int outSize)
{
    char hx[8] = {};

    for(int i = 0; i < outSize; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            hx[j] = input[j + (8 * i)];
        }
        output[i] = strtoul(hx, nullptr, 16);
    }
}
