#include "FlyingEnemyController.h"
#include "Components/PhysicsComponent.h"
#include "Components/SpriteAnimatorRendererComponent.h"
#include "Engine/Engine.h"

FACTORY_REGISTER(FlyingEnemyController)

void FlyingEnemyController::Start()
{
	CharacterBase::Start();

	m_physicsComponent = GetComponent<nu::PhysicsComponent>();
	assert(m_physicsComponent);
	m_rendererComponent = GetComponent<nu::SpriteAnimatorRendererComponent>();
	assert(m_rendererComponent);
}

void FlyingEnemyController::Update(float dt)
{
	nu::Vector2 velocity = m_physicsComponent->GetVelocity();

	float dir = 0;
	
	auto player = m_scene->GetActorByName("PlayerPrototype");
	if (player)
	{
		nu::Vector2 position = GetTransform().position;
		nu::Vector2 playerPosition = player->GetTransform().position;

		if (playerPosition.x < position.x) dir = -1.0f;
		else dir = +1.0f;
		

		nu::Vector2 direction = playerPosition - position;
		m_physicsComponent->SetVelocity(direction.Normalized() * 5.0f);

	}

	


	CharacterBase::Update(dt);
}

void FlyingEnemyController::OnCollision(nu::Actor* other)
{

}

void FlyingEnemyController::Read(const nu::json::value_t& value)
{
	CharacterBase::Read(value);
}
