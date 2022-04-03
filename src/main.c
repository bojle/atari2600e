#include <stdio.h>
#include "cpu.h"
#include "except.h"
#include "log.h"
#include "mspace.h"
#include "tia.h"

void emu_init(int argc, char *argv[]) {
	except_tbl_init();
	inst_tbl_init();
#ifdef ENABLE_DISASSEMBLER
	disassembler_init();
#endif
	load_cartridge(argv[1]);
	tia_init();
	/* Get the CPU runnin' */
	cpu_set_status(1);
}

#ifdef ENABLE_DISASSEMBLER
static state_t state;
#endif

int run_cpu() {
	_Bool cpu_status = cpu_fetch_status();
	if (!cpu_status) {
		return 0;
	}

#ifdef ENABLE_DISASSEMBLER
	record_state(&state);
#endif

	/* Execute an Instruction */
	addr_t pc = fetch_PC();
	byte_t cycles = 0;
	byte_t opcode = fetch_byte(pc);
	cycles += inst_cycles(opcode);
	cycles += inst_exec(opcode);
	pc = fetch_PC();
	pc += inst_bytes(opcode);
	set_PC(pc);

#ifdef ENABLE_DISASSEMBLER
	disassemble(opcode, &state);
#endif
	cnt_machine_cycles(cycles);
	return cycles;
}

int tia_clocks = 0;

int run_tia(int machine_cycles) {
	for (int i = 0; i < (machine_cycles * 3); ++i) {
		tia_exec();
	}
	return (machine_cycles * 3);
}

int main(int argc, char *argv[]) {
	if (argc < 2) {
		fprintf(stderr, "Too few arguments\n");
		return 1;
	}
	emu_init(argc, argv);
	byte_t machine_cycles = 0;
	byte_t color_clocks = 0;
	addr_t pc = fetch_PC();
	while (pc < CARMEM_END - 1) {
		machine_cycles = run_cpu();
		color_clocks = run_tia(machine_cycles);
		pc = fetch_PC();
	}
}
