#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>       
#include "mitar.h"
       
const char uso[]="Uso: tar -c|x|t|a -f archivo_mtar [fich1 fich2 ...]\n";

int main(int argc, char *argv[]) {

  int opt, nExtra, retCode=EXIT_SUCCESS;
  flags flag=NONE;
  char *tarName=NULL;
  char *outTarName=NULL;
  //FILE *tarFile, **inputFiles;
  //headerEntry *header;
  
  //Menor número de argumentos mÌnimo válido=3: mitar -tfFile.tar
  if(argc < 2){
    fprintf(stderr, uso);
    exit(EXIT_FAILURE);
  }
  //Parseamos los flags
  while((opt = getopt(argc, argv, "cxtaf:")) != -1) {
    switch(opt) {
      case 'c':
        flag=(flag==NONE)?CREATE:ERROR;
        break;
      case 't':
        flag=(flag==NONE)?LIST:ERROR;
        break;
      case 'x':
        flag=(flag==NONE)?EXTRACT:ERROR;
        break;
      case 'a':
        flag=(flag==NONE)?APPEND:ERROR;
        break;
      case 'f':
        tarName = optarg;
        break;
      default:
        flag=ERROR;
    }
    //Hemos detectado un flag válido?
    if(flag==ERROR){
      fprintf(stderr, uso);
      exit(EXIT_FAILURE);
    }
  }
  
  //Flag valido + argumento + fichero[s]
  if(flag==NONE || tarName==NULL) {
    fprintf(stderr, uso);
    exit(EXIT_FAILURE);
  }
  
  //#argumentos extra
  nExtra=argc-optind;
  
  //Ejecutamos la accion requerida
  switch(flag) {
    case CREATE:
      retCode=createTar(nExtra, &argv[optind], tarName);
      break;
    case LIST:
      if(nExtra!=0){
        fprintf(stderr, uso);
        exit(EXIT_FAILURE);
      }
      retCode=listTar(tarName);
      break;
    case EXTRACT:
      if(nExtra!=0){
        fprintf(stderr, uso);
        exit(EXIT_FAILURE);
      }
      retCode=extractTar(tarName);
      break;
    case APPEND:
        outTarName=malloc(strlen(tarName)+6);
        strncpy(outTarName,tarName,strlen(tarName)-5);
        strcat(outTarName,".new.mtar");
        retCode=appendTar(nExtra, &argv[optind], tarName,outTarName);
      break;   
    default:
      retCode=EXIT_FAILURE;
  }
  exit(retCode);

}
