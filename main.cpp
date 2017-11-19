#include "stdafx.h"

#define GLM_ENABLE_EXPERIMENTAL

#include <ShaderProgram.hpp>
#include <Viewer.hpp>
#include <Light.hpp>
#include <FrameRenderable.hpp>
#include <GeometricTransformation.hpp>
#include <KeyframedMeshRenderable.hpp>

#include "Car.hpp"
#include "World.hpp"

//void movingCar(Viewer& viewer);
//void movingTree(Viewer& viewer);
//void movingCylinder(Viewer& viewer);

void initialize_scene(Viewer& viewer);

int main()
{
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

void initialize_scene(Viewer& viewer) {
	ShaderProgramPtr flatShader = std::make_shared<ShaderProgram>("shaders/flatVertex.glsl", "shaders/flatFragment.glsl");
	viewer.addShaderProgram(flatShader);

	FrameRenderablePtr frame = std::make_shared<FrameRenderable>(flatShader);
	viewer.addRenderable(frame);

	CarPtr car = std::make_shared<Car>(flatShader);
	car->init();
	viewer.addRenderable(car);

	WorldPtr world = std::make_shared<World>(flatShader);
	world->init();
	viewer.addRenderable(world);

	viewer.startAnimation();
}

//TP5
//
//void movingCar(Viewer& viewer)
//{
//	//Add shader
//	ShaderProgramPtr flatShader = std::make_shared<ShaderProgram>("shaders/flatVertex.glsl", "shaders/flatFragment.glsl");
//	viewer.addShaderProgram(flatShader);
//
//	//Frame
//	FrameRenderablePtr frame = std::make_shared<FrameRenderable>(flatShader);
//	viewer.addRenderable(frame);
//
//	//Animated cylinder
//	auto car = std::make_shared<KeyframedMeshRenderable>(flatShader, "meshes/car.obj");
//	//car->setParentTransform(glm::mat4(1.0));
//
//	GeometricTransformation none;
//	GeometricTransformation t1(glm::vec3(0, 0, 6.0f));
//	GeometricTransformation r1(glm::vec3(0, 0, 0), glm::rotate(glm::mat4(1.0), glm::radians(180.0f), glm::vec3(0, 1, 0)));
//	GeometricTransformation r2(glm::vec3(0, 0, 0), glm::rotate(glm::mat4(1.0), glm::radians(360.0f), glm::vec3(0, 1, 0)));
//
//	// TODO: Keyframes on parent transformation
//	//cylinder->addParentTransformKeyframe(...);
//	car->addLocalTransformKeyframe(none, 0.0f);
//	car->addLocalTransformKeyframe(r1, 2.5f);
//	car->addLocalTransformKeyframe(r2, 5.0f);
//
//	viewer.addRenderable(car);
//
//	viewer.startAnimation();
//	viewer.setAnimationLoop(true, 5.0);
//}
//
//void movingCylinder(Viewer& viewer)
//{
//	//Add shader
//	ShaderProgramPtr flatShader = std::make_shared<ShaderProgram>("shaders/flatVertex.glsl", "shaders/flatFragment.glsl");
//	viewer.addShaderProgram(flatShader);
//
//	//Frame
//	FrameRenderablePtr frame = std::make_shared<FrameRenderable>(flatShader);
//	viewer.addRenderable(frame);
//
//	viewer.getCamera().setViewMatrix(glm::lookAt(glm::vec3(0, -8, 4), glm::vec3(0, 0, 4), glm::vec3(0, 0, 1)));
//
//	//Animated cylinder
//	auto cylinder = std::make_shared<KeyframedCylinderRenderable>(flatShader);
//	cylinder->setParentTransform(glm::mat4(1.0));
//
//	GeometricTransformation none;
//	GeometricTransformation t1(glm::vec3(0, 0, 6.0f));
//	GeometricTransformation r1(glm::vec3(0, 0, 0), glm::rotate(glm::mat4(1.0), glm::radians(180.0f), glm::vec3(1, 1, 1)));
//	GeometricTransformation r2(glm::vec3(0, 0, 0), glm::rotate(glm::mat4(1.0), glm::radians(360.0f), glm::vec3(1, 1, 1)));
//
//	// TODO: Keyframes on parent transformation
//	//cylinder->addParentTransformKeyframe(...);
//	cylinder->addParentTransformKeyframe(none, 0.0f);
//	cylinder->addParentTransformKeyframe(r1, 0.5f);
//	cylinder->addParentTransformKeyframe(r2, 1.0f);
//
//	// TODO: Keyframes on local transformation
//	//cylinder->addLocalTransformKeyframe(...);
//	cylinder->addLocalTransformKeyframe(none, 0.0f);
//	cylinder->addLocalTransformKeyframe(t1, 0.5f);
//	cylinder->addLocalTransformKeyframe(none, 1.0f);
//
//	viewer.addRenderable(cylinder);
//
//	viewer.startAnimation();
//	viewer.setAnimationLoop(true, 1.0);
//}
//
//void movingTree(Viewer& viewer)
//{
//	//Add shader
//	ShaderProgramPtr flatShader = std::make_shared<ShaderProgram>("shaders/flatVertex.glsl", "shaders/flatFragment.glsl");
//	viewer.addShaderProgram(flatShader);
//
//	//Frame
//	FrameRenderablePtr frame = std::make_shared<FrameRenderable>(flatShader);
//	viewer.addRenderable(frame);
//
//	//Tree modeling:
//	//The modeling is hierarchical (setLocalTransform, setParentTransform)
//	//The animation is hierarchical too (addParentTransformKeyframe, addLocalTransformKeyframe)
//
//	// TODO: Create and animate the main branch 
//	KeyframedCylinderRenderablePtr root = std::make_shared<KeyframedCylinderRenderable>(flatShader);
//	root->setLocalTransform(GeometricTransformation(glm::vec3{}, glm::quat(), glm::vec3{ 0.1,0.1,2.0 }).toMatrix());
//	// TODO: root->addParentTransformKeyframe(...)
//	// ...
//
//	//TODO: Add and animate a child branch
//	KeyframedCylinderRenderablePtr r1 = std::make_shared<KeyframedCylinderRenderable>(flatShader);
//	//r1->addParentTransformKeyframe(...);
//	// ...
//	//HierarchicalRenderable::addChild( root, r1 );
//	// ...
//
//	// TODO: Add and animate any other child branchs you want
//
//	viewer.addRenderable(root);
//
//	viewer.startAnimation();
//	viewer.setAnimationLoop(true, 6.0);
//}
