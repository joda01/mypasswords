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
    uint32_t seed_val=startValue;
    std::mt19937 rng;
    rng.seed(seed_val);
    std::uniform_int_distribution<uint32_t> uint_dist(0,range);
    for (uint32_t n = 0;n < size;n++) {
        number[n] = uint_dist(rng);
    }
    mStartNumber++;
}
