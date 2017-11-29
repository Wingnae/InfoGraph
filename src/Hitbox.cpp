#include <Hitbox.h>
#include <gl_helper.hpp>
#include <GL/glew.h>

Hitbox::Hitbox(ShaderProgramPtr program, RenderablePtr source) : Renderable(program), m_source(source){
	m_pBuffer = 0;
	m_cBuffer = 0;
	m_nBuffer = 0;
	m_iBuffer = 0;

	//Create buffers
	glGenBuffers(1, &m_pBuffer); //vertices
	glGenBuffers(1, &m_cBuffer); //colors
	glGenBuffers(1, &m_nBuffer); //normals
	glGenBuffers(1, &m_iBuffer); //indices
}

void Hitbox::do_draw() {

}

void Hitbox::do_animate(float time) {

}

Hitbox::~Hitbox() {
	glcheck(glDeleteBuffers(1, &m_pBuffer));
	glcheck(glDeleteBuffers(1, &m_cBuffer));
	glcheck(glDeleteBuffers(1, &m_nBuffer));
	glcheck(glDeleteBuffers(1, &m_iBuffer));
}