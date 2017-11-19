#include "stdafx.h"
#include "Car.hpp"

Car::Car(ShaderProgramPtr shader) : HierarchicalRenderable(shader) {
	//Car
	setParentTransform(glm::mat4(1.0f));
	setLocalTransform(glm::mat4(1.0f));

	//Body
	body = std::make_shared<KeyframedMeshRenderable>(shader, "meshes/car.obj");
	body->setParentTransform(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
	body->setLocalTransform(glm::mat4(1.0));

	//Wheels
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
	vel = glm::vec3(0.0f, 0.0f, 0.0f);

	lastTime = 0;
}

void Car::init() {
	HierarchicalRenderable::addChild(shared_from_this(), body);
}

void Car::do_keyPressedEvent(sf::Event& e) {
	switch (e.key.code) {
	case sf::Keyboard::Up:
		vel.x = 1.0f;
		break;
	case sf::Keyboard::Down:
		vel.x = -1.0f;
		break;
	case sf::Keyboard::Left:
		vel.y = 1.0f;
		break;
	case sf::Keyboard::Right:
		vel.y = -1.0f;
		break;
	}
}

void Car::do_keyReleasedEvent(sf::Event& event) {
	switch (event.key.code) {
	case sf::Keyboard::Up:
		vel.x = 0.0f;
		break;
	case sf::Keyboard::Down:
		vel.x = 0.0f;
		break;
	case sf::Keyboard::Left:
		vel.y = 0.0f;
		break;
	case sf::Keyboard::Right:
		vel.y = 0.0f;
		break;
	}
}

void Car::do_draw() {

}

void Car::do_animate(float time) {
	std::cout << time << std::endl;
	pos += (time - lastTime) * vel;
	lastTime = time;
	setParentTransform(glm::translate(glm::mat4(1.0f), pos));
}