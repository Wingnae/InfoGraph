// shadertype=<glsl>
#version 400

uniform mat4 projMat, viewMat, modelMat;

in vec3 vPosition;
in vec4 vColor;

out vec4 fragmentColor;

void main()
{
    gl_Position = projMat*viewMat*modelMat*vec4(vPosition, 1.0f);
    fragmentColor = vColor;
}