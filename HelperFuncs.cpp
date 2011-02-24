#include "HelperFuncs.h"

bool		bReadyToExit;

void initHelperFuncs()
{
	srand(time(NULL));

	bReadyToExit = false;
}

void setExit()
{
	bReadyToExit = true;
}


