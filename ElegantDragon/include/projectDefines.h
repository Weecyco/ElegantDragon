#pragma once
#include <iostream>

//Version defines (filled by cmake)
#ifndef DRAGON_VERSION_MAJOR
#define DRAGON_VERSION_MAJOR 0xDEADBEEF
#endif

#ifndef DRAGON_VERSION_MINOR
#define DRAGON_VERSION_MINOR 0xDEADBEEF
#endif

//Universal defines 
#define PI 3.141592653589793238463
#define MSVC_ASSERT_BREAK //for using MSVC and you want to break into debugger
#define OGL_ENABLE_SAFE_CALL //for debugging general OGL calls

//Peformance counter
#define PERF_COUNT_BUFFER_SIZE 300

#define ERROR_ON //turns on error messages
#define WARNINGS_ON //turns on warning messages



//What happens when errors occur? how should it assert?
//determines if it breaks into debugger upon error
#ifdef MSVC_ASSERT_BREAK
#define PROJ_ASSERT(x) if(!(x)) __debugbreak();//__debugbreak is specific (intrensic) to Visual studio (msvc compiler). need to used different one if porting to gcc or clang
#endif
#ifndef PROJ_ASSERT
#define PROJ_ASSERT(x) assert(x);// creates prompt and stops the program. Should allow you check symbols still (but not full debug)
#endif

//Produces a error report
#ifdef ERROR_ON
#define PROJ_ERROR(msg) std::cout << "[ERROR] " << __FILE__ << " : " << __LINE__ << " - " << msg << std::endl;
#endif
#ifndef PROJ_ERROR
#define PROJ_ERROR(msg)
#endif


//Produces a warning report
#ifdef WARNINGS_ON
#define PROJ_WARNING(msg) std::cout << "[WARNING] " << __FILE__ << " : " << __LINE__ << " - " << msg << std::endl;
#endif
#ifndef PROJ_WARNING
#define PROJ_WARNING(msg)
#endif

//duplicate checks but meh
#define PROJ_ASSERT_W_MSG(x, msg) if(!(x)) {PROJ_ERROR(msg); PROJ_ASSERT(0);}; //produces an assert with a message
#define PROJ_SOFT_ASSERT_W_MSG(x, msg) if(!(x)) {PROJ_WARNING(msg)}; // just a warning with a condition

namespace EDrgn
{
    enum class Direction2
    {
        x = 0,
        y = 1
    };
    enum class Direction3
    {
        x = 0,
        y = 1,
        z = 2
    };
    enum class Direction4
    {
        x = 0,
        y = 1,
        z = 2,
        w = 4
    };
} //namespace EDrgn