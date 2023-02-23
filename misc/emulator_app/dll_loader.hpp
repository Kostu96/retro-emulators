#pragma once
#include <memory>
#include <string>
#include <Windows.h>

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

    std::shared_ptr<T> createInstance()
    {
        using allocatorClass = T* (*)();
        using deleterClass = void(*)(T*);

        auto allocator = reinterpret_cast<allocatorClass>(GetProcAddress(m_handle, "allocator"));
        auto deleter = reinterpret_cast<deleterClass>(GetProcAddress(m_handle, "deleter"));

        return std::shared_ptr<T>{ allocator(), [deleter](T* ptr) { deleter(ptr); } };
    }
};
