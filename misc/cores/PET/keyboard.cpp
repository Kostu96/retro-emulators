#include "keyboard.hpp"

//#include "GLFW/glfw3.h"

#include <iostream>

u8 Keyboard::getKey() const
{
    if (m_row > 9)
        std::cerr << "!!!!!!!\n";

    return m_rows[m_row] ^ 0xFF;
}

//void Keyboard::glfwKeyCallback(GLFWwindow* window, int key, int /*scancode*/, int action, int /*mods*/)
//{
//    /*Keyboard* kbd = reinterpret_cast<Keyboard*>(glfwGetWindowUserPointer(window));
//
//    switch (key)
//    {
//    case GLFW_KEY_SPACE: kbd->set(0x92, action); break;
//    case GLFW_KEY_A:     kbd->set(0x40, action); break;
//    }*/
//}

void Keyboard::set(Key key, bool value)
{
    if (value)
        m_rows[key.nibbles.row] |= 1 << key.nibbles.col;
    else
        m_rows[key.nibbles.row] &= ~(1 << key.nibbles.col);
}
