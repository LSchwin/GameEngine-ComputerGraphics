#pragma once

#include "Core/Random.h"
#include "Math/Vector2.h"
#include "Math/Vector3.h"
#include "Math/Transform.h"
#include "Math/MathUtils.h"
#include "Core/StringUtils.h"

#include "Renderer/Text.h"

#include "Core/Factory.h"

// systems
#include "Renderer/Renderer.h"
#include "Input/Input.h"
#include "Core/GameTime.h"
#include "Core/File.h"
#include "Audio/Audio.h"
#include "Renderer/ParticleSystem.h"
#include "Renderer/Texture.h"
#include "Serialization/JSON.h"
#include "Physics/Physics.h"

// framework
#include "Framework/Actor.h"
#include "Framework/Scene.h"
#include "Framework/Game.h"

#include <iostream>
#include <vector>

namespace nu
{
	class Engine
	{
	public:

		static Engine& Get() { static Engine engine; return engine; }

		bool Initialize();
		void Shutdown();

		void Update();

		Input& GetInput() { return m_input; }
		Renderer& GetRenderer() { return m_renderer; }
		Time& GetTime() { return m_time; }

		Engine(const Engine&) = delete;
		Engine& operator = (const Engine&) = delete;

		Audio& GetAudio() { return m_audio; }

		ParticleSystem& GetPS() { return m_particleSystem; }

		Physics& GetPhysics() { return m_physics; }

	private:
		Engine() = default;


	private:
		Input m_input;
		Renderer m_renderer;
		Audio m_audio;
		Time m_time;
		ParticleSystem m_particleSystem;
		Physics m_physics;
	};
}