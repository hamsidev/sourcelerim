#include <stdio.h>

void WriteVersion()
{
#ifndef __WIN32__
	FILE* fp = fopen("VERSION.txt", "w");

	if (fp)
	{
		fprintf(fp, "__GAME_VERSION__: %s\n", __GAME_VERSION__);
		fprintf(fp, "%s@%s:%s\n", "cG9zZWlkb24=", __HOSTNAME__, __PWD__);
		fclose(fp);
	}
#endif
}
// vaffanculo a chi t'e morto martysama
