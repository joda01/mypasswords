///
/// \file       uuidgenerator.cpp
/// \author     Joachim Danmayr
/// \date       2016-09-24
///
/// \brief      An UUID generator class
///

#include "uuidgenerator.h"
#include "RandomNumber.h"
#include <iostream>

namespace crypto
{

char UUIDgenerator::mUUID[UUIDgenerator::UUID_SIZE+UUIDgenerator::NUMBER_OF_DELIMTERS]={"1234-12345"};

///
/// \brief      Constructor
/// \author     Joachim Danmayr
/// \date       2016-09-24
///
UUIDgenerator::UUIDgenerator()
{

}

///
/// \brief      Generates an UUID and returns it as string
/// \author     Joachim Danmayr
/// \date       2016-09-24
///
/// \param[out] size    Number of bytes the UUID is liong
/// \return UUID in string format
char* UUIDgenerator::GenerateUUID(uint32_t& size)
{
    RandomNumber rand;
    char number[UUID_SIZE];
    rand.Generate(number,UUID_SIZE,0,16);
    int u=0;
    for(int n=0;n<UUID_SIZE;n++){
        if(8==n || 12==n || 16==n || 20==n){
            mUUID[u]='-';
            u++;
        }
        char d[2];itoa(number[n],d,16);
        mUUID[u] = d[0];
        u++;
    }
    size = UUID_SIZE+NUMBER_OF_DELIMTERS;
    return mUUID;
}

// Implementation of itoa()
char* UUIDgenerator::itoa(int num, char* str, int base)
{
    int i = 0;
    bool isNegative = false;

    /* Handle 0 explicitely, otherwise empty string is printed for 0 */
    if (num == 0)
    {
        str[i++] = '0';
        str[i] = '\0';
        return str;
    }

    // In standard itoa(), negative numbers are handled only with
    // base 10. Otherwise numbers are considered unsigned.
    if (num < 0 && base == 10)
    {
        isNegative = true;
        num = -num;
    }

    // Process individual digits
    while (num != 0)
    {
        int rem = num % base;
        str[i++] = (rem > 9)? (rem-10) + 'a' : rem + '0';
        num = num/base;
    }

    // If number is negative, append '-'
    if (isNegative)
        str[i++] = '-';

    str[i] = '\0'; // Append string terminator

    // Reverse the string
    reverse(str, i);

    return str;
}

void UUIDgenerator::reverse(char str[], int length)
{
    int start = 0;
    int end = length -1;
    while (start < end)
    {
        std::swap(*(str+start), *(str+end));
        start++;
        end--;
    }
}


}
