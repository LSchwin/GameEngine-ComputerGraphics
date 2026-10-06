#include "EnemyController.h"
#include "Components/PhysicsComponent.h"
#include "Components/SpriteAnimatorRendererComponent.h"
#include "Engine/Engine.h"
#include "Damager.h"
#include "SpriteGame.h"

FACTORY_REGISTER(EnemyController)

void EnemyController::Start()
{
	CharacterBase::Start();

	m_physicsComponent = GetComponent<nu::PhysicsComponent>();
	assert(m_physicsComponent);
	m_rendererComponent = GetComponent<nu::SpriteAnimatorRendererComponent>();
	assert(m_rendererComponent);
}

void EnemyController::Update(float dt)
{
	m_timeAlive += dt;

	nu::Vector2 velocity = m_physicsComponent->GetVelocity();

	float dir = 0;
	

	switch (m_state)
	{
	case CharacterBase::State::Move:
	{
		auto player = m_scene->GetActorByName("PlayerPrototype");
		if (player)
		{
			nu::Vector2 position = GetTransform().position;
			nu::Vector2 playerPosition = player->GetTransform().position;
			nu::Vector2 direction = playerPosition - position;

			if (playerPosition.x < position.x) dir = -1.0f;
			else dir = +1.0f;


			if (position.Distance(playerPosition) < 200.0f)
			{
				m_state = State::Attack;
				m_rendererComponent->Play("attack");
			}

		}

		if (dir != 0)
		{
			velocity.x = dir * 100.0f;
			m_rendererComponent->Play("run");
			m_dir = dir;
		}
		else
		{
			m_rendererComponent->Play("idle");
		}

		m_rendererComponent->SetFlipH(m_dir < 0);
	}
		break;
	case CharacterBase::State::Attack:
		if (m_rendererComponent->IsAnimationDone())
		{
			m_state = State::Move;
			m_rendererComponent->Play("idle");

			auto damager = nu::Factory::Instance().Create<Damager>("DamagerPrototype");
			damager->SetPosition(GetTransform().position + nu::Vector2{ 100.0f * m_dir, 0.0f });
			damager->SetTag("EnemyDamager");
			damager->SetScale(2.0f);
			damager->SetDirection(m_dir);
			m_scene->AddActor(std::move(damager));
		}

		break;
	case CharacterBase::State::Hit:
		if (m_rendererComponent->IsAnimationDone())
		{
			m_state = State::Move;
			m_rendererComponent->Play("idle");
		}
		break;
	case CharacterBase::State::Death:
	{
		//m_rendererComponent->Play("death"); //this is where I'd put my death animation... IF I HAD ONEEE
		if (m_rendererComponent->IsAnimationDone())
		{
			if (m_timeAlive > 1.0f) ((SpriteGame*)(m_scene->GetGame()))->AddPoints(100);
			SetDestroyed();
		}
	}
		break;
	default:
		break;
	}

	m_physicsComponent->SetVelocity(velocity);
	CharacterBase::Update(dt);
}

void EnemyController::OnCollision(nu::Actor* other)
{
	if (nu::EqualsIgnoreCase(other->GetTag(), "PlayerDamager"))
	{
		//std::cout << "hit!" << std::endl;

		m_state = State::Hit;
		m_rendererComponent->Play("hit");
		nu::Engine::Get().GetAudio().PlaySound("swordHit");

		m_health -= 1.0f;
		Damager* damager = dynamic_cast<Damager*>(other);
		if (damager)
		{
			AddPercent(damager->GetDamage());
			m_physicsComponent->SetVelocity(nu::Vector2{ 100.0f * GetPercent() * damager->GetDirection(), 50.0f * GetPercent() });
		}

		other->SetDestroyed();

		if (m_health <= 0)
		{
			m_state = State::Death;
		}
	}
	else if (nu::EqualsIgnoreCase(other->GetTag(), "Death"))
	{
		m_state = State::Death;
	}
}


void EnemyController::Read(const nu::json::value_t& value)
{
	CharacterBase::Read(value);
}
