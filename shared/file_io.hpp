#pragma once
#include <stddef.h>

bool readFile(const char* filename, char* data, size_t& size, bool binary = false);
bool writeFile(const char* filename, const char* data, size_t size, bool binary = false);
