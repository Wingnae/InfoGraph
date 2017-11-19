#include "Ground.hpp"

Ground::Ground(ShaderProgramPtr shader, Viewer& viewer) {
	mesh = std::make_shared<KeyframedMeshRenderable>(shader, "meshes/plane.obj");
	mesh->setParentTransform(glm::mat4(1.0));
	mesh->setLocalTransform(glm::scale(glm::mat4(1.0f), glm::vec3(100.0f, 100.0f, 0.0f)));

	viewer.addRenderable(mesh);
}