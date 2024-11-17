#include <stdlib.h> //needed for exit()
#include "osal.h"

int _init()
{
	int i;
	for(i = 0; i < 10; i++)
	{
		printf("Setting screen brightness to: %d\n", i);
		OsScrBrightness(i);
		sleep(1);
	}

	exit(0); //required because return would continue into the original loader
}