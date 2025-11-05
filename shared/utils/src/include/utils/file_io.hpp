#pragma once
#include <stddef.h>
#include <string>

std::string readFile(const char* filename);
bool readFileInto(const char* filename, char* data, size_t& size);
bool writeFile(const char* filename, const char* data, size_t size, bool binary = false);
