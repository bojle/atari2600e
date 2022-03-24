#include <stdio.h>
#include <stdint.h>

#ifndef DEBUG_MODE
#define DEBUG_MODE 1
#endif

int main() {
	uint16_t inst;
	FILE *fp = fopen("clock.bin", "r");
	uint8_t opcode;
	while (fread(&inst, sizeof(inst), 1, fp) >= 1) {
		opcode = (inst >> 8);
		if (opcode == 0x69) {
			printf("ADCI (%x) : %x\n", opcode, inst);
		}
		else if (opcode == 0xae) {
			printf("ldx (%x) : %x\n", opcode, inst);
		}
		else if (opcode == 0x71) {
			printf("adc im y (%x) : %x\n", opcode, inst);
		}
		else 
			printf("%x\n", inst);
	}
#if 0
	inst = 0xae86;
	opcode = (inst >> 8);
	printf("%x: %x", opcode, inst);
#endif

}
