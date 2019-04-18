#version 450 core
layout(location = 0) out vec4 color;
uniform sampler2D u_texture;
in vec2 v_TexCoord;

void main()
{
	vec4 texColor = texture(u_texture,v_TexCoord);
    color = texColor;
}