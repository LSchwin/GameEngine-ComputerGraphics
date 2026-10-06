#pragma once
#include "Framework/Actor.h"

class CharacterBase : public nu::Actor
{
public:
	enum class State
	{
		Move = 1,
		Attack,
		Hit,
		Death
	};

public:
	void Read(const nu::json::value_t& value) override;
	float GetPercent() const { return m_percent; }
	void AddPercent(float percent) { m_percent += percent; }

protected:
	State m_state = State::Move;
	float m_health = 1.0f;

	float m_stateTimer = 0.0f;

	float m_percent = 0.0f;
};