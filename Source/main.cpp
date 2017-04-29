
#include "GameManager\gamemanager.h"

#include <iostream>

int main()
{
	GameManager gm;
	if (!gm.start())
		return EXIT_FAILURE;
	return EXIT_SUCCESS;
}