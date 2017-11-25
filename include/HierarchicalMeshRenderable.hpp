#ifndef HIERARCHICAL_MESH_RENDERABLE_HPP
#define HIERARCHICAL_MESH_RENDERABLE_HPP

#include "HierarchicalRenderable.hpp"

#include <string>
#include <vector>
#include <glm/glm.hpp>

class HierarchicalMeshRenderable : public HierarchicalRenderable
{
public:
	~HierarchicalMeshRenderable();
	HierarchicalMeshRenderable(ShaderProgramPtr program, const std::string& filename);

private:
	void do_draw();
	void do_animate(float time);
};

typedef std::shared_ptr<HierarchicalMeshRenderable> HierarchicalMeshRenderablePtr;

#endif