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

typedef int (*inst_fptr) (void);

/* Total Instructions */
#define INSTN 256 	/* 2^8 */

typedef struct inst_t {
	int bytes;
	int cycles;
	inst_fptr exec;
	char *name;
} inst_t;

static inst_t inst_tbl[INSTN];

#define inst_assign(opcode, nbytes, ncycles, execfn, iname) \
	inst_tbl[opcode].bytes = nbytes; inst_tbl[opcode].cycles = ncycles; inst_tbl[opcode].exec = execfn; inst_tbl[opcode].name = iname

/* Add with carry immediate */
int adci() {
	addr_t pc = fetch_PC();
	byte_t operand = fetch_byte(pc + 1);
	byte_t A = fetch_A() + operand;
	set_A(A);
	return 0;
}
int adcz() {
}
int adczx() {}
int adc() {}
int adcax() {}
int adcay() {}
int adcinx() {}
int adciny() {}
int andi() {}
int andz() {}
int andzx() {}
int and() {}
int andax() {}
int andyx() {}
int andinx() {}
int andiny() {}
int asla() {}
int aslz() {}
int aslzx() {}
int asl() {}
int aslax() {}
int bcc() {}
int bcs() {}
int beq() {}
int bitz() {}
int bit() {}
int bmi() {}
int bne() {}
int bpl() {}
int brk() {}
int bvc() {}
int bvs() {}
int clc() {}
int cld() {}
int cli() {}
int clv() {}
int cmpi() {}
int cmpz() {}
int cmpzx() {}
int cmp() {}
int cmpax() {}
int cmpay() {}
int cmpinx() {}
int cmpiny() {}
int cpxi() {}
int cpxz() {}
int cpx() {}
int cpyi() {}
int cpyz() {}
int cpy() {}
int decz() {}
int deczx() {}
int dec() {}
int decax() {}
int dex() {}
int dey() {}
int eori() {}
int eorz() {}
int eorzx() {}
int eor() {}
int eorax() {}
int eoray() {}
int eorinx() {}
int eoriny() {}
int incz() {}
int inczx() {}
int inc() {}
int incax() {}
int inx() {}
int iny() {}
int jmp() {}
int jmpin() {}
int jsr() {}

int ldai() {
	addr_t pc = fetch_PC();
	byte_t operand = fetch_byte(pc + 1);
	set_A(operand);
	return 0;
}

int ldaz() {
	addr_t pc = fetch_PC();
	addr_t zaddr = fetch_byte(pc + 1);
	byte_t operand = fetch_byte(zaddr);
	set_A(operand);
	return 0;
}
int ldazx() {
	addr_t pc = fetch_PC();
	addr_t zaddr = fetch_byte(pc + 1);
	zaddr += fetch_X();
	byte_t operand = fetch_byte(zaddr);
	set_A(operand);
	return 0;
}
int lda() {
	addr_t pc = fetch_PC();
	addr_t lower = fetch_byte(pc + 1); // lower first because little-endian
	addr_t upper = fetch_byte(pc + 2);
	upper = (upper << 4);
	addr_t addr = upper + lower;
	byte_t operand = fetch_byte(addr);
	set_A(operand);
	return 0;
}
int ldaax() {
	byte_t extra_cycles = 0;
	addr_t pc = fetch_PC();
	addr_t lower = fetch_byte(pc + 1); // lower first because little-endian
	addr_t upper = fetch_byte(pc + 2);
	byte_t pg_no = upper;
	upper = (upper << 8);
	addr_t addr = upper + lower;
	addr_t naddr = addr + fetch_X();
	byte_t npg_no = (naddr >> 8);
	if (npg_no != pg_no) {
		extra_cycles++;
	}
	byte_t operand = fetch_byte(naddr);
	set_A(operand);
	return extra_cycles;
}

int ldaay() {
	byte_t extra_cycles = 0;
	addr_t pc = fetch_PC();
	addr_t lower = fetch_byte(pc + 1); // lower first because little-endian
	addr_t upper = fetch_byte(pc + 2);
	byte_t pg_no = upper;
	upper = (upper << 8);
	addr_t addr = upper + lower;
	addr_t naddr = addr + fetch_X();
	byte_t npg_no = (naddr >> 8);
	if (npg_no != pg_no) {
		extra_cycles++;
	}
	byte_t operand = fetch_byte(naddr);
	set_A(operand);
	return extra_cycles;
}

int ldainx() {}
int ldainy() {}

int ldxi() {
	
}
int ldxz() {}
int ldxzy() {}
int ldx() {}
int ldxay() {}
int ldyi() {}
int ldyz() {}
int ldyzy() {}
int ldy() {}
int ldyay() {}
int lsra() {}
int lsrz() {}
int lsrzx() {}
int lsr() {}
int lsrax() {}
int nop() {}
int orai() {}
int oraz() {}
int orazx() {}
int ora() {}
int oraax() {}
int oraay() {}
int orainx() {}
int orainy() {}
int pha() {}
int php() {}
int pla() {}
int plp() {}
int rola() {}
int rolz() {}
int rolzx() {}
int rol() {}
int rolax() {}
int rora() {}
int rorz() {}
int rorzx() {}
int ror() {}
int rorax() {}
int rti() {}
int rts() {}
int sbci() {}
int sbcz() {}
int sbczx() {}
int sbca() {}
int sbcax() {}
int sbcay() {}
int sbcinx() {}
int sbciny() {}
int sec() {}
int sed() {}
int sei() {}
int staz() {
	addr_t pc = fetch_PC();
	addr_t addr = fetch_byte(pc + 1);
	set_byte(addr, fetch_A());
	return 0;
}
int stazx() {
	addr_t pc = fetch_PC();
	addr_t addr = fetch_byte(pc + 1);
	addr += fetch_X();
	set_byte(addr, fetch_A());
	return 0;
}
int sta() {
	addr_t pc = fetch_PC();
	addr_t lower = fetch_byte(pc + 1);
	addr_t upper = fetch_byte(pc + 2);
	upper = (upper << 8);
	addr_t addr = upper + lower;
	set_byte(addr, fetch_A());
	return 0;
}
int staax() {
	addr_t pc = fetch_PC();
	addr_t lower = fetch_byte(pc + 1);
	addr_t upper = fetch_byte(pc + 2);
	upper = (upper << 8);
	addr_t addr = upper + lower;
	addr += fetch_X();
	set_byte(addr, fetch_A());
	return 0;
}
int staay() {
	addr_t pc = fetch_PC();
	addr_t lower = fetch_byte(pc + 1);
	addr_t upper = fetch_byte(pc + 2);
	upper = (upper << 8);
	addr_t addr = upper + lower;
	addr += fetch_X();
	set_byte(addr, fetch_A());
	return 0;
}
int stainx() {
}
int stainy() {}
int stxz() {}
int stxzy() {}
int stx() {}
int styz() {}
int styzy() {}
int sty() {}
int tax() {}
int tay() {}
int tsx() {}
int txa() {}
int txs() {}
int tya() {}
int vac() {
	log_fatal("Invalid/Vacant Instruction; Exiting");
	exit(EXIT_FAILURE);
}

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
	inst_assign(0x39, 3, 4, andyx,"andyx");
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
	inst_assign(0xB4, 2, 4, ldyzy,"ldyzy");
	inst_assign(0xAC, 3, 4, ldy,"ldy");
	inst_assign(0xBC, 3, 4, ldyay,"ldyay");

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
	inst_assign(0x94, 2, 4, styzy,"styzy");
	inst_assign(0x8C, 3, 4, sty,"sty");

	inst_assign(0xaa, 1, 2, tax,"tax");
	inst_assign(0xa8, 1, 2, tay,"tay");
	inst_assign(0xba, 1, 2, tsx,"tsx");
	inst_assign(0x8a, 1, 2, txa,"txa");
	inst_assign(0x9a, 1, 2, txs,"txs");
	inst_assign(0x98, 1, 2, tya,"tya");
	/********* Vacant ************/
	inst_assign(0x02, 0, 0, vac,"vac");
	inst_assign(0x03, 0, 0, vac,"vac");
	inst_assign(0x04, 0, 0, vac,"vac");
	inst_assign(0x07, 0, 0, vac,"vac");
	inst_assign(0x0B, 0, 0, vac,"vac");
	inst_assign(0x0C, 0, 0, vac,"vac");
	inst_assign(0x0F, 0, 0, vac,"vac");
	inst_assign(0x12, 0, 0, vac,"vac");
	inst_assign(0x13, 0, 0, vac,"vac");
	inst_assign(0x14, 0, 0, vac,"vac");
	inst_assign(0x17, 0, 0, vac,"vac");
	inst_assign(0x1A, 0, 0, vac,"vac");
	inst_assign(0x1B, 0, 0, vac,"vac");
	inst_assign(0x1C, 0, 0, vac,"vac");
	inst_assign(0x1F, 0, 0, vac,"vac");
	inst_assign(0x42, 0, 0, vac,"vac");
	inst_assign(0x43, 0, 0, vac,"vac");
	inst_assign(0x44, 0, 0, vac,"vac");
	inst_assign(0x47, 0, 0, vac,"vac");
	inst_assign(0x4B, 0, 0, vac,"vac");
	inst_assign(0x4F, 0, 0, vac,"vac");
	inst_assign(0x52, 0, 0, vac,"vac");
	inst_assign(0x53, 0, 0, vac,"vac");
	inst_assign(0x54, 0, 0, vac,"vac");
	inst_assign(0x57, 0, 0, vac,"vac");
	inst_assign(0x5A, 0, 0, vac,"vac");
	inst_assign(0x5B, 0, 0, vac,"vac");
	inst_assign(0x5C, 0, 0, vac,"vac");
	inst_assign(0x5F, 0, 0, vac,"vac");
	inst_assign(0x80, 0, 0, vac,"vac");
	inst_assign(0x82, 0, 0, vac,"vac");
	inst_assign(0x83, 0, 0, vac,"vac");
	inst_assign(0x87, 0, 0, vac,"vac");
	inst_assign(0x89, 0, 0, vac,"vac");
	inst_assign(0x8B, 0, 0, vac,"vac");
	inst_assign(0x8F, 0, 0, vac,"vac");
	inst_assign(0x92, 0, 0, vac,"vac");
	inst_assign(0x93, 0, 0, vac,"vac");
	inst_assign(0x97, 0, 0, vac,"vac");
	inst_assign(0x9B, 0, 0, vac,"vac");
	inst_assign(0x9C, 0, 0, vac,"vac");
	inst_assign(0x9E, 0, 0, vac,"vac");
	inst_assign(0x9F, 0, 0, vac,"vac");
	inst_assign(0xC2, 0, 0, vac,"vac");
	inst_assign(0xC3, 0, 0, vac,"vac");
	inst_assign(0xC7, 0, 0, vac,"vac");
	inst_assign(0xCB, 0, 0, vac,"vac");
	inst_assign(0xCF, 0, 0, vac,"vac");
	inst_assign(0xD2, 0, 0, vac,"vac");
	inst_assign(0xD3, 0, 0, vac,"vac");
	inst_assign(0xD4, 0, 0, vac,"vac");
	inst_assign(0xD7, 0, 0, vac,"vac");
	inst_assign(0xDA, 0, 0, vac,"vac");
	inst_assign(0xDB, 0, 0, vac,"vac");
	inst_assign(0xDC, 0, 0, vac,"vac");
	inst_assign(0xDF, 0, 0, vac,"vac");
	inst_assign(0x22, 0, 0, vac,"vac");
	inst_assign(0x23, 0, 0, vac,"vac");
	inst_assign(0x27, 0, 0, vac,"vac");
	inst_assign(0x2B, 0, 0, vac,"vac");
	inst_assign(0x2F, 0, 0, vac,"vac");
	inst_assign(0x32, 0, 0, vac,"vac");
	inst_assign(0x33, 0, 0, vac,"vac");
	inst_assign(0x34, 0, 0, vac,"vac");
	inst_assign(0x37, 0, 0, vac,"vac");
	inst_assign(0x3A, 0, 0, vac,"vac");
	inst_assign(0x3B, 0, 0, vac,"vac");
	inst_assign(0x3C, 0, 0, vac,"vac");
	inst_assign(0x3F, 0, 0, vac,"vac");
	inst_assign(0x62, 0, 0, vac,"vac");
	inst_assign(0x63, 0, 0, vac,"vac");
	inst_assign(0x64, 0, 0, vac,"vac");
	inst_assign(0x67, 0, 0, vac,"vac");
	inst_assign(0x6B, 0, 0, vac,"vac");
	inst_assign(0x6F, 0, 0, vac,"vac");
	inst_assign(0x72, 0, 0, vac,"vac");
	inst_assign(0x73, 0, 0, vac,"vac");
	inst_assign(0x74, 0, 0, vac,"vac");
	inst_assign(0x77, 0, 0, vac,"vac");
	inst_assign(0x7A, 0, 0, vac,"vac");
	inst_assign(0x7B, 0, 0, vac,"vac");
	inst_assign(0x7C, 0, 0, vac,"vac");
	inst_assign(0x7F, 0, 0, vac,"vac");
	inst_assign(0xA3, 0, 0, vac,"vac");
	inst_assign(0xA7, 0, 0, vac,"vac");
	inst_assign(0xAB, 0, 0, vac,"vac");
	inst_assign(0xAF, 0, 0, vac,"vac");
	inst_assign(0xB2, 0, 0, vac,"vac");
	inst_assign(0xB3, 0, 0, vac,"vac");
	inst_assign(0xB7, 0, 0, vac,"vac");
	inst_assign(0xBB, 0, 0, vac,"vac");
	inst_assign(0xBF, 0, 0, vac,"vac");
	inst_assign(0xE2, 0, 0, vac,"vac");
	inst_assign(0xE3, 0, 0, vac,"vac");
	inst_assign(0xE7, 0, 0, vac,"vac");
	inst_assign(0xEB, 0, 0, vac,"vac");
	inst_assign(0xEF, 0, 0, vac,"vac");
	inst_assign(0xF2, 0, 0, vac,"vac");
	inst_assign(0xF3, 0, 0, vac,"vac");
	inst_assign(0xF4, 0, 0, vac,"vac");
	inst_assign(0xF7, 0, 0, vac,"vac");
	inst_assign(0xFA, 0, 0, vac,"vac");
	inst_assign(0xFB, 0, 0, vac,"vac");
	inst_assign(0xFC, 0, 0, vac,"vac");
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
	return ((inst_tbl[opcode])).exec();
}


void record_state(state_t *s) {
	s->A = fetch_A();
	s->X = fetch_X();
	s->Y = fetch_Y();
	s->S = fetch_S();
	s->P = fetch_P();
	s->PC = fetch_PC();
}

FILE *disas_fp = NULL;
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
	fprintf(disas_fp, "\tP:\t0x%x,%d\t\t0x%x,%d\n", s->P, s->P, fetch_P(), fetch_P());
	fprintf(disas_fp, "\tPC:\t0x%x\t\t0x%x\n", s->PC, fetch_PC());
}
