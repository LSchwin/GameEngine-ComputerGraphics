#pragma once
#include "Framework/Actor.h"
#include "Renderer/Text.h"
#include "Renderer/Font.h"

struct PlayerDesc : public nu::ActorDesc
{
    float speed{ 2000.0f };
};

class Player : public nu::Actor
{
public:
    Player() = default;

    Player(const PlayerDesc& playerDesc) :
        Actor{ playerDesc },
        m_speed{ playerDesc.speed }

    {}

    CLASS_PROTOTYPE(Player)

    void Update(float dt) override;
    void OnCollision(Actor* other) override;
    void Draw(const class nu::Renderer& renderer) const override;

    int GetAmmo() const { return m_ammo; }
    void SetAmmo(int ammo) { m_ammo = ammo; }

    float GetSpeed() const { return m_speed; }

    virtual void Read(const nu::json::value_t& value) override;

public:
    nu::Text* m_ammoText{ nullptr };

private:
	int m_ammo = 3;
    float m_speed = 2000.0f;

};