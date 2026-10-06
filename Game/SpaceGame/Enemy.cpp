#include "Enemy.h"
#include "Player.h"
#include "Renderer/Renderer.h"
#include "Engine/Engine.h"
#include "SpaceGame.h"
#include "Core/Random.h"
#include "AmmoPickup.h"
#include "Assets.h"

#include "Components/PhysicsComponent.h"

#include <iostream>

FACTORY_REGISTER(Enemy)

void Enemy::Update(float dt)
{
    Player* player = m_scene->GetActorByName<Player>("PlayerPrototype");
    if (player)
    {
        auto physicsComponent = GetComponent<nu::PhysicsComponent>();
        if (physicsComponent)
        {
            nu::Vector2 direction = player->GetTransform().position - m_transform.position;
            float rotate = direction.Angle();
            
            nu::Vector2 forward{ 1, 0 };
            nu::Vector2 velocity = forward.Rotate(m_transform.rotation * nu::DegToRad) * m_speed;
            //forward = forward.Rotate(m_transform.rotation * nu::DegToRad); //here problem

            physicsComponent->ApplyForce(forward);
            physicsComponent->SetRotation(rotate);
        }
    }

    //AddVelocity(velocity * dt);

    Actor::Update(dt);
}

void Enemy::OnCollision(Actor* other)
{   
    if (other->GetTag() == "PlayerBullet")
    {
        for (int i = 0; i < 100; i++)
        {
            nu::Particle particle;
            particle.position = m_transform.position;
            //particle.color = { 1.0f, 1.0f, 1.0f };
            particle.lifespan = nu::RandomFloat(0.5f, 2.0f);
            particle.velocity = { nu::RandomFloat(-600.0f, 600.0f), nu::RandomFloat(-600.0f, 600.0f) };

            particle.texture = nu::Resources().Get<nu::Texture>("textures/particle.png", nu::Engine::Get().GetRenderer());

            nu::Engine::Get().GetPS().AddParticle(particle);
        }

        if (nu::RandomInt(3) != 10000)
        {
            auto pickup = nu::Factory::Instance().Create<AmmoPickup>("AmmoPickupPrototype");
            if (pickup) 
            {
                pickup->SetPosition(m_transform.position);
                m_scene->AddActor(std::move(pickup));
            }
        }

        SetDestroyed();
        other->SetDestroyed();

        ((SpaceGame*)m_scene->GetGame())->AddPoints(100);
    }
    else if (other->GetTag() == "PlayerBomb")
    {
        for (int i = 0; i < 100; i++)
        {
            nu::Particle particle;
            particle.position = m_transform.position;
            //particle.color = { 1.0f, 1.0f, 1.0f };
            particle.lifespan = nu::RandomFloat(0.5f, 2.0f);
            particle.velocity = { nu::RandomFloat(-600.0f, 600.0f), nu::RandomFloat(-600.0f, 600.0f) };
            particle.texture = nu::Resources().Get<nu::Texture>("textures/particle2.png", nu::Engine::Get().GetRenderer());

            nu::Engine::Get().GetPS().AddParticle(particle);
        }

        SetDestroyed();

        ((SpaceGame*)m_scene->GetGame())->AddPoints(100);
    }
}

void Enemy::Draw(const nu::Renderer& renderer) const
{
    Actor::Draw(renderer);
}

void Enemy::Read(const nu::json::value_t& value)
{
    Actor::Read(value);

    JSON_READ_NAME(value, "speed", m_speed);
}