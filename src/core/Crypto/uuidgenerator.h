///
/// \file       uuidgenerator.h
/// \author     Joachim Danmayr
/// \date       2016-09-24
///
/// \brief      An UUID generator class
///

#ifndef UUIDGENERATOR_H
#define UUIDGENERATOR_H

#include <cstdint>

namespace crypto
{

///
/// \class      UUIDgenerator
/// \author     Joachim Danmayr
/// \date       2016-09-24
///
/// \brief      An UUID generator class
///
class UUIDgenerator
{
public:
    UUIDgenerator();
    static char* GenerateUUID(uint32_t &size);

private:
    static const int32_t UUID_SIZE = 32;                ///< Size of the UUID in bytes
    static const int32_t NUMBER_OF_DELIMTERS = 4;       ///< Number of delimeters of the UUID
    static char mUUID[UUID_SIZE+NUMBER_OF_DELIMTERS];

    static char* itoa(int num, char* str, int base);
    static void reverse(char str[], int length);
};
}
#endif // UUIDGENERATOR_H
