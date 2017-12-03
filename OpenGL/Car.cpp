#include "stdafx.h"
#include "Car.hpp"
#include <math.h>
#include <Hitbox.hpp>

Car::Car(ShaderProgramPtr shader, float mass, float engine) : HierarchicalRenderable(shader), m_mass(mass), m_engine(engine) {
	//Car
	setParentTransform(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.5f)));

	//Frame
	m_frame = std::make_shared<HierarchicalMeshRenderable>(shader, "meshes/car.obj");
	m_frame->init();

	//Wheels
	for (int i = 0; i < 4; i++) {
		m_wheels[i] = std::make_shared<HierarchicalMeshRenderable>(shader, "meshes/wheel.obj");
		m_wheels[i]->init();
		HierarchicalRenderable::addChild(m_frame, m_wheels[i]);
	}

	m_wheels[0]->setParentTransform(glm::translate(glm::mat4(1.0f), glm::vec3(FRONTCG, 0.8, 0)));
	m_wheels[1]->setParentTransform(glm::translate(glm::mat4(1.0f), glm::vec3(FRONTCG, -0.8, 0)));
	m_wheels[2]->setParentTransform(glm::translate(glm::mat4(1.0f), glm::vec3(-REARCG, 0.8, 0)));
	m_wheels[3]->setParentTransform(glm::translate(glm::mat4(1.0f), glm::vec3(-REARCG, -0.8, 0)));

	m_state.direction = NONE;
	m_state.turn = NONE;

	m_lastTime = 0.0f;

	m_alphaF = 0.0f;
	m_alphaR = 0.0f;
	m_sigma = 0.0f;

	m_gear = 0.0f;
	m_torque = 0.0f;
	m_torquecar = 0.0f;
	m_wradius = 0.5f;
	m_wrotation = 0.0f;

	m_FlatF = 0.0f;
	m_FlatR = 0.0f;
	m_Ftraction = glm::vec3(0, 0, 0);
	m_Frr = glm::vec3(0, 0, 0);
	m_Fdrag = glm::vec3(0, 0, 0);
	m_Fg = glm::vec3(0, 0, 0);
	m_Ftotal = glm::vec3(0, 0, 0);

	reset();
}

void Car::init() {
	HierarchicalRenderable::addChild(shared_from_this(), m_frame);
	m_hitbox = m_frame->getHitbox();
	m_hitbox->updateModelMatrix();
	m_frame->getHitbox()->updateModelMatrix();
	for (auto w : m_wheels)
		w->getHitbox()->updateModelMatrix();
}

void Car::reset() {
	m_velocity = glm::vec3(0, 0, 0);
	m_acceleration = glm::vec3(0, 0, 0);
	m_angvelocity = glm::vec3(0, 0, 0);
	m_angacceleration = glm::vec3(0, 0, 0);
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
	m_wheels[0]->setParentTransform(glm::translate(glm::mat4(1.0f), glm::vec3(FRONTCG, 0.8, 0)));
	m_wheels[0]->setParentTransform(glm::rotate(m_wheels[0]->getParentTransform(), m_sigma, glm::vec3(0, 0, 1)));
	m_wheels[1]->setParentTransform(glm::translate(glm::mat4(1.0f), glm::vec3(FRONTCG, -0.8, 0)));
	m_wheels[1]->setParentTransform(glm::rotate(m_wheels[1]->getParentTransform(), m_sigma, glm::vec3(0, 0, 1)));

	for (int i = 0; i < 4; i++) {
		m_wheels[i]->setLocalTransform(glm::rotate(m_wheels[i]->getLocalTransform(), m_wrotation, glm::vec3(0, 1, 0)));
	}

	m_viewer->getCamera().setViewMatrix(glm::lookAt(
		glm::vec3(getParentTransform()[3]
			- 15.0f * cos(-m_angvelocity.z/5) * getParentTransform()[0]
			+ 15.0f * sin(-m_angvelocity.z/5) * getParentTransform()[1])
			+ glm::vec3(0, 0, 7.5f),
		glm::vec3(getParentTransform()[3]),
		glm::vec3(0, 0, 1)));;
}

void Car::do_animate(float time) {
	float dt = time - m_lastTime;
	m_lastTime = time;

	float old_vel_x = m_velocity.x;

	//Wheel orientation
	if (m_state.turn == NONE) {
		if (abs(m_sigma) > abs(m_angvelocity.z * dt) && m_velocity.x > 0)
			m_sigma += m_angvelocity.z * dt / 2;
		else if (abs(m_sigma) > abs(m_angvelocity.z * dt) && m_velocity.x < 0)
			m_sigma -= m_angvelocity.z * dt / 2;
	}
	else {
		m_sigma += m_state.turn * glm::radians(STEERING) * dt;
		if (m_sigma >  glm::radians(MAXSTEER))
			m_sigma = glm::radians(MAXSTEER);
		else if (m_sigma < -glm::radians(MAXSTEER))
			m_sigma = -glm::radians(MAXSTEER);
	}

	computeTotalForce();

	//Rotation
	m_torquecar = cos(m_sigma) * m_FlatF * FRONTCG - m_FlatR * REARCG;
	m_angacceleration.z = m_torquecar / INERTIA;
	m_angvelocity.z += m_angacceleration.z * dt;

	//Translation
	m_acceleration = m_Ftotal / m_mass;
	m_velocity += m_acceleration * dt;

	m_wrotation = m_velocity.x / m_wradius * dt;

	if (old_vel_x * m_velocity.x < 0.0f)
		reset();

	//Update model
	setParentTransform(glm::translate(getParentTransform(), m_velocity * dt));
	setParentTransform(glm::rotate(getParentTransform(), -m_angvelocity.z * dt, glm::vec3(0, 0, 1)));

	//std::cout << "TX :" << m_velocity.x << "\t/ TY :" << m_velocity.y << "\t/ RZ :" << m_angvelocity.z << "\tSigma :" << m_sigma << std::endl;
}

void Car::computeTotalForce() {
	computeTractionForce();
	computeLateralForce();
	computeDragForce();
	computeResistanceForce();
	//computeGravity();

	m_Ftotal = m_Ftraction + m_Fdrag + m_Frr;
	if (abs(m_velocity.x) > 10.0f) {
		m_Ftotal.x += m_FlatF * sin(m_sigma);
		m_Ftotal.y += m_FlatR + m_FlatF * cos(m_sigma);
	}
}

void Car::computeTractionForce() {
	if (m_state.direction == NONE) {
		if (m_velocity.x > 0)
			m_Ftraction.x = -BRAKES / 10;
		else if (m_velocity.x < 0)
			m_Ftraction.x = BRAKES / 10;
		return;
	}

	if (m_state.direction == FORWARD && m_velocity.x >= 0)
		m_gear = GEAR1;
	else if (m_state.direction == BACKWARD && m_velocity.x <= 0)
		m_gear = GEARR;
	else {
		m_Ftraction.x = m_state.direction * BRAKES;
		return;
	}
	m_torque = m_state.direction * m_gear * m_engine;
	m_Ftraction.x = m_torque / m_wradius;
}

void Car::computeLateralForce() {
	if (m_velocity.x > 0.0f) {
		m_alphaF = atan((-m_velocity.y - m_angvelocity.z * FRONTCG) / m_velocity.x) - m_sigma;
		m_alphaR = atan((-m_velocity.y + m_angvelocity.z * REARCG) / m_velocity.x);
	}
	else if(m_velocity.x < 0.0f){
		m_alphaF = -atan((-m_velocity.y - m_angvelocity.z * FRONTCG) / m_velocity.x) + m_sigma;
		m_alphaR = -atan((-m_velocity.y + m_angvelocity.z * REARCG) / m_velocity.x);
	}
	else {
		m_alphaF = 0.0f;
		m_alphaR = 0.0f;
	}

	m_FlatF = m_alphaF * CORNER;
	m_FlatR = m_alphaR * CORNER;

	m_FlatF *= m_mass * GRAVITY * FRONTCG / (FRONTCG + REARCG);
	m_FlatR *= m_mass * GRAVITY * REARCG / (FRONTCG + REARCG);
}

void Car::computeResistanceForce() {
	m_Frr = -CRR * m_velocity;
}

void Car::computeDragForce() {
	m_Fdrag = -CDRAG * m_velocity * glm::length(m_velocity);
}

void Car::computeGravity() {
	m_Fg.z = -m_mass * GRAVITY;
}