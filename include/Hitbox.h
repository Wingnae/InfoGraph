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
};