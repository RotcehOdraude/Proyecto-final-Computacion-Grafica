#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec3 ourColor;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    ourColor = aColor;
	vec4 pos = projection * view * vec4(aPos, 1.0);
    gl_Position = pos.xyww;
}