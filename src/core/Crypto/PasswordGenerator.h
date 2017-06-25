/**
* @file	PasswordGenerator.h
*
* Password generator
*
* @author	Joachim Danmayr
* @date     206-07-24
*
*/


#ifndef PASSWORDGENERATOR_H
#define PASSWORDGENERATOR_H

#include <string>

namespace sec
{

class PasswordGenerator
{
public:
    PasswordGenerator(int32_t passwordSize,bool useUppercase,bool useLowercase
                                         ,bool useDigits,bool usePlusMinus
                                         ,bool useUnserScore,bool useSpecial
                                         ,bool useBrackets,bool useSpaces);
    std::string GeneratePassword(uint32_t startValue,int32_t minNrOfDigits, int32_t minNrOfUpperCase, int32_t minNrOfLowerCase);

private:
    static const int32_t MAX_PASSWORD_LENGTH = 32;      /**< Maximum password bytes */

    std::string PrepareCharSet();
    int32_t  mPasswordSize;
    bool mUseUppercase;
    bool mUseLowercase;
    bool mUseDigits;
    bool mUsePlusMinus;
    bool mUseUnserScore;
    bool mUseSpecial;
    bool mUseBrackets;
    bool mUseSpaces;


    std::string upperCasLetters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    std::string lowerCasLetters = "abcdefghijklmnopqrtsuvwxyz";
    std::string digits = "0123456789";
    std::string plusMinus = "+-";
    std::string underScore = "_";
    std::string special = "!$\%&";
    std::string brackets = "[]{}()<>";
    std::string space = " ";

};


}
#endif // PASSWORDGENERATOR_H
