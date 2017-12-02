#pragma once

#include <HierarchicalRenderable.hpp>

class Hitbox : public HierarchicalRenderable {
public:
	~Hitbox();
	Hitbox(ShaderProgramPtr program, HierarchicalRenderablePtr source);

private:
	virtual void do_draw();
	virtual void do_animate(float time);

	HierarchicalRenderablePtr m_source;

	std::vector< glm::vec3 > m_positions;
	std::vector< glm::vec3 > m_normals;
	std::vector< glm::vec4 > m_colors;
	std::vector< glm::ivec4 > m_indices;

	unsigned int m_pBuffer;
	unsigned int m_cBuffer;
	unsigned int m_nBuffer;
	unsigned int m_iBuffer;
};

typedef std::shared_ptr<Hitbox> HitboxPtr;