#pragma once
#include "Framework/Actor.h"

class Damager : public nu::Actor
{
public:
	CLASS_PROTOTYPE(Damager)

	void Read(const nu::json::value_t& value) override;

	float GetDamage() const { return m_damage; }
	void SetDamage(float damage) { m_damage = damage; }

	float GetDirection() const { return m_dir; }
	void SetDirection(float dir) { m_dir = dir; }

protected:
	float m_damage = 1.0f;
	float m_dir = 1.0f;
};