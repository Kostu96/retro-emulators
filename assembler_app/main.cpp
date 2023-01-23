#include "asm4040.hpp"

#include <iostream>
#include <fstream>

int main(int argc, char* argv[])
{
	if (argc < 3) return -1;

	if (std::strcmp(argv[1], "-d") == 0)
	{

	}
	else
	{
		std::ifstream file{ argv[2] };
		file.ignore(std::numeric_limits<std::streamsize>::max());
		auto size = file.gcount();
		char* buffer = new char[size + 1];
		file.seekg(0);
		file.read(buffer, size);
		buffer[size] = '\0';
		file.close();

		std::vector<u8> output;

		if (std::strcmp(argv[1], "40xx") == 0)
			ASM40xx::assemble(buffer, output);

		delete[] buffer;
	}
	
	return 0;
}
