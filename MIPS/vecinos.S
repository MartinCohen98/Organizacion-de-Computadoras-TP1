#include<sys/regdef.h>

#define SS 24
#define POS_A4 SS + 16

#define O_GP 20
#define O_FP 16

#define O_LTA_FA 0
#define O_LTA_FS 4
#define O_LTA_CA 8
#define O_LTA_CS 12

#define O_A0 (SS)
#define O_A1 (O_A0 + 4)
#define O_A2 (O_A1 + 4)
#define O_A3 (O_A2 + 4)
#define O_A4 (O_A3 + 4)
#define O_A5 (O_A4 + 4)


.text
.align 2

/*
unsigned int
vecinos(unsigned char* matriz, unsigned int i, unsigned int j, 
					unsigned int alto, unsigned int ancho)
*/ 

.globl vecinos
.ent vecinos

vecinos:

    subu	sp, sp, SS
    
    /* Saved Register Area */
    sw		gp, O_GP(sp)
    sw		fp, O_FP(sp)
    move	fp, sp

    /* Argument Building Area */
    sw		a0, O_A0(fp)
    sw		a1, O_A1(fp)
    sw		a2, O_A2(fp)
    sw		a3, O_A3(fp)


    /* Local and temporary area */
    /* filaAnterior = i - 1 */
    lw		t1, O_A1(fp)				# $t1 = i
    sub		t0, t1, 1				# $t0 = i - 1
    bne		zero, t1, endifFA			# si i != 0 saltear los siguientes pasos
    lw		t0, O_A3(fp)				# $t0 = alto
    sub		t0, t0, 1				# Resto 1 para que el alto coincida con el offset en la matriz
endifFA:
    sw		t0, O_LTA_FA(fp)

	/* filaSiguiente = i + 1 */
    addiu	t0, t1, 1				# $t0 = i + 1
    lw		t2, O_A3(fp)				# $t2 = alto
    sub		t2, t2, 1				# Resto 1 para que el alto coincida con el offset en la matriz
    bne		t1, t2, endifFS				# si i != alto saltear el siguiente paso
    addiu	t0, zero, 0				# $t0 = 0
endifFS:
    sw		t0, O_LTA_FS(fp)

    /* columnaAnterior = j - 1 */
    lw		t1, O_A2(fp)				# $t1 = j
    subu	t0, t1, 1				# $t0 = j - 1
    bne		zero, t1, endifCA			# si j no es 0 saltear los siguientes pasos
    lw		t0, O_A4(fp)				# $t0 = ancho
    sub		t0, t0, 1				# Resto 1 para que el ancho coincida con el offset en la matriz
endifCA:
    sw		t0, O_LTA_CA(fp)

	/* columnaSiguiente = j + 1 */
    addiu	t0, t1, 1				# $t0 = i + 1
    lw		t2, O_A4(fp)				# $t2 = ancho
    sub		t2, t2, 1				# Resto 1 para que el ancho coincida con el offset en la matriz
    bne		t1, t2, endifCS				# si j != ancho saltear el siguiente paso
    addiu	t0, zero, 0				# $t0 = 0
endifCS:
    sw		t0, O_LTA_CS(fp)

	/* Calculo de vecinos */
    addiu	t1, zero, 0				# Uso $t1 como vecinos, vecinos = 0
    lw		t2, O_A0(fp)				# $t2 = matriz
    lw		t0, O_A4(fp)				# $t0 = ancho
    lw		t3, O_LTA_FA(fp)			# $t3 = filaAnterior
    mul		t3, t3, t0				# $t3 = offset en matriz de fila anterior
    lw		t4, O_A1(fp)				# $t4 = i
    mul		t4, t4, t0				# $t4 = offset en matriz i
    lw		t5, O_LTA_FS(fp)			# $t5 = filaSiguiente
    mul		t5, t5, t0				# $t5 = offset en matriz de fila siguiente
    lw		t6, O_LTA_CA(fp)			# $t6 = columnaAnterior
    lw		t7, O_A2(fp)				# $t7 = j
    lw		t8, O_LTA_CS(fp)			# $t8 = columnaSiguiente
    addu	t9, t2, t3				# $t9 = posicion en la matriz de la fila anterior
    addu	t0, t9, t6				# $t0 = posicion de casillero de fila anterior y columna anterior
    lbu		t0, (t0)				# Cargo el casillero a $t0
    addu	t1, t1, t0				# Casillero tiene 0 si esta vacio y 1 si no lo esta, lo sumo a vecinos		
    addu 	t0, t9, t7				# Repito proceso para casilleros de la misma columna...
    lbu		t0, (t0)
    addu	t1, t1, t0
    addu	t0, t9, t8
    lbu		t0, (t0)
    addu	t1, t1, t0
    addu	t9, t2, t4				# Hago lo mismo, pero $t9 tiene la posicion en la matriz de la fila i
    addu	t0, t9, t6
    lbu		t0, (t0)
    addu	t1, t1, t0
    addu	t0, t9, t8				# Salteo $t7 porque i, j es el casillero al que le calculo vecinos
    lbu		t0, (t0)
    addu	t1, t1, t0
    addu	t9, t2, t5				# Ahora $t9 esta en la posicion de fila siguiente
    addu	t0, t9, t6
    lbu		t0, (t0)
    addu	t1, t1, t0
    addu 	t0, t9, t7
    lbu		t0, (t0)
    addu	t1, t1, t0
    addu	t0, t9, t8
    lbu		t0, (t0)
    addu	t1, t1, t0

    /* return (vecinos) */
    addiu	v0, t1, 0

    /* Stack unwinding */
    lw		fp, O_FP(sp)
    lw		gp, O_GP(sp)
    addiu	sp, sp, SS

    jr 		ra
.end vecinos

/* compile with gcc -mips32 -mlong32 -mabi=32 -c strlcpy.S */
