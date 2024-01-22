#version 450

layout(location = 0) in ivec2 a_Position;
//layout(location = 1) in uvec4 a_Color;
layout(location = 1) in vec4 a_Color;

out vec4 v_Color;

void main()
{
    float xpos = (float(a_Position.x) / 512) - 1.0;
	float ypos = 1.0 - (float(a_Position.y) / 256);

    gl_Position = vec4(xpos, ypos, 0.0, 1.0);
	
	//v_Color = vec4(float(a_Color.r) / 255,
	//               float(a_Color.g) / 255,
	//			   float(a_Color.b) / 255,
	//			   float(a_Color.a) / 255);
	v_Color = a_Color;
}
