#include "stdafx.h"
#include "Car.hpp"

Car::Car(ShaderProgramPtr shader) : HierarchicalRenderable(shader), m_mass(1000.0f), m_torque(3000.0f), m_torqueb(1000.0f), m_brakes(10000.0f), m_resistance(1.0f), m_wradius(0.5f) {
	//Car
	setParentTransform(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
	setLocalTransform(glm::mat4(1.0f));

	//Body
	m_body = std::make_shared<KeyframedMeshRenderable>(shader, "meshes/car.obj");
	m_body->setParentTransform(glm::mat4(1.0));
	m_body->setLocalTransform(glm::mat4(1.0));

	//Wheels
	for (int i = 0; i < 4; i++) {
		m_wheels[i] = std::make_shared<KeyframedMeshRenderable>(shader, "meshes/wheel.obj");
		m_wheels[i]->setLocalTransform(glm::mat4(1.0));
		HierarchicalRenderable::addChild(m_body, m_wheels[i]);
	}

	m_wheels[0]->setParentTransform(glm::translate(glm::mat4(1.0f), glm::vec3(0.98, 0.8, 0)));
	m_wheels[1]->setParentTransform(glm::translate(glm::mat4(1.0f), glm::vec3(0.98, -0.8, 0)));
	m_wheels[2]->setParentTransform(glm::translate(glm::mat4(1.0f), glm::vec3(-1.15, 0.8, 0)));
	m_wheels[3]->setParentTransform(glm::translate(glm::mat4(1.0f), glm::vec3(-1.15, -0.8, 0)));

	m_lastTime = 0;

	m_state.direction = NONE;
	m_state.turn = NONE;

	m_velocity = glm::vec3(0.0f, 0.0f, 0.0f);
	m_acceleration = glm::vec3(0.0f, 0.0f, 0.0f);

	m_ftotal = glm::vec3(0.0f, 0.0f, 0.0f);
	m_ftraction = glm::vec3(0.0f, 0.0f, 0.0f);
	m_fbrake = glm::vec3(0.0f, 0.0f, 0.0f);
	m_fdrag = glm::vec3(0.0f, 0.0f, 0.0f);
	m_frr = glm::vec3(0.0f, 0.0f, 0.0f);
	m_fg = glm::vec3(0.0f, 0.0f, 0.0f);
}

void Car::init() {
	HierarchicalRenderable::addChild(shared_from_this(), m_body);
}

void Car::do_keyPressedEvent(sf::Event& e) {
	switch (e.key.code) {
	case sf::Keyboard::Up:
		m_state.direction = FORWARD;
		break;
	case sf::Keyboard::Down:
		m_state.direction = BACKWARD;
		break;
	case sf::Keyboard::Left:
		m_state.turn = LEFT;
		break;
	case sf::Keyboard::Right:
		m_state.turn = RIGHT;
		break;
	}
}

void Car::do_keyReleasedEvent(sf::Event& e) {
	switch (e.key.code) {
	case sf::Keyboard::Up:
		if(m_state.direction == FORWARD)
			m_state.direction = NONE;
		break;
	case sf::Keyboard::Down:
		if (m_state.direction == BACKWARD)
			m_state.direction = NONE;
		break;
	case sf::Keyboard::Left:
		if (m_state.turn == LEFT)
			m_state.turn = NONE;
		break;
	case sf::Keyboard::Right:
		if (m_state.turn == RIGHT)
			m_state.turn = NONE;
		break;
	}
}

void Car::do_draw() {

}

void Car::do_animate(float time) {
	computeTotalForce();

	float dt = m_lastTime - time;
	m_lastTime = time;

	m_acceleration = m_ftotal / m_mass;
	m_velocity += m_acceleration * dt;

	setParentTransform(glm::translate(getParentTransform(), dt * m_velocity));
	m_viewer->getCamera().setViewMatrix(glm::lookAt(glm::vec3(getParentTransform()[3]) + glm::vec3(-15.0f, 0.0f, 7.5f), glm::vec3(getParentTransform()[3]), glm::vec3(0, 0, 1)));
}

void Car::computeTotalForce() {
	computeDragForce();
	computeResistanceForce();
	//computeGravity();
	m_ftotal = m_fdrag + m_frr + m_fg;

	if (m_state.direction == FORWARD || m_state.direction == BACKWARD && m_velocity.x <= 0) {
		computeTractionForce();
		m_ftotal += m_ftraction;
	}
	else if (m_state.direction == BACKWARD) {
		computeBrakeForce();
		m_ftotal += m_fbrake;
	}
}

void Car::computeTractionForce() {
	if(m_state.direction == FORWARD)
		m_ftraction.x = m_torque / m_wradius;
	else if (m_state.direction == BACKWARD)
		m_ftraction.x = -m_torqueb / m_wradius;
}

void Car::computeBrakeForce() {
	m_fbrake.x = -m_brakes;
}

void Car::computeDragForce() {
	m_fdrag = -AERODRAG * m_velocity * glm::length(m_velocity);
}

void Car::computeResistanceForce() {
	m_frr = -m_resistance * m_velocity;
}

void Car::computeGravity() {
	m_fg.z = -m_mass * GRAVITY;
}

////Old
//void Car::do_animate(float time) {
//	float dt = time - lastTime;
//	lastTime = time;
//
//	move(dt);
//	
//	//Car
//	setParentTransform(glm::rotate(getParentTransform(), glm::radians(dt * speedR.z * speed.x/MAXSPEED), glm::vec3(0, 0, 1)));
//	setParentTransform(glm::translate(getParentTransform(), dt * speed));
//
//	//Wheels
//	wheels[0]->setParentTransform(glm::translate(glm::mat4(1.0f), glm::vec3(0.98, 0.8, 0)));
//	wheels[0]->setParentTransform(glm::rotate(wheels[0]->getParentTransform(), glm::radians(speedR.z / 4), glm::vec3(0, 0, 1)));
//	wheels[1]->setParentTransform(glm::translate(glm::mat4(1.0f), glm::vec3(0.98, -0.8, 0)));
//	wheels[1]->setParentTransform(glm::rotate(wheels[1]->getParentTransform(), glm::radians(speedR.z / 4), glm::vec3(0, 0, 1)));
//	
//	for (int i = 0; i < 4; i++) {
//		wheels[i]->setLocalTransform(glm::rotate(wheels[i]->getLocalTransform(), speed.x * dt, glm::vec3(0, 1, 0)));
//	}
//
//	//Camera
//	m_viewer->getCamera().setViewMatrix(glm::lookAt(glm::vec3(getParentTransform()[3] - 15.0f * cos(glm::radians(speedR.z / 4)) * getParentTransform()[0]
//															+ 15.0f * sin(glm::radians(speedR.z/4)) * getParentTransform()[1]) 
//															+ glm::vec3(0, 0, 7.5f),
//													glm::vec3(getParentTransform()[3]), 
//													glm::vec3(0, 0, 1)));
//}
//
//void Car::move(float dt) {
//	//Speed
//	if (state.direction == NONE) {
//		if (speed.x >= ACC * dt)
//			speed.x -= ACC * dt;
//		else if (speed.x < -ACC * dt)
//			speed.x += ACC * dt;
//		else
//			speed.x = 0;
//	}
//	else {
//		speed.x += ACC * state.direction * dt;
//	}
//
//	if (speed.x > MAXSPEED)
//		speed.x = MAXSPEED;
//	if (speed.x < -MAXSPEED)
//		speed.x = -MAXSPEED;
//
//	//Rotation speed
//
//	if (state.turn == NONE) {
//		if (speedR.z >= ACCR * dt)
//			speedR.z -= ACCR * dt;
//		else if (speedR.z < -ACCR * dt)
//			speedR.z += ACCR * dt;
//		else
//			speedR.z = 0;
//	}
//	else {
//		speedR.z += ACCR * state.turn * dt;
//	}
//
//	if (speedR.z > MAXSPEEDR)
//		speedR.z = MAXSPEEDR;
//	if (speedR.z < -MAXSPEEDR)
//		speedR.z = -MAXSPEEDR;
//
//	//Gravity
//	if (getParentTransform()[3][2] < 0.5f)
//		speed.z = 0;
//	else
//		speed.z -= GRAVITY * dt;
//}