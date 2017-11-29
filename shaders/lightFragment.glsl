// shadertype=<glsl>
#version 400

in vec4 fragColor;
in vec3 fragPos;
in vec3 fragNorm;

in vec3 viewPos;

out vec4 outColor;

void main()
{
	vec3 lightPos = vec3(0.0f, 0.0f, 50.0f);
	vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);

	// ambient
    float ambientStrength = 0.05f;
    vec3 ambient = ambientStrength * lightColor;
  	
    // diffuse 
    vec3 norm = normalize(fragNorm);
    vec3 lightDir = normalize(lightPos - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    
    // specular
    float specularStrength = 0.0f;
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;
        
    vec3 result = (ambient + diffuse + specular) * vec3(fragColor);
    outColor = vec4(result, 1.0);
}
