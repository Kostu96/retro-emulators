#include "shared/source/devices/cpu40xx/asm40xx.hpp"
#include "shared/source/devices/cpu8008/asm8008.hpp"

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
		std::vector<ErrorMessage> errors;

		if (std::strcmp(argv[1], "40xx") == 0)
			ASM40xx::assemble(buffer, output, errors);
		else if(std::strcmp(argv[1], "8008") == 0)
			ASM8008::assemble(buffer, output, errors);

		delete[] buffer;

		for (auto& error : errors)
			std::cerr << "Error at line " << error.line << ": " << error.message << '\n';
	}
	
	return 0;
}
