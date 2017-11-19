#pragma once

#include <ShaderProgram.hpp>
#include <Viewer.hpp>
#include <KeyframedMeshRenderable.hpp>

class Car : public HierarchicalRenderable, public std::enable_shared_from_this<Car> {
private:
	glm::vec3 pos;
	glm::vec3 vel;
	KeyframedMeshRenderablePtr body;
	KeyframedMeshRenderablePtr wheels[4];
	float lastTime;

public:
	Car(ShaderProgramPtr shader);
	void init();
	void do_keyPressedEvent(sf::Event& e);
	void do_keyReleasedEvent(sf::Event& e);

	void do_draw();
	void do_animate(float time);
};

typedef std::shared_ptr<Car> CarPtr;