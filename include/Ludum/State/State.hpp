#ifndef Ludum_State_State_hpp
#define Ludum_State_State_hpp

#include <Ludum/State/StateIdentifiers.hpp>
#include <Ludum/Graphics/Program.hpp>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFMl/Graphics/Font.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Window/Event.hpp>

#include <memory>

namespace gb
{
	class StateStack;

	class State
	{
	public:
		typedef std::unique_ptr<State> Ptr;

		struct Context
		{
			Context(sf::RenderWindow& window,
			        sf::Texture& texture,
			        sf::Font& font)
				:window(&window)
				,texture(&texture)
				,font(&font)
			{
			}

			sf::RenderWindow* window;
			sf::Texture* texture;
			sf::Font* font;
		};

		State(StateStack& stack, Context context);
		virtual ~State();

		virtual void draw() = 0;
		virtual bool update(sf::Time dt) = 0;
		virtual bool handleEvent(const sf::Event& event) = 0;

	protected:
		void requestStackPush(States::ID stateID);
		void requestStackPop();
		void requestStateClear();

		Context getContext() const;

	private:
		StateStack* m_stack;
		Context m_context;

	};
}

#endif
