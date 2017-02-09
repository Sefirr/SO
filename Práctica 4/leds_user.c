#include <stdio.h>
#include <string.h>
#include <fcntl.h>
//#include "chardev_leds.c"


int main(){
	int tamano =100; 
	int tamCadena = 0;
	int i = 0;
	int descr;
	//FILE *stdin;
	char cadena[tamano];
	char *enter;
	descr = open("/dev/leds", O_WRONLY);
	write(descr, "123", 4);
	printf("Introduce el texto: ");
	fgets(cadena, tamano, stdin);
	enter = strchr(cadena, '\n');
	if(enter)
	  *enter= '\0';
	tamCadena = strlen(cadena);
	
	printf("La cadena introducida es `""%s""´, se procedera a\nconvertirla a código morse por los leds del teclado\n", cadena);
	printf("\n");
	write(descr, "0", 2);
	usleep(500000);
/*
Senal corta: 1
Senal larga: 12
Senal de inicio/fin: 123
Senal espacio: 23
Senal fin letras: 3
Senal error (no existe el parametro): 13
*/
	for(i = 0; i < tamCadena; i++){

		if(cadena[i] == 'a' || cadena[i] == 'A'){
			usleep(500000);
			write(descr, "1", 2);
			usleep(500000);
			write(descr, "0", 2);
			usleep(500000);
			write(descr, "12", 3);
			usleep(500000);
			write(descr, "0", 2);
			usleep(500000);
			
		}
		if(cadena[i] == 'b' || cadena[i] == 'B'){
			usleep(500000);
			write(descr, "12", 3);
			usleep(500000);
			write(descr, "0", 2);
			usleep(500000);
			write(descr, "1", 2);
			usleep(500000);
			write(descr, "0", 2);
			usleep(500000);
			write(descr, "1", 2);
			usleep(500000);
			write(descr, "0", 2);
			usleep(500000);
			write(descr, "1", 2);
			usleep(500000);
			write(descr, "0", 2);
			usleep(500000);
			
		}
		if(cadena[i] == 'c' || cadena[i] == 'C'){
			usleep(500000);
			write(descr, "12", 3);
			usleep(500000);
			write(descr, "0", 2);
			usleep(500000);
			write(descr, "1", 2);
			usleep(500000);
			write(descr, "0", 2);
			usleep(500000);
			write(descr, "12", 3);
			usleep(500000);
			write(descr, "0", 2);
			usleep(500000);
			write(descr, "1", 2);
			usleep(500000);
			write(descr, "0", 2);
			usleep(500000);
			
		}
		if(cadena[i] == 'd' || cadena[i] == 'D'){
			usleep(500000);
			write(descr, "12", 3);
			usleep(500000);
			write(descr, "0", 2);
			usleep(500000);
			write(descr, "1", 2);
			usleep(500000);
			write(descr, "0", 2);
			usleep(500000);
			write(descr, "1", 2);
			usleep(500000);
			write(descr, "0", 2);
			usleep(500000);
		}
		if(cadena[i] == 'e' || cadena[i] == 'E'){
			usleep(500000);
			write(descr, "1", 2);
			usleep(500000);
			write(descr, "0", 2);
			usleep(500000);
			
		}
		if(cadena[i] == 'f' || cadena[i] == 'F'){
			usleep(500000);
			write(descr, "1", 2);
			usleep(500000);
			write(descr, "0", 2);
			usleep(500000);
			write(descr, "1", 2);
			usleep(500000);
			write(descr, "0", 2);
			usleep(500000);
			write(descr, "12", 3);
			usleep(500000);
			write(descr, "0", 2);
			usleep(500000);
			write(descr, "1", 2);
			usleep(500000);
			write(descr, "0", 2);
			usleep(500000);
			
		}
		if(cadena[i] == 'g' || cadena[i] == 'G'){
			usleep(500000);
			write(descr, "12", 3);
			usleep(500000);
			write(descr, "0", 2);
			usleep(500000);
			write(descr, "12", 3);
			usleep(500000);
			write(descr, "0", 2);
			usleep(500000);
			write(descr, "1", 2);
			usleep(500000);
			write(descr, "0", 2);
			usleep(500000);
			
		}
		if(cadena[i] == 'h' || cadena[i] == 'H'){
			usleep(500000);
			write(descr, "1", 2);
			usleep(500000);
			write(descr, "0", 2);
			usleep(500000);
			write(descr, "1", 2);
			usleep(500000);
			write(descr, "0", 2);
			usleep(500000);
			write(descr, "1", 2);
			usleep(500000);
			write(descr, "0", 2);
			usleep(500000);
			write(descr, "1", 2);
			usleep(500000);
			write(descr, "0", 2);
			usleep(500000);
		}
		if(cadena[i] == 'i' || cadena[i] == 'I'){
			usleep(500000);			
			write(descr, "1", 2);
			usleep(500000);
			write(descr, "0", 2);
			usleep(500000);
			write(descr, "1", 2);
			usleep(500000);
			write(descr, "0", 2);
			usleep(500000);
			
		}
		if(cadena[i] == 'j' || cadena[i] == 'J'){
			usleep(500000);			
			write(descr, "1", 2);
			usleep(500000);
			write(descr, "0", 2);
			usleep(500000);
			write(descr, "12", 3);
			usleep(500000);
			write(descr, "0", 2);
			usleep(500000);
			write(descr, "12", 3);
			usleep(500000);
			write(descr, "0", 2);
			usleep(500000);
			write(descr, "12", 3);
			usleep(500000);
			write(descr, "0", 2);
			usleep(500000);
			
		}
		if(cadena[i] == 'k' || cadena[i] == 'K'){
			usleep(500000);
			write(descr, "12", 3);
			usleep(500000);
			write(descr, "0", 2);
			usleep(500000);
			write(descr, "1", 2);
			usleep(500000);
			write(descr, "0", 2);
			usleep(500000);
			write(descr, "12", 3);
			usleep(500000);
			write(descr, "0", 2);
			usleep(500000);
			
		}
		if(cadena[i] == 'l' || cadena[i] == 'L'){
			usleep(500000);
			write(descr, "1", 2);
			usleep(500000);
			write(descr, "0", 2);
			usleep(500000);
			write(descr, "12", 3);
			usleep(500000);
			write(descr, "0", 2);
			usleep(500000);
			write(descr, "1", 2);
			usleep(500000);
			write(descr, "0", 2);
			usleep(500000);
			write(descr, "1", 2);
			usleep(500000);
			write(descr, "0", 2);
			usleep(500000);
			
		}
		if(cadena[i] == 'm' || cadena[i] == 'M'){
			usleep(500000);
			write(descr, "12", 3);
			usleep(500000);
			write(descr, "0", 2);
			usleep(500000);
			write(descr, "12", 3);
			usleep(500000);
			write(descr, "0", 2);
			usleep(500000);
			
		}
		if(cadena[i] == 'n' || cadena[i] == 'N'){
			usleep(500000);
			write(descr, "12", 3);
			usleep(500000);
			write(descr, "0", 2);
			usleep(500000);
			write(descr, "1", 2);
			usleep(500000);
			write(descr, "0", 2);
			usleep(500000);
			
		}
		if(cadena[i] == 'o' || cadena[i] == 'O'){
			usleep(500000);
			write(descr, "12", 3);
			usleep(500000);
			write(descr, "0", 2);
			usleep(500000);
			write(descr, "12", 3);
			usleep(500000);
			write(descr, "0", 2);
			usleep(500000);
			write(descr, "12", 3);
			usleep(500000);
			write(descr, "0", 2);
			usleep(500000);
			
		}
		if(cadena[i] == 'p' || cadena[i] == 'P'){
			usleep(500000);
			write(descr, "1", 2);
			usleep(500000);
			write(descr, "0", 2);
			usleep(500000);
			write(descr, "12", 3);
			usleep(500000);
			write(descr, "0", 2);
			usleep(500000);
			write(descr, "12", 3);
			usleep(500000);
			write(descr, "0", 2);
			usleep(500000);
			write(descr, "1", 2);
			usleep(500000);
			write(descr, "0", 2);
			usleep(500000);
		}
		if(cadena[i] == 'q' || cadena[i] == 'Q'){
			usleep(500000);
			write(descr, "12", 3);
			usleep(500000);
			write(descr, "0", 2);
			usleep(500000);
			write(descr, "12", 3);
			usleep(500000);
			write(descr, "0", 2);
			usleep(500000);
			write(descr, "1", 2);
			usleep(500000);
			write(descr, "0", 2);
			usleep(500000);
			write(descr, "12", 3);
			usleep(500000);
			write(descr, "0", 2);
			usleep(500000);
			
		}
		if(cadena[i] == 'r' || cadena[i] == 'R'){
			usleep(500000);
			write(descr, "1", 2);
			usleep(500000);
			write(descr, "0", 2);
			usleep(500000);
			write(descr, "12", 3);
			usleep(500000);
			write(descr, "0", 2);
			usleep(500000);
			write(descr, "1", 2);
			usleep(500000);
			write(descr, "0", 2);
			usleep(500000);
			
		}
		if(cadena[i] == 's' || cadena[i] == 'S'){
			usleep(500000);
			write(descr, "1", 2);
			usleep(500000);
			write(descr, "0", 2);
			usleep(500000);
			write(descr, "1", 2);
			usleep(500000);
			write(descr, "0", 2);
			usleep(500000);
			write(descr, "1", 2);
			usleep(500000);
			write(descr, "0", 2);
			usleep(500000);
		}
		if(cadena[i] == 't' || cadena[i] == 'T'){
			usleep(500000);
			write(descr, "12", 3);
			usleep(500000);
			write(descr, "0", 2);
			usleep(500000);
		
		}
		if(cadena[i] == 'u' || cadena[i] == 'U'){
			usleep(500000);
			write(descr, "1", 2);
			usleep(500000);
			write(descr, "0", 2);
			usleep(500000);
			write(descr, "1", 2);
			usleep(500000);
			write(descr, "0", 2);
			usleep(500000);
			write(descr, "12", 3);
			usleep(500000);
			write(descr, "0", 2);
			usleep(500000);
		}
		if(cadena[i] == 'v' || cadena[i] == 'V'){
			usleep(500000);
			write(descr, "1", 2);
			usleep(500000);
			write(descr, "0", 2);
			usleep(500000);
			write(descr, "1", 2);
			usleep(500000);
			write(descr, "0", 2);
			usleep(500000);
			write(descr, "1", 2);
			usleep(500000);
			write(descr, "0", 2);
			usleep(500000);
			write(descr, "12", 3);
			usleep(500000);
			write(descr, "0", 2);
			usleep(500000);
		}
		if(cadena[i] == 'w' || cadena[i] == 'W'){
			usleep(500000);
			write(descr, "1", 2);
			usleep(500000);
			write(descr, "0", 2);
			usleep(500000);
			write(descr, "12", 3);
			usleep(500000);
			write(descr, "0", 2);
			usleep(500000);
			write(descr, "12", 3);
			usleep(500000);
			write(descr, "0", 2);
			usleep(500000);
		}
		if(cadena[i] == 'x' || cadena[i] == 'X'){
			usleep(500000);
			write(descr, "12", 3);
			usleep(500000);
			write(descr, "0", 2);
			usleep(500000);
			write(descr, "1", 2);
			usleep(500000);
			write(descr, "0", 2);
			usleep(500000);
			write(descr, "1", 2);
			usleep(500000);
			write(descr, "0", 2);
			usleep(500000);
			write(descr, "12", 3);
			usleep(500000);
			write(descr, "0", 2);
			usleep(500000);
		}
		if(cadena[i] == 'y' || cadena[i] == 'Y'){
			usleep(500000);
			write(descr, "12", 3);
			usleep(500000);
			write(descr, "0", 2);
			usleep(500000);
			write(descr, "1", 2);
			usleep(500000);
			write(descr, "0", 2);
			usleep(500000);
			write(descr, "12", 3);
			usleep(500000);
			write(descr, "0", 2);
			usleep(500000);
			write(descr, "12", 3);
			usleep(500000);
			write(descr, "0", 2);
			usleep(500000);
		}
		if(cadena[i] == 'z' || cadena[i] == 'Z'){
			usleep(500000);
			write(descr, "12", 3);
			usleep(500000);
			write(descr, "0", 2);
			usleep(500000);
			write(descr, "12", 3);
			usleep(500000);
			write(descr, "0", 2);
			usleep(500000);
			write(descr, "1", 2);
			usleep(500000);
			write(descr, "0", 2);
			usleep(500000);
			write(descr, "1", 2);
			usleep(500000);
			write(descr, "0", 2);
			usleep(500000);
		}
		if(cadena[i] == '0'){
			usleep(500000);
			write(descr, "12", 3);
			usleep(500000);
			write(descr, "0", 2);
			usleep(500000);
			write(descr, "12", 3);
			usleep(500000);
			write(descr, "0", 2);
			usleep(500000);
			write(descr, "12", 3);
			usleep(500000);
			write(descr, "0", 2);
			usleep(500000);
			write(descr, "12", 3);
			usleep(500000);
			write(descr, "0", 2);
			usleep(500000);
			write(descr, "12", 3);
			usleep(500000);
			write(descr, "0", 2);
			usleep(500000);
		}
		if(cadena[i] == '1'){
			usleep(500000);
			write(descr, "1", 2);
			usleep(500000);
			write(descr, "0", 2);
			usleep(500000);
			write(descr, "12", 3);
			usleep(500000);
			write(descr, "0", 2);
			usleep(500000);
			write(descr, "12", 3);
			usleep(500000);
			write(descr, "0", 2);
			usleep(500000);
			write(descr, "12", 3);
			usleep(500000);
			write(descr, "0", 2);
			usleep(500000);
			write(descr, "12", 3);
			usleep(500000);
			write(descr, "0", 2);
			usleep(500000);
		}
		if(cadena[i] == '2'){
			usleep(500000);
			write(descr, "1", 2);
			usleep(500000);
			write(descr, "0", 2);
			usleep(500000);
			write(descr, "1", 2);
			usleep(500000);
			write(descr, "0", 2);
			usleep(500000);
			write(descr, "12", 3);
			usleep(500000);
			write(descr, "0", 2);
			usleep(500000);
			write(descr, "12", 3);
			usleep(500000);
			write(descr, "0", 2);
			usleep(500000);
			write(descr, "12", 3);
			usleep(500000);
			write(descr, "0", 2);
			usleep(500000);
		}
		if(cadena[i] == '3'){
			usleep(500000);
			write(descr, "1", 2);
			usleep(500000);
			write(descr, "0", 2);
			usleep(500000);
			write(descr, "1", 2);
			usleep(500000);
			write(descr, "0", 2);
			usleep(500000);
			write(descr, "1", 2);
			usleep(500000);
			write(descr, "0", 2);
			usleep(500000);
			write(descr, "12", 3);
			usleep(500000);
			write(descr, "0", 2);
			usleep(500000);
			write(descr, "12", 3);
			usleep(500000);
			write(descr, "0", 2);
			usleep(500000);

		}
		if(cadena[i] == '4'){
			usleep(500000);
			write(descr, "1", 2);
			usleep(500000);
			write(descr, "0", 2);
			usleep(500000);
			write(descr, "1", 2);
			usleep(500000);
			write(descr, "0", 2);
			usleep(500000);
			write(descr, "1", 2);
			usleep(500000);
			write(descr, "0", 2);
			usleep(500000);
			write(descr, "1", 2);
			usleep(500000);
			write(descr, "0", 2);
			usleep(500000);
			write(descr, "12", 3);
			usleep(500000);
			write(descr, "0", 2);
			usleep(500000);
		}
		if(cadena[i] == '5'){
			usleep(500000);
			write(descr, "1", 2);
			usleep(500000);
			write(descr, "0", 2);
			usleep(500000);
			write(descr, "1", 2);
			usleep(500000);
			write(descr, "0", 2);
			usleep(500000);
			write(descr, "1", 2);
			usleep(500000);
			write(descr, "0", 2);
			usleep(500000);
			write(descr, "1", 2);
			usleep(500000);
			write(descr, "0", 2);
			usleep(500000);
			write(descr, "1", 2);
			usleep(500000);
			write(descr, "0", 2);
			usleep(500000);
		}
		if(cadena[i] == '6'){
			usleep(500000);
			write(descr, "12", 3);
			usleep(500000);
			write(descr, "0", 2);
			usleep(500000);
			write(descr, "1", 2);
			usleep(500000);
			write(descr, "0", 2);
			usleep(500000);
			write(descr, "1", 2);
			usleep(500000);
			write(descr, "0", 2);
			usleep(500000);
			write(descr, "1", 2);
			usleep(500000);
			write(descr, "0", 2);
			usleep(500000);
			write(descr, "1", 2);
			usleep(500000);
			write(descr, "0", 2);
			usleep(500000);
		}
		if(cadena[i] == '7'){
			usleep(500000);
			write(descr, "12", 3);
			usleep(500000);
			write(descr, "0", 2);
			usleep(500000);
			write(descr, "12", 3);
			usleep(500000);
			write(descr, "0", 2);
			usleep(500000);
			write(descr, "1", 2);
			usleep(500000);
			write(descr, "0", 2);
			usleep(500000);
			write(descr, "1", 2);
			usleep(500000);
			write(descr, "0", 2);
			usleep(500000);
			write(descr, "1", 2);
			usleep(500000);
			write(descr, "0", 2);
			usleep(500000);
		}
		if(cadena[i] == '8'){
			usleep(500000);
			write(descr, "12", 3);
			usleep(500000);
			write(descr, "0", 2);
			usleep(500000);
			write(descr, "12", 3);
			usleep(500000);
			write(descr, "0", 2);
			usleep(500000);
			write(descr, "12", 3);
			usleep(500000);
			write(descr, "0", 2);
			usleep(500000);
			write(descr, "1", 2);
			usleep(500000);
			write(descr, "0", 2);
			usleep(500000);
			write(descr, "1", 2);
			usleep(500000);
			write(descr, "0", 2);
			usleep(500000);
		}
		if(cadena[i] == '9'){
			usleep(500000);
			write(descr, "12", 3);
			usleep(500000);
			write(descr, "0", 2);
			usleep(500000);
			write(descr, "12", 3);
			usleep(500000);
			write(descr, "0", 2);
			usleep(500000);
			write(descr, "12", 3);
			usleep(500000);
			write(descr, "0", 2);
			usleep(500000);
			write(descr, "12", 3);
			usleep(500000);
			write(descr, "0", 2);
			usleep(500000);
			write(descr, "1", 2);
			usleep(500000);
			write(descr, "0", 2);
			usleep(500000);
		}
		if(cadena[i] == ' '){
			usleep(500000);
			write(descr, "23", 3);
			usleep(500000);
			write(descr, "0", 2);
			usleep(500000);
		}
		if(cadena[i] <= 'a' || cadena[i] >= 'z' || cadena[i] <= 'A' || cadena[i] >= 'A' || cadena[i] <= '0' || cadena[i] >= '9' ){
			usleep(500000);
			write(descr, "13", 3);
			usleep(500000);
			write(descr, "0", 2);
			usleep(500000);
			write(descr, "13", 3);
			usleep(500000);
			write(descr, "0", 2);
			usleep(500000);
			write(descr, "13", 3);
			usleep(500000);
			write(descr, "0", 2);
			usleep(500000);
			write(descr, "13", 3);
			usleep(500000);
			write(descr, "0", 2);
			usleep(500000);
		}
	write(descr, "3", 2);
	usleep(500000);
	write(descr, "0", 2);
	}
	usleep(500000);
	write(descr, "123", 4);
	usleep(1000000);
	write(descr, "0", 2);
	close(descr);
 return 0;
}
