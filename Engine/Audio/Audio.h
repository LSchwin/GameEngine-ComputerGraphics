#pragma once
#include <fmod.hpp>
#include <string>
#include <map>

#include <fmod_errors.h>

#include <iostream>

namespace nu
{
	class Audio
	{
	public:
		Audio() = default;

		bool Initialize();
		void Shutdown();

		void Update();

		bool AddSound(const std::string& name, const std::string& filename);
		bool PlaySound(const std::string& name);

		bool AddLoopSound(const std::string& name, const std::string& filename);

	private:
		bool CheckFMODResult(FMOD_RESULT result);

	private:
		FMOD::System* m_fmodSystem = nullptr;
		std::map<std::string, FMOD::Sound*> m_sounds;
	};
}