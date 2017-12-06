#include <Hitbox.hpp>
#include <gl_helper.hpp>
#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#define MAXFLOAT	std::numeric_limits<float>::max()

Hitbox::Hitbox(ShaderProgramPtr program, std::vector<glm::vec3>& pos) : HierarchicalRenderable(program) {
	m_maxX = -MAXFLOAT, m_minX = MAXFLOAT, m_maxY = -MAXFLOAT, m_minY = MAXFLOAT, m_maxZ = -MAXFLOAT, m_minZ = MAXFLOAT;

	for (const glm::vec3& p : pos) {
		if (m_maxX < p.x) m_maxX = p.x;
		if (m_minX > p.x) m_minX = p.x;

		if (m_maxY < p.y) m_maxY = p.y;
		if (m_minY > p.y) m_minY = p.y;

		if (m_maxZ < p.z) m_maxZ = p.z;
		if (m_minZ > p.z) m_minZ = p.z;
	}

	m_center = glm::vec3((m_maxX - m_minX) / 2, (m_maxY - m_minY) / 2, (m_maxZ - m_minZ) / 2);

	m_positions.push_back(glm::vec3(m_minX, m_minY, m_minZ));
	m_positions.push_back(glm::vec3(m_minX, m_minY, m_maxZ));
	m_positions.push_back(glm::vec3(m_minX, m_maxY, m_maxZ));
	m_positions.push_back(glm::vec3(m_minX, m_maxY, m_minZ));
	m_positions.push_back(glm::vec3(m_maxX, m_maxY, m_minZ));
	m_positions.push_back(glm::vec3(m_maxX, m_maxY, m_maxZ));
	m_positions.push_back(glm::vec3(m_maxX, m_minY, m_maxZ));
	m_positions.push_back(glm::vec3(m_maxX, m_minY, m_minZ));

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

void Hitbox::setMaterial(const MaterialPtr& material) {
	m_material = material;
}


glm::vec3 Hitbox::collide(const HitboxPtr hb) const {
	std::vector<glm::vec3> realP(m_positions.size()), realP2(hb->m_positions.size());

	for (int i = 0; i < m_positions.size(); i++)
		realP[i] = glm::vec3(m_model * glm::vec4(m_positions[i], 1.0f));
	for (int i = 0; i < hb->m_positions.size(); i++)
		realP2[i] = glm::vec3(hb->m_model * glm::vec4(hb->m_positions[i], 1.0f));
	
	//SAT X1
	if (!collideAlongAxis(realP, realP2, glm::normalize(realP[7] - realP[0])))
		return glm::vec3(0.0f);
	//SAT Y1
	if (!collideAlongAxis(realP, realP2, glm::normalize(realP[3] - realP[0])))
		return glm::vec3(0.0f);
	//SAT Z1
	if (!collideAlongAxis(realP, realP2, glm::normalize(realP[1] - realP[0])))
		return glm::vec3(0.0f);

	//SAT X2
	if (!collideAlongAxis(realP, realP2, glm::normalize(realP2[7] - realP2[0])))
		return glm::vec3(0.0f);
	//SAT Y2
	if (!collideAlongAxis(realP, realP2, glm::normalize(realP2[3] - realP2[0])))
		return glm::vec3(0.0f);
	//SAT Z2
	if (!collideAlongAxis(realP, realP2, glm::normalize(realP2[1] - realP2[0])))
		return glm::vec3(0.0f);

	glm::vec3 realCenter = m_model * glm::vec4(m_center, 1.0);
	glm::vec3 realCenter2 = hb->m_model * glm::vec4(hb->m_center, 1.0);
	
	return glm::normalize(realCenter2 - realCenter);
}

bool Hitbox::collideAlongAxis(const std::vector<glm::vec3>& a, const std::vector<glm::vec3>& b, glm::vec3& axis) const {
	// Handles the cross product = {0,0,0} case
	if (axis == glm::vec3(0, 0, 0))
		return true;

	float aMin = MAXFLOAT;
	float aMax = -MAXFLOAT;
	float bMin = MAXFLOAT;
	float bMax = -MAXFLOAT;

	// Define two intervals, a and b. Calculate their min and max values
	for (int i = 0; i < a.size(); i++) {
		float aDist = glm::dot(a[i], axis);
		aMin = (aDist < aMin) ? aDist : aMin;
		aMax = (aDist > aMax) ? aDist : aMax;
		float bDist = glm::dot(b[i], axis);
		bMin = (bDist < bMin) ? bDist : bMin;
		bMax = (bDist > bMax) ? bDist : bMax;
	}

	// One-dimensional intersection test between a and b
	float longSpan = std::max(aMax, bMax) - std::min(aMin, bMin);
	float sumSpan = aMax - aMin + bMax - bMin;
	return longSpan < sumSpan;
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