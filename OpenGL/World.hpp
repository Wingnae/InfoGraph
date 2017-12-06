#pragma once

#include <ShaderProgram.hpp>
#include <Viewer.hpp>
#include <lighting/LightedMeshRenderable.hpp>
#include "Car.hpp"

#define WORLD_SIZE	150.0f

class World : public HierarchicalRenderable {
private:
	LightedMeshRenderablePtr ground;
	std::vector<LightedMeshRenderablePtr> boxes;
	CarPtr car;

public:
	World(ShaderProgramPtr shader);
	void init();

	void do_draw();
	void do_animate(float time);
	void afterAnimate(float time);
};

typedef std::shared_ptr<World> WorldPtr;