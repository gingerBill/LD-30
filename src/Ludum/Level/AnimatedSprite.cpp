#include <Ludum/Level/AnimatedSprite.hpp>

namespace gb
{
	AnimatedSprite::AnimatedSprite()
		:Sprite()
	{
	}

	void AnimatedSprite::update(sf::Time dt)
	{
		if (m_frames.size() <= 1)
			return;

		m_ft += dt;
		if (m_ft >= m_frameTime)
		{
			m_ft = sf::seconds(0);
			m_currentFrame += 1;
			if (m_currentFrame >= m_frames.size())
				m_currentFrame = 0;
			generateMesh(m_frames[m_currentFrame]);
		}
	}

	void AnimatedSprite::addFrame(int tileType)
	{
		m_frames.push_back(tileType);
	}

	void AnimatedSprite::setFrameTime(sf::Time dt)
	{
		m_frameTime = dt;
	}
}
