#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 ourColor;
uniform float xOffset;
out vec4 vertexColor;
void main()
{
    gl_Position = vec4(aPos.x + xOffset, -aPos.y, aPos.z, 1.0);
    vertexColor = vec4(aPos, 1.0);
}
