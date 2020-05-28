#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

int ejecutarConway(int argc, char** argv);
int llenarMatriz(unsigned char* matriz, FILE* archivo, unsigned int alto, unsigned int ancho);
int avanzarTiempo(unsigned char* matriz, unsigned int alto, unsigned int ancho, char* nombre);
unsigned int vecinos(unsigned char* matriz, unsigned int i, unsigned int j,
						unsigned int alto, unsigned int ancho);
unsigned char realizarCambio(unsigned char estado, unsigned char vecinos);

const size_t BUFLEN = 100;

int main(int argc, char** argv) {
	bool argsOk = false;
	int retorno = 0;
    if((argc == 2) && ((strcmp(argv[1], "-h") == 0) ||
    					(strcmp(argv[1], "--help") == 0))) {
    	printf("Uso:\n"
    		"\tconway -h\n"
    		"\tconway -V\n"
    		"\tconway i M N inputfile [-o outputprefix]\n"
    		"Opciones:\n"
    		"\t-h, --help \tImprime este mensaje.\n"
    		"\t-V, --version \tDa la versión del programa.\n"
    		"\t-o \tPrefijo de los archivos de salida.\n"
    		"Ejemplos:\n"
    		"\tconway 10 20 20 glider -o estado\n"
    		"\tRepresenta 10 iteraciones del Juego de la Vida en una matriz de "
    		"20x20,\n\tcon un estado inicial tomado del archivo ‘‘glider’’.\n\t"
    		"Los archivos de salida se llamarán estado_n.pbm.\n"
    		"\tSi no se da un prefijo para los archivos de salida,\n\tel prefijo"
    		" será el nombre del archivo de entrada.\n");
    	argsOk = true;
    }

    if((argc == 2) && ((strcmp(argv[1], "-V") == 0) ||
    					(strcmp(argv[1], "-v") == 0) ||
        				(strcmp(argv[1], "--version") == 0))) {
    	printf("Conway-1.0.1-C\n");
    	argsOk = true;
    }

    if((argc == 5) || ((argc == 7) && (strcmp(argv[5], "-o") == 0))) {
    	argsOk = true;
    	retorno = ejecutarConway(argc, argv);
    }

    if(!argsOk) {
    	fprintf(stderr, "Error en entrada de parametros, ejecutar ./conway -h "
    			"para recibir ayuda\n");
    	retorno = -1;
    }
    printf("\n");
    return retorno;
}


int ejecutarConway(int argc, char** argv) {
	printf("Generando matriz...\n");
	unsigned int alto = atoi(argv[2]);
	unsigned int ancho = atoi(argv[3]);

	if ((alto == 0) || (ancho == 0)) {
		fprintf(stderr, "Error al generar matriz, checkear dimensiones\n");
		return -2;
	}
	unsigned char* matriz = malloc(alto * ancho);
	if (matriz == NULL) {
		fprintf(stderr, "Error al generar matriz, no hay memoria\n");
		return -3;
	}
	printf("Matriz de %ux%u generada\n", alto, ancho);

	printf("Leyendo archivo de entrada...\n");
	FILE* archivo;

	archivo = fopen(argv[4], "r");
	if (archivo == NULL) {
		fprintf(stderr, "Error al arbir archivo, checkear nombre de archivo\n");
		free(matriz);
		return -4;
	}
	printf("Archivo abierto\n");

	int retorno = llenarMatriz(matriz, archivo, alto, ancho);

	fclose(archivo);

	char prefijo[BUFLEN];
	if (argc == 7) {
		snprintf(prefijo, BUFLEN, "%s", argv[6]);
	}

	unsigned int iteraciones = atoi(argv[1]);
	if (retorno == 0) {
		for (int i = 0; i < iteraciones; i++) {
			char terminacion[50];
			char nombreArchivo[BUFLEN];
			snprintf(terminacion, 50, "%i.pbm", i);
			snprintf(nombreArchivo, BUFLEN, "%s", prefijo);
			strcat(nombreArchivo, terminacion);
			retorno = avanzarTiempo(matriz, alto, ancho, nombreArchivo);
		}
	}
	free(matriz);
	return retorno;
}



int llenarMatriz(unsigned char* matriz, FILE* archivo, unsigned int alto, unsigned int ancho) {
	for (int i = 0; i < (alto * ancho); i++) {
		matriz[i] = 0;
	}
	unsigned int fila, columna;
	int resultadoLectura;
	resultadoLectura = fscanf(archivo, "%u %u", &fila, &columna);
	while (resultadoLectura == 2) {
		printf("Punto encontrado en %u, %u\n", fila, columna);
		if ((fila > alto) || (columna > ancho)) {
			fprintf(stderr, "Error al llenar matriz, se excedio el rango de la matriz\n");
			return -5;
		} else {
			matriz[((fila - 1) * ancho) + columna - 1] = 1;
		}
		resultadoLectura = fscanf(archivo, "%u %u", &fila, &columna);
	}
	if (resultadoLectura != EOF) {
		fprintf(stderr, "Error al llenar matriz, formato de archivo incorrecto\n");
		return -6;
	} else {
		printf("Matriz cargada\n\n");
		for (int i = 0; i < alto; i++) {
			for (int j = 0; j < ancho; j++) {
				printf("%u ", matriz[(i * ancho) + j]);
			}
			printf("\n");
		}
		printf("\n");
	}
	return 0;
}


int avanzarTiempo(unsigned char* matriz, unsigned int alto, unsigned int ancho, char* nombre) {
	printf("Iterando...\n\n");
	FILE* archivo = fopen(nombre, "w");
	fprintf(archivo, "P1\n# %s\n%u %u\n", nombre, ancho, alto);

	unsigned char* matrizVecinos = malloc(alto * ancho);
	if (matrizVecinos == NULL) {
		fprintf(stderr, "Error al generar matriz, no hay memoria\n");
		return -3;
	}

	unsigned int i = 0;
	unsigned int j = 0;

	for (i = 0; i < alto; i++) {
		for (j = 0; j < ancho; j++) {
			unsigned char vecinosActuales = vecinos(matriz, i, j, alto, ancho);
			matrizVecinos[(i * ancho) + j] = vecinosActuales;
		}
	}

	for (i = 0; i < alto; i++) {
		for (j = 0; j < ancho; j++) {
			matriz[(i * ancho) + j] = realizarCambio(matriz[(i * ancho) + j], matrizVecinos[(i * ancho) + j]);
			printf("%u ", matriz[(i * ancho) + j]);
			fprintf(archivo, "%u ", matriz[(i * ancho) + j]);
		}
		printf("\n");
		fprintf(archivo, "\n");
	}
	printf("\n");
	free(matrizVecinos);
	fclose(archivo);
	return 0;
}


unsigned int vecinos(unsigned char* matriz, unsigned int i, unsigned int j,
						unsigned int alto, unsigned int ancho) {
	unsigned int vecinos = 0;
	unsigned int filaAnterior = i - 1;
	unsigned int columnaAnterior = j - 1;
	unsigned int filaPosterior = i + 1;
	unsigned int columnaPosterior = j + 1;
	if (i == 0) filaAnterior = (alto - 1);
	if (j == 0) columnaAnterior = (ancho - 1);
	if (i == (alto - 1)) filaPosterior = 0;
	if (j == (ancho - 1)) columnaPosterior = 0;

	vecinos += matriz[(filaAnterior * ancho) + columnaAnterior];
	vecinos += matriz[(filaAnterior * ancho) + j];
	vecinos += matriz[(filaAnterior * ancho) + columnaPosterior];
	vecinos += matriz[(i * ancho) + columnaAnterior];
	vecinos += matriz[(i * ancho) + columnaPosterior];
	vecinos += matriz[(filaPosterior * ancho) + columnaAnterior];
	vecinos += matriz[(filaPosterior * ancho) + j];
	vecinos += matriz[(filaPosterior * ancho) + columnaPosterior];
	return vecinos;
}


unsigned char realizarCambio(unsigned char estado, unsigned char vecinos) {
	unsigned char nuevoEstado = estado;
	if ((vecinos < 2) || (vecinos > 3)) {
		nuevoEstado = 0;
	}
	if (vecinos == 3) {
		nuevoEstado = 1;
	}
	return nuevoEstado;
}
