#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "mitar.h"

extern char *uso;

int copynFile(FILE *origen, FILE *destino, int nBytes);
int readHeader(FILE *tarFile, stHeaderEntry **header, int *nFiles);
int loadstr(FILE *file, char** buf);


/** Copia el nBytes del fichero origen al fichero destino.
 *
 * origen: puntero al descriptor FILE del fichero origen
 * destino: puntero al descriptor FILE del fichero destino
 * nBytes: numero de bytes a copiar
 *
 * Devuelve el numero de bytes copiados o -1 en caso de error. 
 */
int copynFile(FILE *origen, FILE *destino, int nBytes) {
  // Completar la función
   int numBytes = 0;
   int c; 

   /* Si el fichero origen y destin no se abren correctamente, error. En contrario, se copia el fichero origen en el fichero destino. */
   if(origen == NULL || destino == NULL) {
      return -1;
   }

   /* Mientras no se llegue al final de fichero y el número de bytes leídos sean distinto de el nBytes máximos a copiar, se lee caracter 
   a caracter el fichero origen y se copia en el de destino. */
   do {
         c = getc(origen); 
         if(c != EOF) { 
            putc(c, destino);
            numBytes++;
         }
   }while((c != EOF) && (numBytes != nBytes));

   return numBytes;
   
}

/** Carga en memoria la cabecera del tarball.
 *
 * tarFile: puntero al descriptor FILE del tarball
 * header: dirección de un puntero que se inicializará con la dirección de un
 * buffer que contenga los descriptores de los ficheros cargados.
 * nFiles: dirección de un entero (puntero a entero) que se inicializará con el
 * número de ficheros contenido en el tarball (primeros bytes de la cabecera).
 *
 * Devuelve EXIT_SUCCESS en caso de exito o EXIT_FAILURE en caso de error
 * (macros definidas en stdlib.h).
 */
int readHeader(FILE *tarFile, stHeaderEntry **header, int *nFiles) {
   // Completar la función 
   stHeaderEntry *array = NULL;
   int nr_files = 0;
   int i;

   /* Si el fichero tarFile no se abre correctamente, error. En contrario, se continúa creando el fichero comprimido. */
   if(tarFile == NULL)
      return (EXIT_FAILURE);
  
   /* Leemos el número de ficheros (N) del tarFile y lo volcamos en nr_files */
   fread(&nr_files, sizeof(int), 1, tarFile);
   fscanf(tarFile, "\n");

   /* Reservamos memoria para el array */
   array = (stHeaderEntry*) malloc(sizeof(stHeaderEntry) * nr_files);
   
   /* Leemos la metainformación del tarFile y lo volcamos en el array */
   for(i= 0; i < nr_files; i++) {
      array[i].name = malloc(PATH_MAX);
      loadstr(tarFile, &array[i].name);
      fread(&array[i].size, sizeof(int), 1, tarFile);
      fscanf(tarFile, "\n");
   }

   /* Devolvemos lo valores leídos a la función invocadora */
   (*nFiles) = nr_files;
   (*header) = array;

   return (EXIT_SUCCESS);
 
}

/** Carga una cadena de caracteres de un fichero.
 *
 * file: dirección de la estructura FILE del fichero
 * buf: dirección del puntero en que se inicializará con la dirección donde se
 * copiará la cadena. Será una dirección del heap obtenida con malloc.
 *
 * Devuelve: 0 si tuvo exito, -1 en caso de error.
 */

int loadstr(FILE *file, char** buf) {
   // Completar la función
   char *buffer = NULL;
   int len;
   int ch;
   
   /* Si el fichero file no se abre correctamente, error. En contrario, se carga una cadena de caracteres del fichero file en el buffer. */
   if(file == NULL)
      return -1;

   /* Se reserva memoria dinámica para el puntero buffer y se inicializa la variable len que contiene la longitud del buffer */
   buffer = malloc(PATH_MAX);
   len = 0;

  /* Se lee caracter a caracter del fichero file y se añade el caracter al buffer */
   do {
      ch = getc(file);
      if(ch != '\n')  {
         buffer[len++] = ch;
      }
   } while(ch != '\n');

   /*Finalmente, se añade el caracter de terminación de la cadena*/
   buffer[len] = '\0';

   /* Devolvemos lo valores leídos a la función invocadora */
   (*buf) = buffer;

   return 0;
}

/** crea un tarball a partir de unos ficheros de entrada.
 *
 * nfiles:    numero de ficheros de entrada
 * filenames: array con los nombres de los ficheros de entrada (rutas)
 * tarname:   nombre del tarball
 * 
 * devuelve exit_success en caso de exito y exit_failure en caso de error
 * (macros definidas en stdlib.h).
 *
 * ayuda: calcular primero el espacio necesario para la cabecera del tarball,
 * saltar dicho espacio desde el comienzo del archivo e ir copiando uno a uno
 * los ficheros, rellenando a la vez una representación de la cabecera del
 * tarball en memoria. al final copiar la cabecera al comienzo del tarball.
 * recordar que  que no vale un simple sizeof de stheaderentry para saber el
 * tamaño necesario en disco para el descriptor de un fichero puesto que sizeof
 * no calcula el tamaño de la cadena apuntada por name, sino el tamaño del
 * propio puntero. para calcular el tamaño de una cadena c puede usarse la
 * función strlen.
 */
int createTar(int nFiles, char *fileNames[], char tarName[]) {
   // Completar la función 
   FILE *inputFile = NULL;
   FILE *tarFile = NULL;
   stHeaderEntry *array= NULL;

   int nBytes = 0;
   int i;
   
   /* Si el fichero tarFile no se abre correctamente, error. En contrario, se continúa creando el fichero comprimido. */
   if((tarFile = fopen(tarName, "w")) == NULL)
      return (EXIT_FAILURE);
   
   /* Reservamos memoría dinamica para el puntero array */ 
   array = malloc(sizeof(stHeaderEntry) * nFiles);
   
   /* El número de bytes a desplazar son los de la cabecera del fichero comprimido más los bytes de salto de linea (expresados como (nFiles + 1)) */
   nBytes = (nFiles + 1) + sizeof(int) + nFiles * sizeof(unsigned int); 
   
   /* Se copia la cabecera del fichero en array*/
   for(i = 0; i < nFiles; i++) {
      array[i].name = malloc(strlen(fileNames[i]) + 1);
      strcpy(array[i].name,fileNames[i]);
      nBytes += strlen(fileNames[i]) + 1;
   }
   
   /* Desplazamos el indicador de fichero tantos bytes como los que tenga la cabecera*/
   fseek(tarFile, nBytes, SEEK_SET);

   /* Copiamos el contenido de los ficheros en el fichero comprimido */
   for(i = 0; i < nFiles; i++) {
      inputFile = fopen(fileNames[i], "r");
      if(inputFile == NULL)
         return (EXIT_FAILURE);
      nBytes = copynFile(inputFile, tarFile, INT_MAX);
      fclose(inputFile);
      array[i].size = nBytes;
   }
   /* Nos posicionamos en el byte 0 en el fichero tar*/   
   rewind(tarFile);

   /* Escribir el número de ficheros en fichero comprimido*/  
   fwrite(&nFiles, sizeof(int), 1, tarFile);
   fwrite("\n", sizeof(char), 1, tarFile);

   /* Para cada estructura stdHeaderEntry, escribir la ruta del fichero y el número de bytes que ocupa el fichero 
      Posteriormente, liberamos la memoría dinamica que ocupa cada ruta del archivo i correspondiente a la estructura stHeaderEntry */
   for(i = 0; i < nFiles;i++) {
      fwrite(array[i].name,strlen(fileNames[i]), 1, tarFile);
      fwrite("\n", sizeof(char), 1, tarFile);
      fwrite(&array[i].size, sizeof(unsigned int), 1, tarFile);
      fwrite("\n", sizeof(char), 1, tarFile);
      free(array[i].name);
   }

   printf("Fichero mitar creado con exito\n");
   

   free(array);
   fclose(tarFile);

   return (EXIT_SUCCESS);
  
}

/** Extrae todos los ficheros de un tarball.
 *
 * tarName: cadena C con el nombre del tarball
 *
 * Devuelve EXIT_SUCCESS en caso de exito y EXIT_FAILURE en caso de error (macros
 * definidas en stdlib.h).
 *
 * AYUDA: debemos cargar primero la cabecera, con lo que nos situaremos al
 * comienzo del primer fichero almacenado. Entonces recorriendo la cabecera
 * sabremos el nombre que debemos ir dando a los ficheros y la cantidad de bytes
 * que debemos leer del fichero para cargar su contenido.
 */
int extractTar(char tarName[]) {
   // Completar la función
   FILE *outputFile = NULL;
   FILE *tarFile = NULL;
   stHeaderEntry *array= NULL;  

   int nr_files = 0;
   int i;

   /* Si el fichero tarFile no se abre correctamente, error. En contrario, se continúa creando el fichero comprimido. */
   if((tarFile = fopen(tarName, "r")) == NULL)
      return (EXIT_FAILURE);

   /* Se lee la cabecera del fichero comprimido */
   readHeader(tarFile, &array, &nr_files);
   
   /* Se descomprime el fichero comprimido, es decir, se copía tanto bytes como tengan los ficheros a comprimir con el nombre indicado. 
   Toda está información viene en la cabecera del fichero. */
   for(i = 0; i < nr_files; i++) {
      outputFile = fopen(array[i].name, "w");
      copynFile(tarFile, outputFile, array[i].size);
      printf("[%d]: Creando fichero %s, tamano %d Bytes...Ok\n",i,array[i].name,array[i].size); 
      fclose(outputFile);
   }

   free(array);
   fclose(tarFile);

   return (EXIT_SUCCESS);
}
