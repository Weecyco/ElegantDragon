#include "DragonDB.h"
#include <iostream>


DragonDB::DragonDB(const unsigned int contextIndexIn, const float homeScalingIn, const std::pair<unsigned int, unsigned int>& resolutionIn) :
    contextIndex(contextIndexIn),
    resolution(resolutionIn), 
    homeScaling(homeScalingIn)
{
    std::cout << "Initializing Dragon App with resolution: " << resolutionIn.first << "x" << resolution.second << std::endl;

    //setup graphics
    if (graphicsSetup())
    {
        std::cout << "[ERROR] Graphics Setup failed" << std::endl;
        PROJ_ASSERT(0);
    }

}

DragonDB::~DragonDB()
{
    //TO DO Lots of mem leak at the moment lol
}