#pragma once

#include <ShaderProgram.hpp>
#include <Viewer.hpp>
#include <KeyframedMeshRenderable.hpp>

#define GEAR1		40.0f
#define GEARR		20.0f
#define STEERING	90.0f
#define MAXSTEER	45.0f

#define PI			3.141592653589793238463f
#define GRAVITY		9.81f
#define	CDRAG		5.0f
#define CRR			500.0f
#define CORNER		0.01f
#define BURN		5.0f
#define INERTIA		((FRONTCG * REARCG) * 3)

#define FRONTCG		0.98f
#define REARCG		1.15f

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
	void computeLateralForce();
	void computeTractionForce();
	void computeResistanceForce();
	void computeDragForce();
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
	glm::vec3 m_velocity;
	glm::vec3 m_acceleration;
	glm::vec3 m_angvelocity;
	glm::vec3 m_angacceleration;

	//Forces
	float m_FlatF;
	float m_FlatR;
	glm::vec3 m_Ftraction;
	glm::vec3 m_Frr;
	glm::vec3 m_Fdrag;
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
//1. V.x = Vlong, V.y = Vlat																			DONE
//2. Compute alpha : alphaF = atan((V.y + w*F)/V.x) - sigma * sign(V.x)		for front					DONE
//					 alphaR = atan((V.y - w*R)/V.x)							for rear					DONE
//3. FlatR = cornering * alphaR																			DONE
	//FlatF = cornering * alphaF																		DONE
//4. Cap Flat to max																					DONE
//5. Flat *= load																						DONE
//6.7.8.9.10 Compute engine torque Te																	DONE
	//6. TOR = Vx 60 * gk*G / (2PI * rw)																SKIPPED
	//7. Clamp the engine turn over rate from 6 to the defined redline									SKIPPED
	//8. If use automatic transmission call automaticTransmission() function							SKIPPED
	//to shift the gear																					SKIPPED
	//9. Compute the constant that define the torque curve line from the								SKIPPED
	//engine turn over rate																				SKIPPED
	//10. From 9, compute the maximum engine torque, Te													SKIPPED
//11. Tw = Te * gear																					DONE
//12. Ftraction = Tw / Wradius																			DONE
	//13. If the player is braking Ftraction = -brakes													DONE
//15. Frr = -Crr * V																					DONE
//16. Fdrag = -Cdrag * V * |V|																			DONE
//17. Ftotal.x = Ftraction + Flatf * sin(sigma) * Fresistance.x											DONE
	//Ftotal.y = Flatr + Flatf * cos(sigma) * Fresistance.y												DONE
	//Ftotal.z = Fg																						DONE
//18. Tcar = cos(sigma) * Flatf * b – Flatr * c															DONE
//19. Acc = Ftotal / m																					DONE
//20. angAcc = Tcar / I																					DONE
//21. V += dt * Acc																						DONE
//22. P += dt * V																						DONE
//23. Move the camera																					DONE
//24. angV += dt * angAcc																				DONE
//25. angP += dt * angV																					DONE
//26. Wrot = V / Wradius
//27. Render the car																					DONE

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
//Fg			=	m_Fg
//Ftotal		=	m_Ftotal