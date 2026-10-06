#include "PlayerController.h"
#include "Components/PhysicsComponent.h"
#include "Components/SpriteAnimatorRendererComponent.h"
#include "Engine/Engine.h"
#include "Damager.h"
#include "SpriteGame.h"

FACTORY_REGISTER(PlayerController)

void PlayerController::Start()
{
	Actor::Start();

	m_physicsComponent = GetComponent<nu::PhysicsComponent>();
	assert(m_physicsComponent);
	m_rendererComponent = GetComponent<nu::SpriteAnimatorRendererComponent>();
	assert(m_rendererComponent);
}

void PlayerController::Update(float dt)
{
	nu::Vector2 velocity = m_physicsComponent->GetVelocity();

	switch (m_state)
	{
	case CharacterBase::State::Move:
	{
		float dir = 0;
		if (nu::Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_A)) dir = -1.0;
		if (nu::Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_D)) dir = +1.0;
		if (nu::Engine::Get().GetInput().GetKeyPressed(SDL_SCANCODE_SPACE) && m_grounded)
		{
			velocity.y = -1000.0f;
			m_grounded = false;
			m_jumping = true;
		}

		if (dir != 0)
		{
			velocity.x = dir * 500.0f;
			if (m_grounded == true) m_rendererComponent->Play("run");
			m_dir = dir;
			m_rendererComponent->SetFlipH(m_dir < 0);
		}
		else if (m_grounded == true) m_rendererComponent->Play("idle");

		if (m_grounded == false && m_jumping) m_rendererComponent->Play("jump");
		else if (m_grounded == false && m_jumping == false) m_rendererComponent->Play("fall");

		//if ((m_physicsComponent->GetVelocity().y > 0.01f) && (m_jumping == false)) m_rendererComponent->Play("fall");


		if (nu::Engine::Get().GetInput().GetButtonDown(nu::Input::MouseButton::Left))
		{
			attackTimer = 2.0f;
			m_state = State::Attack;
			m_rendererComponent->SetOffset(101.0f);

			std::string temp1 = (m_grounded) ? "ground" : "air";
			std::string temp2 = "_attack";
			std::string temp3 = std::to_string(nextAttack);
			m_rendererComponent->Play(temp1 + temp2 + temp3);

			nu::Engine::Get().GetAudio().PlaySound("laser");
		}

	}
		break;
	case CharacterBase::State::Attack:
	{
		if (m_rendererComponent->IsAnimationDone())
		{
			m_state = State::Move;
			m_rendererComponent->SetOffset(0.0f);
			m_rendererComponent->Play("idle");

			auto damager = nu::Factory::Instance().Create<Damager>("DamagerPrototype");
			damager->SetPosition(GetTransform().position + nu::Vector2{ 100.0f * m_dir, 0.0f });
			damager->SetTag("PlayerDamager");
			damager->SetScale(2.0f);
			damager->SetDirection(m_dir);
			damager->SetDamage(5.0f);
			m_scene->AddActor(std::move(damager));

			nextAttack = (nextAttack++ % 3) + 1;
		}
		
	}
		break;
	case CharacterBase::State::Hit:

		break;
	case CharacterBase::State::Death:
	{
		((SpriteGame*)(m_scene->GetGame()))->onPlayerDead();
		SetDestroyed();
	}
		break;
	default:

		break;
	}

	attackTimer -= dt;
	if (attackTimer <= 0)
	{
		attackTimer = 2.0f;
		nextAttack = 1;
	}

	m_physicsComponent->SetVelocity(velocity);
	nu::Engine::Get().GetRenderer().SetCamera(m_physicsComponent->GetPosition());
	Actor::Update(dt);
}

void PlayerController::OnCollision(nu::Actor* other)
{
	if (other->GetTag() == "EnemyDamager")
	{
		Damager* damager = dynamic_cast<Damager*>(other);
		if (damager)
		{
			AddPercent(damager->GetDamage());
			m_physicsComponent->SetVelocity(nu::Vector2{ 100.0f * GetPercent() * damager->GetDirection(), 500.0f });
		}
		m_grounded = false;
	}
	else if (other->GetTag() == "Ground")
	{
		m_grounded = true;
		m_jumping = false;
	}
	else if (other->GetTag() == "Death")
	{
		m_state = State::Death;
	}
}

void PlayerController::Read(const nu::json::value_t& value)
{
	Actor::Read(value);
}
