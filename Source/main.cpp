#include <iostream>

#include "GameManager/gamemanager.h"

int main()
{
	GameManager gm;
	if (!gm.start())
		return EXIT_FAILURE;
	return EXIT_SUCCESS;
}
