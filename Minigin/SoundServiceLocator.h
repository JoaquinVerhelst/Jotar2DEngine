#pragma once
#include "SoundSystem.h"
#include <memory>


namespace Jotar
{
	class SoundServiceLocator final
	{
	private:
		static std::unique_ptr<SoundSystem> _ss_instance; 
	public:
		static SoundSystem& GetSoundSystem();
		static void RegisterSoundSystem(std::unique_ptr<SoundSystem>&& ss);
	};
}