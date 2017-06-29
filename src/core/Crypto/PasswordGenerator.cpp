/**
* @file	PasswordGenerator.h
*
* Password generator
*
* @author	Joachim Danmayr
* @date     206-07-24
*
*/

#include "PasswordGenerator.h"
#include "../Crypto/RandomNumber.h"
#include <cstdint>


namespace sec
{

/**
 * Constructor
 */
PasswordGenerator::PasswordGenerator(int32_t passwordSize
                                     ,bool useUppercase,bool useLowercase
                                     ,bool useDigits,bool usePlusMinus
                                     ,bool useUnserScore,bool useSpecial
                                     ,bool useBrackets,bool useSpaces):
    mPasswordSize(passwordSize),
    mUseUppercase (useUppercase ),
    mUseLowercase (useLowercase ),
    mUseDigits    (useDigits    ),
    mUsePlusMinus (usePlusMinus ),
    mUseUnserScore(useUnserScore),
    mUseSpecial   (useSpecial   ),
    mUseBrackets  (useBrackets  ),
    mUseSpaces    (useSpaces    )

{

}

/**
 * Generates a random password
 *
 * @param[in] startValue  Value to init the random function with
 * @return generated password
 */
std::string PasswordGenerator::GeneratePassword(uint32_t startValue,int32_t minNrOfDigits, int32_t minNrOfUpperCase, int32_t minNrOfLowerCase)
{
    char randomNumbe[MAX_PASSWORD_LENGTH];
    std::string charSet = PrepareCharSet();
    RandomNumber randomGenerator;
    randomGenerator.Generate(randomNumbe,mPasswordSize,startValue);


    std::string password;
    if(charSet.size()>0){
        for( int i = 0; i < mPasswordSize; i++ ){
            password += charSet[randomNumbe[i] % charSet.length()];
        }
    }
    return password;
}

/**
 * Prepares the charset for the password generator
 *
 * @return string with all allowed characters
 */
std::string PasswordGenerator::PrepareCharSet()
{
    std::string wort;
    if(mUseBrackets){
        wort.append(brackets);
    }
    if(mUseUppercase){
        wort.append(upperCasLetters);
    }
    if(mUseLowercase){
        wort.append(lowerCasLetters);
    }
    if(mUseDigits){
        wort.append(digits);
    }
    if(mUsePlusMinus){
        wort.append(plusMinus);
    }
    if(mUseUnserScore){
        wort.append(underScore);
    }
    if(mUseSpecial){
        wort.append(special);
    }
    if(mUseSpaces){
        wort.append(space);
    }

    return wort;
}
}
