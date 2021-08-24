#pragma once
#include <GL/glew.h>
#include "projectDefines.h"
// graphics utilities is for graphical error handling and 

//**Below are definitions dependent on tthe project defines


/* Definitions for GL_SAFE:
 * Disable GL_SAFE for better performance
 */
#ifdef OGL_ENABLE_SAFE_CALL
#define GL_SAFE(x) \
    glLoopError(false);\
    x;\
    PROJ_ASSERT(!glLoopError(true, #x, __FILE__, __LINE__));
#endif

#ifndef GL_SAFE
#define GL_SAFE(x) x;
#endif

void errorCallbackFunc(int errorCode, const char* description);
bool glLoopError(bool output);
bool glLoopError(bool output, const char* function, const char* file, int line);
