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
#define	CDRAG		10.0f
#define CRR			0.01f

#define NONE		0
#define FORWARD		1
#define BACKWARD	-1
#define LEFT		1
#define RIGHT		-1

class Car : public HierarchicalRenderable, public std::enable_shared_from_this<Car> {
public:
	Car(ShaderProgramPtr shader, float mass, float engine, float brakes);
	void init();
	void do_keyPressedEvent(sf::Event& e);
	void do_keyReleasedEvent(sf::Event& e);
	void do_draw();
	void do_animate(float time);
	
	void computeTotalForce();
	void computeTractionForce();
	void computeDragForce();
	void computeResistanceForce();
	void computeGravity();

private:
	struct State {
		int direction;
		int turn;
	};

	//Consts
	const float m_mass;
	const float m_engine;
	const float m_brakes;

	//Angles
	float m_alphaF;
	float m_alphaR;
	float m_sigma;
	
	//Traction
	float m_gear;
	float m_torque;

	//Wheels
	float m_wradius;
	float m_wrotation;
	float m_torquecar;

	//Movement
	glm::vec3 m_position;
	glm::vec3 m_velocity;
	glm::vec3 m_acceleration;
	glm::vec3 m_angacceleration;
	glm::vec3 m_angvelocity;
	glm::vec3 m_angposition;

	//Forces
	glm::vec3 m_FlatF;
	glm::vec3 m_FlatR;
	glm::vec3 m_Frr;
	glm::vec3 m_Fdrag;
	glm::vec3 m_Ftraction;
	glm::vec3 m_Fresistance;
	glm::vec3 m_Fg;
	glm::vec3 m_Ftotal;

	KeyframedMeshRenderablePtr m_frame;
	KeyframedMeshRenderablePtr m_wheels[4];
	float m_lastTime;
	State m_state;
};

typedef std::shared_ptr<Car> CarPtr;

//---------------------------NOTES---------------------------
//
//---------------------------LINEAR--------------------------
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
//
//--------------------------CORNERING-----------------------
//
//Cornering					Fcornering = Ca * alpha
//Slip angle front			alpha = atan((Vlat + w*b)/Vlong) - sigma * sign(Vlong)		b = distance between CG and front axle
//																						sigma = steering angle
//																						w = angular vel
//Slip angle rear			alpha = atan((Vlat - w*c)/Vlong)							c = distance between CG and rear axle
//Resulting torque			T = cos(sigma) * Flat,front * b - Flat,rear * C
//
//Angular acc				wacc = T / inertia
//Angular vel				w = w + wacc * dt
//Angular pos				w. = w. + w * dt


//------------------------ALGORITHM------------------------
//
//1. V.x = Vlong, V.y = Vlat
//2. Compute alpha : alphaF = atan((V.y + w*F)/V.x) - sigma * sign(V.x)		for front
//					 alphaR = atan((V.y - w*R)/V.x)							for rear
//3. FlatR = cornering * alphaR
	//FlatF = cornering * alphaF
//4. Cap Flat to max
//5. Flat *= load
//6.7.8.9.10 Compute engine torque Te
	//6. TOR = Vx 60 * gk*G / (2PI * rw)
	//7. Clamp the engine turn over rate from 6 to the defined redline
	//8. If use automatic transmission call automaticTransmission() function
	//to shift the gear
	//9. Compute the constant that define the torque curve line from the
	//engine turn over rate
	//10. From 9, compute the maximum engine torque, Te
//11. Tw = Te * gear
//12. Ftraction = Tw / Wradius
	//13. If the player is braking Ftraction = -brakes
//15. Frr = -Crr * V
//16. Fdrag = -Cdrag * V * |V|
//17. Fresistance = Frr + Fdrag
//18. Ftotal.x = Ftraction + Flatf * sin(sigma) * Fresistance.x
	//Ftotal.y = Flatr + Flatf * cos(sigma) * Fresistance.y
	//Ftotal.z = Fg
//19. Tcar = cos(sigma) * Flatf * b – Flatr * c
//20. Acc = Ftotal / m																					DONE
//21. angAcc = Tcar / I																					
//22. V += dt * Acc																						DONE
//23. P += dt * V																						DONE
//24. Move the camera																					DONE
//25. angV += dt * angAcc																				DONE
//26. angP += dt * angV																					DONE
//27. Wrot = V / Wradius
//28. Render the car																					DONE

//-----------------------GLOSSARY-----------------------
//
//P				=	m_position
//V				=	m_velocity
//Acc			=	m_acceleration
//angAcc		=	m_angacceleration
//angV			=	m_angvelocity
//angP			=	m_angposition
//
//alphaF		=	m_alphaF
//alphaR		=	m_alphaR
//sigma			=	m_sigma
//m				=	m_mass
//load			=	m_mass/2
//Te			=	m_engine
//gear			=	m_gear
//Tw			=	m_torque
//brakes		=	m_brakes
//Tcar			=	m_torquecar
//Wradius		=	m_wradius
//Wrot			=	m_wrotation
//
//Crr			=	CRR
//Cdrag			=	CDRAG
//G				=	GRAVITY
//
//FlatF			=	m_FlatF
//FlatR			=	m_FlatR
//Frr			=	m_Frr
//Fdrag			=	m_Fdrag
//Ftraction		=	m_Ftraction
//Fresistance	=	m_Fresistance
//Fg			=	m_Fg
//Ftotal		=	m_Ftotal