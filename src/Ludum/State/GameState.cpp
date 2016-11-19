#define GLEW_STATIC
#include <GL/glew.h>

#include <SFML/OpenGL.hpp>

#include <Ludum/State/GameState.hpp>

#include <Ludum/Sound.hpp>
#include <Ludum/Common.hpp>
#include <Ludum/Graphics/Camera.hpp>

#include <SFML/Graphics/RenderWindow.hpp>

#include <SFML/Graphics/RectangleShape.hpp>

#include <sstream>

namespace gb
{
	static int MAX_LEVELS = 4;

	GameState::GameState(StateStack& stack, Context context)
		:State(stack, context)
		,m_window(*context.window)
		,m_levelNumber(1)

	{
		{
			std::fstream f(resourcePath() + "Data/Map/numberOfMaps.txt", std::ios_base::in);

			int a;
			if (f >> a)
				MAX_LEVELS = a;
		}

		m_material.texture = new Texture(getContext().texture->copyToImage());
		m_material.color = sf::Color::White;

		sf::Vector2u screenSize = m_window.getSize();
		sf::Mouse::setPosition(sf::Vector2i(screenSize.x/2, screenSize.y/2), m_window);
		m_window.setMouseCursorVisible(false);

		m_shaderProgram = Program::loadFromFile(resourcePath() + "Data/Shader/BasicVertex.vs", resourcePath() + "Data/Shader/BasicFragment.fs");
		m_shaderProgram->use();

		m_overMusic.openFromFile(resourcePath() + "Data/Sound/overMusic.aif");
		m_underMusic.openFromFile(resourcePath() + "Data/Sound/underMusic.aif");

		m_overMusic.setVolume(0);
		m_underMusic.setVolume(0);

		m_overMusic.setLoop(true);
		m_underMusic.setLoop(true);

		loadLevel();
	}

	GameState::~GameState()
	{
		m_overMusic.stop();
		m_underMusic.stop();
	}

	void GameState::draw()
	{
		if (!m_shaderProgram->isInUse())
			m_shaderProgram->use();

		sf::Texture::bind(getContext().texture);

		m_shaderProgram->setUniform("camera", m_level.getPlayerCamera()->matrix());
		m_shaderProgram->setUniform("transform", glm::mat4());
		m_shaderProgram->setUniform("tex", 0);
		m_shaderProgram->setUniform("color", sf::Color::White);

		bool includeFog = false;
		if (m_level.getCurrentWorldType() == Level::UnderWorld)
			includeFog = true;
		m_shaderProgram->setUniform("includeFog", includeFog);

		m_level.draw(m_shaderProgram);

		m_window.pushGLStates();
		{
			drawUI();
		}
		m_window.popGLStates();
	}

	bool GameState::update(sf::Time dt)
	{
		if (m_overMusic.getStatus() != sf::SoundSource::Status::Playing)
			m_overMusic.play();
		if (m_underMusic.getStatus() != sf::SoundSource::Status::Playing)
			m_underMusic.play();

		m_window.setMouseCursorVisible(false);

		if (!m_level.levelCompleted())
			m_level.update(dt);
		else
		{
			if (m_levelNumber <= MAX_LEVELS)
			{
				loadLevel();
				return true;
			}
			else
			{
				m_overMusic.stop();
				m_underMusic.stop();
				requestStackPush(States::GameOver);
				return false;
			}
		}


		float volume = 30;

		if (m_level.getCurrentWorldType() == Level::OverWorld)
		{
			m_overMusic.setVolume(volume);
			m_underMusic.setVolume(0);
		}
		else if (m_level.getCurrentWorldType() == Level::UnderWorld)
		{
			m_overMusic.setVolume(0);
			m_underMusic.setVolume(volume);
		}

		sf::Vector2i mousePos = sf::Mouse::getPosition(m_window);
		const float mouseSensitivity = 0.15f;
		float sign = 1;
		if (m_level.getCurrentWorldType() == Level::UnderWorld)
			sign = -1;

		sf::Vector2u screenSize = m_window.getSize();

		m_level.getPlayerCamera()->offsetOrientation(sign * mouseSensitivity * (mousePos.y - screenSize.y / 2.0f),
		                                             sign * mouseSensitivity * (mousePos.x - screenSize.x / 2.0f));

		sf::Mouse::setPosition(sf::Vector2i(screenSize.x / 2.0f, screenSize.y / 2.0f), m_window);

		return true;
	}

	bool GameState::handleEvent(const sf::Event& event)
	{
		// Escape pressed, trigger the pause screen
		if ((event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
		  || event.type == sf::Event::LostFocus)
		{
			requestStackPush(States::Pause);
			m_underMusic.pause();
			m_overMusic.pause();
			getContext().window->setMouseCursorVisible(true);
		}

		return true;
	}

	void GameState::loadLevel()
	{
		m_level = Level();

		m_level.getPlayerCamera()->setFieldOfView(60);
		sf::Vector2u screenSize = m_window.getSize();
		float aspect = static_cast<float>(screenSize.x) / static_cast<float>(screenSize.y);
		m_level.getPlayerCamera()->setViewportAspectRatio(aspect);

		std::string num = std::to_string(m_levelNumber);

		if (num.length() < 2)
			num = '0' + num;

		if (!m_level.loadFromFile(resourcePath() + "Data/Map/over" + num + ".png", resourcePath() + "Data/Map/under" + num + ".png"))
			throw std::runtime_error("Could not load level " + num + "\n");

		Sound::Level.play();

		m_levelNumber++;
	}

	void GameState::drawUI()
	{
		sf::View view = m_window.getView();
		m_window.setView(m_window.getDefaultView());

		std::stringstream ss;
		ss << "Keys: " << m_level.getNumberOfKeys() << "\n";
		ss << "Portals: " << m_level.getNumberOfPortalsLeft();



		sf::Text cameraInfo;
		cameraInfo.setString(ss.str());
		cameraInfo.setFont(*getContext().font);
		cameraInfo.setCharacterSize(24);
		cameraInfo.setColor(sf::Color::White);
		cameraInfo.setPosition(10,
		                       m_window.getSize().y - cameraInfo.getLocalBounds().height - 11);

		m_window.draw(cameraInfo);



		m_window.setView(view);
	}
}
