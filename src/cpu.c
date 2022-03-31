#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "cpu.h"
#include "log.h"
#include "mspace.h"

/* ADDRESSED ARE STORED WITH LOW BYTE FIRST 
 * naming convention
 * Immediate: name+i
 * Absolute: name
 * Zero pg Absolute: name+z
 * Implied: name
 * accumulator: name+a
 * indexed: name+id
 * 0pg indexed: name+idz
 * Indirect: name+in
 * Pre-indexed Indirect: name+pin
 * Post-Indexed indirect: name+pon
 * relative: name+r
 */

/* Function pointer type for an instruction-function. */
typedef int (*inst_fptr) (byte_t opcode);

typedef struct inst_t {
	int bytes;
	int cycles;
	inst_fptr exec;
	char *name;
} inst_t;

/* Total Number of Instructions */
#define INSTN 256 	/* 2^8 */

/* Table that holds all the instructions and related information */
static inst_t inst_tbl[INSTN];

/* Used by inst_tbl_init() */
#define inst_assign(opcode, nbytes, ncycles, execfn, iname) \
	inst_tbl[opcode].bytes = nbytes; inst_tbl[opcode].cycles = ncycles; inst_tbl[opcode].exec = execfn; inst_tbl[opcode].name = iname

/************** INSTRUCTIONS ****************************/

/* Add with carry immediate */
int adci(byte_t opcode) {
	byte_t operand = fetch_operand(opcode);
	byte_t A = fetch_A();
	// TODO: Check for Overflow (STATUS_V)
	if (A + operand > 255) {
		set_STATUS(STATUS_C);
	}
	A += operand;
	if ((A >> 7) == 1) { 	// 7th bit of A is set
		set_STATUS(STATUS_N);
	}
	if (A == 0) {			// Result of last operation was zero
		set_STATUS(STATUS_Z);
	}
	set_A(A);
	return 0;
}
int adcz(byte_t opcode) {
	addr_t operand = fetch_operand(opcode);
	byte_t value = fetch_byte(operand);
	byte_t A = fetch_A();
	if (A + value > 255) {
		set_STATUS(STATUS_C);
	}
	A += value;
	if ((A >> 7) == 1) { 	// 7th bit of A is set
		set_STATUS(STATUS_N);
	}
	if (A == 0) {			// Result of last operation was zero
		set_STATUS(STATUS_Z);
	}
	set_A(A);
	return 0;
}
int adczx(byte_t opcode) {
	addr_t operand = fetch_operand(opcode);
	operand += fetch_X();
	byte_t value = fetch_byte(operand);
	byte_t A = fetch_A();
	if (A + value > 255) {
		set_STATUS(STATUS_C);
	}
	A += value;
	if ((A >> 7) == 1) { 	// 7th bit of A is set
		set_STATUS(STATUS_N);
	}
	if (A == 0) {			// Result of last operation was zero
		set_STATUS(STATUS_Z);
	}
	set_A(A);
	return 0;
}
int adc(byte_t opcode) {
	return adcz(opcode);
}
int adcax(byte_t opcode) {
	byte_t extra_cycles = 0;
	addr_t addr = fetch_operand(opcode);
	addr_t new_addr = addr + fetch_X();
	if (page_boundary_crossed(addr, new_addr)) {
		extra_cycles++;
	}
	byte_t value = fetch_byte(new_addr);
	byte_t A = fetch_A();
	if (A + value > 255) {
		set_STATUS(STATUS_C);
	}
	A += value;
	if ((A >> 7) == 1) { 	// 7th bit of A is set
		set_STATUS(STATUS_N);
	}
	if (A == 0) {			// Result of last operation was zero
		set_STATUS(STATUS_Z);
	}
	set_A(A);
	return extra_cycles;
}
int adcay(byte_t opcode) {
	byte_t extra_cycles = 0;
	addr_t addr = fetch_operand(opcode);
	addr_t new_addr = addr + fetch_Y();
	if (page_boundary_crossed(addr, new_addr)) {
		extra_cycles++;
	}
	byte_t value = fetch_byte(new_addr);
	byte_t A = fetch_A();
	if (A + value > 255) {
		set_STATUS(STATUS_C);
	}
	A += value;
	if ((A >> 7) == 1) { 	// 7th bit of A is set
		set_STATUS(STATUS_N);
	}
	if (A == 0) {			// Result of last operation was zero
		set_STATUS(STATUS_Z);
	}
	set_A(A);
	return extra_cycles;
}

int adcinx(byte_t opcode) {
	
}
int adciny(byte_t opcode) {

}

int andi(byte_t opcode) {
	byte_t operand = fetch_operand(opcode);
	byte_t A = fetch_A();
	A &= operand;
	if ((A >> 7) == 1) { 	// 7th bit of A is set
		set_STATUS(STATUS_N);
	}
	if (A == 0) {			// Result of last operation was zero
		set_STATUS(STATUS_Z);
	}
	set_A(A);
	return 0;
}
int andz(byte_t opcode) {
	addr_t operand = fetch_operand(opcode);
	byte_t value = fetch_byte(operand);
	byte_t A = fetch_A();
	A &= value;
	if ((A >> 7) == 1) { 	// 7th bit of A is set
		set_STATUS(STATUS_N);
	}
	if (A == 0) {			// Result of last operation was zero
		set_STATUS(STATUS_Z);
	}
	set_A(A);
	return 0;
}
int andzx(byte_t opcode) {
	addr_t operand = fetch_operand(opcode);
	operand += fetch_X();
	byte_t value = fetch_byte(operand);
	byte_t A = fetch_A();
	A &= value;
	if ((A >> 7) == 1) { 	// 7th bit of A is set
		set_STATUS(STATUS_N);
	}
	if (A == 0) {			// Result of last operation was zero
		set_STATUS(STATUS_Z);
	}
	set_A(A);
	return 0;
}
int and(byte_t opcode) {
	addr_t addr = fetch_operand(opcode);
	byte_t value = fetch_byte(addr);
	byte_t A = fetch_A();
	A &= value;
	if ((A >> 7) == 1) { 	// 7th bit of A is set
		set_STATUS(STATUS_N);
	}
	if (A == 0) {			// Result of last operation was zero
		set_STATUS(STATUS_Z);
	}
	set_A(A);
	return 0;
}
int andax(byte_t opcode) {
	byte_t extra_cycles = 0;
	addr_t addr = fetch_operand(opcode);
	addr_t new_addr = addr + fetch_X();
	byte_t value = fetch_byte(new_addr);
	if (page_boundary_crossed(addr, new_addr)) {
		extra_cycles++;
	}
	byte_t A = fetch_A();
	A &= value;	
	if ((A >> 7) == 1) { 	// 7th bit of A is set
		set_STATUS(STATUS_N);
	}
	if (A == 0) {			// Result of last operation was zero
		set_STATUS(STATUS_Z);
	}
	set_A(A);
	return extra_cycles;
}

int anday(byte_t opcode) {
	byte_t extra_cycles = 0;
	addr_t addr = fetch_operand(opcode);
	addr_t new_addr = addr + fetch_X();
	byte_t value = fetch_byte(new_addr);
	if (page_boundary_crossed(addr, new_addr)) {
		extra_cycles++;
	}
	byte_t A = fetch_A();
	A &= value;	
	if ((A >> 7) == 1) { 	// 7th bit of A is set
		set_STATUS(STATUS_N);
	}
	if (A == 0) {			// Result of last operation was zero
		set_STATUS(STATUS_Z);
	}
	set_A(A);
	return extra_cycles;
}

int andinx(byte_t opcode) {}
int andiny(byte_t opcode) {}

int asla(byte_t opcode) {
	byte_t A = fetch_A();
	byte_t seventh_bit = (A >> 7);
	A <<= 1;
	(seventh_bit == 0) ? clear_STATUS(STATUS_C) : set_STATUS(STATUS_C);
	if (A == 0) {
		set_STATUS(STATUS_Z);
	}
	if ((A >> 7) == 1) { 	// 7th bit of A is set
		set_STATUS(STATUS_N);
	}
	set_A(A);
	return 0;
}
int aslz(byte_t opcode) {
	addr_t operand = fetch_operand(opcode);
	byte_t value = fetch_byte(operand);
	byte_t seventh_bit = (value >> 7);
	value <<= 1;
	(seventh_bit == 0) ? clear_STATUS(STATUS_C) : set_STATUS(STATUS_C);
	if (value == 0) {
		set_STATUS(STATUS_Z);
	}
	if ((value >> 7) == 1) { 	// 7th bit of A is set
		set_STATUS(STATUS_N);
	}
	set_byte(operand, value);
	return 0;
}
int aslzx(byte_t opcode) {
	addr_t addr = fetch_operand(opcode);
	addr_t new_addr = addr + fetch_X();
	byte_t value = fetch_byte(new_addr);
	byte_t seventh_bit = (value >> 7);
	value <<= 1;
	(seventh_bit == 0) ? clear_STATUS(STATUS_C) : set_STATUS(STATUS_C);
	if (value == 0) {
		set_STATUS(STATUS_Z);
	}
	if ((value >> 7) == 1) { 	// 7th bit of A is set
		set_STATUS(STATUS_N);
	}
	set_byte(new_addr, value);
	return 0;
}

int asl(byte_t opcode) {
	return aslz(opcode);
}

int aslax(byte_t opcode) {
	return aslzx(opcode);
}

int branch_aux(byte_t opcode) {
	byte_t extra_cycles = 0;
	addr_t pc = fetch_PC();
	addr_t new_pc = pc;
	byte_t offset = fetch_operand(opcode);
	if (offset > 0x7f) { // Negative Number
		/* Extracting a 2's complement number */
		offset ^= 0xff;
		offset += 1;
		new_pc = pc - offset;
	}
	else {
		new_pc = pc + offset;
	}
	if (page_boundary_crossed(pc, new_pc)) {
		extra_cycles += 2;
	}
	else {
		extra_cycles += 1;
	}
	set_PC(new_pc);
	return extra_cycles;
}

int bcc(byte_t opcode) {
	if (fetch_STATUS(STATUS_C) != 0) {
		return 0;
	}
	return branch_aux(opcode);
}

int bcs(byte_t opcode) {
	if (fetch_STATUS(STATUS_C) != 1) {
		return 0;
	}
	return branch_aux(opcode);
}
int beq(byte_t opcode) {
	if (fetch_STATUS(STATUS_Z) != 1) {
		return 0;
	}
	return branch_aux(opcode);
}

int bitz(byte_t opcode) {}
int bit(byte_t opcode) {}

int bmi(byte_t opcode) {
	if (fetch_STATUS(STATUS_N) != 1) {
		return 0;
	}
	return branch_aux(opcode);
}

int bne(byte_t opcode) {
	if (fetch_STATUS(STATUS_Z) != 0) {
		return 0;
	}
	return branch_aux(opcode);
}
int bpl(byte_t opcode) {
	if (fetch_STATUS(STATUS_N) != 0) {
		return 0;
	}
	return branch_aux(opcode);
}

int brk(byte_t opcode) {}

int bvc(byte_t opcode) {
	if (fetch_STATUS(STATUS_V) != 0) {
		return 0;
	}
	return branch_aux(opcode);
}

int bvs(byte_t opcode) {
	if (fetch_STATUS(STATUS_V) != 1) {
		return 0;
	}
	return branch_aux(opcode);
}

int clc(byte_t opcode) {
	clear_STATUS(STATUS_C);
}
int cld(byte_t opcode) {
	clear_STATUS(STATUS_D);
}
int cli(byte_t opcode) {
	clear_STATUS(STATUS_I);
}
int clv(byte_t opcode) {
	clear_STATUS(STATUS_V);
}

void compare(byte_t b1, byte_t b2) {
	if (b1 < b2) {
		set_STATUS(STATUS_N);
		clear_STATUS(STATUS_Z);
		clear_STATUS(STATUS_C);
	}
	else if (b1 == b2) {
		clear_STATUS(STATUS_N);
		set_STATUS(STATUS_Z);
		clear_STATUS(STATUS_C);
	}
	else if (b1 > b2) {
		clear_STATUS(STATUS_N);
		clear_STATUS(STATUS_Z);
		set_STATUS(STATUS_C);
	}
}

int cmpi(byte_t opcode) {
	byte_t operand = fetch_operand(opcode);
	compare(fetch_A(), operand);
	return 0;
}

int cmpz(byte_t opcode) {
	addr_t addr = fetch_operand(opcode);
	byte_t value = fetch_byte(addr);
	compare(fetch_A(), value);
	return 0;
}

int cmpzx(byte_t opcode) {
	addr_t addr = fetch_operand(opcode);
	addr += fetch_X();
	byte_t value = fetch_byte(addr);
	compare(fetch_A(), value);
	return 0;
}

int cmp(byte_t opcode) {
	return cmpz(opcode);
}

int cmpax(byte_t opcode) {
	byte_t extra_cycles = 0;
	addr_t addr = fetch_operand(opcode);
	addr_t new_addr = addr + fetch_X();
	if (page_boundary_crossed(addr, new_addr)) {
		extra_cycles++;
	}
	byte_t value = fetch_byte(new_addr);
	compare(fetch_A(), value);
	return extra_cycles;
}

int cmpay(byte_t opcode) {
	byte_t extra_cycles = 0;
	addr_t addr = fetch_operand(opcode);
	addr_t new_addr = addr + fetch_Y();
	if (page_boundary_crossed(addr, new_addr)) {
		extra_cycles++;
	}
	byte_t value = fetch_byte(new_addr);
	compare(fetch_A(), value);
	return extra_cycles;
}

int cmpinx(byte_t opcode) {

}

int cmpiny(byte_t opcode) {}

int cpxi(byte_t opcode) {
	byte_t operand = fetch_operand(opcode);
	compare(fetch_X(), operand);
	return 0;
}
int cpxz(byte_t opcode) {
	addr_t addr = fetch_operand(opcode);
	byte_t value = fetch_byte(addr);
	compare(fetch_X(), value);
	return 0;
}

int cpx(byte_t opcode) {
	return cpxz(opcode);
}

int cpyi(byte_t opcode) {
	byte_t operand = fetch_operand(opcode);
	compare(fetch_Y(), operand);
	return 0;
}

int cpyz(byte_t opcode) {
	addr_t addr = fetch_operand(opcode);
	byte_t value = fetch_byte(addr);
	compare(fetch_Y(), value);
	return 0;
}

int cpy(byte_t opcode) {
	return cpyz(opcode);
}

int decz(byte_t opcode) {
	addr_t operand = fetch_operand(opcode);
	byte_t value = fetch_byte(operand);
	value--;
	if ((value >> 7) == 1) { 	// 7th bit of A is set
		set_STATUS(STATUS_N);
	}
	if (value == 0) {			// Result of last operation was zero
		set_STATUS(STATUS_Z);
	}
	set_byte(operand, value);
	return 0;
}
int deczx(byte_t opcode) {
	addr_t operand = fetch_operand(opcode);
	operand += fetch_X();
	byte_t value = fetch_byte(operand);
	value--;
	if ((value >> 7) == 1) { 	// 7th bit of A is set
		set_STATUS(STATUS_N);
	}
	if (value == 0) {			// Result of last operation was zero
		set_STATUS(STATUS_Z);
	}
	set_byte(operand, value);
	return 0;
}
int dec(byte_t opcode) {
	return decz(opcode);
}
int decax(byte_t opcode) {
	return deczx(opcode);
}
int dex(byte_t opcode) {
	byte_t value = fetch_X();
	value--;
	if ((value >> 7) == 1) { 	// 7th bit of A is set
		set_STATUS(STATUS_N);
	}
	if (value == 0) {			// Result of last operation was zero
		set_STATUS(STATUS_Z);
	}
	set_X(value);
	return 0;

}
int dey(byte_t opcode) {
	byte_t value = fetch_Y();
	value--;
	if ((value >> 7) == 1) { 	// 7th bit of A is set
		set_STATUS(STATUS_N);
	}
	if (value == 0) {			// Result of last operation was zero
		set_STATUS(STATUS_Z);
	}
	set_Y(value);
	return 0;
}

int eori(byte_t opcode) {
	byte_t opereor = fetch_opereor(opcode);
	byte_t A = fetch_A();
	A ^= opereor;
	if ((A >> 7) == 1) { 	// 7th bit of A is set
		set_STATUS(STATUS_N);
	}
	if (A == 0) {			// Result of last operation was zero
		set_STATUS(STATUS_Z);
	}
	set_A(A);
	return 0;
}
int eorz(byte_t opcode) {
	addr_t opereor = fetch_opereor(opcode);
	byte_t value = fetch_byte(opereor);
	byte_t A = fetch_A();
	A ^= value;
	if ((A >> 7) == 1) { 	// 7th bit of A is set
		set_STATUS(STATUS_N);
	}
	if (A == 0) {			// Result of last operation was zero
		set_STATUS(STATUS_Z);
	}
	set_A(A);
	return 0;
}
int eorzx(byte_t opcode) {
	addr_t opereor = fetch_opereor(opcode);
	opereor += fetch_X();
	byte_t value = fetch_byte(opereor);
	byte_t A = fetch_A();
	A ^= value;
	if ((A >> 7) == 1) { 	// 7th bit of A is set
		set_STATUS(STATUS_N);
	}
	if (A == 0) {			// Result of last operation was zero
		set_STATUS(STATUS_Z);
	}
	set_A(A);
	return 0;
}
int eor(byte_t opcode) {
	addr_t addr = fetch_opereor(opcode);
	byte_t value = fetch_byte(addr);
	byte_t A = fetch_A();
	A ^= value;
	if ((A >> 7) == 1) { 	// 7th bit of A is set
		set_STATUS(STATUS_N);
	}
	if (A == 0) {			// Result of last operation was zero
		set_STATUS(STATUS_Z);
	}
	set_A(A);
	return 0;
}
int eorax(byte_t opcode) {
	byte_t extra_cycles = 0;
	addr_t addr = fetch_opereor(opcode);
	addr_t new_addr = addr + fetch_X();
	byte_t value = fetch_byte(new_addr);
	if (page_boundary_crossed(addr, new_addr)) {
		extra_cycles++;
	}
	byte_t A = fetch_A();
	A ^= value;	
	if ((A >> 7) == 1) { 	// 7th bit of A is set
		set_STATUS(STATUS_N);
	}
	if (A == 0) {			// Result of last operation was zero
		set_STATUS(STATUS_Z);
	}
	set_A(A);
	return extra_cycles;
}

int eoray(byte_t opcode) {
	byte_t extra_cycles = 0;
	addr_t addr = fetch_opereor(opcode);
	addr_t new_addr = addr + fetch_X();
	byte_t value = fetch_byte(new_addr);
	if (page_boundary_crossed(addr, new_addr)) {
		extra_cycles++;
	}
	byte_t A = fetch_A();
	A ^= value;	
	if ((A >> 7) == 1) { 	// 7th bit of A is set
		set_STATUS(STATUS_N);
	}
	if (A == 0) {			// Result of last operation was zero
		set_STATUS(STATUS_Z);
	}
	set_A(A);
	return extra_cycles;
}

// TODO: Add Impl
int eorinx(byte_t opcode) {}
int eoriny(byte_t opcode) {}

int incz(byte_t opcode) {
	addr_t operand = fetch_operand(opcode);
	byte_t value = fetch_byte(operand);
	value++;
	if ((value >> 7) == 1) { 	// 7th bit of A is set
		set_STATUS(STATUS_N);
	}
	if (value == 0) {			// Result of last operation was zero
		set_STATUS(STATUS_Z);
	}
	set_byte(operand, value);
	return 0;
}
int inczx(byte_t opcode) {
	addr_t operand = fetch_operand(opcode);
	operand += fetch_X();
	byte_t value = fetch_byte(operand);
	value++;
	if ((value >> 7) == 1) { 	// 7th bit of A is set
		set_STATUS(STATUS_N);
	}
	if (value == 0) {			// Result of last operation was zero
		set_STATUS(STATUS_Z);
	}
	set_byte(operand, value);
	return 0;
}
int inc(byte_t opcode) {
	return incz(opcode);
}
int incax(byte_t opcode) {
	return inczx(opcode);
}

int inx(byte_t opcode) {
	byte_t value = fetch_X();
	value++;
	if ((value >> 7) == 1) { 	// 7th bit of A is set
		set_STATUS(STATUS_N);
	}
	if (value == 0) {			// Result of last operation was zero
		set_STATUS(STATUS_Z);
	}
	set_X(value);
	return 0;
}

int iny(byte_t opcode) {
	byte_t value = fetch_X();
	value++;
	if ((value >> 7) == 1) { 	// 7th bit of A is set
		set_STATUS(STATUS_N);
	}
	if (value == 0) {			// Result of last operation was zero
		set_STATUS(STATUS_Z);
	}
	set_Y(value);
	return 0;
}

int jmp(byte_t opcode) {
}
int jmpin(byte_t opcode) {
}
int jsr(byte_t opcode) {}

int ldai(byte_t opcode) {
	byte_t operand = fetch_operand(opcode);
	set_A(operand);
	if ((operand >> 7) == 1) { 	// 7th bit of A is set
		set_STATUS(STATUS_N);
	}
	if (operand == 0) {			// Result of last operation was zero
		set_STATUS(STATUS_Z);
	}
	return 0;
}

int ldaz(byte_t opcode) {
	addr_t zaddr = fetch_operand(opcode);
	byte_t operand = fetch_byte(zaddr);
	set_A(operand);
	if ((operand >> 7) == 1) { 	// 7th bit of A is set
		set_STATUS(STATUS_N);
	}
	if (operand == 0) {			// Result of last operation was zero
		set_STATUS(STATUS_Z);
	}
	return 0;
}
int ldazx(byte_t opcode) {
	addr_t zaddr = fetch_operand(opcode);
	zaddr += fetch_X();
	byte_t operand = fetch_byte(zaddr);
	set_A(operand);
	if ((operand >> 7) == 1) { 	// 7th bit of A is set
		set_STATUS(STATUS_N);
	}
	if (operand == 0) {			// Result of last operation was zero
		set_STATUS(STATUS_Z);
	}
	return 0;
}
int lda(byte_t opcode) {
	addr_t addr = fetch_operand(opcode);
	byte_t operand = fetch_byte(addr);
	set_A(operand);
	if ((operand >> 7) == 1) { 	// 7th bit of A is set
		set_STATUS(STATUS_N);
	}
	if (operand == 0) {			// Result of last operation was zero
		set_STATUS(STATUS_Z);
	}
	return 0;
}
int ldaax(byte_t opcode) {
	byte_t extra_cycles = 0;
	addr_t addr = fetch_operand(opcode);
	addr_t new_addr = addr + fetch_X();
	if (page_boundary_crossed(addr, new_addr)) {
		extra_cycles++;
	}
	byte_t operand = fetch_byte(new_addr);
	set_A(operand);
	if ((operand >> 7) == 1) { 	// 7th bit of A is set
		set_STATUS(STATUS_N);
	}
	if (operand == 0) {			// Result of last operation was zero
		set_STATUS(STATUS_Z);
	}
	return extra_cycles;
}

int ldaay(byte_t opcode) {
	byte_t extra_cycles = 0;
	addr_t addr = fetch_operand(opcode);
	addr_t new_addr = addr + fetch_Y();
	if (page_boundary_crossed(addr, new_addr)) {
		extra_cycles++;
	}
	byte_t operand = fetch_byte(new_addr);
	set_A(operand);
	if ((operand >> 7) == 1) { 	// 7th bit of A is set
		set_STATUS(STATUS_N);
	}
	if (operand == 0) {			// Result of last operation was zero
		set_STATUS(STATUS_Z);
	}
	return extra_cycles;
}

/* TODO: Refactor to include fetch_operands() */
int ldainx(byte_t opcode) {
	addr_t pc = fetch_PC();
	byte_t operand = fetch_byte(pc + 1);
	operand += fetch_X();
	addr_t addr_lower = fetch_byte(operand);
	addr_t addr_upper = fetch_byte(operand + 1);
	addr_upper = addr_upper << 8;
	addr_upper += addr_lower;
	set_A(fetch_byte(addr_upper));
	if ((operand >> 7) == 1) { 	// 7th bit of A is set
		set_STATUS(STATUS_N);
	}
	if (operand == 0) {			// Result of last operation was zero
		set_STATUS(STATUS_Z);
	}
	return 0;
}
/* TODO: Refactor to include fetch_operands() */
int ldainy(byte_t opcode) {
	byte_t extra_cycles = 0;
	addr_t pc = fetch_PC();
	byte_t operand = fetch_byte(pc + 1);
	addr_t addr_lower = fetch_byte(operand);
	addr_t addr_upper = fetch_byte(operand + 1);
	byte_t pg_no = addr_upper;
	addr_upper <<= 8;
	addr_upper += addr_lower;
	addr_upper += fetch_Y();
	byte_t npg_no = (addr_upper >> 8);
	if (pg_no != npg_no) {
		extra_cycles++;
	}
	set_A(fetch_byte(addr_upper));
	if ((operand >> 7) == 1) { 	// 7th bit of A is set
		set_STATUS(STATUS_N);
	}
	if (operand == 0) {			// Result of last operation was zero
		set_STATUS(STATUS_Z);
	}
	return extra_cycles;
}

int ldxi(byte_t opcode) {
	byte_t operand = fetch_operand(opcode);
	set_X(operand);
	if ((operand >> 7) == 1) { 	// 7th bit of A is set
		set_STATUS(STATUS_N);
	}
	if (operand == 0) {			// Result of last operation was zero
		set_STATUS(STATUS_Z);
	}
	return 0;
}
int ldxz(byte_t opcode) {
	addr_t zaddr = fetch_operand(opcode);
	byte_t operand = fetch_byte(zaddr);
	set_X(operand);
	if ((operand >> 7) == 1) { 	// 7th bit of A is set
		set_STATUS(STATUS_N);
	}
	if (operand == 0) {			// Result of last operation was zero
		set_STATUS(STATUS_Z);
	}
	return 0;
}
int ldxzy(byte_t opcode) {
	addr_t zaddr = fetch_operand(opcode);
	zaddr += fetch_Y();
	byte_t operand = fetch_byte(zaddr);
	set_X(operand);
	if ((operand >> 7) == 1) { 	// 7th bit of A is set
		set_STATUS(STATUS_N); } if (operand == 0) {			// Result of last operation was zero
		set_STATUS(STATUS_Z);
	}
	return 0;
}

int ldx(byte_t opcode) {
	addr_t addr = fetch_operand(opcode);
	byte_t operand = fetch_byte(addr);
	set_X(operand);
	if ((operand >> 7) == 1) { 	// 7th bit of A is set
		set_STATUS(STATUS_N);
	}
	if (operand == 0) {			// Result of last operation was zero
		set_STATUS(STATUS_Z);
	}
	return 0;
}

int ldxay(byte_t opcode) {
	byte_t extra_cycles = 0;
	addr_t addr = fetch_operand(opcode);
	addr_t new_addr = addr + fetch_Y();
	if (page_boundary_crossed(addr, new_addr)) {
		extra_cycles++;
	}
	byte_t operand = fetch_byte(new_addr);
	set_X(operand);
	if ((operand >> 7) == 1) { 	// 7th bit of A is set
		set_STATUS(STATUS_N);
	}
	if (operand == 0) {			// Result of last operation was zero
		set_STATUS(STATUS_Z);
	}
	return extra_cycles;
}

int ldyi(byte_t opcode) {
	byte_t operand = fetch_operand(opcode);
	set_Y(operand);
	if ((operand >> 7) == 1) { 	// 7th bit of A is set
		set_STATUS(STATUS_N);
	}
	if (operand == 0) {			// Result of last operation was zero
		set_STATUS(STATUS_Z);
	}
	return 0;
}
int ldyz(byte_t opcode) {
	addr_t zaddr = fetch_operand(opcode);
	byte_t operand = fetch_byte(zaddr);
	set_Y(operand);
	if ((operand >> 7) == 1) { 	// 7th bit of A is set
		set_STATUS(STATUS_N);
	}
	if (operand == 0) {			// Result of last operation was zero
		set_STATUS(STATUS_Z);
	}
	return 0;
}
int ldyzx(byte_t opcode) {
	addr_t zaddr = fetch_operand(opcode);
	zaddr += fetch_X();
	byte_t operand = fetch_byte(zaddr);
	set_Y(operand);
	if ((operand >> 7) == 1) { 	// 7th bit of A is set
		set_STATUS(STATUS_N);
	}
	if (operand == 0) {			// Result of last operation was zero
		set_STATUS(STATUS_Z);
	}
	return 0;
}
int ldy(byte_t opcode) {
	addr_t addr = fetch_operand(opcode);
	byte_t operand = fetch_byte(addr);
	set_X(operand);
	if ((operand >> 7) == 1) { 	// 7th bit of A is set
		set_STATUS(STATUS_N);
	}
	if (operand == 0) {			// Result of last operation was zero
		set_STATUS(STATUS_Z);
	}
	return 0;
}
int ldyax(byte_t opcode) {
	byte_t extra_cycles = 0;
	addr_t addr = fetch_operand(opcode);
	addr_t new_addr = addr + fetch_Y();
	if (page_boundary_crossed(addr, new_addr)) {
		extra_cycles++;
	}
	byte_t operand = fetch_byte(new_addr);
	set_X(operand);
	if ((operand >> 7) == 1) { 	// 7th bit of A is set
		set_STATUS(STATUS_N);
	}
	if (operand == 0) {			// Result of last operation was zero
		set_STATUS(STATUS_Z);
	}
	return extra_cycles;
}

int lsra(byte_t opcode) {
	byte_t A = fetch_A();
	byte_t first_bit = (A & 0x01);
	A >>= 1;
	clear_STATUS(STATUS_N);
	(first_bit == 0) ? clear_STATUS(STATUS_C) : set_STATUS(STATUS_C);
	if (A == 0) {			// Result of last operation was zero
		set_STATUS(STATUS_Z);
	}
	set_A(A);
	return 0;
}

int lsrz(byte_t opcode) {
	addr_t operand = fetch_operand(opcode);
	byte_t value = fetch_byte(operand);
	byte_t first_bit = (value & 0x01);
	value >>= 1;
	if (value == 0) {			// Result of last operation was zero
		set_STATUS(STATUS_Z);
	}
	(first_bit == 0) ? clear_STATUS(STATUS_C) : set_STATUS(STATUS_C);
	clear_STATUS(STATUS_N);

	set_byte(operand, value);
	return 0;
}

int lsrzx(byte_t opcode) {
	addr_t addr = fetch_operand(opcode);
	addr_t new_addr = addr + fetch_X();
	byte_t value = fetch_byte(new_addr);
	byte_t first_bit = (value & 0x01);
	(first_bit == 0) ? clear_STATUS(STATUS_C) : set_STATUS(STATUS_C);
	clear_STATUS(STATUS_N);
	if (value == 0) {			// Result of last operation was zero
		set_STATUS(STATUS_Z);
	}
	value >>= 1;
	set_byte(new_addr, value);
	return 0;
}

int lsr(byte_t opcode) {
	return lsrz(opcode);
}

int lsrax(byte_t opcode) {
	return lsrzx(opcode);
}

int nop(byte_t opcode) {
	return 0;
}

int orai(byte_t opcode) {
	byte_t operora = fetch_operora(opcode);
	byte_t A = fetch_A();
	A |= operora;
	if ((A >> 7) == 1) { 	// 7th bit of A is set
		set_STATUS(STATUS_N);
	}
	if (A == 0) {			// Result of last operation was zero
		set_STATUS(STATUS_Z);
	}
	set_A(A);
	return 0;
}

int oraz(byte_t opcode) {
	addr_t operora = fetch_operora(opcode);
	byte_t value = fetch_byte(operora);
	byte_t A = fetch_A();
	A |= value;
	if ((A >> 7) == 1) { 	// 7th bit of A is set
		set_STATUS(STATUS_N);
	}
	if (A == 0) {			// Result of last operation was zero
		set_STATUS(STATUS_Z);
	}
	set_A(A);
	return 0;
}
int orazx(byte_t opcode) {
	addr_t operora = fetch_operora(opcode);
	operora += fetch_X();
	byte_t value = fetch_byte(operora);
	byte_t A = fetch_A();
	A |= value;
	if ((A >> 7) == 1) { 	// 7th bit of A is set
		set_STATUS(STATUS_N);
	}
	if (A == 0) {			// Result of last operation was zero
		set_STATUS(STATUS_Z);
	}
	set_A(A);
	return 0;
}
int ora(byte_t opcode) {
	addr_t addr = fetch_operora(opcode);
	byte_t value = fetch_byte(addr);
	byte_t A = fetch_A();
	A |= value;
	if ((A >> 7) == 1) { 	// 7th bit of A is set
		set_STATUS(STATUS_N);
	}
	if (A == 0) {			// Result of last operation was zero
		set_STATUS(STATUS_Z);
	}
	set_A(A);
	return 0;
}
int oraax(byte_t opcode) {
	byte_t extra_cycles = 0;
	addr_t addr = fetch_operora(opcode);
	addr_t new_addr = addr + fetch_X();
	byte_t value = fetch_byte(new_addr);
	if (page_boundary_crossed(addr, new_addr)) {
		extra_cycles++;
	}
	byte_t A = fetch_A();
	A |= value;	
	if ((A >> 7) == 1) { 	// 7th bit of A is set
		set_STATUS(STATUS_N);
	}
	if (A == 0) {			// Result of last operation was zero
		set_STATUS(STATUS_Z);
	}
	set_A(A);
	return extra_cycles;
}

int oraay(byte_t opcode) {
	byte_t extra_cycles = 0;
	addr_t addr = fetch_operora(opcode);
	addr_t new_addr = addr + fetch_X();
	byte_t value = fetch_byte(new_addr);
	if (page_boundary_crossed(addr, new_addr)) {
		extra_cycles++;
	}
	byte_t A = fetch_A();
	A |= value;	
	if ((A >> 7) == 1) { 	// 7th bit of A is set
		set_STATUS(STATUS_N);
	}
	if (A == 0) {			// Result of last operation was zero
		set_STATUS(STATUS_Z);
	}
	set_A(A);
	return extra_cycles;
}

int orainx(byte_t opcode) {}
int orainy(byte_t opcode) {}

int pha(byte_t opcode) {
	stack_push(fetch_A());
	return 0;
}
int php(byte_t opcode) {
	stack_push(fetch_P());
	return 0;
}

int pla(byte_t opcode) {
	byte_t A = stack_pop();
	set_A(A);
	return 0;
}

int plp(byte_t opcode) {
	byte_t P = stack_pop();
	set_P(P);
	return 0;
}

/* C is assumed to be either 0 or 1 */
byte_t roll_left(byte_t value, byte_t *C) {
	/* extract */
	byte_t seventh_bit = (value >> 7);
	/* rotate */
	value <<= 1;
	/* set LSB to *C */
	value |= (*C);
	*C = seventh_bit;
	return value;
}

byte_t roll_right(byte_t value, byte_t *C) {
	byte_t first_bit = (value & 0x01);
	value >>= 1;
	/* The 1 or 0 at the LSB of *C should be at MSB of *C
	 * so it can be used to set the MSB of value */
	*C <<= 7;
	value |= *C;
	*C = first_bit;
	return value;
}

int rola(byte_t opcode) {
	byte_t carry = fetch_STATUS(STATUS_C);
	byte_t A = fetch_A();
	A = roll_left(A, &carry);
	if ((A >> 7) == 1) { 	// 7th bit of A is set
		set_STATUS(STATUS_N);
	}
	if (A == 0) {			// Result of last operation was zero
		set_STATUS(STATUS_Z);
	}
	(carry == 0) ? clear_STATUS(STATUS_C) : set_STATUS(STATUS_C);
	set_A(A);
	return 0;
}

int rolz(byte_t opcode) {
	byte_t carry = fetch_STATUS(STATUS_C);
	addr_t addr = fetch_operand(opcode);
	byte_t value = fetch_byte(addr);
	value = roll_left(value, &carry);
	if ((value >> 7) == 1) { 	// 7th bit of A is set
		set_STATUS(STATUS_N);
	}
	if (value == 0) {			// Result of last operation was zero
		set_STATUS(STATUS_Z);
	}
	(carry == 0) ? clear_STATUS(STATUS_C) : set_STATUS(STATUS_C);
	set_byte(addr, value);
	return 0;

}
int rolzx(byte_t opcode) {
	byte_t carry = fetch_STATUS(STATUS_C);
	addr_t addr = fetch_operand(opcode);
	addr += fetch_X();
	byte_t value = fetch_byte(addr);
	value = roll_left(value, &carry);
	if ((value >> 7) == 1) { 	// 7th bit of A is set
		set_STATUS(STATUS_N);
	}
	if (value == 0) {			// Result of last operation was zero
		set_STATUS(STATUS_Z);
	}
	(carry == 0) ? clear_STATUS(STATUS_C) : set_STATUS(STATUS_C);
	set_byte(addr, value);
	return 0;
}

int rol(byte_t opcode) {
	return rolz(opcode);
}

int rolax(byte_t opcode) {
	return rolzx(opcode);
}

int rora(byte_t opcode) {
	byte_t carry = fetch_STATUS(STATUS_C);
	byte_t A = fetch_A();
	A = roll_right(A, &carry);
	if ((A >> 7) == 1) { 	// 7th bit of A is set
		set_STATUS(STATUS_N);
	}
	if (A == 0) {			// Result of last operation was zero
		set_STATUS(STATUS_Z);
	}
	(carry == 0) ? clear_STATUS(STATUS_C) : set_STATUS(STATUS_C);
	set_A(A);
	return 0;
}
int rorz(byte_t opcode) {
	byte_t carry = fetch_STATUS(STATUS_C);
	addr_t addr = fetch_operand(opcode);
	byte_t value = fetch_byte(addr);
	value = roll_right(value, &carry);
	if ((value >> 7) == 1) { 	// 7th bit of A is set
		set_STATUS(STATUS_N);
	}
	if (value == 0) {			// Result of last operation was zero
		set_STATUS(STATUS_Z);
	}
	(carry == 0) ? clear_STATUS(STATUS_C) : set_STATUS(STATUS_C);
	set_byte(addr, value);
	return 0;
}

int rorzx(byte_t opcode) {
	byte_t carry = fetch_STATUS(STATUS_C);
	addr_t addr = fetch_operand(opcode);
	addr += fetch_X();
	byte_t value = fetch_byte(addr);
	value = roll_right(value, &carry);
	if ((value >> 7) == 1) { 	// 7th bit of A is set
		set_STATUS(STATUS_N);
	}
	if (value == 0) {			// Result of last operation was zero
		set_STATUS(STATUS_Z);
	}
	(carry == 0) ? clear_STATUS(STATUS_C) : set_STATUS(STATUS_C);
	set_byte(addr, value);
	return 0;
}

int ror(byte_t opcode) {
	return rorz(opcode);
}
int rorax(byte_t opcode) {
	return rorzx(opcode);
}

int rti(byte_t opcode) {}
int rts(byte_t opcode) {}

int sbci(byte_t opcode) {
	byte_t operand = fetch_operand(opcode);
	byte_t A = fetch_A();
	// TODO: Check for Overflow (STATUS_V)
	if (A + operand < 0) {
		set_STATUS(STATUS_C);
	}
	A -= operand;
	if ((A >> 7) == 1) { 	// 7th bit of A is set
		set_STATUS(STATUS_N);
	}
	if (A == 0) {			// Result of last operation was zero
		set_STATUS(STATUS_Z);
	}
	set_A(A);
	return 0;
}
int sbcz(byte_t opcode) {
	addr_t operand = fetch_operand(opcode);
	byte_t value = fetch_byte(operand);
	byte_t A = fetch_A();
	if (A + value < 0) {
		set_STATUS(STATUS_C);
	}
	A -= value;
	if ((A >> 7) == 1) { 	// 7th bit of A is set
		set_STATUS(STATUS_N);
	}
	if (A == 0) {			// Result of last operation was zero
		set_STATUS(STATUS_Z);
	}
	set_A(A);
	return 0;
}
int sbczx(byte_t opcode) {
	addr_t operand = fetch_operand(opcode);
	operand += fetch_X();
	byte_t value = fetch_byte(operand);
	byte_t A = fetch_A();
	if (A + value < 0) {
		set_STATUS(STATUS_C);
	}
	A -= value;
	if ((A >> 7) == 1) { 	// 7th bit of A is set
		set_STATUS(STATUS_N);
	}
	if (A == 0) {			// Result of last operation was zero
		set_STATUS(STATUS_Z);
	}
	set_A(A);
	return 0;
}

int sbca(byte_t opcode) {
	return sbcz(opcode);
}

int sbcax(byte_t opcode) {
	byte_t extra_cycles = 0;
	addr_t addr = fetch_operand(opcode);
	addr_t new_addr = addr + fetch_X();
	if (page_boundary_crossed(addr, new_addr)) {
		extra_cycles++;
	}
	byte_t value = fetch_byte(new_addr);
	byte_t A = fetch_A();
	if (A + value < 0) {
		set_STATUS(STATUS_C);
	}
	A -= value;
	if ((A >> 7) == 1) { 	// 7th bit of A is set
		set_STATUS(STATUS_N);
	}
	if (A == 0) {			// Result of last operation was zero
		set_STATUS(STATUS_Z);
	}
	set_A(A);
	return extra_cycles;
}
int sbcay(byte_t opcode) {
	byte_t extra_cycles = 0;
	addr_t addr = fetch_operand(opcode);
	addr_t new_addr = addr + fetch_Y();
	if (page_boundary_crossed(addr, new_addr)) {
		extra_cycles++;
	}
	byte_t value = fetch_byte(new_addr);
	byte_t A = fetch_A();
	if (A + value < 0) {
		set_STATUS(STATUS_C);
	}
	A -= value;
	if ((A >> 7) == 1) { 	// 7th bit of A is set
		set_STATUS(STATUS_N);
	}
	if (A == 0) {			// Result of last operation was zero
		set_STATUS(STATUS_Z);
	}
	set_A(A);
	return extra_cycles;
}

int sbcinx(byte_t opcode) {}
int sbciny(byte_t opcode) {}

int sec(byte_t opcode) {
	set_STATUS(STATUS_C);
}
int sed(byte_t opcode) {
	set_STATUS(STATUS_D);
}
int sei(byte_t opcode) {
	set_STATUS(STATUS_I);
}

int staz(byte_t opcode) {
	addr_t addr = fetch_operand(opcode);
	set_byte(addr, fetch_A());
	return 0;
}
int stazx(byte_t opcode) {
	addr_t addr = fetch_operand(opcode);
	addr += fetch_X();
	set_byte(addr, fetch_A());
	return 0;
}
int sta(byte_t opcode) {
	addr_t addr = fetch_operand(opcode);
	set_byte(addr, fetch_A());
	return 0;
}
int staax(byte_t opcode) {
	addr_t addr = fetch_operand(opcode);
	addr += fetch_X();
	set_byte(addr, fetch_A());
	return 0;
}
int staay(byte_t opcode) {
	addr_t addr = fetch_operand(opcode);
	addr += fetch_Y();
	set_byte(addr, fetch_A());
	return 0;
}
int stainx(byte_t opcode) {
}
int stainy(byte_t opcode) {
}
int stxz(byte_t opcode) {
	addr_t addr = fetch_operand(opcode);
	set_byte(addr, fetch_X());
	return 0;
}
int stxzy(byte_t opcode) {
	addr_t addr = fetch_operand(opcode);
	addr += fetch_Y();
	set_byte(addr, fetch_X());
	return 0;
}
int stx(byte_t opcode) {
	addr_t addr = fetch_operand(opcode);
	set_byte(addr, fetch_X());
	return 0;
}
int styz(byte_t opcode) {
	addr_t addr = fetch_operand(opcode);
	set_byte(addr, fetch_Y());
	return 0;
}
int styzx(byte_t opcode) {
	addr_t addr = fetch_operand(opcode);
	addr += fetch_X();
	set_byte(addr, fetch_Y());
	return 0;
}

int sty(byte_t opcode) {
	addr_t addr = fetch_operand(opcode);
	set_byte(addr, fetch_Y());
	return 0;
}

int tax(byte_t opcode) {
	byte_t A = fetch_A();
	set_X(A);
	if ((A >> 7) == 1) {
		set_STATUS(STATUS_N);
	}
	if (A == 0) {
		set_STATUS(STATUS_Z);
	}
	return 0;	
}
int tay(byte_t opcode) {
	byte_t A = fetch_A();
	set_Y(A);
	if ((A >> 7) == 1) {
		set_STATUS(STATUS_N);
	}
	if (A == 0) {
		set_STATUS(STATUS_Z);
	}
	return 0;	
}

int tsx(byte_t opcode) {
	byte_t S = fetch_S();
	set_X(A);
	if ((S >> 7) == 1) {
		set_STATUS(STATUS_N);
	}
	if (S == 0) {
		set_STATUS(STATUS_Z);
	}
	return 0;	

}

int txa(byte_t opcode) {
	byte_t X = fetch_X();
	set_A(X);
	if ((X >> 7) == 1) {
		set_STATUS(STATUS_N);
	}
	if (X == 0) {
		set_STATUS(STATUS_Z);
	}
	return 0;	
}

int txs(byte_t opcode) {
	set_S(fetch_X());
	return 0;
}

int tya(byte_t opcode) {
	byte_t Y = fetch_Y();
	set_A(Y);
	if ((Y >> 7) == 1) {
		set_STATUS(STATUS_N);
	}
	if (Y == 0) {
		set_STATUS(STATUS_Z);
	}
	return 0;	
}

int vac(byte_t opcode) {
	log_fatal("Vacant/Illegal Instruction: %02x", opcode);
	exit(EXIT_FAILURE);
}

/******************* END ****************************/

void inst_tbl_init() {
	inst_assign(0x69, 2, 2, adci,"adci");
	inst_assign(0x65, 2, 3, adcz,"adcz");
	inst_assign(0x75, 2, 4, adczx,"adczx");
	inst_assign(0x60, 3, 4, adc,"adc");
	inst_assign(0x7D, 3, 4, adcax,"adcax");
	inst_assign(0x79, 3, 4, adcay,"adcay");
	inst_assign(0x61, 2, 6, adcinx,"adcinx");
	inst_assign(0x71, 2, 5, adciny,"adciny");

	inst_assign(0x29, 2, 2, andi,"andi");
	inst_assign(0x25, 2, 3, andz,"andz");
	inst_assign(0x35, 2, 4, andzx,"andzx");
	inst_assign(0x2D, 3, 4, and,"and");
	inst_assign(0x3D, 3, 4, andax,"andax");
	inst_assign(0x39, 3, 4, anday,"anday");
	inst_assign(0x21, 2, 6, andinx,"andinx");
	inst_assign(0x31, 2, 5, andiny,"andiny");

	inst_assign(0x0A, 1, 2, asla,"asla");
	inst_assign(0x06, 2, 5, aslz,"aslz");
	inst_assign(0x16, 2, 6, aslzx,"aslzx");
	inst_assign(0x0E, 3, 6, asl,"asl");
	inst_assign(0x1E, 3, 7, aslax,"aslax");

	/************* B ************/
	inst_assign(0x90, 2, 2, bcc,"bcc");
	inst_assign(0xb0, 2, 2, bcs,"bcs");
	inst_assign(0xf0, 2, 2, beq,"beq");
	inst_assign(0x24, 2, 3, bitz,"bitz");
	inst_assign(0x2C, 3, 4, bit,"bit");
	inst_assign(0x30, 2, 2, bmi,"bmi");
	inst_assign(0xd0, 2, 2, bne,"bne");
	inst_assign(0x10, 2, 2, bpl,"bpl");
	inst_assign(0x00, 1, 7, brk,"brk");
	inst_assign(0x50, 2, 2, bvc,"bvc");
	inst_assign(0x00, 1, 7, bvs,"bvs");


	/*********** C *************/
	inst_assign(0x18, 1, 2, clc,"clc");
	inst_assign(0xd8, 1, 2, cld,"cld");
	inst_assign(0x58, 1, 2, cli,"cli");
	inst_assign(0xb8, 1, 2, clv,"clv");

	inst_assign(0xC9, 2, 2, cmpi,"cmpi");
	inst_assign(0xC5, 2, 3, cmpz,"cmpz");
	inst_assign(0xD5, 2, 4, cmpzx,"cmpzx");
	inst_assign(0xCD, 3, 4, cmp,"cmp");
	inst_assign(0xDD, 3, 4, cmpax,"cmpax");
	inst_assign(0xD9, 3, 4, cmpay,"cmpay");
	inst_assign(0xC1, 2, 6, cmpinx,"cmpinx");
	inst_assign(0xD1, 2, 5, cmpiny,"cmpiny");

	inst_assign(0xE0, 2, 2, cpxi,"cpxi");
	inst_assign(0xE4, 2, 3, cpxz,"cpxz");
	inst_assign(0xEC, 3, 4, cpx,"cpx");

	inst_assign(0xC0, 2, 2, cpyi,"cpyi");
	inst_assign(0xC4, 2, 3, cpyz,"cpyz");
	inst_assign(0xCC, 3, 4, cpy,"cpy");

	/*********** D **************/
	inst_assign(0xC6, 2, 5, decz,"decz");
	inst_assign(0xD6, 2, 6, deczx,"deczx");
	inst_assign(0xCE, 3, 6, dec,"dec");
	inst_assign(0xDE, 3, 7, decax,"decax");
	inst_assign(0xca, 1, 2, dex,"dex");
	inst_assign(0x88, 1, 2, dey,"dey");

	inst_assign(0x49, 2, 2, eori,"eori");
	inst_assign(0x45, 2, 3, eorz,"eorz");
	inst_assign(0x55, 2, 4, eorzx,"eorzx");
	inst_assign(0x40, 3, 4, eor,"eor");
	inst_assign(0x5D, 3, 4, eorax,"eorax");
	inst_assign(0x59, 3, 4, eoray,"eoray");
	inst_assign(0x41, 2, 6, eorinx,"eorinx");
	inst_assign(0x51, 2, 5, eoriny,"eoriny");

	inst_assign(0xE6, 2, 5, incz,"incz");
	inst_assign(0xF6, 2, 6, inczx,"inczx");
	inst_assign(0xEE, 3, 6, inc,"inc");
	inst_assign(0xFE, 3, 7, incax,"incax");
	inst_assign(0xe8, 1, 2, inx,"inx");
	inst_assign(0xc8, 1, 2, iny,"iny");

	inst_assign(0x4C, 3, 3, jmp,"jmp");
	inst_assign(0x6C, 3, 5, jmpin,"jmpin");
	inst_assign(0x20, 3, 6, jsr,"jsr");

	inst_assign(0xA9, 2, 2, ldai,"ldai");
	inst_assign(0xA5, 2, 3, ldaz,"ldaz");
	inst_assign(0xB5, 2, 4, ldazx,"ldazx");
	inst_assign(0xAD, 3, 4, lda,"lda");
	inst_assign(0xBD, 3, 4, ldaax,"ldaax");
	inst_assign(0xB9, 3, 4, ldaay,"ldaay");
	inst_assign(0xA1, 2, 6, ldainx,"ldainx");
	inst_assign(0xB1, 2, 5, ldainy,"ldainy");

	inst_assign(0xA2, 2, 2, ldxi,"ldxi");
	inst_assign(0xA6, 2, 3, ldxz,"ldxz");
	inst_assign(0xB6, 2, 4, ldxzy,"ldxzy");
	inst_assign(0xAE, 3, 4, ldx,"ldx");
	inst_assign(0xBE, 3, 4, ldxay,"ldxay");

	inst_assign(0xA0, 2, 2, ldyi,"ldyi");
	inst_assign(0xA4, 2, 3, ldyz,"ldyz");
	inst_assign(0xB4, 2, 4, ldyzx,"ldyzx");
	inst_assign(0xAC, 3, 4, ldy,"ldy");
	inst_assign(0xBC, 3, 4, ldyax,"ldyax");

	inst_assign(0x4A, 1, 2, lsra,"lsra");
	inst_assign(0x46, 2, 5, lsrz,"lsrz");
	inst_assign(0x56, 2, 6, lsrzx,"lsrzx");
	inst_assign(0x4E, 3, 6, lsr,"lsr");
	inst_assign(0x5E, 3, 7, lsrax,"lsrax");

	inst_assign(0xEA, 1, 2, nop,"nop");

	inst_assign(0x09, 2, 2, orai,"orai");
	inst_assign(0x05, 2, 3, oraz,"oraz");
	inst_assign(0x15, 2, 4, orazx,"orazx");
	inst_assign(0x0D, 3, 4, ora,"ora");
	inst_assign(0x10, 3, 4, oraax,"oraax");
	inst_assign(0x19, 3, 4, oraay,"oraay");
	inst_assign(0x01, 2, 6, orainx,"orainx");
	inst_assign(0x11, 2, 5, orainy,"orainy");

	inst_assign(0x48, 1, 3, pha,"pha");
	inst_assign(0x08, 1, 3, php,"php");
	inst_assign(0x68, 1, 4, pla,"pla");
	inst_assign(0x28, 1, 4, plp,"plp");

	inst_assign(0x2A, 1, 2, rola,"rola");
	inst_assign(0x26, 2, 5, rolz,"rolz");
	inst_assign(0x36, 2, 6, rolzx,"rolzx");
	inst_assign(0x2E, 3, 6, rol,"rol");
	inst_assign(0x3E, 3, 7, rolax,"rolax");

	inst_assign(0x6A, 1, 2, rora,"rora");
	inst_assign(0x66, 2, 5, rorz,"rorz");
	inst_assign(0x76, 2, 6, rorzx,"rorzx");
	inst_assign(0x6E, 3, 6, ror,"ror");
	inst_assign(0x7E, 3, 7, rorax,"rorax");


	inst_assign(0x4D, 1, 6, rti,"rti");
	inst_assign(0x60, 1, 6, rts,"rts");

	inst_assign(0xE9, 2, 2, sbci,"sbci");
	inst_assign(0xE5, 2, 3, sbcz,"sbcz");
	inst_assign(0xF5, 2, 4, sbczx,"sbczx");
	inst_assign(0xED, 3, 4, sbca,"sbca");
	inst_assign(0xFD, 3, 4, sbcax,"sbcax");
	inst_assign(0xF9, 3, 4, sbcay,"sbcay");
	inst_assign(0xE1, 2, 6, sbcinx,"sbcinx");
	inst_assign(0xF1, 2, 5, sbciny,"sbciny");

	inst_assign(0x38, 1, 2, sec,"sec");
	inst_assign(0xf8, 1, 2, sed,"sed");
	inst_assign(0x78, 1, 2, sei,"sei");

	inst_assign(0x85, 2, 3, staz,"staz");
	inst_assign(0x95, 2, 4, stazx,"stazx");
	inst_assign(0x8D, 3, 4, sta,"sta");
	inst_assign(0x9D, 3, 5, staax,"staax");
	inst_assign(0x99, 3, 5, staay,"staay");
	inst_assign(0x81, 2, 6, stainx,"stainx");
	inst_assign(0x91, 2, 6, stainy,"stainy");

	inst_assign(0x86, 2, 3, stxz,"stxz");
	inst_assign(0x96, 2, 4, stxzy,"stxzy");
	inst_assign(0x8E, 3, 4, stx,"stx");

	inst_assign(0x84, 2, 3, styz,"styz");
	inst_assign(0x94, 2, 4, styzx,"styzx");
	inst_assign(0x8C, 3, 4, sty,"sty");

	inst_assign(0xaa, 1, 2, tax,"tax");
	inst_assign(0xa8, 1, 2, tay,"tay");
	inst_assign(0xba, 1, 2, tsx,"tsx");
	inst_assign(0x8a, 1, 2, txa,"txa");
	inst_assign(0x9a, 1, 2, txs,"txs");
	inst_assign(0x98, 1, 2, tya,"tya");
	/********* Vacant ************/
	inst_assign(0x02, 0, 0, vac,"vac"); inst_assign(0x03, 0, 0, vac,"vac");
	inst_assign(0x04, 0, 0, vac,"vac"); inst_assign(0x07, 0, 0, vac,"vac");
	inst_assign(0x0B, 0, 0, vac,"vac"); inst_assign(0x0C, 0, 0, vac,"vac");
	inst_assign(0x0F, 0, 0, vac,"vac"); inst_assign(0x12, 0, 0, vac,"vac");
	inst_assign(0x13, 0, 0, vac,"vac"); inst_assign(0x14, 0, 0, vac,"vac");
	inst_assign(0x17, 0, 0, vac,"vac"); inst_assign(0x1A, 0, 0, vac,"vac");
	inst_assign(0x1B, 0, 0, vac,"vac"); inst_assign(0x1C, 0, 0, vac,"vac");
	inst_assign(0x1F, 0, 0, vac,"vac"); inst_assign(0x42, 0, 0, vac,"vac");
	inst_assign(0x43, 0, 0, vac,"vac"); inst_assign(0x44, 0, 0, vac,"vac");
	inst_assign(0x47, 0, 0, vac,"vac"); inst_assign(0x4B, 0, 0, vac,"vac");
	inst_assign(0x4F, 0, 0, vac,"vac"); inst_assign(0x52, 0, 0, vac,"vac");
	inst_assign(0x53, 0, 0, vac,"vac"); inst_assign(0x54, 0, 0, vac,"vac");
	inst_assign(0x57, 0, 0, vac,"vac"); inst_assign(0x5A, 0, 0, vac,"vac");
	inst_assign(0x5B, 0, 0, vac,"vac"); inst_assign(0x5C, 0, 0, vac,"vac");
	inst_assign(0x5F, 0, 0, vac,"vac"); inst_assign(0x80, 0, 0, vac,"vac");
	inst_assign(0x82, 0, 0, vac,"vac"); inst_assign(0x83, 0, 0, vac,"vac");
	inst_assign(0x87, 0, 0, vac,"vac"); inst_assign(0x89, 0, 0, vac,"vac");
	inst_assign(0x8B, 0, 0, vac,"vac"); inst_assign(0x8F, 0, 0, vac,"vac");
	inst_assign(0x92, 0, 0, vac,"vac"); inst_assign(0x93, 0, 0, vac,"vac");
	inst_assign(0x97, 0, 0, vac,"vac"); inst_assign(0x9B, 0, 0, vac,"vac");
	inst_assign(0x9C, 0, 0, vac,"vac"); inst_assign(0x9E, 0, 0, vac,"vac");
	inst_assign(0x9F, 0, 0, vac,"vac"); inst_assign(0xC2, 0, 0, vac,"vac");
	inst_assign(0xC3, 0, 0, vac,"vac"); inst_assign(0xC7, 0, 0, vac,"vac");
	inst_assign(0xCB, 0, 0, vac,"vac"); inst_assign(0xCF, 0, 0, vac,"vac");
	inst_assign(0xD2, 0, 0, vac,"vac"); inst_assign(0xD3, 0, 0, vac,"vac");
	inst_assign(0xD4, 0, 0, vac,"vac"); inst_assign(0xD7, 0, 0, vac,"vac");
	inst_assign(0xDA, 0, 0, vac,"vac"); inst_assign(0xDB, 0, 0, vac,"vac");
	inst_assign(0xDC, 0, 0, vac,"vac"); inst_assign(0xDF, 0, 0, vac,"vac");
	inst_assign(0x22, 0, 0, vac,"vac"); inst_assign(0x23, 0, 0, vac,"vac");
	inst_assign(0x27, 0, 0, vac,"vac"); inst_assign(0x2B, 0, 0, vac,"vac");
	inst_assign(0x2F, 0, 0, vac,"vac"); inst_assign(0x32, 0, 0, vac,"vac");
	inst_assign(0x33, 0, 0, vac,"vac"); inst_assign(0x34, 0, 0, vac,"vac");
	inst_assign(0x37, 0, 0, vac,"vac"); inst_assign(0x3A, 0, 0, vac,"vac");
	inst_assign(0x3B, 0, 0, vac,"vac"); inst_assign(0x3C, 0, 0, vac,"vac");
	inst_assign(0x3F, 0, 0, vac,"vac"); inst_assign(0x62, 0, 0, vac,"vac");
	inst_assign(0x63, 0, 0, vac,"vac"); inst_assign(0x64, 0, 0, vac,"vac");
	inst_assign(0x67, 0, 0, vac,"vac"); inst_assign(0x6B, 0, 0, vac,"vac");
	inst_assign(0x6F, 0, 0, vac,"vac"); inst_assign(0x72, 0, 0, vac,"vac");
	inst_assign(0x73, 0, 0, vac,"vac"); inst_assign(0x74, 0, 0, vac,"vac");
	inst_assign(0x77, 0, 0, vac,"vac"); inst_assign(0x7A, 0, 0, vac,"vac");
	inst_assign(0x7B, 0, 0, vac,"vac"); inst_assign(0x7C, 0, 0, vac,"vac");
	inst_assign(0x7F, 0, 0, vac,"vac"); inst_assign(0xA3, 0, 0, vac,"vac");
	inst_assign(0xA7, 0, 0, vac,"vac"); inst_assign(0xAB, 0, 0, vac,"vac");
	inst_assign(0xAF, 0, 0, vac,"vac"); inst_assign(0xB2, 0, 0, vac,"vac");
	inst_assign(0xB3, 0, 0, vac,"vac"); inst_assign(0xB7, 0, 0, vac,"vac");
	inst_assign(0xBB, 0, 0, vac,"vac"); inst_assign(0xBF, 0, 0, vac,"vac");
	inst_assign(0xE2, 0, 0, vac,"vac"); inst_assign(0xE3, 0, 0, vac,"vac");
	inst_assign(0xE7, 0, 0, vac,"vac"); inst_assign(0xEB, 0, 0, vac,"vac");
	inst_assign(0xEF, 0, 0, vac,"vac"); inst_assign(0xF2, 0, 0, vac,"vac");
	inst_assign(0xF3, 0, 0, vac,"vac"); inst_assign(0xF4, 0, 0, vac,"vac");
	inst_assign(0xF7, 0, 0, vac,"vac"); inst_assign(0xFA, 0, 0, vac,"vac");
	inst_assign(0xFB, 0, 0, vac,"vac"); inst_assign(0xFC, 0, 0, vac,"vac");
	inst_assign(0xFF, 0, 0, vac,"vac");
	log_trace("inst_tbl_init(): Initialized Instruction Table");
}

char *inst_name(byte_t opcode) {
	return (inst_tbl[opcode]).name;
}

byte_t inst_bytes(byte_t opcode) {
	return (inst_tbl[opcode]).bytes;
}

byte_t inst_cycles(byte_t opcode) {
	return (inst_tbl[opcode]).cycles;
}

byte_t inst_exec(byte_t opcode) {
	return ((inst_tbl[opcode])).exec(opcode);
}

addr_t fetch_operand(byte_t opcode) {
	addr_t pc = fetch_PC();
	pc++;
	addr_t operand = 0;
	byte_t size = inst_bytes(opcode);
	byte_t byte = 0;
	/* Bytes are stored in the memory in little-endian order */
	for (int i = 0; i < size-1; ++i) {
		byte = fetch_byte(pc++);
		operand += (byte << (8 * i));
	}
	return operand;
}

/* If the high byte of two addresses are the same, they belong to the
 * same 'page'
 */
byte_t page_boundary_crossed(addr_t old_addr, addr_t new_addr) {
	byte_t old_pgno = (old_addr >> 8);
	byte_t new_pgno = (new_addr >> 8);
	return (old_pgno != new_pgno);
}


void record_state(state_t *s) {
	s->A = fetch_A();
	s->X = fetch_X();
	s->Y = fetch_Y();
	s->S = fetch_S();
	s->P = fetch_P();
	s->PC = fetch_PC();
	s->P_C = ((s->P & STATUS_C) == 0 ? 0 : 1);
	s->P_Z = ((s->P & STATUS_Z) == 0 ? 0 : 1);
	s->P_I = ((s->P & STATUS_I) == 0 ? 0 : 1);
	s->P_D = ((s->P & STATUS_D) == 0 ? 0 : 1);
	s->P_B = ((s->P & STATUS_B) == 0 ? 0 : 1);
	s->P_V = ((s->P & STATUS_V) == 0 ? 0 : 1);
	s->P_N = ((s->P & STATUS_N) == 0 ? 0 : 1);
}

/* File that disassemble() will output to */
FILE *disas_fp = NULL;
/* Name of the said file */
#define DISAS_FILENAME "dis.asm"

void disassembler_init() {
	disas_fp = fopen(DISAS_FILENAME, "w+");
	if (!disas_fp) {
		log_fatal("disassembler_init(): %s", strerror(errno));
		exit(EXIT_FAILURE);
	}
}

void disassemble(byte_t opcode, state_t *s) {
	addr_t pc = s->PC;
	pc++;
	addr_t operand = 0;
	byte_t size = inst_bytes(opcode);
	byte_t byte = 0;
	for (int i = 0; i < size-1; ++i) {
		byte = fetch_byte(pc++);
		operand += (byte << (8 * i));
	}
	fprintf(disas_fp, "%s (0x%02x,%d,%d)\t0x%04x\n", inst_name(opcode),
			opcode, size, inst_cycles(opcode), operand);
	fprintf(disas_fp, "\tOLD STATE\t\tNEW STATE\n");
	fprintf(disas_fp, "\tA:\t0x%x,%d\t\t0x%x,%d\n", s->A, s->A, fetch_A(), fetch_A());
	fprintf(disas_fp, "\tX:\t0x%x,%d\t\t0x%x,%d\n", s->X, s->X, fetch_X(), fetch_X());
	fprintf(disas_fp, "\tY:\t0x%x,%d\t\t0x%x,%d\n", s->Y, s->Y, fetch_Y(), fetch_Y());
	fprintf(disas_fp, "\tS:\t0x%x,%d\t\t0x%x,%d\n", s->S, s->S, fetch_S(), fetch_S());
	//fprintf(disas_fp, "\tP:\t0x%x,%d\t\t0x%x,%d\n", s->P, s->P, fetch_P(), fetch_P());
	fprintf(disas_fp, "\tN V B D I Z C\t\tN V B D I Z C\n");
	fprintf(disas_fp, "\t%d %d %d %d %d %d %d\t\t%d %d %d %d %d %d %d\n",
			s->P_N, s->P_V, s->P_B, s->P_D, s->P_I, s->P_Z, s->P_C,
			fetch_STATUS(STATUS_N), fetch_STATUS(STATUS_V), fetch_STATUS(STATUS_B),
			fetch_STATUS(STATUS_D), fetch_STATUS(STATUS_I), fetch_STATUS(STATUS_Z),
			fetch_STATUS(STATUS_C)
		   );
	fprintf(disas_fp, "\tPC:\t0x%x\t\t0x%x\n", s->PC, fetch_PC());
}
