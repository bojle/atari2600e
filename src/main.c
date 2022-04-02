#include <stdio.h>
#include "cpu.h"
#include "except.h"
#include "log.h"
#include "mspace.h"

#define ENABLE_DISASSEMBLER 1

static int total_cycles = 0;

void emu_init(int argc, char *argv[]) {
	except_tbl_init();
	inst_tbl_init();
	disassembler_init();
	load_cartridge(argv[1]);
	/* Get the CPU runnin' */
	cpu_set_status(1);
}

#if (ENABLE_DISASSEMBLER == 1)
static state_t state;
#endif

void run_cpu() {
	_Bool cpu_status = cpu_fetch_status();
	if (!cpu_status) {
		return;
	}

#if (ENABLE_DISASSEMBLER == 1)
	record_state(&state);
#endif

	/* Execute an Instruction */
	addr_t pc = fetch_PC();
	byte_t opcode = fetch_byte(pc);
	total_cycles += inst_cycles(opcode);
	total_cycles += inst_exec(opcode);
	pc = fetch_PC();
	pc += inst_bytes(opcode);
	set_PC(pc);

#if (ENABLE_DISASSEMBLER == 1)
	disassemble(opcode, &state);
#endif
}

int main(int argc, char *argv[]) {
	if (argc < 2) {
		fprintf(stderr, "Too few arguments\n");
		return 1;
	}
	emu_init(argc, argv);
	while (1) {
		run_cpu();
	}
}
