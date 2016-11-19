#include <Ludum/State/State.hpp>

#include <Ludum/State/StateStack.hpp>

namespace gb
{

	State::State(StateStack& stack, Context context)
	 :m_stack(&stack)
	 ,m_context(context)
	{
	}

	State::~State()
	{
	}

	void State::requestStackPush(States::ID stateID)
	{
		m_stack->pushState(stateID);
	}

	void State::requestStackPop()
	{
		m_stack->popState();
	}

	void State::requestStateClear()
	{
		m_stack->clearStates();
	}

	State::Context State::getContext() const
	{
		return m_context;
	}

}
