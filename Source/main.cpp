#include "Event/eventmanager.h"
#include "GameManager/gamemanager.h"
#include "Utility/config.h"
#include "Utility/locator.h"

int main()
{
	Locator::provideConfig(std::make_unique<Config>());
	Locator::provideEventManager(std::make_unique<EventManager>());

	GameManager gm;
	if (!gm.start())
		return EXIT_FAILURE;
	return EXIT_SUCCESS;
}
