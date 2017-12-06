#ifndef LIGHTED_MESH_RENDERABLE_HPP
#define LIGHTED_MESH_RENDERABLE_HPP

#include "./../lighting/Material.hpp"
#include "./../lighting/Light.hpp"

#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <HierarchicalMeshRenderable.hpp>

class LightedMeshRenderable : public HierarchicalMeshRenderable
{
    public:
        ~LightedMeshRenderable();
        LightedMeshRenderable(ShaderProgramPtr program, const std::string& filename);
        LightedMeshRenderable(ShaderProgramPtr shaderProgram, const std::vector< glm::vec3 >& positions, const std::vector<unsigned int>& indices,
                              const std::vector<glm::vec3>& normals, const std::vector<glm::vec2>& texCoords);
        void setMaterial(const MaterialPtr& material);
        std::vector< glm::vec3 > & positions();
        const std::vector< glm::vec3 > & positions() const;
        std::vector< unsigned int >& indices();
        const std::vector< unsigned int >& indices() const;

    private:
        void do_draw();
        void do_animate( float time );

        MaterialPtr m_material;
};

typedef std::shared_ptr<LightedMeshRenderable> LightedMeshRenderablePtr;

#endif
