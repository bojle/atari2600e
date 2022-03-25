#ifndef MSPACE_H
#define MSPACE_H

#include <stdint.h>

/* Width of the data bus */
typedef uint8_t byte_t;
/* Width of the address bus */
typedef uint16_t addr_t;

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

enum {
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

void set_S(byte_t b);
byte_t fetch_S();

void set_P(byte_t b);
byte_t fetch_P();

void load_cartridge(char *filename);

#endif