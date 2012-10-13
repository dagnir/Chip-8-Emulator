#include "chip8disasm.h"

int main(int argc, char* argv[]) {

	chip8disasm("ibm.bin", "ibm.bin.disasm");
	return 0;
}