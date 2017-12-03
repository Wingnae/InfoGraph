#include "stdafx.h"
#include "World.hpp"
#include <Hitbox.hpp>

World::World(ShaderProgramPtr shader) : HierarchicalRenderable(shader) {
	ground = std::make_shared<HierarchicalMeshRenderable>(shader, "meshes/plane.obj");
	ground->init();
	ground->setParentTransform(glm::scale(glm::mat4(1.0f), glm::vec3(100.0f, 100.0f, 100.0f)));

	for (int i = 0; i < 20; i++) {
		boxes.push_back(std::make_shared<HierarchicalMeshRenderable>(shader, "meshes/box.obj"));
		boxes[i]->init();
		boxes[i]->setParentTransform(glm::translate(glm::mat4(1.0), glm::vec3((float)rand()/(float)RAND_MAX - 0.5f, (float)rand() / (float)RAND_MAX - 0.5f, 0)));
		boxes[i]->setParentTransform(glm::scale(boxes[i]->getParentTransform(), glm::vec3((float)rand() / (float)RAND_MAX, (float)rand() / (float)RAND_MAX, (float)rand() / (float)RAND_MAX) * 3.0f + 1.5f));
		boxes[i]->setParentTransform(glm::scale(boxes[i]->getParentTransform(), glm::vec3(0.01f, 0.01f, 0.01f)));
		HierarchicalRenderable::addChild(ground, boxes[i]);
	}

	car = std::make_shared<Car>(shader, 1000.0f, 300.0f);
	car->init();
}

void World::init() {
	HierarchicalRenderable::addChild(shared_from_this(), ground);
	HierarchicalRenderable::addChild(shared_from_this(), car);
}

void World::do_draw() {

}

void World::do_animate(float time) {

}

void World::afterAnimate(float time) {
	for (size_t i = 0; i<m_children.size(); ++i)
		m_children[i]->animate(time);
	for (int i = 0; i < boxes.size(); i++) {
		if (boxes[i]->getHitbox()->collide(car->getHitbox())) {
			//std::cout << "collision" << i << std::endl;
		}
	}
}