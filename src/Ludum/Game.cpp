#define GLEW_STATIC
#include <GL/glew.h>

#include <Ludum/Game.hpp>

#include <Ludum/Common.hpp>
#include <Ludum/FPS.hpp>
#include <Ludum/Sound.hpp>

// States
#include <Ludum/State/MenuState.hpp>
#include <Ludum/State/AboutState.hpp>
#include <Ludum/State/GameState.hpp>
#include <Ludum/State/PauseState.hpp>
#include <Ludum/State/GameOverState.hpp>

#include <glm/glm.hpp>

#include <vector>
#include <iostream>
#include <cmath>
#include <string>
#include <fstream>
#include <sstream>

namespace gb
{
	namespace
	{
		static const sf::Time TICK = sf::seconds(1.0f/60.0f);

		static const unsigned int WIDTH  = 1280;
		static const unsigned int HEIGHT = 720;

		static sf::Image ICON;
	}

	Game::Game()
		:m_hasFocus(false)
		,m_window()
		,m_font()
		,m_levelNumber(1)

		,m_stateStack(
			State::Context(m_window,
			               m_texture,
			               m_font)
		              )
	{
		sf::ContextSettings settings;
		settings.depthBits = 24;
		settings.majorVersion = 2;
		settings.minorVersion = 1;
		settings.antialiasingLevel = 0;



		m_window.create(sf::VideoMode(WIDTH, HEIGHT), "Flipside - FPS: 0", sf::Style::Close, settings);
		m_window.setVerticalSyncEnabled(true);
		m_window.setFramerateLimit(600);

		ICON.loadFromFile(resourcePath() + "Data/Texture/icon.png");

		m_window.setIcon(ICON.getSize().x, ICON.getSize().y, ICON.getPixelsPtr());

		if (glewInit())
			throw std::runtime_error("Could not init GLEW\n");

		init();

		registerStates();
		m_stateStack.pushState(States::Menu); // Initial State
	}


	void Game::run()
	{
		sf::Clock deltaClock;
		sf::Time accumulator;

		gb::FPS fps;

		while (m_window.isOpen())
		{
			accumulator += deltaClock.restart();

			if (fps.update())
			{
				std::stringstream ss;
				ss << "Flipside - FPS: " << fps.getFPS();
				m_window.setTitle(ss.str());
			}

			handleEvents();

			while (accumulator >= TICK)
			{
				accumulator -= TICK;
				handleEvents();
				update(TICK);

				if (m_stateStack.isEmpty())
					m_window.close();
			}

			render();
		}

		m_window.close();
	}

	void Game::init()
	{
		glFrontFace(GL_CW);

		glCullFace(GL_BACK);
		glEnable(GL_CULL_FACE);

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);

		glEnable(GL_BLEND);
		glBlendEquation(GL_FUNC_ADD);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_TEXTURE_2D);

		sf::Image img;

		if (!img.loadFromFile(resourcePath() + "Data/Texture/texture.png"))
			throw std::runtime_error("Could not load texture.png\n");

		img.flipVertically();

		m_texture.loadFromImage(img);


		if (!m_font.loadFromFile(resourcePath() + "Data/Font/Minecraftia.ttf"))
			throw std::runtime_error("Could not load main font\n");

		sf::Mouse::setPosition(sf::Vector2i(WIDTH/2.0f, HEIGHT/2.0f), m_window);
	}

	void Game::handleEvents()
	{
		sf::Event event;
		while (m_window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				m_window.close();

			m_stateStack.handleEvent(event);
		}
	}

	void Game::update(sf::Time dt)
	{
		m_stateStack.update(dt);
	}

	void Game::render()
	{
		clearWindow(sf::Color(140, 230, 255, 255));

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		m_stateStack.draw();

		m_window.display();
	}


	void Game::clearWindow(const sf::Color& color)
	{
		glClearColor(color.r/255.0f, color.g/255.0f, color.b/255.0f, color.a/255.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void Game::registerStates()
	{
		m_stateStack.registerState<MenuState>(States::Menu);
		m_stateStack.registerState<AboutState>(States::About);
		m_stateStack.registerState<GameState>(States::Game);
		m_stateStack.registerState<PauseState>(States::Pause);
		m_stateStack.registerState<GameOverState>(States::GameOver);
	}

}
