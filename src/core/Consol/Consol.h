/**
 * @file	Consol.h
 *
 * Consol view
 *
 * @author	Joachim Danmayr
 * @date	206-07-16
 *
 */

#ifndef CONSOL_H_
#define CONSOL_H_

#include <cstdint>
#include "..\PasswordContainer\Container.h"


class Consol
{
public:
	Consol(Container* passwordContainer);
	int Start();

private:
    static const uint32_t TABLE_WIDTH = 15;

	void AddEntry();
	void ShowEntry();
    void ShowStatistic();
    void PrintEntry(char* data, unsigned int size);
    void ShowMenuForOpenContainer();
	Container* mPasswordContainer;
};

#endif //CONSOL_H_
