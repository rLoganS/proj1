#include <iostream>

#include "RandomTester.hpp"

// Leak Detection (begin)
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
// Leak Detection (end)

void run(AbstractTester* tester)
{
	tester->runAll();

	tester->report();

	delete tester;
}

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	run(new RandomTester());

	system("pause");

	return 0;
}
