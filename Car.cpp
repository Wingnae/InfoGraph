#include "Car.hpp"

Car::Car(ShaderProgramPtr shader, Viewer& viewer) {
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

	vel_x = 0.1f;
	vel_y = 0;
	vel_z = 0;
}

void Car::handle_event() {
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
		move(LEFT);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		move(RIGHT);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
		move(FORWARD);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		move(BACKWARD);
}

void Car::move(int direction) {
	switch (direction) {
	case FORWARD:
		body->setParentTransform(glm::translate(body->getParentTransform(), glm::vec3(vel_x, vel_y, vel_z)));
		break;
	case BACKWARD:
		body->setParentTransform(glm::translate(body->getParentTransform(), glm::vec3(-vel_x, -vel_y, vel_z)));
		break;
	case LEFT:
		body->setParentTransform(glm::rotate(body->getParentTransform(), glm::radians(1.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
		break;
	case RIGHT:
		body->setParentTransform(glm::rotate(body->getParentTransform(), glm::radians(-1.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
		break;
	}
}