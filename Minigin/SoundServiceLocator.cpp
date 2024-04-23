#include "SoundServiceLocator.h"

namespace Jotar
{
	std::unique_ptr<SoundSystem> SoundServiceLocator::_ss_instance = std::make_unique<NullSoundSystem>();


	SoundSystem& SoundServiceLocator::GetSoundSystem()
	{
		return *_ss_instance;
	}

	void SoundServiceLocator::RegisterSoundSystem(std::unique_ptr<SoundSystem>&& ss)
	{
		_ss_instance = ss == nullptr ? std::make_unique<NullSoundSystem>() : std::move(ss);
	}
}