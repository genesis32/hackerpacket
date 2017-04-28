// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <time.h>
#include <vector>
// TODO: reference additional headers your program requires here

#pragma warning(disable: 4018)
#define RANDOM_NUM (static_cast<float>(rand())/static_cast<float>(RAND_MAX))