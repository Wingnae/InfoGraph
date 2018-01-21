#version 400
//uniforms
uniform mat4 projMat, viewMat;
uniform vec3 billboard_world_position;
uniform vec2 billboard_world_dimensions;

//attributes
in vec2 vShift;
in vec4 vColor;  // Currently not used. You can use it to replace or combine with the diffuse component of the material

// Surfel: a SURFace ELement. All coordinates are in camera space
out vec2 surfel_texCoord;
out vec3 surfel_position;
out vec3 surfel_normal;
out vec4 surfel_color;

void main()
{
  surfel_position = 
    vec3( viewMat * vec4( billboard_world_position, 1.0 )) + 
  	vec3( billboard_world_dimensions * vShift, 0 );
  	 
  surfel_normal = vec3(0,0,-1);
  surfel_color  = vColor;
  surfel_texCoord = vec2(0.5,0.5) + vShift;
  
  // Define the fragment position on the screen
  gl_Position = projMat*vec4(surfel_position, 1.0 );
}
