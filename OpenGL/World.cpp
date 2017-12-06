#include "stdafx.h"
#include "World.hpp"
#include <Hitbox.hpp>

World::World(ShaderProgramPtr shader) : HierarchicalRenderable(shader) {
	MaterialPtr pearl = Material::WhiteRubber();

	ground = std::make_shared<LightedMeshRenderable>(shader, "meshes/plane.obj");
	ground->init();
	ground->setMaterial(pearl);
	ground->setParentTransform(glm::scale(glm::mat4(1.0f), glm::vec3(WORLD_SIZE / 2, WORLD_SIZE / 2, 1.0f)));

	for (int i = 0; i < 20; i++) {
		boxes.push_back(std::make_shared<LightedMeshRenderable>(shader, "meshes/box.obj"));
		boxes[i]->init();
		boxes[i]->setMaterial(pearl);
		boxes[i]->setParentTransform(glm::translate(glm::mat4(1.0), glm::vec3(2.0/3.0 * WORLD_SIZE * (float)rand()/(float)RAND_MAX - WORLD_SIZE / 3.0, 2.0/3.0 * WORLD_SIZE * (float)rand() / (float)RAND_MAX - WORLD_SIZE / 3.0, 0)));
		boxes[i]->setParentTransform(glm::scale(boxes[i]->getParentTransform(), glm::vec3((float)rand() / (float)RAND_MAX / 2.0, (float)rand() / (float)RAND_MAX / 2.0, (float)rand() / (float)RAND_MAX) * 3.0f + 1.5f));
	}

	car = std::make_shared<Car>(shader, 1000.0f, 300.0f);
	car->init();
}

void World::init() {
	HierarchicalRenderable::addChild(shared_from_this(), ground);
	HierarchicalRenderable::addChild(shared_from_this(), car);

	ground->getHitbox()->updateModelMatrix();
	for (auto b : boxes) {
		b->getHitbox()->updateModelMatrix();
		HierarchicalRenderable::addChild(shared_from_this(), b);
	}
}

void World::do_draw() {

}

void World::do_animate(float time) {

}

void World::afterAnimate(float time) {
	for (size_t i = 0; i<m_children.size(); ++i)
		m_children[i]->animate(time);
	for (int i = 0; i < boxes.size(); i++) {
		auto col = car->getHitbox()->collide(boxes[i]->getHitbox());
		if (col != glm::vec3(0.0f)) {
			boxes[i]->setMaterial(Material::RedPlastic());
			//std::cout << col.x << " " << col.y << " " << col.z << std::endl;
		}
		else {
			boxes[i]->setMaterial(Material::WhitePlastic());
		}
	}
}