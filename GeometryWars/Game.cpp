#include "Game.h"

#include <iostream>
#include <fstream>
#include <algorithm>
#include "Util.h"

Game::Game(const std::string& config)
{
	init(config);
}

void Game::init(const std::string& path)
{
	// TODO: read in config file
	// use the PlayerConfig, EnemyConfig, and BulletConfig structs to store the data
	//std::ifstream fin(path);

	//if (!fin.is_open())
	//{
	//	std::cout << "Failed to open config file: " << path << std::endl;
	//	return;
	//}

	//fin >> m_playerConfig.SR >> m_playerConfig.CR >> m_playerConfig.FR >> m_playerConfig.FG >> m_playerConfig.FB >> m_playerConfig.OR >> m_playerConfig.OG >> m_playerConfig.OB >> m_playerConfig.OT >> m_playerConfig.V >> m_playerConfig.S;
	//fin >> m_enemyConfig.SR >> m_enemyConfig.CR >> m_enemyConfig.OR >> m_enemyConfig.OG >> m_enemyConfig.OB >> m_enemyConfig.OT >> m_enemyConfig.VMIM >> m_enemyConfig.VMAX >> m_enemyConfig.L >> m_enemyConfig.SI >> m_enemyConfig.SMIN >> m_enemyConfig.SMAX;
	//fin >> m_bulletConfig.SR >> m_bulletConfig.CR >> m_bulletConfig.FR >> m_bulletConfig.FG >> m_bulletConfig.FB >> m_bulletConfig.OR >> m_bulletConfig.OG >> m_bulletConfig.OB >> m_bulletConfig.OT >> m_bulletConfig.V >> m_bulletConfig.L >> m_bulletConfig.S;

	//fin.close();

	// setup default window parameters
	m_window.create(sf::VideoMode(1280, 720), "Geometry Wars");
	m_window.setFramerateLimit(60);

	spawnPlayer();
}

void Game::run()
{
	// TODO: add pause functionality
	// some system should function while paused (e.g. render)
	// some systems should not function while paused (e.g. movement/input)
	while (m_running)
	{
		m_entities.update();

		sEnemySpawner();
		sMovement();
		sLifeSpan();
		sCollision();
		sUserInput();
		sRender();

		m_currentFrame++;
	}
}

void Game::setPaused(bool paused)
{
	m_paused = paused;
}

void Game::spawnPlayer()
{
	// TODO: finish adding all properties of the player with the correct values from the config

	// We create every entity by calling EntityManager::addEntity(tag)
	// This returns a std::shared_ptr<Entity> to the entity we just created, so we use 'auto' to save typing
	auto entity = m_entities.addEntity("player");

	float mx = m_window.getSize().x / 2.0f;
	float my = m_window.getSize().y / 2.0f;

	// Give this entity a Transform component so it spawn at (200, 200) with velocity of (1, 1) and angle of 0
	entity->cTransform = std::make_shared<CTransform>(Vec2(mx, my), Vec2(0.0f, 0.0f), 0.0f);

	// Exmaple instead of 8 we would use m_playerConfig.V
	// The entity's shape will have radius 32, 8 sides, dark grey fill, and red outline of thickness 4
	entity->cShape = std::make_shared<CShape>(32.0f, 8, sf::Color(10, 10, 10), sf::Color(255, 0, 0), 4.0f);

	// Add an input component to the player so that we can use inputs
	entity->cInput = std::make_shared<CInput>();

	entity->cCollision = std::make_shared<CCollision>(32.0f);

	// Since we want this Entity to be our player, set our Game's player variable to be this Entity
	// This goes slightly against the Entity-Component-System design pattern, but it's a useful shortcut since we use the player so much
	m_player = entity;
}

void Game::spawnEnemy()
{
	// TODO: make sure the enemy is spawned properly with the m_enemyConfig variables
	// the enemy must be spawned completely within the bounds of the window

	auto entity = m_entities.addEntity("enemy");

	// TODO: will need to modify this
	float ex = rand() % m_window.getSize().x;
	float ey = rand() % m_window.getSize().y;

	// Set from config
	entity->cTransform = std::make_shared<CTransform>(Vec2(ex, ey), Vec2(1.0f, 1.0f), 0.0f);
	entity->cShape = std::make_shared<CShape>(16.0f, 3, sf::Color(0, 0, 255), sf::Color(255, 255, 255), 4.0f);
	entity->cCollision = std::make_shared<CCollision>(16.0f);

	// Set random initial velocity within a range (e.g., -2.0 to 2.0 in both x and y directions)
	float minVelocity = -2.0f;
	float maxVelocity = 2.0f;
	entity->cTransform->velocity.x = minVelocity + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (maxVelocity - minVelocity)));
	entity->cTransform->velocity.y = minVelocity + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (maxVelocity - minVelocity)));

	// record when the most recent enemy was spawned
	m_lastEnemySpawnTime = m_currentFrame;
}

void Game::spawnSmallEnemies(std::shared_ptr<Entity> entity)
{
	// TODO: spawn small enemies at the location of input enemy e

	// when we create the smaller enemy, we have to read the values of the original enemy
	// - spawn a number of small enemies equal to the vertices of the original enemy
	// - set each small enemy to the same color as the original, half the size
	// - small enemies are worth double points of the original enemy
}

void Game::spawnBullet(std::shared_ptr<Entity> entity, const Vec2& target)
{
	auto bullet = m_entities.addEntity("bullet");

	bullet->cTransform = std::make_shared<CTransform>(entity->cTransform->position, Vec2(1.0f, 1.0f), 0.0f);
	bullet->cShape = std::make_shared<CShape>(10.0f, 8, sf::Color(255, 255, 255), sf::Color(255, 0, 0), 2.0f);
	bullet->cCollision = std::make_shared<CCollision>(10.0f);
	bullet->cLifespan = std::make_shared<CLifespan>(100);

	Vec2 difference = target - bullet->cTransform->position;
	float distanceSq = difference.x * difference.x + difference.y * difference.y;
	float length = Util::sqrt(distanceSq);
	Vec2 normalizedVector = Vec2(difference.x / length, difference.y / length);

	bullet->cTransform->velocity = Vec2(5.0f * normalizedVector.x, 5.0f * normalizedVector.y);
}

void Game::spawnSpecialWeapon(std::shared_ptr<Entity> entity)
{
	// TODO: implement your own special weapon
}

void Game::sMovement()
{
	m_player->cTransform->velocity = { 0, 0 };

	// implement player movement
	if (m_player->cInput->up)
	{
		//m_player->cTransform->velocity.y = -m_playerConfig.V;
		m_player->cTransform->velocity.y = -5;
	}

	if (m_player->cInput->left)
	{
		//m_player->cTransform->velocity.x = -m_playerConfig.V;
		m_player->cTransform->velocity.x = -5;
	}

	if (m_player->cInput->down)
	{
		//m_player->cTransform->velocity.x = m_playerConfig.V;
		m_player->cTransform->velocity.y = 5;
	}

	if (m_player->cInput->right)
	{
		//m_player->cTransform->velocity.x = m_playerConfig.V;
		m_player->cTransform->velocity.x = 5;
	}

	// Sample movement speed update
	m_player->cTransform->position.x += m_player->cTransform->velocity.x;
	m_player->cTransform->position.y += m_player->cTransform->velocity.y;

	// Enemy movement
	for (auto& e : m_entities.getEntities("enemy"))
	{
		// check if the shape is out of bound and reverse its direction if needed
		if (e->cTransform->position.x < 0 || e->cTransform->position.x + (e->cShape->circle.getLocalBounds().width / 2) > m_window.getSize().x)
		{
			e->cTransform->velocity.x *= -1.0f;
		}
		if (e->cTransform->position.y < 0 || e->cTransform->position.y + (e->cShape->circle.getLocalBounds().height / 2) > m_window.getSize().y)
		{
			e->cTransform->velocity.y *= -1.0f;
		}

		e->cTransform->position.x += e->cTransform->velocity.x;
		e->cTransform->position.y += e->cTransform->velocity.y;
	}

	// Bullet movement
	for (auto& b : m_entities.getEntities("bullet"))
	{
		b->cTransform->position.x += b->cTransform->velocity.x;
		b->cTransform->position.y += b->cTransform->velocity.y;
	}
}

void Game::sLifeSpan()
{
	for (auto& e : m_entities.getEntities())
	{
		if (e->cLifespan)
		{
			if (e->cLifespan->remaining > 0)
			{
				e->cLifespan->remaining -= 1;

				int alpha = (e->cLifespan->remaining * 255) / e->cLifespan->total;

				e->cShape->circle.setFillColor(sf::Color(255, 255, 255, alpha));
				e->cShape->circle.setOutlineColor(sf::Color(255, 0, 0, alpha));
			}
			else
			{
				e->destroy();
			}
		}
	}
}

void Game::sCollision()
{
	for (auto& b : m_entities.getEntities("bullet"))
	{
		for (auto& e : m_entities.getEntities("enemy"))
		{
			Vec2 difference = e->cTransform->position - b->cTransform->position;
			float distanceSq = difference.dist(difference);
			bool collision = distanceSq < (e->cCollision->radius + b->cCollision->radius) * (e->cCollision->radius + b->cCollision->radius);

			if (collision)
			{
				e->destroy();
			}
		}
	}

	for (auto& e : m_entities.getEntities("enemy"))
	{
		for (auto& p : m_entities.getEntities("player"))
		{
			Vec2 difference = e->cTransform->position - p->cTransform->position;
			float distanceSq = difference.dist(difference);
			bool collision = distanceSq < (e->cCollision->radius + p->cCollision->radius) * (e->cCollision->radius + p->cCollision->radius);

			if (collision && p->isActive())
			{
				p->destroy();
				spawnPlayer();
			}
		}
	}
}

void Game::sRender()
{
	m_window.clear();

	for (auto& e : m_entities.getEntities())
	{
		if (e->cShape)
		{
			e->cShape->circle.setPosition(e->cTransform->position.x, e->cTransform->position.y);
			e->cTransform->angle += 1.0f;
			e->cShape->circle.setRotation(e->cTransform->angle);

			m_window.draw(e->cShape->circle);
		}
	}

	m_window.display();
}

void Game::sUserInput()
{
	sf::Event event;
	while (m_window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			m_running = false;
		}

		if (event.type == sf::Event::KeyPressed)
		{
			switch (event.key.code)
			{
			case sf::Keyboard::W:
				m_player->cInput->up = true;
				break;
			case sf::Keyboard::A:
				m_player->cInput->left = true;
				break;
			case sf::Keyboard::S:
				m_player->cInput->down = true;
				break;
			case sf::Keyboard::D:
				m_player->cInput->right = true;
				break;
			default: break;
			}
		}

		if (event.type == sf::Event::KeyReleased)
		{
			switch (event.key.code)
			{
			case sf::Keyboard::W:
				m_player->cInput->up = false;
				break;
			case sf::Keyboard::A:
				m_player->cInput->left = false;
				break;
			case sf::Keyboard::S:
				m_player->cInput->down = false;
				break;
			case sf::Keyboard::D:
				m_player->cInput->right = false;
				break;
			default: break;
			}
		}

		if (event.type == sf::Event::MouseButtonPressed)
		{
			if (event.mouseButton.button == sf::Mouse::Left)
			{
				std::cout << "Left mouse button clicked at (" << event.mouseButton.x << ", " << event.mouseButton.y << ")" << std::endl;
				spawnBullet(m_player, Vec2(event.mouseButton.x, event.mouseButton.y));
			}

			if (event.mouseButton.button == sf::Mouse::Right)
			{
				std::cout << "Right mouse button clicked at (" << event.mouseButton.x << ", " << event.mouseButton.y << ")" << std::endl;
				// call spawnSpecialWeapon() here
			}
		}
	}
}

void Game::sEnemySpawner()
{
	if (m_currentFrame - m_lastEnemySpawnTime > 120)
	{
		spawnEnemy();
	}
}
