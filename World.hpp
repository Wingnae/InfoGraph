#pragma once

#include <ShaderProgram.hpp>
#include <Viewer.hpp>
#include <KeyframedMeshRenderable.hpp>

class World : public HierarchicalRenderable, public std::enable_shared_from_this<World> {
private:
	KeyframedMeshRenderablePtr ground;

public:
	World(ShaderProgramPtr shader);
	void init();

	void do_draw();
	void do_animate(float time);
};

typedef std::shared_ptr<World> WorldPtr;