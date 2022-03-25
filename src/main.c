#include <stdio.h>
#include "cpu.h"
#include "except.h"
#include "log.h"
#include "mspace.h"

int main() {
	except_tbl_init();
	inst_tbl_init();
	load_cartridge("./bfr_dawn/thin");
	set_PC(CARMEM_START);
	
	byte_t size = 0;
	byte_t cycles = 0;
	for (addr_t pc = fetch_PC(); pc < CARMEM_END; ) {
		byte_t opcode = fetch_byte(pc);
		exec(opcode, &size, &cycles);
		printf("Opcode: %x\tSize: %d\tCycles: %d\n", opcode, size, cycles);
		pc += size;
		set_PC(pc);
	}
}
