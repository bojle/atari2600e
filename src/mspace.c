#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "mspace.h"
#include "log.h"
#include "except.h"
#include "tia.h"


/* The Address/Memory Space accessible to the CPU */
static byte_t mspace[0xffff];

/* CPU Registers */

static byte_t A;			/* Accumulator */
static byte_t X;			/* General Purpose Register X */	
static byte_t Y;			/* General Purpose Register X */	
static addr_t S = RAM_END;	/* Stack Pointer */
static byte_t P = 32;		/* Program Status Word. 32 bcoz the 5th bit
							   is supposed to be logical 1 at all times */
static addr_t PC;			/* Program Counter */

byte_t fetch_byte(addr_t addr) {
	return mspace[addr];
}
/* Set addr to b */
void set_byte(addr_t addr, byte_t b) {
	if (is_strobe(addr)) {
		strobe_dispatch(addr, b);
	}
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

void set_S(addr_t b) {
	S = b;
}
addr_t fetch_S() {
	return S;
}

void set_P(byte_t b) {
	P = b;
}

byte_t fetch_P() {
	return P;
}

void set_STATUS(enum status_t st) {
	P |= st;
}

void clear_STATUS(enum status_t st) {
	P &= (~st);
}

byte_t fetch_STATUS(enum status_t st) {
	return ((P & st) == 0 ? 0 : 1);
}

// TODO: Check for stack overflow
void stack_push(byte_t b) {
	mspace[S] = b;
	S--;
}

byte_t stack_pop() {
	S++;
	return mspace[S];
}

byte_t stack_top() {
	return mspace[S+1];
}

void load_cartridge(char *filename) {
	FILE *fp = fopen(filename, "r");
	if (!fp) {
		log_fatal("%s: %s\n", filename, strerror(errno));
		exit(EXIT_FAILURE);
	}
	const int cart_size = CARMEM_END - CARMEM_START;
	byte_t tbuf[cart_size];
	int read_size = fread(tbuf, sizeof(byte_t), cart_size, fp);
	memcpy(mspace + CARMEM_START, tbuf, read_size);
	log_trace("load_cartridge(): Loaded Cartridge Into Memory");
	fclose(fp);

	addr_t l = fetch_byte(CARMEM_END - 3);
	addr_t h = fetch_byte(CARMEM_END - 2);
	addr_t cart_entrypoint = (h << 8) + l;
	if (cart_entrypoint < CARMEM_START) {
		cart_entrypoint = CARMEM_START;
	}
	set_PC(cart_entrypoint);
}

int p2(int n) {
	int r = 1;
	while (n >= 0) {
		r *= 2;
		n--;
	}
	return r;
}
