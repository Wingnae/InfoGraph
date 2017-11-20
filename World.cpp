#include "stdafx.h"
#include "World.hpp"

World::World(ShaderProgramPtr shader) : HierarchicalRenderable(shader) {
	ground = std::make_shared<KeyframedMeshRenderable>(shader, "meshes/plane.obj");
	ground->setParentTransform(glm::mat4(1.0));
	ground->setLocalTransform(glm::scale(glm::mat4(1.0f), glm::vec3(100.0f, 100.0f, 100.0f)));
}

void World::init() {
	HierarchicalRenderable::addChild(shared_from_this(), ground);
}

void World::do_draw() {

}

void World::do_animate(float time) {

}