#include "./../../include/dynamics/SpringForceField.hpp"

SpringForceField::SpringForceField(const ParticlePtr p1, const ParticlePtr p2, float stiffness, float equilibriumLength, float damping) :
    m_p1(p1),
    m_p2(p2),
    m_stiffness(stiffness),
    m_equilibriumLength(equilibriumLength),
    m_damping(damping)
{}

void SpringForceField::do_addForce()
{
    //TODO: Implement a damped spring
    //Functions to use:
    //glm::length( vec3 ): Return the length of a vector
    //glm::normalize( vec3 ): Return the normalization of a vector
    //Particle::getVelocity(), Particle::getForce(), Particle::setForce()
    //Nb:   Compute force ONLY IF the displacement length is above std::numeric_limits<float>::epsilon()
    //      Otherwise the computation is useless
	float length = glm::length(m_p1->getPosition() - m_p2->getPosition());
	glm::vec3 norm = glm::normalize(m_p1->getPosition() - m_p2->getPosition());
	glm::vec3 idealForce = -m_stiffness * (length - m_equilibriumLength) * norm;
	glm::vec3 damperForce = -m_damping * glm::dot(m_p1->getVelocity() - m_p2->getVelocity(), norm) * norm;

	if (length > std::numeric_limits<float>::epsilon()) {
		m_p1->incrForce(idealForce + damperForce);
		m_p2->incrForce(-idealForce - damperForce);
	}

    //1. Compute displacement vector
    //2. Compute displacement length
    //3. Compute spring force corresponding to the displacement 
}

ParticlePtr SpringForceField::getParticle1() const
{
    return m_p1;
}

ParticlePtr SpringForceField::getParticle2() const
{
    return m_p2;
}
