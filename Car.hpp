#pragma once

#include <ShaderProgram.hpp>
#include <Viewer.hpp>
#include <KeyframedMeshRenderable.hpp>

#define FORWARD		1
#define BACKWARD	2
#define LEFT		3
#define RIGHT		4

class Car {
private:
	glm::vec3 pos;
	glm::vec3 vel;
	KeyframedMeshRenderablePtr body;
	KeyframedMeshRenderablePtr wheels[4];
	Viewer* viewer;

public:
	Car(ShaderProgramPtr shader, Viewer& viewer);
	void handle_event();
	void move(int direction);
};