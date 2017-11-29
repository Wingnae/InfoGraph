#pragma once

#include <Renderable.hpp>

class Hitbox : public Renderable {
public:
	~Hitbox();
	Hitbox(ShaderProgramPtr program, RenderablePtr source);

private:
	virtual void do_draw();
	virtual void do_animate(float time);

	RenderablePtr m_source;

	std::vector< glm::vec3 > m_positions;
	std::vector< glm::vec3 > m_normals;
	std::vector< glm::vec4 > m_colors;
	std::vector< unsigned int > m_indices;

	unsigned int m_pBuffer;
	unsigned int m_cBuffer;
	unsigned int m_nBuffer;
	unsigned int m_iBuffer;
};