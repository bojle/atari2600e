#ifndef MSPACE_H
#define MSPACE_H

#include <stdint.h>

/* Width of the data bus */
typedef uint8_t byte_t;
/* Width of the address bus */
typedef uint16_t addr_t;

/* Used by cycle counters */
typedef uint32_t cycles_t;
/* Signed cycles_t */
typedef int32_t scycles_t;

/* Data bus: 8-bit, address bus: 16-bit
 * Addresses addressed by a26: 0x0000 - 0xffff 
 * 0000-002C  TIA Write
 * 0000-000D  TIA Read (sometimes mirrored at 0030-003D)
 * 0080-00FF  PIA RAM (128 bytes)
 * 0280-0297  PIA Ports and Timer
 * F000-FFFF  Cartridge Memory (4 Kbytes area)
 */

/* Cartridge Memory Boundaries */
#define CARMEM_START 0xf000
#define CARMEM_END 0xffff

#define RAM_START 0x0180
#define RAM_END 0x01ff

enum tia_write_addresses {
	VSYNC  = 0x00,
	VBLANK = 0x01, 
	WSYNC  = 0x02, 
	RSYNC  = 0x03, 
	NUSIZ0 = 0x04, 
	NUSIZ1 = 0x05, 
	COLUP0 = 0x06, 
	COLUP1 = 0x07, 
	COLUPF = 0x08, 
	COLUBK = 0x09, 
	CTRLPF = 0x0A, 
	REFP0  = 0x0B, 
	REFP1  = 0x0C, 
	PF0    = 0x0D, 
	PF1    = 0x0E, 
	PF2    = 0x0F, 
	RESP0  = 0x10, 
	RESP1  = 0x11, 
	RESM0  = 0x12, 
	RESM1  = 0x13, 
	RESBL  = 0x14, 
	AUDC0  = 0x15, 
	AUDC1  = 0x16, 
	AUDF0  = 0x17, 
	AUDF1  = 0x18, 
	AUDV0  = 0x19, 
	AUDV1  = 0x1A, 
	GRP0   = 0x1B, 
	GRP1   = 0x1C, 
	ENAM0  = 0x1D, 
	ENAM1  = 0x1E, 
	ENABL  = 0x1F, 
	HMP0   = 0x20, 
	HMP1   = 0x21, 
	HMM0   = 0x22, 
	HMM1   = 0x23, 
	HMBL   = 0x24, 
	VDELP0 = 0x25, 
	VDELP1 = 0x26, 
	VDELBL = 0x27, 
	RESMP0 = 0x28, 
	RESMP1 = 0x29, 
	HMOVE  = 0x2A, 
	HMCLR  = 0x2B, 
	CXCLR  = 0x2C 
};

enum tia_read_addresses {
	CXM0P  = 0x00,   
	CXM1P  = 0x01,   
	CXP0FB = 0x02,   
	CXP1FB = 0x03,   
	CXM0FB = 0x04,   
	CXM1FB = 0x05,   
	CXBLPF = 0x06,   
	CXPPMM = 0x07,   
	INPT0  = 0x08,   
	INPT1  = 0x09,   
	INPT2  = 0x0A,   
	INPT3  = 0x0B,   
	INPT4  = 0x0C,   
	INPT5  = 0x0D   
};

enum pia_addresses {
	SWCHA 	= 0x0280, // Port A; input or output (read or write)
	SWACNT 	= 0x0281, // Port A DDR, 0= input, 1=output
	SWCHB 	= 0x0282, // Port B; console switches (read only)
	SWBCNT 	= 0x0283, // Port B DDR (hardwired as input)
	INTIM 	= 0x0284, // Timer output (read only)
	TIM1T 	= 0x0294, // set 1 clock interval (838 nsec/interval)
	TIM8T 	= 0x0295, // set 8 clock interval (6.7 usec/interval)
	TIM64T 	= 0x0296, // set 64 clock interval (53.6 usec/interval)
	T1024T 	= 0x0297  // set 1024 clock interval (858.2 usec/interval)
};


enum status_t {
	STATUS_C = 1,         /* Carry         (0=No Carry, 1=Carry)                      */
	STATUS_Z = 2,         /* Zero          (0=Nonzero, 1=Zero)                        */
	STATUS_I = 4,         /* IRQ Disable   (0=IRQ Enable, 1=IRQ Disable)              */
	STATUS_D = 8,         /* Decimal Mode  (0=Normal, 1=BCD Mode for ADC/SBC opcodes) */
	STATUS_B = 16,        /* Break Flag    (0=IRQ/NMI, 1=RESET or BRK/PHP opcode)     */
	STATUS_V = 64,        /* Overflow      (0=No Overflow, 1=Overflow)                */
	STATUS_N = 128        /* Negative/Sign (0=Positive, 1=Negative)                   */
};

/******************************************
 * 				Functions                 *
 ******************************************/

/* Return the byte at addr from mspace[] */
byte_t fetch_byte(addr_t addr);
/* Set addr to b in mspace[] */
void set_byte(addr_t addr, byte_t b);

void set_PC(addr_t addr);
addr_t fetch_PC();

void set_A(byte_t b);
byte_t fetch_A();

void set_X(byte_t b);
byte_t fetch_X();

void set_Y(byte_t b);
byte_t fetch_Y();

void set_S(addr_t b);
addr_t fetch_S();

void set_P(byte_t b);
byte_t fetch_P();

void set_STATUS(enum status_t st);
byte_t fetch_STATUS(enum status_t st);
void clear_STATUS(enum status_t st);

void stack_push(byte_t b);
byte_t stack_pop();
byte_t stack_top();

void load_cartridge(char *filename);

int p2(int n);

#define set_bit(reg, n) (reg |= p2(n))
#define clear_bit(reg, n) (reg &= (~p2(n)))

#endif
