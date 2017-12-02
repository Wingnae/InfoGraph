#pragma once

#include <ShaderProgram.hpp>
#include <Viewer.hpp>
#include <HierarchicalMeshRenderable.hpp>

class World : public HierarchicalRenderable {
private:
	HierarchicalMeshRenderablePtr ground;
	HierarchicalMeshRenderablePtr boxes[20];

public:
	World(ShaderProgramPtr shader);
	void init();

	void do_draw();
	void do_animate(float time);
};

typedef std::shared_ptr<World> WorldPtr;