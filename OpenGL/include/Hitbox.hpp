#pragma once

#include <HierarchicalMeshRenderable.hpp>
#include <lighting/Material.hpp>

class Hitbox : public HierarchicalRenderable {
public:
	~Hitbox();
	Hitbox(ShaderProgramPtr program, std::vector<glm::vec3>& pos);
	void setMaterial(const MaterialPtr& material);
	glm::vec3 collide(const HitboxPtr hb) const;
	bool collideAlongAxis(const std::vector<glm::vec3>& hb1, const std::vector<glm::vec3>& hb2, glm::vec3& axis) const;

private:
	virtual void do_draw();
	virtual void do_animate(float time);

	glm::vec3 m_center;
	glm::vec3 m_xAxis, m_yAxis, m_zAxis;
	float m_minX, m_maxX, m_minY, m_maxY, m_minZ, m_maxZ;

	std::vector< glm::vec3 > m_positions;
	std::vector< glm::vec3 > m_normals;
	std::vector< glm::vec4 > m_colors;
	std::vector< glm::ivec4 > m_indices;

	unsigned int m_pBuffer;
	unsigned int m_cBuffer;
	unsigned int m_nBuffer;
	unsigned int m_iBuffer;

	MaterialPtr m_material;
};

typedef std::shared_ptr<Hitbox> HitboxPtr;