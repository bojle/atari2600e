#ifndef CPU_H
#define CPU_H

#include "mspace.h"

void inst_tbl_init();

char *inst_name(byte_t opcode);
byte_t inst_bytes(byte_t opcode);
byte_t inst_cycles(byte_t opcode);
byte_t inst_exec(byte_t opcode);

#endif
