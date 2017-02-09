#include <stdio.h>
#include <stdlib.h>
#include "archi.h"

int main(int argc,char* argv[])
{

	int i;

	printf("Hola ...%d\n", VAR);

	if( argc >= 1 )
		for( i=0; i < argc; i++)
			printf("Parametro %d: %s\n",i,argv[i]);

	exit(1);
}
