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
	for (addr_t pc = fetch_PC(); pc < CARMEM_END; ) {
		byte_t upper = fetch_byte(pc++);
		byte_t lower = fetch_byte(pc++);
		set_PC(pc);
	}
}
