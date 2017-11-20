#include "stdafx.h"
#include "Car.hpp"

Car::Car(ShaderProgramPtr shader) : HierarchicalRenderable(shader) {
	//Car
	setParentTransform(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
	setLocalTransform(glm::mat4(1.0f));

	//Body
	body = std::make_shared<KeyframedMeshRenderable>(shader, "meshes/car.obj");
	body->setParentTransform(glm::mat4(1.0));
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

	speed = glm::vec3(0.0f, 0.0f, 0.0f);
	speedR = glm::vec3(0.0f, 0.0f, 0.0f);

	lastTime = 0;

	state.direction = NONE;
	state.turn = NONE;
}

void Car::init() {
	HierarchicalRenderable::addChild(shared_from_this(), body);
}

void Car::do_keyPressedEvent(sf::Event& e) {
	switch (e.key.code) {
	case sf::Keyboard::Up:
		state.direction = FORWARD;
		break;
	case sf::Keyboard::Down:
		state.direction = BACKWARD;
		break;
	case sf::Keyboard::Left:
		state.turn = LEFT;
		break;
	case sf::Keyboard::Right:
		state.turn = RIGHT;
		break;
	}
}

void Car::do_keyReleasedEvent(sf::Event& e) {
	switch (e.key.code) {
	case sf::Keyboard::Up:
		if(state.direction == FORWARD)
			state.direction = NONE;
		break;
	case sf::Keyboard::Down:
		if (state.direction == BACKWARD)
			state.direction = NONE;
		break;
	case sf::Keyboard::Left:
		if (state.turn == LEFT)
			state.turn = NONE;
		break;
	case sf::Keyboard::Right:
		if (state.turn == RIGHT)
			state.turn = NONE;
		break;
	}
}

void Car::do_draw() {

}

void Car::do_animate(float time) {
	float dt = time - lastTime;
	lastTime = time;

	move(dt);
	
	//Car
	setParentTransform(glm::rotate(getParentTransform(), glm::radians(dt * speedR.z * speed.x/MAXSPEED), glm::vec3(0, 0, 1)));
	setParentTransform(glm::translate(getParentTransform(), dt * speed));

	//Wheels
	wheels[0]->setParentTransform(glm::translate(glm::mat4(1.0f), glm::vec3(0.98, 0.8, 0)));
	wheels[0]->setParentTransform(glm::rotate(wheels[0]->getParentTransform(), glm::radians(speedR.z / 4), glm::vec3(0, 0, 1)));
	wheels[1]->setParentTransform(glm::translate(glm::mat4(1.0f), glm::vec3(0.98, -0.8, 0)));
	wheels[1]->setParentTransform(glm::rotate(wheels[1]->getParentTransform(), glm::radians(speedR.z / 4), glm::vec3(0, 0, 1)));
	
	for (int i = 0; i < 4; i++) {
		wheels[i]->setLocalTransform(glm::rotate(wheels[i]->getLocalTransform(), speed.x * dt, glm::vec3(0, 1, 0)));
	}

	//Camera
	m_viewer->getCamera().setViewMatrix(glm::lookAt(glm::vec3(getParentTransform()[3] - 15.0f * cos(glm::radians(speedR.z / 4)) * getParentTransform()[0]
															+ 15.0f * sin(glm::radians(speedR.z/4)) * getParentTransform()[1]) 
															+ glm::vec3(0, 0, 7.5f),
													glm::vec3(getParentTransform()[3]), 
													glm::vec3(0, 0, 1)));
}

void Car::move(float dt) {
	//Speed
	if (state.direction == NONE) {
		if (speed.x >= ACC * dt)
			speed.x -= ACC * dt;
		else if (speed.x < -ACC * dt)
			speed.x += ACC * dt;
		else
			speed.x = 0;
	}
	else {
		speed.x += ACC * state.direction * dt;
	}

	if (speed.x > MAXSPEED)
		speed.x = MAXSPEED;
	if (speed.x < -MAXSPEED)
		speed.x = -MAXSPEED;

	//Rotation speed

	if (state.turn == NONE) {
		if (speedR.z >= ACCR * dt)
			speedR.z -= ACCR * dt;
		else if (speedR.z < -ACCR * dt)
			speedR.z += ACCR * dt;
		else
			speedR.z = 0;
	}
	else {
		speedR.z += ACCR * state.turn * dt;
	}

	if (speedR.z > MAXSPEEDR)
		speedR.z = MAXSPEEDR;
	if (speedR.z < -MAXSPEEDR)
		speedR.z = -MAXSPEEDR;

	//Gravity
	if (getParentTransform()[3][2] < 0.5f)
		speed.z = 0;
	else
		speed.z -= GRAVITY * dt;
}