#include <stdio.h>
#include "cpu.h"
#include "except.h"
#include "log.h"
#include "mspace.h"

int main(int argc, char *argv[]) {
	if (argc < 2) {
		fprintf(stderr, "Too few arguments\n");
		return 1;
	}
	except_tbl_init();
	inst_tbl_init();
	disassembler_init();
	load_cartridge(argv[1]);
	
	byte_t size = 0;
	byte_t cycles = 0;
	char *name;
	byte_t opcode;
	state_t state;
	for (addr_t pc = fetch_PC(); pc < (CARMEM_END - 1); ) {
		record_state(&state);
		opcode = fetch_byte(pc);
		size = inst_bytes(opcode);
		cycles = inst_cycles(opcode);
		name = inst_name(opcode);
		inst_exec(opcode);
		pc = fetch_PC();
		pc += size;
		set_PC(pc);
		disassemble(opcode, &state);
	}
}
