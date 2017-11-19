#pragma once

#include <ShaderProgram.hpp>
#include <Viewer.hpp>
#include <KeyframedMeshRenderable.hpp>

class Ground {
private:
	KeyframedMeshRenderablePtr mesh;

public:
	Ground(ShaderProgramPtr shader, Viewer& viewer);
};