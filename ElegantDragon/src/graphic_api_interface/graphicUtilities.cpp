#include "graphicUtilities.h"

#include <iostream>

void errorCallbackFunc(int errorCode, const char* description)
{
    std::cout << "[ERROR] [GLFW] (" << errorCode << ") " << description << std::endl;
}

//simplified overload
bool glLoopError(bool output)
{
    bool errorFlag = false;
    while (GLenum errorCode = glGetError())
    {
        errorFlag = true;
        if (output)
            std::cout << "[ERROR][OPENGL_ERROR_CODE] (0x" << std::hex << errorCode << ")" << std::endl << std::endl;
    }
    if (errorFlag)
        return 1;
    return 0;
}

//complex overload
bool glLoopError(bool output, const char* function, const char* file, int line)
{
    GLenum errorCode = glGetError();//this style of of puting the def and recalling in the loop can be simplified. see the simplified overload
    bool errorFlag = false;
    while (errorCode)
    {
        errorFlag = true;
        if (output)
            std::cout << "[ERROR][OPENGL] (0x" << std::hex << errorCode << ") in " <<
            file << ": " << function << " line " << std::dec << line << std::endl << std::endl;
        errorCode = glGetError();
    }
    if (errorFlag)
        return 1;
    return 0;
}
