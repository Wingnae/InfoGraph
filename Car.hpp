#pragma once

#include <ShaderProgram.hpp>
#include <Viewer.hpp>
#include <KeyframedMeshRenderable.hpp>

#define MAXSPEED	80.0f
#define MAXSPEEDR	180.0f
#define ACC			40.0f
#define ACCR		360.0f
#define GRAVITY		9.81f

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
	void move(float dt);

private:
	struct State {
		int direction;
		int turn;
	};

	glm::vec3 speed;
	glm::vec3 speedR;
	KeyframedMeshRenderablePtr body;
	KeyframedMeshRenderablePtr wheels[4];
	float lastTime;
	State state;
};

typedef std::shared_ptr<Car> CarPtr;