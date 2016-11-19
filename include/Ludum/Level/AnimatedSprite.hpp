#ifndef Ludum_Level_AnimatedSprite_hpp
#define Ludum_Level_AnimatedSprite_hpp

#include <Ludum/Level/Sprite.hpp>

namespace gb
{
	class AnimatedSprite : public Sprite
	{
	public:
		AnimatedSprite();

		virtual void update(sf::Time dt);

		void addFrame(int tileType);

		void setFrameTime(sf::Time dt);

	private:
		std::vector<int> m_frames;
		sf::Time m_frameTime = sf::seconds(2.0f);
		int m_currentFrame = 0;
		mutable sf::Time m_ft;


	};
}

#endif
