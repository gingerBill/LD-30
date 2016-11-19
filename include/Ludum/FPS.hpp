#ifndef Ludum_FPS_hpp
#define Ludum_FPS_hpp

#include <SFML/System/Clock.hpp>

namespace gb
{
	class FPS
	{
	public:
		FPS()
			:m_frame(0)
			,m_fps(0)
		{
		}

		bool update();

		unsigned int getFPS() const
		{
			return m_fps;
		}

	private:
		unsigned int m_frame;
		unsigned int m_fps;
		sf::Clock m_clock;
	};

	bool FPS::update()
	{
		bool reset = false;
		if (m_clock.getElapsedTime().asSeconds() >= 1.f)
		{
			m_fps = m_frame;
			m_frame = 0;
			reset = true;
			m_clock.restart();
		}

		m_frame++;

		return reset;
	}
}

#endif
