#include "stdafx.h"

#define GLM_ENABLE_EXPERIMENTAL

#include <ShaderProgram.hpp>
#include <Viewer.hpp>
#include <Light.hpp>
#include <FrameRenderable.hpp>
#include <GeometricTransformation.hpp>
#include <KeyframedMeshRenderable.hpp>
#include <KeyframedCylinderRenderable.hpp>

#include "Car.hpp"
#include "World.hpp"

void movingTree(Viewer& viewer);
void movingCylinder(Viewer& viewer);

void initialize_scene(Viewer& viewer);

int main()
{
	srand(time(NULL));

	Viewer viewer(1280, 720);
	initialize_scene(viewer);

	while (viewer.isRunning())
	{
		viewer.handleEvent();
		viewer.animate();
		viewer.draw();
		viewer.display();
	}

	return EXIT_SUCCESS;
}

#define SHADER	lightShader

void initialize_scene(Viewer& viewer) {
	/*ShaderProgramPtr defaultShader = std::make_shared<ShaderProgram>("shaders/defaultVertex.glsl", "shaders/defaultFragment.glsl");
	ShaderProgramPtr lightShader = std::make_shared<ShaderProgram>("shaders/lightVertex.glsl", "shaders/lightFragment.glsl");
	viewer.addShaderProgram(SHADER);

	FrameRenderablePtr frame = std::make_shared<FrameRenderable>(SHADER);
	viewer.addRenderable(frame);

	CarPtr car = std::make_shared<Car>(SHADER, 1000.0f, 300.0f, 10000.0f);
	car->init();
	viewer.addRenderable(car);

	WorldPtr world = std::make_shared<World>(SHADER);
	world->init();
	viewer.addRenderable(world);

	viewer.startAnimation();*/
	movingTree(viewer);
}

//TP5
void movingCylinder(Viewer& viewer)
{
	//Add shader
	ShaderProgramPtr flatShader = std::make_shared<ShaderProgram>("shaders/defaultVertex.glsl", "shaders/defaultFragment.glsl");
	viewer.addShaderProgram(flatShader);

	//Frame
	FrameRenderablePtr frame = std::make_shared<FrameRenderable>(flatShader);
	viewer.addRenderable(frame);

	//Animated cylinder
	auto cylinder = std::make_shared<KeyframedCylinderRenderable>(flatShader);
	cylinder->setParentTransform(glm::mat4(1.0));

	viewer.getCamera().setViewMatrix(glm::lookAt(glm::vec3(-5, -5, 3), glm::vec3(0, 0, 0), glm::vec3(0, 0, 1)));

	GeometricTransformation none;
	GeometricTransformation t1(glm::vec3(0, 0, 6.0f));
	GeometricTransformation r1(glm::vec3(0, 0, 0), glm::rotate(glm::mat4(1.0), glm::radians(180.0f), glm::vec3(0, 0, 1)));
	GeometricTransformation r2(glm::vec3(0, 0, 0), glm::rotate(glm::mat4(1.0), glm::radians(360.0f), glm::vec3(0, 0, 1)));

	// TODO: Keyframes on parent transformation
	//cylinder->addParentTransformKeyframe(...);
	cylinder->addParentTransformKeyframe(none, 0.0f);
	cylinder->addParentTransformKeyframe(r1, 1.5f);
	cylinder->addParentTransformKeyframe(r2, 3.0f);

	viewer.addRenderable(cylinder);

	viewer.startAnimation();
	viewer.setAnimationLoop(true, 3.0);
}

void movingTree(Viewer& viewer)
{
	//Add shader
	ShaderProgramPtr flatShader = std::make_shared<ShaderProgram>("shaders/defaultVertex.glsl", "shaders/defaultFragment.glsl");
	viewer.addShaderProgram(flatShader);

	//Frame
	FrameRenderablePtr frame = std::make_shared<FrameRenderable>(flatShader);
	viewer.addRenderable(frame);

	viewer.getCamera().setViewMatrix(glm::lookAt(glm::vec3(10, 0, 5), glm::vec3(0, 0, 3), glm::vec3(0, 0, 1)));

	GeometricTransformation none;
	GeometricTransformation t1(glm::vec3(0, 0, 6.0f));

	GeometricTransformation r1(glm::vec3(0, 0, 0), glm::rotate(glm::mat4(1.0), glm::radians(180.0f), glm::vec3(0, 1, 0)));
	GeometricTransformation r2(glm::vec3(0, 0, 0), glm::rotate(glm::mat4(1.0), glm::radians(360.0f), glm::vec3(0, 1, 0)));

	// TODO: Create and animate the main branch
	KeyframedCylinderRenderablePtr root = std::make_shared<KeyframedCylinderRenderable>(flatShader);
	KeyframedCylinderRenderablePtr c1 = std::make_shared<KeyframedCylinderRenderable>(flatShader);
	c1->setParentTransform(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 2.0f, 0.0f)));
	// TODO: root->addParentTransformKeyframe(...)
	root->addParentTransformKeyframe(none, 0.0f);
	root->addParentTransformKeyframe(t1, 1.5f);
	root->addParentTransformKeyframe(none, 3.0f);

	//TODO: Add and animate a child branch
	c1->addLocalTransformKeyframe(none, 0.0f);
	c1->addLocalTransformKeyframe(r1, 1.5f);
	c1->addLocalTransformKeyframe(r2, 3.0f);

	HierarchicalRenderable::addChild(root, c1);

	viewer.addRenderable(root);

	viewer.startAnimation();
	viewer.setAnimationLoop(true, 3.0);
}
