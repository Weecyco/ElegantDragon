#include "DragonDB.h"
#include <iostream>


DragonDB::DragonDB(const unsigned int contextIndexIn, const float homeScalingIn, const std::pair<unsigned int, unsigned int>& resolutionIn) :
    contextIndex(contextIndexIn),
    resolution(resolutionIn), 
    homeScaling(homeScalingIn), 
    camLoc(MathVec<float>(2))
{
    std::cout << "Initializing Dragon App with resolution: " << resolutionIn.first << "x" << resolution.second << std::endl;
    camScaling = homeScalingIn;
    camLoc[0] = 0;//-20;
    camLoc[1] = 0;//-12;

    xScale = 1.0;
    yScale = 1.0;

    //simplified of what's below. defines how compressed a side needs to be 
    //to account for the screen space stretching
    (resolution.first > resolution.second) ? \
        xScale = static_cast<float>(resolution.second) / static_cast<float>(resolution.first) : \
        yScale = static_cast<float>(resolution.first) / static_cast<float>(resolution.second);
    //if (resolution.first == resolution.second)
    //{
    //    xScale = yScale = 1.0;
    //}
    //else if (resolution.first < resolution.second)
    //{
    //    xScale = 1.0;
    //    yScale = static_cast<float>(resolution.first) / static_cast<float>(resolution.second); //scale down y input to match x
    //}
    //else if (resolution.first > resolution.second)
    //{
    //    xScale = static_cast<float>(resolution.second) / static_cast<float>(resolution.first); //scale down x input to match y
    //    yScale = 1.0;
    //}
    std::cout << "xScale: " << xScale << ", yScale: " << yScale << std::endl;

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

double DragonDB::getCurrentScaling()
{
    return camScaling;
}

float* DragonDB::worldToScreen(const float* src, unsigned int size, unsigned int entrySize/* = 2*/)
{
    PROJ_ASSERT_W_MSG(entrySize > 1, "World to screen conversion Entry size must be greater than 1");
    float* out = new float[size];
    for (int drawIdx = 0; drawIdx + 1 < size; drawIdx += entrySize)
    {
        out[drawIdx] = (camLoc[0] + src[drawIdx]) * camScaling * xScale;
        out[drawIdx + 1] = (camLoc[1] + src[drawIdx + 1]) * camScaling * yScale;
    }
    return out;
}

void DragonDB::transCam(CameraTranType transitionTypeIn, std::chrono::milliseconds durationIn, MathVec<float> location)
{
    
}

void DragonDB::cameraHandle()
{
    switch (transitionType)
    {
    case CameraTranType::Jump:
    case CameraTranType::Linear:
    case CameraTranType::Smoothed:
        break;
    }
}

