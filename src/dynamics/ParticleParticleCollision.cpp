#include "./../../include/dynamics/ParticleParticleCollision.hpp"
#include <glm/gtx/norm.hpp>

ParticleParticleCollision::~ParticleParticleCollision()
{

}

ParticleParticleCollision::ParticleParticleCollision(ParticlePtr particle1, ParticlePtr particle2, float restitution) :
    Collision(restitution)
{
    m_p1 = particle1;
    m_p2 = particle2;
}

void ParticleParticleCollision::do_solveCollision()
{
    //Don't process fixed particles (Let's assume that the ground plane is fixed)
    if (m_p1->isFixed() && m_p2->isFixed()) return;

    //Compute interpenetration distance
    float particleParticleDist = glm::distance(m_p1->getPosition(),m_p2->getPosition());
    float interpenetrationDist = m_p1->getRadius()+m_p2->getRadius()-particleParticleDist;

    //Compute particle-particle vector
    glm::vec3 k = glm::normalize(m_p1->getPosition()-m_p2->getPosition());

    //Project each particle along the particle-particle vector with half of the interpenetration distance
    //To be more precise, we ponderate the distance with the mass of the particle
    glm::vec3 prev_x1 = m_p1->getPosition();
    glm::vec3 prev_x2 = m_p2->getPosition();
    if(m_p1->isFixed())
    {
        m_p2->setPosition(prev_x2 - interpenetrationDist*k);
    }
    else if(m_p2->isFixed())
    {
        m_p1->setPosition(prev_x1 + interpenetrationDist*k);
    }
    else
    {
        float c1 = m_p1->getMass()/(m_p1->getMass()+m_p2->getMass());
        float c2 = m_p2->getMass()/(m_p1->getMass()+m_p2->getMass());
        m_p1->setPosition(prev_x1 + c2*interpenetrationDist*k);
        m_p2->setPosition(prev_x2 - c1*interpenetrationDist*k);
    }

    //Compute post-collision velocity
    glm::vec3 prev_v1 = m_p1->getVelocity();
    glm::vec3 prev_v2 = m_p2->getVelocity();
    float proj_v = (1.0f+m_restitution)*glm::dot(k, prev_v1-prev_v2)/(1.0 / m_p1->getMass() + 1.0 / m_p2->getMass());
    glm::vec3 new_v1 = prev_v1 - proj_v/m_p1->getMass()*k;
    glm::vec3 new_v2 = prev_v2 + proj_v/m_p2->getMass()*k;
    m_p1->setVelocity(new_v1);
    m_p2->setVelocity(new_v2);
}


bool testParticleParticle(const ParticlePtr &p1, const ParticlePtr &p2)
{
    /* p1( c1, r1 ) and p2( c2, r2 ) particles
   * intersection if || c2 - c1 || <= r1 + r2.
   * We avoid as much as possible to compute of a square root, so we check if
   * dot( c2 - c1, c2 - c1 ) <= (r1 + r2)²
   */

    //Vector between sphere centers
    if(p1==p2) return false;
    //Sum of sphere radii
    float r = p1->getRadius() + p2->getRadius();
    float c = glm::distance2(p1->getPosition(),p2->getPosition()) - r*r;
    return (c<0.0f) ? true : false;
}
