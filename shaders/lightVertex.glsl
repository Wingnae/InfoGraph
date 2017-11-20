// shadertype=<glsl>
#version 400

uniform mat4 projMat, viewMat, modelMat;

in vec3 vPosition;
in vec3 vNormal;
in vec4 vColor;

out vec3 fragPos;
out vec4 fragColor;
out vec3 fragNorm;
out vec3 viewPos;

void main()
{
	fragPos = vec3(modelMat * vec4(vPosition, 1.0f));
    fragNorm = mat3(transpose(inverse(modelMat))) * vNormal;
	fragColor = vColor;
	viewPos = vec3(viewMat[3]);

	gl_Position = projMat * viewMat * vec4(fragPos, 1.0f);
}
