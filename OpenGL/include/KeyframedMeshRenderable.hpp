#pragma once

#include "HierarchicalRenderable.hpp"
#include "KeyframeCollection.hpp"

#include <glm/glm.hpp>

class GeometricTransformation;

class KeyframedMeshRenderable : public HierarchicalRenderable
{
public:
	~KeyframedMeshRenderable();
	KeyframedMeshRenderable(ShaderProgramPtr program, const std::string& filename);

	/**
	* \brief Add a keyframe for the local transformation of the renderable.
	*
	* Add a keyframe to m_localKeyframes described by a geometric transformation and a time.
	* \param transformation The geometric transformation of the keyframe.
	* \param time The time of the keyframe.
	*/
	void addLocalTransformKeyframe(const GeometricTransformation& transformation, float time);

	/**
	* \brief Add a keyframe for the parent transformation of the renderable.
	*
	* Add a keyframe to m_parentKeyframes described by a geometric transformation and a time.
	* \param transformation The geometric transformation of the keyframe.
	* \param time The time of the keyframe.
	*/
	void addParentTransformKeyframe(const GeometricTransformation& transformation, float time);

private:
	void do_draw();

	void do_animate(float time);

	std::vector< glm::vec3 > m_positions;
	std::vector< glm::vec4 > m_colors;
	std::vector< glm::vec3 > m_normals;
	std::vector< unsigned int > m_indices;

	KeyframeCollection m_localKeyframes; /*!< A collection of keyframes for the local transformation of renderable. */
	KeyframeCollection m_parentKeyframes; /*!< A collection of keyframes for the parent transformation of renderable. */

	unsigned int m_pBuffer;
	unsigned int m_cBuffer;
	unsigned int m_nBuffer;
	unsigned int m_iBuffer;
};

typedef std::shared_ptr<KeyframedMeshRenderable> KeyframedMeshRenderablePtr;
