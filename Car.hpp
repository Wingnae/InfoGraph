#pragma once

#include <ShaderProgram.hpp>
#include <Viewer.hpp>
#include <KeyframedMeshRenderable.hpp>

////Old
//#define MAXSPEED	80.0f
//#define MAXSPEEDR	180.0f
//#define ACC			40.0f
//#define ACCR		360.0f

#define GRAVITY		9.81f
#define AERODRAG	10.0f
#define CRR			0.01f

#define NONE		0
#define FORWARD		1
#define BACKWARD	-1
#define LEFT		1
#define RIGHT		-1

class Car : public HierarchicalRenderable, public std::enable_shared_from_this<Car> {
public:
	Car(ShaderProgramPtr shader);
	void init();
	void do_keyPressedEvent(sf::Event& e);
	void do_keyReleasedEvent(sf::Event& e);
	void do_draw();
	void do_animate(float time);
	
	void computeTotalForce();
	void computeTractionForce();
	void computeBrakeForce();
	void computeDragForce();
	void computeResistanceForce();
	void computeGravity();

private:
	struct State {
		int direction;
		int turn;
	};

	const float m_mass;
	const float m_torque;
	const float m_torqueb;
	const float m_brakes;
	const float m_wradius;

	glm::vec3 m_velocity;
	glm::vec3 m_acceleration;

	glm::vec3 m_ftotal;
	glm::vec3 m_ftraction;
	glm::vec3 m_fbrake;
	glm::vec3 m_fdrag;
	glm::vec3 m_frr;
	glm::vec3 m_fg;

	KeyframedMeshRenderablePtr m_body;
	KeyframedMeshRenderablePtr m_wheels[4];
	float m_lastTime;
	State m_state;
};

typedef std::shared_ptr<Car> CarPtr;

//TODO
//Position					P = P + v * dt
//Velocity					V = V + a * dt
//Acceleration				A = F / M				
//
//Total Force				F = Ftraction + Fdrag + Frr + Fg
//						or	F = Fbrake + Fdrag + Frr + Fg
//
//Traction Force			Ftraction = Twheel / Rwheel
//Brake Force				Fbrake = -u * K
//Aerodynamic drag			Fdrag = -K * V^2
//Rolling resistance		Frr = -K * V
//Gravity					Fg = -m * G