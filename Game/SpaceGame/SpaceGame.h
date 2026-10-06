#pragma once
#include "Renderer/Text.h"
#include "Renderer/Font.h"
#include "Framework/Game.h"
#include "Resources/ResourceManager.h"

class SpaceGame : public nu::Game
{
public:
	enum class GameState
	{
		Title,
		StartGame,
		StartLevel,
		Game,
		GameOver
	};

public:

	//static SpaceGame& Get() { static SpaceGame game; return game; }

	SpaceGame() = default;
	SpaceGame(nu::Scene* scene) :
		Game{ scene }
	{ }

	bool Initialize() override;

	void Update(float dt) override;
	void Draw(class nu::Renderer& renderer) override;

	void onPlayerDead();
	void AddPoints(int points) { m_score += points; }

public:
	GameState m_gameState = GameState::Title;
	//nu::Font* m_gameFont{ nullptr };

private:
	void SpawnPlayer();
	void SpawnEnemy();

private:
	int m_score{ 0 };
	int m_lives{ 0 };

	float m_stateTimer = 5.0f;
	float m_spawnTimer = 0.0f;
	float m_spawnTimerMin = 3.0f;
	float m_spawnTimerMax = 5.0f;

	nu::res_t<nu::Font> m_titleFont;
	nu::res_t<nu::Font> m_gameFont;

	//nu::Font* m_titleFont{ nullptr };
	nu::Text* m_titleText{ nullptr };

	//nu::Font* m_gameOverFont{ nullptr };
	nu::Text* m_gameOverText{ nullptr };

	nu::Text* m_scoreText{ nullptr };
	nu::Text* m_livesText{ nullptr };
	nu::Text* m_enemyText{ nullptr };
};