#pragma once

#include "Framework/Actor.h"
#include "Player.h"


class AmmoPickup : public nu::Actor
{
public:
    AmmoPickup() = default;

    CLASS_PROTOTYPE(AmmoPickup)

    //void Draw(const class nu::Renderer& renderer) const override;

    virtual void Read(const nu::json::value_t& value) override
    {
        Actor::Read(value);
    }
};

