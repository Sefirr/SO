#ifndef _MITAR_H
#define _MITAR_H

#include <limits.h>

typedef enum{
  NONE,
  ERROR,
  CREATE,
  EXTRACT,
  APPEND,
  LIST
} flags;

typedef struct {
  char *name;
  unsigned int size;
} stHeaderEntry;

int createTar(int nFiles, char *fileNames[], char tarName[]);
int extractTar(char tarName[]);
int listTar(char tarName[]);
int appendTar(int nFiles, char *fileNames[], char tarName[],char newTarName[]);


#endif /* _MITAR_H */
