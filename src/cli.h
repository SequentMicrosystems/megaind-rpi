#ifndef __CLI__
#define __CLI__

#include "megaind.h"


typedef struct
{
	const char *name;
	const int namePos;
	int (*pFunc)(int, char**);
	const char *help;
	const char *usage1;
	const char *usage2;
	const char *example;
} CliCmdType;



#endif
