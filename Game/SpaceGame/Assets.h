#pragma once
#include "Renderer/Model.h"
#include <memory>

namespace assets
{
	extern std::shared_ptr<nu::Model> playerModel;
	extern std::shared_ptr<nu::Model> enemyModel;
	extern std::shared_ptr<nu::Model> bulletModel;
		   
	extern std::shared_ptr<nu::Model> bombModel;
	extern std::shared_ptr<nu::Model> bombExplosionModel;
	extern std::shared_ptr<nu::Model> AmmoPickupModel;
}