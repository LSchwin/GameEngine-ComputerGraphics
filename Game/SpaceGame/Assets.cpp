#include "Assets.h"

using namespace nu;

namespace assets
{
    Mesh meshPlayer{
        {
        Vector2{ 4, 0 },
        Vector2{ 0, -1 },
        Vector2{ -1, 0 },
        Vector2{ 0, 1 },
        Vector2{ 4, 0 }
        },
        Color{ 0.7f, 0.6f, 0.6f }
    };

    Mesh meshPlayer2{
        {
        Vector2{ -1, 1 },
        Vector2{ -2, 1 },
        Vector2{ -3, 3 },
        Vector2{ -1, 2 },
        Vector2{ -1, 1 },
        },
        Color{  0.7f, 0.1f, 0.1f  }
    };

    Mesh meshPlayer3{
        {
        Vector2{ -1, -1 },
        Vector2{ -2, -1 },
        Vector2{ -3, -3 },
        Vector2{ -1, -2 },
        Vector2{ -1, -1 },
        },
        Color{  0.7f, 0.1f, 0.1f  }
    };
	std::shared_ptr<Model> playerModel = std::make_shared<Model>( std::vector<Mesh>{ meshPlayer, meshPlayer2, meshPlayer3 } );


    Mesh meshEnemy{
        {
        Vector2{ 4, 0 },
        Vector2{ 0, -1 },
        Vector2{ -1, 0 },
        Vector2{ 0, 1 },
        Vector2{ 4, 0 }
        },
        Color{ 30.0f, 0.6f, 0.6f }
    };

    Mesh meshEnemy2{
        {
        Vector2{ -1, 1 },
        Vector2{ -2, 1 },
        Vector2{ -3, 3 },
        Vector2{ -1, 2 },
        Vector2{ -1, 1 },
        },
        Color{  30.0f, 0.1f, 0.1f  }
    };

    Mesh meshEnemy3{
        {
        Vector2{ -1, -1 },
        Vector2{ -2, -1 },
        Vector2{ -3, -3 },
        Vector2{ -1, -2 },
        Vector2{ -1, -1 },
        },
        Color{  30.0f, 0.1f, 0.1f  }
    };
    std::shared_ptr<Model> enemyModel = std::make_shared<Model>( std::vector<Mesh>{ meshEnemy, meshEnemy2, meshEnemy3 } );


    Mesh bulletMesh{
    {
        Vector2{ -1, -1 },
        Vector2{ 1, 0 },
        Vector2{ -1, 1 },
        Vector2{ -1, -1 },
        },
    Color{ 1.0f, 0.0f, 0.0f }
    };
    std::shared_ptr<Model> bulletModel = std::make_shared<Model>( std::vector<Mesh>{ bulletMesh } );


    Mesh bombMesh{
    {
        Vector2{ 0, -2 },
        Vector2{ 2, 0 },
        Vector2{ 0, 2 },
        Vector2{ -2, 0 },
        Vector2{ 0, -2 },
        },
    Color{ 1.0f, 0.0f, 30.0f }
    };
    std::shared_ptr<Model> bombModel = std::make_shared<Model>( std::vector<Mesh>{ bombMesh } );



    Mesh bombExplosionMesh{
    {
        Vector2{ -3, 0 },
        Vector2{ 0, -3 },
        Vector2{ 3, 0 },
        Vector2{ 3, -3 },
        Vector2{ -3, -3 },
        Vector2{ -3, 0 },
        Vector2{ 0, 3 },
        Vector2{ 3, 0 },
        Vector2{ 3, 3 },
        Vector2{ -3, 3 },
        Vector2{ -3, 0 },
        Vector2{ -3, -3 },
        Vector2{ -5, 0 },
        Vector2{ -3, 3 },
        Vector2{ 0, 5 },
        Vector2{ 3, 3 },
        Vector2{ 5, 0 },
        Vector2{ 3, -3 },
        Vector2{ 0, -5 },
        Vector2{ -3, -3 },
        },
    Color{ 1.0f, 0.0f, 30.0f }
    };
    std::shared_ptr<Model> bombExplosionModel = std::make_shared<Model>( std::vector<Mesh>{ bombExplosionMesh } );

    Mesh AmmoPickupMesh{
    {
        Vector2{ -1, 1 },
        Vector2{ 1, 1 },
        Vector2{ 1, -1 },
        Vector2{ -1, -1 },
        Vector2{ -1, 1 },
        },
    Color{ 1.0f, 30.0f, 30.0f }
    };
    std::shared_ptr<Model> AmmoPickupModel = std::make_shared<Model>( std::vector<Mesh>{ AmmoPickupMesh } );
}