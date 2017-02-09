#ifndef _MYFS_H_
#define _MYFS_H_

#include <stdio.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include <assert.h>

#define false 0
#define true 1

#define BIT unsigned 
#define TAM_BLOQUE_BYTES 4096
#define NUM_BITS (TAM_BLOQUE_BYTES/sizeof(BIT))
#define MAX_BLOQUES_CON_NODOSI 5
#define MAX_BLOQUES_POR_ARCHIVO 100
#define MAX_ARCHIVOS_POR_DIRECTORIO 100
#define MAX_TAM_NOMBRE_ARCHIVO 15
#define DISK_LBA int
#define BOOLEAN int

#define SUPERBLOQUE_IDX 0
#define MAPA_BITS_IDX 1
#define DIRECTORIO_IDX 2
#define NODOI_IDX 3

// ESTRUCTURAS
typedef struct EstructuraArchivo {
  int  idxNodoI;                                // Nodo-i asociado
  char nombreArchivo[MAX_TAM_NOMBRE_ARCHIVO+1]; // Nombre archivo
  BOOLEAN libre;                                // Archivo libre
} EstructuraArchivo;

typedef struct EstructuraDirectorio {
  int numArchivos;                                         // Núm. archivos
  EstructuraArchivo archivos[MAX_ARCHIVOS_POR_DIRECTORIO]; // Archivos
} EstructuraDirectorio;

typedef struct EstructuraNodoI {
  int numBloques;                               // Núm. bloques
  int tamArchivo;                               // Tamaño archivo
  time_t tiempoModificado;                      // Tiempo de modificación
  DISK_LBA idxBloques[MAX_BLOQUES_POR_ARCHIVO]; // Bloques
  BOOLEAN libre;                                // Nodo libre
} EstructuraNodoI;

#define NODOSI_POR_BLOQUE (TAM_BLOQUE_BYTES/sizeof(EstructuraNodoI))
#define MAX_NODOSI (NODOSI_POR_BLOQUE * MAX_BLOQUES_CON_NODOSI)

typedef struct EstructuraSuperBloque {
  time_t fechaCreacion;     // Fecha en la que se creó el sistema de ficheros

  int tamDiscoEnBloques;    // Núm. de bloques en disco
  int numBloquesLibres;     // Núm. de bloques libres

  int tamBloque;            // Tamaño de bloque
  int maxTamNombreArchivo;  // Tamaño máx. de nombre de archivo
  int maxBloquesPorArchivo; // Tamaño máx. de bloques por archivo
} EstructuraSuperBloque;

typedef struct MiSistemaDeFicheros {
    int fdDiscoVirtual;                  // Descriptor del fichero que almacena 'mi sistema de ficheros'
    EstructuraSuperBloque superBloque;   // Superbloque
    BIT mapaDeBits[NUM_BITS];            // Mapa de bits
    EstructuraDirectorio directorio;     // Directorio raíz
    EstructuraNodoI* nodosI[MAX_NODOSI]; // Array de punteros a Nodos-i
    int numNodosLibres;                  // Número de nodos-i libres
} MiSistemaDeFicheros;


// Formatea el disco virtual. Guarda el mapa de bits del super bloque y el directorio único.
int myMkfs(MiSistemaDeFicheros* miSistemaDeFicheros, int tamDisco, char* nombreArchivo);
// (Parte opcional) Realiza el montaje de un disco virtual ya formateado
int myMount(MiSistemaDeFicheros* miSistemaDeFicheros, char* nombreArchivo);
// Libera memoria y cierra el sistema de ficheros
void myFree(MiSistemaDeFicheros* miSistemaDeFicheros);


int escribeMapaDeBits(MiSistemaDeFicheros* miSistemaDeFicheros);
int escribeNodoI(MiSistemaDeFicheros* miSistemaDeFicheros, int numNodoI, EstructuraNodoI* nodoI);
void initSuperBloque(MiSistemaDeFicheros* miSistemaDeFicheros, int tamDisco);
int escribeSuperBloque(MiSistemaDeFicheros* miSistemaDeFicheros);
int escribeDirectorio(MiSistemaDeFicheros* miSistemaDeFicheros);
int escribeDatos(MiSistemaDeFicheros* miSistemaDeFicheros, int archivoExterno, int numNodoI);
int calculaPosNodoI(int numNodoI);
void initNodosI(MiSistemaDeFicheros* miSistemaDeFicheros);
int leeNodoI(MiSistemaDeFicheros* miSistemaDeFicheros, int numNodoI, EstructuraNodoI* nodoI);
void copiaNodoI(EstructuraNodoI* dest, EstructuraNodoI* src);
int buscaNodoLibre(MiSistemaDeFicheros* miSistemaDeFicheros);
void reservaBloquesNodosI(MiSistemaDeFicheros* miSistemaDeFicheros, DISK_LBA idxBloques[], int numBloques);
int buscaPosDirectorio(MiSistemaDeFicheros* miSistemaDeFicheros, char* nombre);
int myQuota(MiSistemaDeFicheros* miSistemaDeFicheros);

#endif
