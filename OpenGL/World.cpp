#include "stdafx.h"
#include "World.hpp"

World::World(ShaderProgramPtr shader) : HierarchicalRenderable(shader) {
	ground = std::make_shared<HierarchicalMeshRenderable>(shader, "meshes/plane.obj");
	ground->setParentTransform(glm::mat4(1.0));
	ground->setLocalTransform(glm::scale(glm::mat4(1.0f), glm::vec3(100.0f, 100.0f, 100.0f)));

	for (int i = 0; i < 20; i++) {
		boxes[i] = std::make_shared<HierarchicalMeshRenderable>(shader, "meshes/box.obj");
		boxes[i]->setParentTransform(glm::translate(glm::mat4(1.0), glm::vec3((float)rand()/(float)RAND_MAX * 100.0f - 50.0f, (float)rand() / (float)RAND_MAX * 100.0f - 50.0f, 0)));
		boxes[i]->setLocalTransform(glm::scale(glm::mat4(1.0), glm::vec3((float)rand() / (float)RAND_MAX, (float)rand() / (float)RAND_MAX, (float)rand() / (float)RAND_MAX) * 3.0f +1.5f));
		HierarchicalRenderable::addChild(ground, boxes[i]);
	}
}

void World::init() {
	HierarchicalRenderable::addChild(shared_from_this(), ground);
}

void World::do_draw() {

}

void World::do_animate(float time) {

}