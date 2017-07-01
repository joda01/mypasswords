/**
 * @file	RnadomNumber.H
 *
 * Generates a random number with given length
 *
 * @author	Joachim Danmayr
 * @date	206-07-16
 *
 */

#include <cstdint>

class RandomNumber
{
public:
    RandomNumber();
    static void Generate(char *number, uint32_t size, uint32_t startValue=0, uint32_t range=256);
    static void Generate(uint32_t& number, uint32_t range);
    static uint32_t mStartNumber;
};
