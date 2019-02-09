// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files
#include <windows.h>




#ifdef CLIENT_H3

#define CDISPOSER_INSTANCE_ADDY 0x00CD93F4
#define CDISPOSER_SENDPACKET_FUNC_ADDY 0x00407EF0
#define EPOCHID_INSTANCE_ADDY 0x00D17988

#else
#define CDISPOSER_INSTANCE_ADDY nullptr
#define CDISPOSER_SENDPACKET_FUNC_ADDY nullptr
#define EPOCHID_INSTANCE_ADDY nullptr
#endif 

// reference additional headers your program requires here
