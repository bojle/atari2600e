#include "cpu.h"
#include "log.h"

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

typedef void (*inst_fptr) (void);

/* Total Instructions */
#define INSTN 256 	/* 2^8 */

typedef struct inst_t {
	int bytes;
	int cycles;
	inst_fptr exec;
} inst_t;

static inst_t inst_tbl[INSTN];

#define inst_assign(opcode, nbytes, ncycles, execfn) \
	inst_tbl[opcode].bytes = nbytes; inst_tbl[opcode].cycles = ncycles; inst_tbl[opcode].exec = execfn

#if 0
enum inst_opcodes {
	ADCI 	= 0x69,   						// Add Memory to Accumulator with Carry
	ADCZ 	= 0x65,					// Zero Page
	ADCIDZ 	= 0x75,
	ADC		= 0x60,
	ADCAX 	= 0x7d,
	ADCAY 	= 0x79,
	ADCINX 	= 0x61,
	ADCINY 	= 0x71,
	
	AND   						// "AND" Memory with Accumulator
	ASL   						// Shift Left One Bit (Memory or Accumulator)
	BCC   						// Branch on Carry Clear
	BCS   						// Branch on Carry Set
	BEQ   						// Branch on Result Zero
	BIT   						// Test Bits in Memory with Accumulator
	BMI   						// Branch on Result Minus
	BNE   						// Branch on Result not Zero
	BPL   						// Branch on Result Plus
	BRK   						// Force Break
	BVC   						// Branch on Overflow Clear
	BVS   						// Branch on Overflow Set
	      						//                                                            
	CLC   						// Clear Carry Flag
	CLD   						// Clear Decimal Mode
	CLI   						// Clear interrupt Disable Bit
	CLV   						// Clear Overflow Flag
	CMP   						// Compare Memory and Accumulator
	CPX   						// Compare Memory and Index X
	CPY   						// Compare Memory and Index Y
	      						//                                                            
	DEC   						// Decrement Memory by One
	DEX   						// Decrement Index X by One
	DEY   						// Decrement Index Y by One
	      						//                                                            
	EOR   						// "Exclusive-Or" Memory with Accumulator
	      						//                                                            
	INC   						// Increment Memory by One
	INX   						// Increment Index X by One
	INY   						// Increment Index Y by One
	      						//                                                            
	JMP   						// Jump to New Location
	JSR   						// Jump to New Location Saving Return Address                 
	      						//                                                            
	LDA   						// Load Accumulator with Memory                               
	LDX   						// Load Index X with Memory                                   
	LDY   						// Load Index Y with Memory                                   
	LSR   						// Shift Right One Bit (Memory or Accumulator)                
	      						//                                                            
	NOP   						// No Operation                                               
	      						//                                                            
	ORA   						// "OR" Memory with Accumulator                               
	      						//                                                            
	PHA   						// Push Accumulator on Stack                                  
	PHP   						// Push Processor Status on Stack                             
	PLA   						// Pull Accumulator from Stack                                
	PLP   						// Pull Processor Status from Stack                           
	      						//                                                            
	ROL   						// Rotate One Bit Left (Memory or Accumulator)                
	ROR   						// Rotate One Bit Right (Memory or Accumulator)               
	RTI   						// Return from Interrupt                                      
	RTS   						// Return from Subroutine                                     
	      						//                                                            
	SBC   						// Subtract Memory from Accumulator with Borrow               
	SEC   						// Set Carry Flag                                             
	SED   						// Set Decimal Mode                                           
	SEI   						// Set Interrupt Disable Status                               
	STA   						// Store Accumulator in Memory                                
	STX   						// Store Index X in Memory                                    
	STY   						// Store Index Y in Memory                                    
	      						//                                                            
	TAX   						// Transfer Accumulator to Index X                            
	TAY   						// Transfer Accumulator to Index Y                            
	TSX   						// Transfer Stack Pointer to Index X                          
	TXA   						// Transfer Index X to Accumulator                            
	TXS   						// Transfer Index X to Stack Pointer                          
	TYA   						// Transfer Index Y to Accumulator                            
};
#endif

void inst_tbl_init() {
	inst_assign(0x69, 2, 2, adci);
	inst_assign(0x65, 2, 3, adcz);
	inst_assign(0x75, 2, 4, adczx);
	inst_assign(0x60, 3, 4, adc);
	inst_assign(0x7D, 3, 4, adcax);
	inst_assign(0x79, 3, 4, adcay);
	inst_assign(0x61, 2, 6, adcinx);
	inst_assign(0x71, 2, 5, adciny);

	inst_assign(0x29, 2, 2, andi);
	inst_assign(0x25, 2, 3, andz);
	inst_assign(0x35, 2, 4, andzx);
	inst_assign(0x2D, 3, 4, and);
	inst_assign(0x3D, 3, 4, andax);
	inst_assign(0x39, 3, 4, andyx);
	inst_assign(0x21, 2, 6, andinx);
	inst_assign(0x31, 2, 5, andiny);
	
	inst_assign(0x0A, 1, 2, asla);
	inst_assign(0x06, 2, 5, aslz);
	inst_assign(0x16, 2, 6, aslzx);
	inst_assign(0x0E, 3, 6, asl);
	inst_assign(0x1E, 3, 7, aslax);

	/************* B ************/
	inst_assign(0x90, 2, 2, bcc);

	inst_assign(0xb0, 2, 2, bcs);

	inst_assign(0xf0, 2, 2, beq);

	inst_assign(0x24, 2, 3, bitz);
	inst_assign(0x2C, 3, 4, bit);

	inst_assign(0x30, 2, 2, bmi);

	inst_assign(0xd0, 2, 2, bne);

	inst_assign(0x10, 2, 2, bpl);

	inst_assign(0x00, 1, 7, brk);

	inst_assign(0x50, 2, 2, bvc);
	inst_assign(0x00, 1, 7, bvs);


	/*********** C *************/
	inst_assign(0x18, 1, 2, clc);
	inst_assign(0xd8, 1, 2, cld);
	inst_assign(0x58, 1, 2, cli);
	inst_assign(0xb8, 1, 2, clv);

	inst_assign(0xC9, 2, 2, cmpi);
	inst_assign(0xC5, 2, 3, cmpz);
	inst_assign(0xD5, 2, 4, cmpzx);
	inst_assign(0xCD, 3, 4, cmp);
	inst_assign(0xDD, 3, 4, cmpax);
	inst_assign(0xD9, 3, 4, cmpay);
	inst_assign(0xC1, 2, 6, cmpinx);
	inst_assign(0xD1, 2, 5, cmpiny);

	inst_assign(0xE0, 2, 2, cpxi);
	inst_assign(0xE4, 2, 3, cpxz);
	inst_assign(0xEC, 3, 4, cpx);

	inst_assign(0xC0, 2, 2, cpyi);
	inst_assign(0xC4, 2, 3, cpyz);
	inst_assign(0xCC, 3, 4, cpy);

	/*********** D **************/
	inst_assign(0xC6, 2, 5, decz);
	inst_assign(0xD6, 2, 6, deczx);
	inst_assign(0xCE, 3, 6, dec);
	inst_assign(0xDE, 3, 7, decax);

	inst_assign(0xca, 1, 2, dex);
	inst_assign(0x88, 1, 2, dey);

	inst_assign(0x49, 2, 2, eori);
	inst_assign(0x45, 2, 3, eorz);
	inst_assign(0x55, 2, 4, eorzx);
	inst_assign(0x40, 3, 4, eor);
	inst_assign(0x5D, 3, 4, eorax);
	inst_assign(0x59, 3, 4, eoray);
	inst_assign(0x41, 2, 6, eorinx);
	inst_assign(0x51, 2, 5, eoriny);

	inst_assign(0xE6, 2, 5, incz);
	inst_assign(0xF6, 2, 6, inczx);
	inst_assign(0xEE, 3, 6, inc);
	inst_assign(0xFE, 3, 7, incax);

	inst_assign(0xe8, 1, 2, inx);

	inst_assign(0xc8, 1, 2, iny);

	inst_assign(0x4C, 3, 3, jmp);
	inst_assign(0x6C, 3, 5, jmpin);

	inst_assign(0x20, 3, 6, jsr);

	inst_assign(0xA9, 2, 2, ldai);
	inst_assign(0xA5, 2, 3, ldaz);
	inst_assign(0xB5, 2, 4, ldazx);
	inst_assign(0xAD, 3, 4, lda);
	inst_assign(0xBD, 3, 4, ldaax);
	inst_assign(0xB9, 3, 4, ldaay);
	inst_assign(0xA1, 2, 6, ldainx);
	inst_assign(0xB1, 2, 5, ldainy);

	inst_assign(0xA2, 2, 2, ldxi);
	inst_assign(0xA6, 2, 3, ldxz);
	inst_assign(0xB6, 2, 4, ldxzy);
	inst_assign(0xAE, 3, 4, ldx);
	inst_assign(0xBE, 3, 4, ldxay);

	inst_assign(0xA0, 2, 2, ldyi);
	inst_assign(0xA4, 2, 3, ldyz);
	inst_assign(0xB4, 2, 4, ldyzy);
	inst_assign(0xAC, 3, 4, ldy);
	inst_assign(0xBC, 3, 4, ldyay);

	inst_assign(0x4A, 1, 2, lsra);
	inst_assign(0x46, 2, 5, lsrz);
	inst_assign(0x56, 2, 6, lsrzx);
	inst_assign(0x4E, 3, 6, lsr);
	inst_assign(0x5E, 3, 7, lsrax);

	inst_assign(0xEA, 1, 2, nop);

	inst_assign(0x09, 2, 2, orai);
	inst_assign(0x05, 2, 3, oraz);
	inst_assign(0x15, 2, 4, orazx);
	inst_assign(0x0D, 3, 4, ora);
	inst_assign(0x10, 3, 4, oraax);
	inst_assign(0x19, 3, 4, oraay);
	inst_assign(0x01, 2, 6, orainx);
	inst_assign(0x11, 2, 5, orainy);

	inst_assign(0x48, 1, 3, pha);
	inst_assign(0x08, 1, 3, php);
	inst_assign(0x68, 1, 4, pla);

	inst_assign(0x28, 1, 4, plp);

	inst_assign(0x2A, 1, 2, rola);
	inst_assign(0x26, 2, 5, rolz);
	inst_assign(0x36, 2, 6, rolzx);
	inst_assign(0x2E, 3, 6, rol);
	inst_assign(0x3E, 3, 7, rolax);

	inst_assign(0x6A, 1, 2, rora);
	inst_assign(0x66, 2, 5, rorz);
	inst_assign(0x76, 2, 6, rorzx);
	inst_assign(0x6E, 3, 6, ror);
	inst_assign(0x7E, 3, 7, rorax);


	inst_assign(0x4D, 1, 6, rti);
	inst_assign(0x60, 1, 6, rts);

	inst_assign(0xE9, 2, 2, sbci);
	inst_assign(0xE5, 2, 3, sbcz);
	inst_assign(0xF5, 2, 4, sbczx);
	inst_assign(0xED, 3, 4, sbca);
	inst_assign(0xFD, 3, 4, sbcax);
	inst_assign(0xF9, 3, 4, sbcay);
	inst_assign(0xE1, 2, 6, sbcinx);
	inst_assign(0xF1, 2, 5, sbciny);

	inst_assign(0x38, 1, 2, sec);
	inst_assign(0xf8, 1, 2, sed);
	inst_assign(0x78, 1, 2, sei);

	inst_assign(0x85, 2, 3, staz);
	inst_assign(0x95, 2, 4, stazx);
	inst_assign(0x8D, 3, 4, sta);
	inst_assign(0x9D, 3, 5, staax);
	inst_assign(0x99, 3, 5, staay);
	inst_assign(0x81, 2, 6, stainx);
	inst_assign(0x91, 2, 6, stainy);

	inst_assign(0x86, 2, 3, stxz);
	inst_assign(0x96, 2, 4, stxzy);
	inst_assign(0x8E, 3, 4, stx);

	inst_assign(0x84, 2, 3, styz);
	inst_assign(0x94, 2, 4, styzy);
	inst_assign(0x8C, 3, 4, sty);

	inst_assign(0xaa, 1, 2, tax);
	inst_assign(0xa8, 1, 2, tay);
	inst_assign(0xba, 1, 2, tsx);
	inst_assign(0x8a, 1, 2, txa);
	inst_assign(0x9a, 1, 2, txs);
	inst_assign(0x98, 1, 2, tya);
	log_trace("inst_tbl_init(): Initialized Instruction Table");
}

void adci() {}
void adcz() {}
void adczx() {}
void adc() {}
void adcax() {}
void adcay() {}
void adcinx() {}
void adciny() {}
void andi() {}
void andz() {}
void andzx() {}
void and() {}
void andax() {}
void andyx() {}
void andinx() {}
void andiny() {}
void asla() {}
void aslz() {}
void aslzx() {}
void asl() {}
void aslax() {}
void bcc() {}
void bcs() {}
void beq() {}
void bitz() {}
void bit() {}
void bmi() {}
void bne() {}
void bpl() {}
void brk() {}
void bvc() {}
void bvs() {}
void clc() {}
void cld() {}
void cli() {}
void clv() {}
void cmpi() {}
void cmpz() {}
void cmpzx() {}
void cmp() {}
void cmpax() {}
void cmpay() {}
void cmpinx() {}
void cmpiny() {}
void cpxi() {}
void cpxz() {}
void cpx() {}
void cpyi() {}
void cpyz() {}
void cpy() {}
void decz() {}
void deczx() {}
void dec() {}
void decax() {}
void dex() {}
void dey() {}
void eori() {}
void eorz() {}
void eorzx() {}
void eor() {}
void eorax() {}
void eoray() {}
void eorinx() {}
void eoriny() {}
void incz() {}
void inczx() {}
void inc() {}
void incax() {}
void inx() {}
void iny() {}
void jmp() {}
void jmpin() {}
void jsr() {}
void ldai() {}
void ldaz() {}
void ldazx() {}
void lda() {}
void ldaax() {}
void ldaay() {}
void ldainx() {}
void ldainy() {}
void ldxi() {}
void ldxz() {}
void ldxzy() {}
void ldx() {}
void ldxay() {}
void ldyi() {}
void ldyz() {}
void ldyzy() {}
void ldy() {}
void ldyay() {}
void lsra() {}
void lsrz() {}
void lsrzx() {}
void lsr() {}
void lsrax() {}
void nop() {}
void orai() {}
void oraz() {}
void orazx() {}
void ora() {}
void oraax() {}
void oraay() {}
void orainx() {}
void orainy() {}
void pha() {}
void php() {}
void pla() {}
void plp() {}
void rola() {}
void rolz() {}
void rolzx() {}
void rol() {}
void rolax() {}
void rora() {}
void rorz() {}
void rorzx() {}
void ror() {}
void rorax() {}
void rti() {}
void rts() {}
void sbci() {}
void sbcz() {}
void sbczx() {}
void sbca() {}
void sbcax() {}
void sbcay() {}
void sbcinx() {}
void sbciny() {}
void sec() {}
void sed() {}
void sei() {}
void staz() {}
void stazx() {}
void sta() {}
void staax() {}
void staay() {}
void stainx() {}
void stainy() {}
void stxz() {}
void stxzy() {}
void stx() {}
void styz() {}
void styzy() {}
void sty() {}
void tax() {}
void tay() {}
void tsx() {}
void txa() {}
void txs() {}
void tya() {}
