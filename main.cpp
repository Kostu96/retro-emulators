#include <cstring>
#include <memory>
#include <string>

#include <Windows.h>

#include "emulator_core.hpp"

template<typename T>
class DllLoader
{
private:
    HMODULE m_handle = nullptr;
    std::string m_libPath;
public:
    explicit DllLoader(const std::string& libPath) : m_libPath{ libPath } {}
    ~DllLoader() = default;

    bool openLib()
    {
        m_handle = LoadLibrary(m_libPath.c_str());
        return m_handle;
    }

    bool closeLib()
    {
        return FreeLibrary(m_handle) != 0;
    }

    std::shared_ptr<T> getInstance()
    {
        using allocatorClass = EmulatorCore * (*)();
        using deleterClass = void(*)(EmulatorCore*);

        auto allocator = reinterpret_cast<allocatorClass>(GetProcAddress(m_handle, "allocator"));
        auto deleter = reinterpret_cast<deleterClass>(GetProcAddress(m_handle, "deleter"));

        return std::shared_ptr<T>{ allocator(), [deleter](EmulatorCore* ptr) { deleter(ptr); } };
    }
};

int main(int argc, char* argv[])
{
    if (argc == 2)
    {
        DllLoader<EmulatorCore> loader{ argv[1] };
        loader.openLib();
        auto core = loader.getInstance();

        core->fn();
    }

    return 0;
}
