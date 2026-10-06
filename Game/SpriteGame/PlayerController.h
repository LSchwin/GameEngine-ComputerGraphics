#pragma once
#include "CharacterBase.h"

namespace nu 
{
	class PhysicsComponent;
	class SpriteAnimatorRendererComponent;
}

class PlayerController : public CharacterBase
{
public:
	CLASS_PROTOTYPE(PlayerController)

	void Start() override;
	void Update(float dt) override;

	void OnCollision(nu::Actor* other) override;
	void Read(const nu::json::value_t& value) override;

protected:
	nu::PhysicsComponent* m_physicsComponent{ nullptr };
	nu::SpriteAnimatorRendererComponent* m_rendererComponent = nullptr;

	float m_dir = 1.0f;
	int nextAttack = 1;

	float attackTimer = 2.0f;

	bool m_jumping = false; //move this to a player state enum thing
};


