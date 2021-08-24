// ElegantDragon.cpp : Defines the entry point for the application.
//
#include "cmakeConfig.h"

#include "ElegantDragon.h"
#include "projectDefines.h"
#include "DragonDB.h"
#include "dragonApp.h"

#include <unordered_map>

int main()
{
	printf(" === Welcome to Elegant Dragon! Version %d.%d ===\n", DRAGON_VERSION_MAJOR, DRAGON_VERSION_MINOR);
	dragonApp(1920, 900);

	return 0;
}
