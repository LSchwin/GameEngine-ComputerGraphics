#include "Engine/Engine.h"
#include "Bomb.h"
#include "Math/MathUtils.h"
#include "Assets.h"

#include "Renderer/Texture.h"
#include "Resources/ResourceManager.h"

FACTORY_REGISTER(Bomb)

void Bomb::Update(float dt)
{
    if (!m_exploded)
    {
        nu::Vector2 forward{ 0.2f, 0.0f };
        nu::Vector2 velocity = forward.Rotate(m_transform.rotation * nu::DegToRad) * m_speed;

        //SetVelocity(velocity);
    }
    else 
    {
        //SetVelocity({0.0f, 0.0f});
    }
	
	Actor::Update(dt);
}

void Bomb::OnCollision(Actor* other)
{
    if (other->GetTag() == "Enemy")
    {
        if (!m_exploded) m_transform.scale *= 3.0f;
        m_exploded = true;
        m_lifespan = 1.0f;

        nu::Engine::Get().GetAudio().PlaySound("BombExplode");
    }
}

void Bomb::Draw(const nu::Renderer& renderer) const
{
    Actor::Draw(renderer);
}

void Bomb::Read(const nu::json::value_t& value)
{
    Actor::Read(value);

    JSON_READ_NAME(value, "speed", m_speed);
    JSON_READ_NAME(value, "exploded", m_exploded);
}
