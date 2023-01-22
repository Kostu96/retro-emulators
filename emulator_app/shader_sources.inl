const char* pointVertSource =
R"(
#version 450 core

layout(location = 0) in uvec2 a_Position;
layout(location = 1) in vec4 a_Color;

layout(location = 0) out vec4 v_Color;

void main()
{
    float xpos = float(a_Position.x) / (64.0 * 0.5) - 1.0;
	float ypos = -(float(a_Position.y) / (32.0 * 0.5) - 1.0);

    v_Color = a_Color;
    gl_Position = vec4(xpos, ypos, 0.0, 1.0);
}
)";

const char* pointFragSource =
R"(
#version 450 core

layout(location = 0) in vec4 v_Color;

layout(location = 0) out vec4 fragColor;

void main()
{
    fragColor = v_Color;
}
)";

const char* textureVertSource =
R"(
#version 450 core

layout(location = 0) in vec2 a_Position;
layout(location = 1) in vec2 a_TexCoord;

layout(location = 0) out vec2 v_TexCoord;

void main()
{
    v_TexCoord = a_TexCoord;
    gl_Position = vec4(a_Position, 0.0, 1.0);
}
)";

const char* textureFragSource =
R"(
#version 450 core

layout(location = 0) in vec2 v_TexCoord;

layout(location = 0) out vec4 fragColor;

uniform sampler2D u_Texture;

void main()
{
    fragColor = texture(u_Texture, v_TexCoord);
}
)";
