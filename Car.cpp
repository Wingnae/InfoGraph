#include "stdafx.h"
#include "Car.hpp"

Car::Car(ShaderProgramPtr shader, float mass, float engine, float brakes) : HierarchicalRenderable(shader), m_mass(mass), m_engine(engine), m_brakes(brakes) {
	//Car
	setParentTransform(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
	setLocalTransform(glm::mat4(1.0f));

	//Frame
	m_frame = std::make_shared<KeyframedMeshRenderable>(shader, "meshes/car.obj");
	m_frame->setParentTransform(glm::mat4(1.0));
	m_frame->setLocalTransform(glm::mat4(1.0));

	//Wheels
	for (int i = 0; i < 4; i++) {
		m_wheels[i] = std::make_shared<KeyframedMeshRenderable>(shader, "meshes/wheel.obj");
		m_wheels[i]->setLocalTransform(glm::mat4(1.0));
		HierarchicalRenderable::addChild(m_frame, m_wheels[i]);
	}

	m_wheels[0]->setParentTransform(glm::translate(glm::mat4(1.0f), glm::vec3(0.98, 0.8, 0)));
	m_wheels[1]->setParentTransform(glm::translate(glm::mat4(1.0f), glm::vec3(0.98, -0.8, 0)));
	m_wheels[2]->setParentTransform(glm::translate(glm::mat4(1.0f), glm::vec3(-1.15, 0.8, 0)));
	m_wheels[3]->setParentTransform(glm::translate(glm::mat4(1.0f), glm::vec3(-1.15, -0.8, 0)));
	
	m_state.direction = NONE;
	m_state.turn = NONE;

	m_lastTime = 0;

	//Angles
	m_alphaF = 0.0f;
	m_alphaR = 0.0f;
	m_sigma = 0.0f;

	//Torque
	m_gear = 10.0f;
	m_torque = 0.0f;

	//Wheels
	m_wradius = 0.5f;
	m_wrotation = 0.0f;
	m_torquecar = 0.0f;

	//Movement
	//m_position = glm::vec3(0, 0, 1);
	m_velocity = glm::vec3(0, 0, 0);
	m_acceleration = glm::vec3(0, 0, 0);
	//m_angposition = glm::vec3(0, 0, 0);
	m_angvelocity = glm::vec3(0, 0, 0);
	m_angacceleration = glm::vec3(0, 0, 0);

	//Forces
	m_FlatF = glm::vec3(0, 0, 0);
	m_FlatR = glm::vec3(0, 0, 0);
	m_Frr = glm::vec3(0, 0, 0);
	m_Fdrag = glm::vec3(0, 0, 0);
	m_Ftraction = glm::vec3(0, 0, 0);
	m_Fresistance = glm::vec3(0, 0, 0);
	m_Fg = glm::vec3(0, 0, 0);
	m_Ftotal = glm::vec3(0, 0, 0);
}

void Car::init() {
	HierarchicalRenderable::addChild(shared_from_this(), m_frame);
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

	float dt = time - m_lastTime;
	m_lastTime = time;

	m_acceleration = m_Ftotal / m_mass;
	m_velocity += m_acceleration * dt;
	//m_position += m_velocity * dt;

	m_angacceleration.z = m_torquecar / m_mass;
	m_angvelocity.z += m_angacceleration.z * dt;
	//m_angposition.z += m_angvelocity.z * dt;

	std::cout << "speed :" << ceil(m_velocity.x * 3.6f) << "kmh /  fps:" << ceil(1.0f / dt) << std::endl;

	setParentTransform(glm::translate(getParentTransform(), m_velocity * dt));
	setParentTransform(glm::rotate(getParentTransform(), m_angvelocity.z  * dt, glm::vec3(0, 0, 1)));
	m_viewer->getCamera().setViewMatrix(glm::lookAt(glm::vec3(getParentTransform()[3]) + glm::vec3(-15.0f, 5.0f, 7.5f), glm::vec3(getParentTransform()[3]), glm::vec3(0, 0, 1)));
}

void Car::computeTotalForce() {
	computeDragForce();
	computeResistanceForce();
	//computeGravity();
	m_Ftotal = m_Fdrag + m_Frr + m_Fg;

	if (m_state.direction == FORWARD && m_velocity.x >= 0) {
		m_gear = 10.0f;
		computeTractionForce();
		m_Ftotal.x += m_Ftraction.x;
	}
	else if (m_state.direction == BACKWARD && m_velocity.x <= 0) {
		m_gear = 2.0f;
		computeTractionForce();
		m_Ftotal.x += m_Ftraction.x;
	}
	else if (m_state.direction != NONE) {
		m_Ftotal.x += m_state.direction * m_brakes;
	}
}

void Car::computeTractionForce() {
	m_torque = m_state.direction * m_gear * m_engine;
	m_Ftraction.x = m_torque / m_wradius;
}

void Car::computeDragForce() {
	m_Fdrag = -CDRAG * m_velocity * glm::length(m_velocity);
}

void Car::computeResistanceForce() {
	m_Frr = -CRR * m_velocity;
}

void Car::computeGravity() {
	m_Fg.z = -m_mass * GRAVITY;
}