#include <Hitbox.hpp>
#include <gl_helper.hpp>
#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

Hitbox::Hitbox(ShaderProgramPtr program, std::vector<glm::vec3>& pos) : HierarchicalRenderable(program) {
	float maxX = 0.0f, minX = 0.0f, maxY = 0.0f, minY = 0.0f, maxZ = 0.0f, minZ = 0.0f;

	for (const glm::vec3& p : pos) {
		if (maxX < p.x) maxX = p.x;
		if (minX > p.x) minX = p.x;

		if (maxY < p.y) maxY = p.y;
		if (minY > p.y) minY = p.y;

		if (maxZ < p.z) maxZ = p.z;
		if (minZ > p.z) minZ = p.z;
	}

	m_positions.push_back(glm::vec3(minX, minY, minZ));
	m_positions.push_back(glm::vec3(minX, minY, maxZ));
	m_positions.push_back(glm::vec3(minX, maxY, maxZ));
	m_positions.push_back(glm::vec3(minX, maxY, minZ));
	m_positions.push_back(glm::vec3(maxX, maxY, minZ));
	m_positions.push_back(glm::vec3(maxX, maxY, maxZ));
	m_positions.push_back(glm::vec3(maxX, minY, maxZ));
	m_positions.push_back(glm::vec3(maxX, minY, minZ));

	for (const glm::vec3& p : m_positions) {
		m_normals.push_back(glm::normalize(p));
		m_colors.push_back(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
	}

	m_indices.push_back(glm::ivec4(0, 1, 2, 3));
	m_indices.push_back(glm::ivec4(3, 2, 5, 4));
	m_indices.push_back(glm::ivec4(3, 0, 7, 4));
	m_indices.push_back(glm::ivec4(7, 6, 1, 0));
	m_indices.push_back(glm::ivec4(2, 1, 6, 5));
	m_indices.push_back(glm::ivec4(4, 5, 6, 7));

	glGenBuffers(1, &m_pBuffer); //vertices
	glGenBuffers(1, &m_cBuffer); //colors
	glGenBuffers(1, &m_nBuffer); //normals
	glGenBuffers(1, &m_iBuffer); //indices

	glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_pBuffer));
	glcheck(glBufferData(GL_ARRAY_BUFFER, m_positions.size() * sizeof(glm::vec3), m_positions.data(), GL_STATIC_DRAW));
	glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_cBuffer));
	glcheck(glBufferData(GL_ARRAY_BUFFER, m_colors.size() * sizeof(glm::vec4), m_colors.data(), GL_STATIC_DRAW));
	glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_nBuffer));
	glcheck(glBufferData(GL_ARRAY_BUFFER, m_normals.size() * sizeof(glm::vec3), m_normals.data(), GL_STATIC_DRAW));
	glcheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iBuffer));
	glcheck(glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(glm::ivec4), m_indices.data(), GL_STATIC_DRAW));
}

bool Hitbox::collide(const HitboxPtr hb) {
	float maxX = 0.0f, minX = 0.0f, maxY = 0.0f, minY = 0.0f, maxZ = 0.0f, minZ = 0.0f;
	float maxX2 = 0.0f, minX2 = 0.0f, maxY2 = 0.0f, minY2 = 0.0f, maxZ2 = 0.0f, minZ2 = 0.0f;

	for (const glm::vec3& p : m_positions) {
		glm::vec3 realP = glm::vec3(m_model * glm::vec4(p, 1.0f));

		if (maxX < realP.x) maxX = realP.x;
		if (minX > realP.x) minX = realP.x;

		if (maxY < realP.y) maxY = realP.y;
		if (minY > realP.y) minY = realP.y;

		if (maxZ < realP.z) maxZ = realP.z;
		if (minZ > realP.z) minZ = realP.z;
	}
	for (const glm::vec3& p : hb->m_positions) {
		glm::vec3 realP = glm::vec3(hb->m_model * glm::vec4(p, 1.0f));

		if (maxX2 < realP.x) maxX2 = realP.x;
		if (minX2 > realP.x) minX2 = realP.x;

		if (maxY2 < realP.y) maxY2 = realP.y;
		if (minY2 > realP.y) minY2 = realP.y;

		if (maxZ2 < realP.z) maxZ2 = realP.z;
		if (minZ2 > realP.z) minZ2 = realP.z;
	}

	//SAT x
	if (maxX < minX2 || maxX2 < minX)
		return false;
	//SAT y
	if (maxY < minY2 ||  maxY2 < minY)
		return false;
	//SAT z
	if (maxZ < minZ2 ||  maxZ2 < minZ)
		return false;

	return true;
}

void Hitbox::do_draw() {
	int positionLocation = m_shaderProgram->getAttributeLocation("vPosition");
	int colorLocation = m_shaderProgram->getAttributeLocation("vColor");
	int normalLocation = m_shaderProgram->getAttributeLocation("vNormal");
	int modelLocation = m_shaderProgram->getUniformLocation("modelMat");

	if (modelLocation != ShaderProgram::null_location)
		glcheck(glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(getModelMatrix())));

	if (positionLocation != ShaderProgram::null_location)
	{
		glcheck(glEnableVertexAttribArray(positionLocation));
		glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_pBuffer));
		glcheck(glVertexAttribPointer(positionLocation, 3, GL_FLOAT, GL_FALSE, 0, (void*)0));
	}

	if (colorLocation != ShaderProgram::null_location)
	{
		glcheck(glEnableVertexAttribArray(colorLocation));
		glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_cBuffer));
		glcheck(glVertexAttribPointer(colorLocation, 4, GL_FLOAT, GL_FALSE, 0, (void*)0));
	}

	if (normalLocation != ShaderProgram::null_location)
	{
		glcheck(glEnableVertexAttribArray(normalLocation));
		glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_nBuffer));
		glcheck(glVertexAttribPointer(normalLocation, 3, GL_FLOAT, GL_FALSE, 0, (void*)0));
	}

	//Draw triangles elements
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glcheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iBuffer));
	glcheck(glDrawElements(GL_QUADS, m_indices.size() * 4, GL_UNSIGNED_INT, (void*)0));
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	if (positionLocation != ShaderProgram::null_location)
	{
		glcheck(glDisableVertexAttribArray(positionLocation));
	}

	if (colorLocation != ShaderProgram::null_location)
	{
		glcheck(glDisableVertexAttribArray(colorLocation));
	}

	if (normalLocation != ShaderProgram::null_location)
	{
		glcheck(glDisableVertexAttribArray(normalLocation));
	}
}

void Hitbox::do_animate(float time) {

}

Hitbox::~Hitbox() {
	glcheck(glDeleteBuffers(1, &m_pBuffer));
	glcheck(glDeleteBuffers(1, &m_cBuffer));
	glcheck(glDeleteBuffers(1, &m_nBuffer));
	glcheck(glDeleteBuffers(1, &m_iBuffer));
}