#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

char buffer[BUFSIZ];	// Buffer de transferencia
int tam;		// N�mero de bytes realmente usados en el buffer

int main(int argc, char *argv[]) {
	int fd_origen, fd_destino;	// Descriptores de fichero
	int nbytes;

	// Comprobaci�n de que el n�mero de argumentos de la l�nea de orden es correcto
	if ((argc < 3) || (argc > 4))  {
		fprintf(stderr, "Forma de uso: %s origen destino [TBLOQUE]\n", argv[0]);
		exit(-1);
	}

	// Seleccion del tama�o de bloque unidad de transferencia
	tam = (argc == 4) ? atoi(argv[3]) : sizeof(buffer);
	// Comprobacion: el tama�o del bloque no debe ser negativo
	//		ni superar el valor maximo reservado para 'buffer'
	if ((tam <= 0) || (tam > sizeof(buffer)))
		fprintf(stderr, "%s debe estar entre 1 y %ld\n", argv[3], (long int)sizeof(buffer)), exit(-1);

	// Apertura del fichero_fuente, solo para lectura
	if (!strcmp(argv[1], "-"))	// Si el primer argumento es "-", usar STDIN
		fd_origen = STDIN_FILENO;
	else if ((fd_origen = open(argv[1], O_RDONLY)) == -1)
		perror("open origen"), exit(-1);

	// Apertura del fichero_destino, solo para escritura.
	//		Si ya existe, lo trunca; si no, lo crea con permisos rw-rw-rw-
	if (!strcmp(argv[2], "-"))	// Si el segundo argumento es "-", usar STDOUT
		fd_destino = STDOUT_FILENO;
	else if ((fd_destino = open(argv[2], O_WRONLY|O_TRUNC|O_CREAT, 0666)) == -1)
		perror("open destino"), exit(-1);

	printf("Comienzo de COPIA\n");
	// ordena leer 'tam' bytes; lee efectivamente 'nbytes'
	while ((nbytes = read(fd_origen, buffer, tam)) > 0)
		// ordena escribir 'nbytes'; deber�a escribirlos todos
		if (write(fd_destino, buffer, nbytes) < 0)
			perror("write destino"), exit(-1);
	printf("Fin de COPIA\n");

	// si el n�mero de bytes leidos es 0, esto indica FIN DE FICHERO
	if (nbytes != 0) perror("read origen"), exit(-1);

	// Cierra los ficheros abiertos.
	//	Como est� al final del programa no es estrictamente necesario
	//		la llamada exit() los cierra de cualquier modo
	close(fd_origen);
	close(fd_destino);

	exit(0);
}
