#ifndef _DEBUG
#define _DEBUG

#define DEBUG
#ifdef DEBUG
	#define debug printf
#else
	#define debug(...)
#endif

#endif