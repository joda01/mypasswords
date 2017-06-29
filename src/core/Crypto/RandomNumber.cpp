/**
 * @file	RnadomNumber.cpp
 *
 * @author	Joachim Danmayr
 * @date	206-07-16
 *
 */

#include "RandomNumber.h"
#include <cstdlib>
#include <iostream>
#include <ctime>
#include <random>

uint32_t RandomNumber::mStartNumber = 0;

RandomNumber::RandomNumber()
{

}

void RandomNumber::Generate(char* number, uint32_t size, uint32_t startValue, uint32_t range)
{
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<float> dist(0, range);
    for (unsigned int i=0; i<size; ++i){
        number[i] = (char)dist(mt);
    }

    mStartNumber++;
}
