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
#include "zxcvbn/zxcvbn.h"
#include <cstdint>
#include <algorithm>
#include <iostream>

namespace sec
{

///
/// \brief     Constructor
/// \author    Joachim Danmayr
/// \date      2017-07-01
///
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

///
/// \brief     Generates a random password
/// \author    Joachim Danmayr
/// \date      2017-07-01
/// \param[in] minNrOfDigits    Password must contain at least this number of digits (0 if there is no minimum number)
/// \param[in] minNrOfUpperCase Password must contain at least this number of upper case letters (0 if there is no minimum number)
/// \param[in] minNrOfLowerCase Password must contain at least this number of loer case letters (0 if there is no minimum number)
/// \return generated password
///
std::string PasswordGenerator::GeneratePassword(uint32_t minNrOfDigits, uint32_t minNrOfUpperCase, uint32_t minNrOfLowerCase)
{

    //////////////////////////////////////////////////////////////////////////////////
    // Generate random signes of the values which have defined a fixed minim number
    uint32_t minNrOfPlusMinus  =0;
    uint32_t minNrOfUnderScore =0;
    uint32_t minNumberOfSpecial=0;
    uint32_t minNumberOfBractes=0;
    uint32_t minNumberOfSpaces =0;
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

    //////////////////////////////////////////////////////////////////////////////////
    // Fill the rest to the given password length with random chars from the selected areas
    int32_t numberOfFixedChar = (minNrOfDigits+minNrOfUpperCase+minNrOfLowerCase+minNrOfPlusMinus+minNrOfUnderScore+minNumberOfSpecial+minNumberOfBractes+minNumberOfSpaces);
    if(mPasswordSize > numberOfFixedChar){
        uint32_t restSize = mPasswordSize - (minNrOfDigits+minNrOfUpperCase+minNrOfLowerCase+minNrOfPlusMinus+minNrOfUnderScore+minNumberOfSpecial+minNumberOfBractes+minNumberOfSpaces);
        std::string stringSet = PrepareCharSet();
        password.append(GenerateRandomString(stringSet,restSize));
    }

    //////////////////////////////////////////////////////////////////////////////////
    // Shuffel password
    random_shuffle(password.begin(), password.end());

    return password;
}

///
/// \brief     Generates nr random characters of the given Charset
/// \author    Joachim Danmayr
/// \date      2017-07-01
/// \param[in] charSet  String of characters to generate a random stream from
/// \param[in] nr       Number of random streams which should be generated
/// \return    Random string
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

///
/// \brief     Prepares the charset for the password generator
/// \author    Joachim Danmayr
/// \date      2017-07-01
/// \return    string with all allowed characters
///
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

///
/// \brief     Calcultes the entropy of the given
///            password. A higher value is a better password
///            For calculation zxcvbn library is used
/// \author    Joachim Danmayr
/// \date      2017-10-08
/// \param[in] password     Password which should be checked
/// \return    entropy of the password
///
float PasswordGenerator::CheckQuality(std::string password)
{
    const char *UsrDict[] ={"Onename.Twoname@example.com", "Onename", "Twoname", "example.com", "example",0};
    ZxcMatch_t *Info;
    float e = ZxcvbnMatch(password.c_str(), UsrDict, &Info);
    return e;
}


}
