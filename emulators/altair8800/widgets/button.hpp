#pragma once
#include <functional>
#include <string>

class Button
{
public:
    explicit Button(const char* text = "") :
        m_text{ text } {}

    void setCallback(std::function<void()> callback) { m_callback = callback; }

    void render()
    {

    }
private:
    std::string m_text;
    std::function<void()> m_callback;
};
