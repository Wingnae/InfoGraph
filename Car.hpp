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
	float vel_x, vel_y, vel_z;
	KeyframedMeshRenderablePtr body;
	KeyframedMeshRenderablePtr wheels[4];

public:
	Car(ShaderProgramPtr shader, Viewer& viewer);
	void handle_event();
	void move(int direction);
};