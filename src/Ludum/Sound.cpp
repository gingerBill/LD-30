#include <Ludum/Sound.hpp>

#include <Ludum/Common.hpp>

namespace gb
{
	Sound::Sound(const std::string& filename)
	{
		m_buffer.loadFromFile(filename);
		m_sound.setBuffer(m_buffer);
		m_sound.setVolume(20);
	}

	void Sound::play()
	{
		m_sound.play();
	}

	void Sound::setVolume(float v)
	{
		m_sound.setVolume(v);
	}

	Sound Sound::Portal      = Sound(resourcePath() + "Data/Sound/portal.wav");
	Sound Sound::Key         = Sound(resourcePath() + "Data/Sound/key.wav");
	Sound Sound::Level       = Sound(resourcePath() + "Data/Sound/level.wav");
	Sound Sound::Potion      = Sound(resourcePath() + "Data/Sound/potion.wav");
	Sound Sound::Door        = Sound(resourcePath() + "Data/Sound/door.wav");
	Sound Sound::PlacePortal = Sound(resourcePath() + "Data/Sound/placePortal.wav");
}
