#pragma once

#include <ShaderProgram.hpp>
#include <Viewer.hpp>
#include <HierarchicalMeshRenderable.hpp>
#include "Car.hpp"

class World : public HierarchicalRenderable {
private:
	HierarchicalMeshRenderablePtr ground;
	std::vector<HierarchicalMeshRenderablePtr> boxes;
	CarPtr car;

public:
	World(ShaderProgramPtr shader);
	void init();

	void do_draw();
	void do_animate(float time);
	void afterAnimate(float time);
};

typedef std::shared_ptr<World> WorldPtr;