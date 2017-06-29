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
#include <algorithm>
#include <iostream>



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
std::string PasswordGenerator::GeneratePassword(uint32_t startValue,uint32_t minNrOfDigits, uint32_t minNrOfUpperCase, uint32_t minNrOfLowerCase)
{

    uint32_t minNrOfPlusMinus  =0;
    uint32_t minNrOfUnderScore =0;
    uint32_t minNumberOfSpecial=0;
    uint32_t minNumberOfBractes=0;
    uint32_t minNumberOfSpaces =0;

    RandomNumber randomGenerator;
    if(0==minNrOfDigits && mUseDigits){
        randomGenerator.Generate(minNrOfDigits,mPasswordSize);
    }

    if(0==minNrOfUpperCase && mUseUppercase){
        randomGenerator.Generate(minNrOfUpperCase,mPasswordSize);
    }

    if(0==minNrOfLowerCase && mUseLowercase){
        randomGenerator.Generate(minNrOfLowerCase,mPasswordSize);
    }

    if(0==minNrOfPlusMinus && mUsePlusMinus){
        randomGenerator.Generate(minNrOfPlusMinus,mPasswordSize);
    }

    if(0==minNrOfUnderScore && mUseUnserScore){
        randomGenerator.Generate(minNrOfUnderScore,mPasswordSize);
    }

    if(0==minNumberOfSpecial && mUseSpecial){
        randomGenerator.Generate(minNumberOfSpecial,mPasswordSize);
    }

    if(0==minNumberOfBractes && mUseBrackets){
        randomGenerator.Generate(minNumberOfBractes,mPasswordSize);
    }

    if(0==minNumberOfSpaces && mUseSpaces){
        randomGenerator.Generate(minNumberOfSpaces,mPasswordSize);
    }

    std::string password;

    if(mUseDigits){
        password.append(GenerateRandomString(digitsA,minNrOfDigits));
    }

    if(mUseUppercase){
        password.append(GenerateRandomString(upperCasLetters,minNrOfUpperCase));
    }

    if(mUseLowercase){
        password.append(GenerateRandomString(lowerCasLetters,minNrOfLowerCase));
    }

    if(mUsePlusMinus){
        password.append(GenerateRandomString(plusMinus,minNrOfPlusMinus));
    }

    if(mUseUnserScore){
        password.append(GenerateRandomString(underScore,minNrOfUnderScore));
    }

    if(mUseSpecial){
        password.append(GenerateRandomString(special,minNumberOfSpecial));
    }

    if(mUseBrackets){
        password.append(GenerateRandomString(brackets,minNumberOfBractes));
    }

    if(mUseSpaces){
        password.append(GenerateRandomString(space,minNumberOfSpaces));
    }

    // Shuffel password
    random_shuffle(password.begin(), password.end());
    //password.resize(mPasswordSize);



    return password;
}


///
/// \brief Generates nr random characters of the given Charset
/// \param charSet  String of characters to generate a random stream from
/// \param nr       Number of random streams which should be generated
/// \return         Random string
///
std::string PasswordGenerator::GenerateRandomString(std::string& charSet, uint32_t nr)
{
    std::string password;
    char randomNumbe[MAX_PASSWORD_LENGTH];

    RandomNumber randomGenerator;
    randomGenerator.Generate(randomNumbe,nr,0);

    if(charSet.size()>0){
        for(unsigned int i = 0; i < nr; i++ ){
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
        wort.append(digitsA);
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
