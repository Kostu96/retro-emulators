#include "utils/file_io.hpp"

#include <fstream>
#include <limits>

std::string readFile(const char* filename)
{
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if (!file)
        throw std::runtime_error("Failed to open file");

    auto size = file.tellg();
    std::string buffer(size, '\0');
    file.seekg(0);
    file.read(buffer.data(), size);
    file.close();
    return buffer;
}

bool readFileInto(const char* filename, char* data, size_t& size)
{
    std::ifstream fin(filename, std::ios::binary);
    if (!fin.is_open())
        return false;

    bool retVal;
    if (data) {
        retVal = (bool)fin.read(data, size);
    }
    else {
        fin.ignore(std::numeric_limits<std::streamsize>::max());
        retVal = true;
    }

    size = fin.gcount();
    fin.close();
    return retVal;
}

bool writeFile(const char* filename, const char* data, size_t size, bool binary)
{
    std::ofstream fout(filename, binary ? std::ios::binary : std::ios::out);
    if (!fout.is_open())
        return false;

    bool retVal = (bool)fout.write(data, size);

    fout.close();
    return retVal;
}
