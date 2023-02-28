#pragma once
#include <string>

class Label
{
public:
    explicit Label(const char* text = "") :
        m_text{ text }
    {}
private:
    std::string m_text;
};
