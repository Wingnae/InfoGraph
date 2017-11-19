#include "Car.hpp"

Car::Car(ShaderProgramPtr shader, Viewer& viewer) {
	this->viewer = &viewer;
	body = std::make_shared<KeyframedMeshRenderable>(shader, "meshes/car.obj");
	body->setParentTransform(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
	body->setLocalTransform(glm::mat4(1.0));
	viewer.addRenderable(body);

	for (int i = 0; i < 4; i++) {
		wheels[i] = std::make_shared<KeyframedMeshRenderable>(shader, "meshes/wheel.obj");
		wheels[i]->setLocalTransform(glm::mat4(1.0));
		HierarchicalRenderable::addChild(body, wheels[i]);
	}

	wheels[0]->setParentTransform(glm::translate(glm::mat4(1.0f), glm::vec3(0.98, 0.8, 0)));
	wheels[1]->setParentTransform(glm::translate(glm::mat4(1.0f), glm::vec3(0.98, -0.8, 0)));
	wheels[2]->setParentTransform(glm::translate(glm::mat4(1.0f), glm::vec3(-1.15, 0.8, 0)));
	wheels[3]->setParentTransform(glm::translate(glm::mat4(1.0f), glm::vec3(-1.15, -0.8, 0)));

	pos = glm::vec3(0.0f, 0.0f, 0.0f);
	vel = glm::vec3(1.0f, 0.0f, 0.0f);
}

void Car::handle_event() {
	
}

void Car::move(int direction) {
	switch (direction) {
	case FORWARD:
		body->setParentTransform(glm::translate(body->getParentTransform(), glm::vec3(vel.x, vel.y, vel.z)));
		break;
	case BACKWARD:
		body->setParentTransform(glm::translate(body->getParentTransform(), glm::vec3(-vel.x, -vel.y, vel.z)));
		break;
	case LEFT:
		body->setParentTransform(glm::rotate(body->getParentTransform(), glm::radians(1.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
		break;
	case RIGHT:
		body->setParentTransform(glm::rotate(body->getParentTransform(), glm::radians(-1.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
		break;
	}
}