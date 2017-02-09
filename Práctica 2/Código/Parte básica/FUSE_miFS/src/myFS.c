#include "myFS.h"
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>



// Formatea el disco virtual. Guarda el mapa de bits del super bloque y el directorio único.
int myMkfs(MiSistemaDeFicheros* miSistemaDeFicheros, int tamDisco, char* nombreArchivo) {
	// Creamos el disco virtual:
	miSistemaDeFicheros->fdDiscoVirtual = open(nombreArchivo, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);

	// Algunas comprobaciones mínimas:
	assert(sizeof (EstructuraSuperBloque) <= TAM_BLOQUE_BYTES);
	assert(sizeof (EstructuraDirectorio) <= TAM_BLOQUE_BYTES);
	int numBloques = tamDisco / TAM_BLOQUE_BYTES;
	int minNumBloques = 3 + MAX_BLOQUES_CON_NODOSI + 1;
	int maxNumBloques = NUM_BITS;
	if (numBloques < minNumBloques) {
		return 1;
	}
	if (numBloques >= maxNumBloques) {
		return 2;
	}

	/// MAPA DE BITS
	// Inicializamos el mapa de bits
	int i;
	for (i = 0; i < NUM_BITS; i++) {
		miSistemaDeFicheros->mapaDeBits[i] = 0;
	}

	// Los primeros tres bloques tendrán el superbloque, mapa de bits y directorio
	miSistemaDeFicheros->mapaDeBits[MAPA_BITS_IDX] = 1;
	miSistemaDeFicheros->mapaDeBits[SUPERBLOQUE_IDX] = 1;
	miSistemaDeFicheros->mapaDeBits[DIRECTORIO_IDX] = 1;
	// Los siguientes NUM_INODE_BLOCKS contendrán nodos-i
	for (i = 3; i < 3 + MAX_BLOQUES_CON_NODOSI; i++) {
		miSistemaDeFicheros->mapaDeBits[i] = 1;
	}
	escribeMapaDeBits(miSistemaDeFicheros);

	/// DIRECTORIO
	// Inicializamos el directorio
	miSistemaDeFicheros->directorio.numArchivos = 0;
	for (i = 0; i < MAX_ARCHIVOS_POR_DIRECTORIO; i++) {
		miSistemaDeFicheros->directorio.archivos[i].libre = 1;
	}
	escribeDirectorio(miSistemaDeFicheros);

	/// NODOS-I
	EstructuraNodoI nodoActual;
	nodoActual.libre = 1;
	for (i = 0; i < MAX_NODOSI; i++) {
		escribeNodoI(miSistemaDeFicheros, i, &nodoActual);
	}

	/// SUPERBLOQUE
	initSuperBloque(miSistemaDeFicheros, tamDisco);
	escribeSuperBloque(miSistemaDeFicheros);
	sync();

	// Al finalizar tenemos al menos un bloque
	assert(myQuota(miSistemaDeFicheros) >= 1);

	initNodosI(miSistemaDeFicheros);
	
	printf("SF: %s, %d B (%d B/bloque), %d bloques\n", nombreArchivo, tamDisco, TAM_BLOQUE_BYTES, numBloques);
	printf("1 bloque para SUPERBLOQUE (%u B)\n", (unsigned int)sizeof(EstructuraSuperBloque));
	printf("1 bloque para MAPA DE BITS, que cubre %u bloques, %u B\n", (unsigned int)NUM_BITS, (unsigned int)(NUM_BITS * TAM_BLOQUE_BYTES));
	printf("1 bloque para DIRECTORIO (%u B)\n", (unsigned int)sizeof(EstructuraDirectorio));
	printf("%d bloques para nodos-i (a %u B/nodo-i, %u nodos-i)\n",MAX_BLOQUES_CON_NODOSI,(unsigned int)sizeof(EstructuraNodoI),(unsigned int)MAX_NODOSI);
	printf("%d bloques para datos (%d B)\n",miSistemaDeFicheros->superBloque.numBloquesLibres,TAM_BLOQUE_BYTES*miSistemaDeFicheros->superBloque.numBloquesLibres);
	printf("¡Formato completado!\n");
	
	return 0;
}

/* Inicializa el superbloque */
void initSuperBloque(MiSistemaDeFicheros* miSistemaDeFicheros, int tamDisco) {
    miSistemaDeFicheros->superBloque.tamDiscoEnBloques = tamDisco / TAM_BLOQUE_BYTES;
    miSistemaDeFicheros->superBloque.numBloquesLibres = myQuota(miSistemaDeFicheros);

	miSistemaDeFicheros->superBloque.fechaCreacion=time(NULL);

    miSistemaDeFicheros->superBloque.tamBloque = TAM_BLOQUE_BYTES;
    miSistemaDeFicheros->superBloque.maxTamNombreArchivo = MAX_TAM_NOMBRE_ARCHIVO;
    miSistemaDeFicheros->superBloque.maxBloquesPorArchivo = MAX_BLOQUES_POR_ARCHIVO;
}

void myFree(MiSistemaDeFicheros* miSistemaDeFicheros) {
	int i;
    close(miSistemaDeFicheros->fdDiscoVirtual);
    for(i=0; i<MAX_NODOSI; i++) {
    	free(miSistemaDeFicheros->nodosI[i]);
    	miSistemaDeFicheros->nodosI[i] = NULL;
    }
}


int escribeMapaDeBits(MiSistemaDeFicheros* miSistemaDeFicheros) {
    if (lseek(miSistemaDeFicheros->fdDiscoVirtual, TAM_BLOQUE_BYTES * MAPA_BITS_IDX, SEEK_SET) == (off_t) - 1) {
        perror("Falló lseek en escribeMapaDeBits");
        return -1;
    }
    if (write(miSistemaDeFicheros->fdDiscoVirtual,
            miSistemaDeFicheros->mapaDeBits, sizeof (BIT) * NUM_BITS) == -1) {
        perror("Falló write en escribeMapaDeBits");
        return -1;
    }
    return 0;
}

int escribeNodoI(MiSistemaDeFicheros* miSistemaDeFicheros, int numNodoI, EstructuraNodoI* nodoI) {
    int posNodoI;
    assert(numNodoI < MAX_NODOSI);
    posNodoI = calculaPosNodoI(numNodoI);

    if (lseek(miSistemaDeFicheros->fdDiscoVirtual, posNodoI, SEEK_SET) == (off_t) - 1) {
        perror("Falló lseek en escribeNodoI");
        return -1;
    }
    if (write(miSistemaDeFicheros->fdDiscoVirtual, nodoI, sizeof (EstructuraNodoI)) == -1) {
        perror("Falló write en escribeNodoI");
    }
    sync();
    return 0;
}

int escribeSuperBloque(MiSistemaDeFicheros* miSistemaDeFicheros) {
    if (lseek(miSistemaDeFicheros->fdDiscoVirtual, TAM_BLOQUE_BYTES * SUPERBLOQUE_IDX, SEEK_SET) == (off_t) - 1) {
        perror("Falló lseek en escribeSuperBloque");
        return -1;
    }
    if (write(miSistemaDeFicheros->fdDiscoVirtual, &(miSistemaDeFicheros->superBloque), sizeof (EstructuraSuperBloque)) == -1) {
        perror("Falló write en escribeSuperBloque");
        return -1;
    }
    return 0;
}

int escribeDirectorio(MiSistemaDeFicheros* miSistemaDeFicheros) {
    if (lseek(miSistemaDeFicheros->fdDiscoVirtual, TAM_BLOQUE_BYTES * DIRECTORIO_IDX, SEEK_SET) == (off_t) - 1) {
        perror("Falló lseek en escribeDirectorio");
        return -1;
    }
    if (write(miSistemaDeFicheros->fdDiscoVirtual, &(miSistemaDeFicheros->directorio), sizeof (EstructuraDirectorio)) == -1) {
        perror("Falló write en escribeDirectorio");
        return -1;
    }
    return 0;
}

int calculaPosNodoI(int numNodoI) {
    int whichInodeBlock;
    int whichInodeInBlock;
    int inodeLocation;

    whichInodeBlock = numNodoI / NODOSI_POR_BLOQUE;
    whichInodeInBlock = numNodoI % NODOSI_POR_BLOQUE;

    inodeLocation = (NODOI_IDX + whichInodeBlock) * TAM_BLOQUE_BYTES + whichInodeInBlock * sizeof (EstructuraNodoI);
    return inodeLocation;
}

void initNodosI(MiSistemaDeFicheros* miSistemaDeFicheros) {
    int numNodoI;
    EstructuraNodoI* temp = malloc(sizeof (EstructuraNodoI));

    for (numNodoI = 0; numNodoI < MAX_NODOSI; numNodoI++) {
        leeNodoI(miSistemaDeFicheros, numNodoI, temp);
        if (temp->libre) {
            miSistemaDeFicheros->nodosI[numNodoI] = NULL;
        } else {
            miSistemaDeFicheros->numNodosLibres--;
            miSistemaDeFicheros->nodosI[numNodoI] = malloc(sizeof (EstructuraNodoI));
            copiaNodoI(miSistemaDeFicheros->nodosI[numNodoI], temp);
        }
    }
}

int leeNodoI(MiSistemaDeFicheros* miSistemaDeFicheros, int numNodoI, EstructuraNodoI* nodoI) {
    int posNodoI;
    assert(numNodoI < MAX_NODOSI);
    posNodoI = calculaPosNodoI(numNodoI);

    lseek(miSistemaDeFicheros->fdDiscoVirtual, posNodoI, SEEK_SET);
    read(miSistemaDeFicheros->fdDiscoVirtual, nodoI, sizeof (EstructuraNodoI));
    return 0;
}

void copiaNodoI(EstructuraNodoI* dest, EstructuraNodoI* src) {
    int i;

    dest->numBloques = src->numBloques;
    dest->tamArchivo = src->tamArchivo;
    dest->tiempoModificado = src->tiempoModificado;
    dest->libre = src->libre;

    for (i = 0; i < MAX_BLOQUES_POR_ARCHIVO; i++)
        dest->idxBloques[i] = src->idxBloques[i];
}

int buscaNodoLibre(MiSistemaDeFicheros* miSistemaDeFicheros) {
    int i;
    for (i = 0; i < MAX_NODOSI; i++) {
        if (miSistemaDeFicheros->nodosI[i] == NULL)
            return i;
    }
    return -1; // NO hay nodos-i libres. Esto no debería ocurrir.
}

int buscaPosDirectorio(MiSistemaDeFicheros* miSistemaDeFicheros, char* nombre) {
    int i;

    for (i = 0; i < MAX_ARCHIVOS_POR_DIRECTORIO; i++) {
        if (miSistemaDeFicheros->directorio.archivos[i].libre == false) {
            if (strcmp(nombre, miSistemaDeFicheros->directorio.archivos[i].nombreArchivo) == 0)
                return i;
        }
    }
    return -1;
}


// Devuelve el no de bloques libres en el FS.
int myQuota(MiSistemaDeFicheros* miSistemaDeFicheros) {
    int freeCount = 0;
    int i;
    // Calculamos el número de bloques libres
    for (i = 0; i < miSistemaDeFicheros->superBloque.tamDiscoEnBloques; i++) {
        // Ahora estamos usando uint para representar cada bit.
        // Podríamos usar todos los bits para mejorar el almacenamiento
        if (miSistemaDeFicheros->mapaDeBits[i] == 0) {
            freeCount++;
        }
    }
    return freeCount;
}

void reservaBloquesNodosI(MiSistemaDeFicheros* miSistemaDeFicheros, DISK_LBA idxBloques[], int numBloques) {
    int i = 0;
    int bloqueActual = 0;

    while ((bloqueActual < numBloques) && (i < NUM_BITS)) {
        if (miSistemaDeFicheros->mapaDeBits[i] == 0) {
            miSistemaDeFicheros->mapaDeBits[i] = 1;
            idxBloques[bloqueActual] = i;
            bloqueActual++;
        }
        ++i;
    }
}

/* Código Parte Opcional (A COMPLETAR) */

int leeMapaDebits(MiSistemaDeFicheros* miSistemaDeFicheros) {
	if (lseek(miSistemaDeFicheros->fdDiscoVirtual, TAM_BLOQUE_BYTES * MAPA_BITS_IDX, SEEK_SET) == (off_t) - 1) {
        	perror("Falló lseek en leeMapaDeBits");
        	return -1;
    	}
	if (read(miSistemaDeFicheros->fdDiscoVirtual, miSistemaDeFicheros->mapaDeBits, sizeof (BIT) * NUM_BITS) == -1) {
		perror("Falló read en leeMapaDeBits");
		return -1;
	}
	
	return 0;	
}

int leeDirectorio(MiSistemaDeFicheros* miSistemaDeFicheros) {
	if (lseek(miSistemaDeFicheros->fdDiscoVirtual, TAM_BLOQUE_BYTES * DIRECTORIO_IDX, SEEK_SET) == (off_t) - 1) {
		perror("Falló lseek en leeDirectorio");
		return -1;
	}
	if (read(miSistemaDeFicheros->fdDiscoVirtual, &(miSistemaDeFicheros->directorio), sizeof (EstructuraDirectorio)) == -1) {
		perror("Falló read en leeDirectorio");
		return -1;
	}

	return 0;
}

int leeSuperbloque(MiSistemaDeFicheros* miSistemaDeFicheros) {
	if (lseek(miSistemaDeFicheros->fdDiscoVirtual, TAM_BLOQUE_BYTES * SUPERBLOQUE_IDX, SEEK_SET) == (off_t) - 1) {
		perror("Falló lseek en leeSuperBloque");
		return -1;
	}
	if (read(miSistemaDeFicheros->fdDiscoVirtual, &(miSistemaDeFicheros->superBloque), sizeof (EstructuraSuperBloque)) == -1) {
		perror("Falló read en leeSuperBloque");
		return -1;
	}

	return 0;
}

int leeNodosI(MiSistemaDeFicheros* miSistemaDeFicheros) {
	
	int numNodoI;
	EstructuraNodoI* temp = malloc(sizeof (EstructuraNodoI));

	for (numNodoI = 0; numNodoI < MAX_NODOSI; numNodoI++) {
		leeNodoI(miSistemaDeFicheros, numNodoI, temp);
		if (temp->libre) {
		    miSistemaDeFicheros->nodosI[numNodoI] = NULL;
		} else {
		    miSistemaDeFicheros->numNodosLibres--;
		    miSistemaDeFicheros->nodosI[numNodoI] = malloc(sizeof (EstructuraNodoI));
		    copiaNodoI(miSistemaDeFicheros->nodosI[numNodoI], temp);
		}
	}
	
	return 0;
}

// Realiza el montaje de un disco virtual ya formateado
int myMount(MiSistemaDeFicheros* miSistemaDeFicheros, char* nombreArchivo) {
	// Abrir el disco virtual
	if ((miSistemaDeFicheros->fdDiscoVirtual = open(nombreArchivo, O_RDWR))==-1){
		perror(nombreArchivo);
		return 1;
	}
	
	if (leeMapaDebits(miSistemaDeFicheros)!=0){
		fprintf(stderr,"Error al leer mapa de bits\n");
		return 2;
	}
	
	if (leeSuperbloque(miSistemaDeFicheros)!=0){
		fprintf(stderr,"Error al leer el superbloque\n");
		return 3;
	}

	if (leeNodosI(miSistemaDeFicheros)!=0){
		fprintf(stderr,"Error al leer los nodos-i\n");
		return 4;
	}	

	if (leeDirectorio(miSistemaDeFicheros)!=0){
		fprintf(stderr,"Error al leer el directorio\n");
		return 5;
	}
	
        printf("SF: %s, %d B (%d B/bloque), %d bloques\n", nombreArchivo, miSistemaDeFicheros->superBloque.tamDiscoEnBloques*TAM_BLOQUE_BYTES, TAM_BLOQUE_BYTES, miSistemaDeFicheros->superBloque.tamDiscoEnBloques);
        printf("1 bloque para SUPERBLOQUE (%u B)\n",(unsigned int)sizeof(EstructuraSuperBloque));
        printf("1 bloque para MAPA DE BITS, que cubre %u bloques, %u B\n", (unsigned int)NUM_BITS, (unsigned int) NUM_BITS * TAM_BLOQUE_BYTES);
        printf("1 bloque para DIRECTORIO (%u B)\n", (unsigned int)sizeof(EstructuraDirectorio));
        printf("%d bloques para nodos-i (a %u B/nodo-i, %u nodos-i)\n",MAX_BLOQUES_CON_NODOSI,(unsigned int)sizeof(EstructuraNodoI),(unsigned int)MAX_NODOSI);
        printf("%d bloques para datos (%d B)\n",miSistemaDeFicheros->superBloque.numBloquesLibres,TAM_BLOQUE_BYTES*miSistemaDeFicheros->superBloque.numBloquesLibres);
        printf("Carga completada!\n");
	return 0;
} 

