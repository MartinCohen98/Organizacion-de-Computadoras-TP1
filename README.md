# Organizacion-de-Computadoras-TP1

La carpeta MIPS tiene la version de MIPS32 del programa, la carpeta de C tiene la version escrita totalmente en C.

Ambos se pueden compilar con gcc sin ningun flag (asumiendo que la version MIPS se compile en una mauina virtual con arquitectura MIPS)

Para la version C:

gcc conway.c -o conway


Para la version MIPS:

gcc conway.c vecinos.S -o conway
