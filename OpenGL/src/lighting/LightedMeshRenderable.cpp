#include "./../../include/lighting/LightedMeshRenderable.hpp"
#include "./../../include/gl_helper.hpp"
#include "./../../include/log.hpp"
#include "./../../include/Io.hpp"
#include "./../../include/Utils.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>
#include <Hitbox.hpp>

LightedMeshRenderable::~LightedMeshRenderable()
{
}

LightedMeshRenderable::LightedMeshRenderable(ShaderProgramPtr shaderProgram, const std::vector< glm::vec3 >& positions, const std::vector<unsigned int>& indices,
                      const std::vector<glm::vec3>& normals, const std::vector<glm::vec2>& texCoords) :
	HierarchicalMeshRenderable(shaderProgram, positions, indices, normals, texCoords)
{
    m_colors.resize( m_positions.size(), glm::vec4(1.0,1.0,1.0,1.0) );
}

LightedMeshRenderable::LightedMeshRenderable( ShaderProgramPtr shaderProgram, const std::string& filename) :
	HierarchicalMeshRenderable(shaderProgram, filename)
{

}

void LightedMeshRenderable::do_draw()
{
    //Location
    int positionLocation = m_shaderProgram->getAttributeLocation("vPosition");
    int colorLocation = m_shaderProgram->getAttributeLocation("vColor");
    int normalLocation = m_shaderProgram->getAttributeLocation("vNormal");
    int modelLocation = m_shaderProgram->getUniformLocation("modelMat");
    int nitLocation = m_shaderProgram->getUniformLocation("NIT");

    //Send material uniform to GPU
    Material::sendToGPU(m_shaderProgram, m_material);

    //Send data to GPU
    if(modelLocation != ShaderProgram::null_location)
    {
        glcheck(glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(getModelMatrix())));
    }

    if(positionLocation != ShaderProgram::null_location)
    {
        //Activate location
        glcheck(glEnableVertexAttribArray(positionLocation));
        //Bind buffer
        glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_pBuffer));
        //Specify internal format
        glcheck(glVertexAttribPointer(positionLocation, 3, GL_FLOAT, GL_FALSE, 0, (void*)0));
    }

    if(colorLocation != ShaderProgram::null_location)
    {
        glcheck(glEnableVertexAttribArray(colorLocation));
        glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_cBuffer));
        glcheck(glVertexAttribPointer(colorLocation, 4, GL_FLOAT, GL_FALSE, 0, (void*)0));
    }

    if(normalLocation != ShaderProgram::null_location)
    {
        glcheck(glEnableVertexAttribArray(normalLocation));
        glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_nBuffer));
        glcheck(glVertexAttribPointer(normalLocation, 3, GL_FLOAT, GL_FALSE, 0, (void*)0));
    }

    if( nitLocation != ShaderProgram::null_location )
      {
        glcheck(glUniformMatrix3fv( nitLocation, 1, GL_FALSE,
          glm::value_ptr(glm::transpose(glm::inverse(glm::mat3(getModelMatrix()))))));
      }

    //Draw triangles elements
    glcheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iBuffer));
    glcheck(glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, (void*)0));

    if(positionLocation != ShaderProgram::null_location)
    {
        glcheck(glDisableVertexAttribArray(positionLocation));
    }

    if(colorLocation != ShaderProgram::null_location)
    {
        glcheck(glDisableVertexAttribArray(colorLocation));
    }

    if(normalLocation != ShaderProgram::null_location)
    {
        glcheck(glDisableVertexAttribArray(normalLocation));
    }
}

std::vector< glm::vec3 > & LightedMeshRenderable::positions()
{
    return m_positions;
}

const std::vector< glm::vec3 > & LightedMeshRenderable::positions() const
{
    return m_positions;
}

std::vector< unsigned int >& LightedMeshRenderable::indices()
{
    return m_indices;
}

const std::vector< unsigned int >& LightedMeshRenderable::indices() const
{
    return m_indices;
}

void LightedMeshRenderable::do_animate(float time) {}

void LightedMeshRenderable::setMaterial(const MaterialPtr& material)
{
    m_material = material;
}
