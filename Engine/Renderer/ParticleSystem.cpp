#include "pch.h"
#include "ParticleSystem.h"
#include "Renderer.h"

namespace nu
{
	bool ParticleSystem::Initialize(size_t size)
	{
		// reserve # particles in container
		m_particles.resize(size);

		return true;
	}

	void ParticleSystem::Shutdown()
	{
		// clear all particles
		m_particles.clear();
	}

	void ParticleSystem::Update(float dt)
	{
		// update active particles
		for (auto& particle : m_particles)
		{
			if (!particle.active) continue;

			// reduce particle.lifespan by subtracting delta time
			particle.lifespan -= dt;

			// set active 
			if (particle.lifespan > 0) particle.active = true;
			else particle.active = false;

			// update position with velocity (multiply by dt)
			particle.position += particle.velocity * dt;
		}
	}

	void ParticleSystem::Draw(const Renderer& renderer)
	{
		// draw all active particlee
		for (auto& particle : m_particles)
		{
			if (particle.active)
			{
				if (particle.texture)
				{
					renderer.DrawTexture(*particle.texture.get(), particle.position.x, particle.position.y);
				}
				else
				{
					// set particle color and draw point at current position
					particle.color = { 1.0f, 1.0f, 1.0f };
					// TODO: set color with particle color
					renderer.SetColor(particle.color.r, particle.color.g, particle.color.b, 1.0f);
					// TODO: draw point with particle position
					renderer.DrawPoint(particle.position.x, particle.position.y);
				}
			}
		}
	}

	void ParticleSystem::AddParticle(const Particle& particle)
	{
		// get free particle
		Particle* freeParticle = GetFreeParticle();
		// check if free particle is not nullptr
		if (freeParticle)
		{
			// set free particle object with particle
			// TODO: dereference (*) free particle and set it to particle
			*freeParticle = particle;
			// set particle active
			// TODO: set free particle active to true
			freeParticle->active = true;
		}
	}

	Particle* ParticleSystem::GetFreeParticle()
	{
		// find and return inactive particle 
		for (auto& particle : m_particles)
		{
			// return pointer to inactive particle
			if (!particle.active)
				return &particle;
		}

		// no free particles, return nullptr
		return nullptr;
	}
}