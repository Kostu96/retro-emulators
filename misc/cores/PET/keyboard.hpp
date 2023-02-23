#pragma once
#include "../type_aliases.hpp"

struct GLFWwindow;

class Keyboard
{
public:
	Keyboard() = default;

	u8 getKey() const;
	u8 getRow() const { return m_row; }
	void setRow(u8 row) { m_row = row; }

	static void glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
private:
	union Key
	{
		struct {
			u8 col : 4;
			u8 row : 4;
		} nibbles;
		u8 byte;

		Key(u8 key) : byte{ key } {}
	};

	void set(Key key, bool value);

	u8 m_row = 0;
	u8 m_rows[10]{};
};
