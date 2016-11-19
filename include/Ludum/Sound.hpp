#ifndef Ludum_Sound_hpp
#define Ludum_Sound_hpp

#include <SFML/Audio.hpp>

#include <string>

namespace gb
{
	class Sound
	{
	public:
		Sound(const std::string& filename);

		void play();
		void setVolume(float v);

	private:
		sf::Sound m_sound;
		sf::SoundBuffer m_buffer;

	public:

		static Sound Portal;
		static Sound Key;
		static Sound Level;
		static Sound Potion;
		static Sound Door;
		static Sound PlacePortal;

	};
}

#endif
