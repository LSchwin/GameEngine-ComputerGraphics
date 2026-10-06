#pragma once

#include "Framework/Actor.h"

struct BombDesc : public nu::ActorDesc
{
    float speed = 0;
};


class Bomb : public nu::Actor
{
public:
    Bomb() = default;

    Bomb(const BombDesc& BombDesc) :
        Actor{ BombDesc },
        m_speed{ BombDesc.speed }
    {
    }

    CLASS_PROTOTYPE(Bomb)

    void Update(float dt) override;

    void OnCollision(Actor* other) override;

    void Draw(const class nu::Renderer& renderer) const override;


    virtual void Read(const nu::json::value_t& value) override;

private:
    float m_speed = 800.0f;
    float m_exploded = false;
};