#include "fuseLib.h"
#include "myFS.h"

#include <stdlib.h>
#include <string.h>


MiSistemaDeFicheros miSistemaDeFicheros;

#define USO		"Uso: %s [-t tamDisco] [-m] -a nombreArchivo -f 'opciones fuse'\n"
#define EJEMPLO		"Ejemplo:\n%s -t 2097152 -a disco-virtual -f '-d -s punto-montaje'\n"
#define EJEMPLO2	"Ejemplo:\n%s -a disco-virtual -m -f '-d -s punto-montaje'\n"

int main(int argc, char **argv) {
	miSistemaDeFicheros.numNodosLibres = MAX_NODOSI;

	int ret; // Código de retorno de las llamadas a funciones

	int opt, tamDisco=-1;
	char * nombreArchivo=NULL;
	char *argsFUSE=NULL;
	int mount=0;
	
	char *argvNew[MAX_FUSE_NARGS];
	char *pTmp;
	
	while((opt = getopt(argc, argv, "t:a:f:m")) != -1) {
		switch(opt) {
			case 't':
				tamDisco=atoi(optarg);
				break;
			case 'a':
				nombreArchivo=optarg;
				break;
			case 'f':
				argsFUSE=optarg;
				break;
			case 'm':
				mount=1;
				break;
			default: /* '?' */
				fprintf(stderr, USO, argv[0]);
				fprintf(stderr, EJEMPLO, argv[0]);
				exit(-1);
		}
	}


	if (!mount) {
		//Falta algún parámetro?
		if(tamDisco==-1 || nombreArchivo==NULL || argsFUSE==NULL){
			fprintf(stderr, USO, argv[0]);
			fprintf(stderr, EJEMPLO, argv[0]);
			exit(-1);
		}
		
		// Formateamos el fichero según nuestro formato
		ret = myMkfs(&miSistemaDeFicheros, tamDisco, nombreArchivo);
	}else {
		//Falta algún parámetro?
		if(nombreArchivo==NULL || argsFUSE==NULL){
			fprintf(stderr, USO, argv[0]);
			fprintf(stderr, EJEMPLO2, argv[0]);
			exit(-1);
		}
		
		//Montamos el sistema de ficheros
		ret = myMount(&miSistemaDeFicheros, nombreArchivo);	
	}
	
	if(ret) {
		fprintf(stderr, "Incapaz de formatear/montar, código de error: %d\n", ret);
		exit(-1);
	}
	fprintf(stderr, "Sistema de ficheros disponible\n");
	
	//Parseamos los arugumentos para FUSE
	argc=1;
	argvNew[0]=argv[0];

	pTmp = strtok(argsFUSE, " ");
	while (pTmp && argc < MAX_FUSE_NARGS){
		argvNew[argc++] = pTmp;
		pTmp = strtok(0, " ");
	}

	//Montamos el SF, saldremos con Control-C
	if( (ret=fuse_main(argc, argvNew, &myFS_operations, NULL)) ){
		fprintf(stderr, "Error al montar sistema de ficheros fuse\n");
		return(ret);
	}
	
	myFree(&miSistemaDeFicheros);
	
	return(0);
}
