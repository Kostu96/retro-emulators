#include "../include/emu_common/file_io.hpp"

#include <fstream>

bool readFile(const char* filename, char* data, size_t& size, bool binary)
{
    std::ifstream fin(filename, binary ? std::ios::binary : std::ios::in);
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
    return true;
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
