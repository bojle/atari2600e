#include <errno.h>
#include <stdio.h>
#include "mspace.h"


/* The Address/Memory Space accessible to the CPU */
static byte_t mspace[0xffff];

/* CPU Registers */

static byte_t A;			/* Accumulator */
static byte_t X;			/* General Purpose Register X */	
static byte_t Y;			/* General Purpose Register X */	
static byte_t S;			/* Stack Pointer */
static byte_t P;			/* Program Status Word */
static uint16_t PC;			/* Program Counter */

#if 0
static struct {
	C:1    					/* Carry         (0=No Carry, 1=Carry)                      */
	Z:1     				/* Zero          (0=Nonzero, 1=Zero)                        */
	I:1     				/* IRQ Disable   (0=IRQ Enable, 1=IRQ Disable)              */
	D:1     				/* Decimal Mode  (0=Normal, 1=BCD Mode for ADC/SBC opcodes) */
	B:1     				/* Break Flag    (0=IRQ/NMI, 1=RESET or BRK/PHP opcode)     */
	V:1     				/* Overflow      (0=No Overflow, 1=Overflow)                */
	N:1     				/* Negative/Sign (0=Positive, 1=Negative)                   */
} P;						/* Program Status Register */
#endif 

byte_t fetch_byte(addr_t addr) {
	return mspace[addr];
}
/* Set addr to b */
void set_byte(addr_t addr, byte_t b) {
	mspace[addr] = b;
}

void set_PC(addr_t addr) {
	PC = addr;
}

addr_t fetch_PC() {
	return PC;
}

void set_A(byte_t b) {
	A = b;
}
byte_t fetch_A() {
	return A;
}

void set_X(byte_t b) {
	X = b;
}
byte_t fetch_X() {
	return X;
}

void set_Y(byte_t b) {
	Y = b;
}

byte_t fetch_Y() {
	return Y;
}

void set_S(byte_t b) {
	S = b;
}
byte_t fetch_S() {
	return S;
}

void set_P(byte_t b) {
	P = b;
}

byte_t fetch_P() {
	return P;
}

void load_cartridge(char *filename) {
	FILE *fp = fopen(filename, "r");
	

}
