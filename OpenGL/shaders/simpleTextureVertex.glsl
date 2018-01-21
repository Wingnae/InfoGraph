#version 400
uniform mat4 projMat, viewMat, modelMat;

in vec3 vPosition;
in vec2 vTexCoord;
out vec2 surfel_texCoord;

void main()
{
    gl_Position = projMat*viewMat*modelMat*vec4(vPosition, 1.0f);
    // simply pass the texture coordinate to the fragment
    surfel_texCoord = vTexCoord;
}
