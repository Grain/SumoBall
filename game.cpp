#include "game.hpp"

Game::Game()
{
	players[0] = Player(true);
	players[1] = Player(false);

	ring.setRadius(295);
	ring.setPosition(sf::Vector2f(105, 5));
	ring.setFillColor(sf::Color::White);
	ring.setOutlineColor(sf::Color::Black);
	ring.setOutlineThickness(5);

	for (int a = 0; a < 2; ++a)
	{
		for (int b = 0; b < 4; ++b)
		{
			cooldown[a][b] = 0;
		}
	}

	key[0][0] = sf::Keyboard::W;
	key[0][1] = sf::Keyboard::S;
	key[0][2] = sf::Keyboard::A;
	key[0][3] = sf::Keyboard::D;

	key[0][4] = sf::Keyboard::R;
	key[0][5] = sf::Keyboard::T;
	key[0][6] = sf::Keyboard::F;
	key[0][7] = sf::Keyboard::G;

	key[1][0] = sf::Keyboard::U;
	key[1][1] = sf::Keyboard::J;
	key[1][2] = sf::Keyboard::H;
	key[1][3] = sf::Keyboard::K;

	key[1][4] = sf::Keyboard::O;
	key[1][5] = sf::Keyboard::P;
	key[1][6] = sf::Keyboard::L;
	key[1][7] = sf::Keyboard::SemiColon;
}

Game::~Game()
{
	for (int a = 0; a < 2; ++a)
	{
		for (unsigned int b = 0; b < projectiles[a].size(); ++b)
		{
			delete projectiles[a][b];
		}
	}
}

void Game::draw(sf::RenderTarget * window)
{
	window->draw(ring);
	players[0].draw(window);
	players[1].draw(window);

	for (int a = 0; a < 2; ++a)
	{
		for (unsigned int b = 0; b < projectiles[a].size(); ++b)
		{
			projectiles[a][b]->draw(window);
		}
	}
}

float Game::collisionSpeed(float v1, float v2, double m1, double m2)
{
	return (v1*(m1-m2) + 2*m2*v2) / (m1 + m2);
}
float dot(sf::Vector2f a, sf::Vector2f b)
{
	return a.x * b.x + a.y * b.y;
}

float Game::getDistance(sf::Vector2f pos1, sf::Vector2f pos2)
{
	return sqrt(pow(pos1.x - pos2.x,2) + pow(pos1.y - pos2.y, 2));
}

bool Game::isOutside(sf::Vector2f pos)
{
	return (getDistance(pos, sf::Vector2f(400, 300)) > 295);
}

float Game::getAngle(sf::Vector2f vel)
{
	if (vel.x == 0)
		return (-vel.y >= 0) ? PI/2 : -PI/2;
	else if (vel.x > 0)
		return  atan(-vel.y/vel.x);
	else
		return PI + atan(-vel.y/vel.x);
}

int Game::update()
{
	for (int a = 0; a < 2; ++a)		//COOLDOWNS
	{
		for (int b = 0; b < 4; ++b)
		{
			cooldown[a][b] -= 1;
		}
	}

    for (int a = 0; a < 2; ++a)
    {
        for (int i = 0; i < 4; ++i)	//MOVEMENT
        {
            if (sf::Keyboard::isKeyPressed(key[a][i]))
            {
                players[a].accelerate(i);
            }
        }

        if (isOutside(players[a].getPosition()))    //someone is out?
        {
            return a * -1 + 2;
        }
    }

	int timer = 1; //one second cooldown for everything

	for (int i = 4; i < 8; ++i)	//projectiles/boosts
	{
	    for (int a = 0; a < 2; ++a)
        {
            if (sf::Keyboard::isKeyPressed(key[a][i]))
            {
                if (cooldown[a][i - 4] <= 0)
                {
                    if (i == 4)
                    {
                        int power = 3; //power of forward boost
                        players[a].changeVelocity(sf::Vector2f(power * cos(players[a].getAngle() * (PI / 180)), power * sin(players[a].getAngle() * (PI / 180))));
                    }
                    else if (i == 5)
                    {
                        int power = -6;  //power of backward boost
                        players[a].changeVelocity(sf::Vector2f(power * cos(players[a].getAngle() * (PI / 180)), power * sin(players[a].getAngle() * (PI / 180))));
                    }
                    else
                    {
                        projectiles[a].push_back(new Projectile(&players[a], i - 6));
                    }
                    cooldown[a][i - 4] = 60 * timer;
                }
            }
        }
	}

	for (int a = 0; a < 2; ++a)
	{
		for (unsigned int b = 0; b < projectiles[a].size(); ++b)
		{
			if (isOutside(projectiles[a][b]->getPosition()))
			{
				delete projectiles[a][b];
				projectiles[a].erase(projectiles[a].begin() + b);
				b--;
			}
			else
			{
				projectiles[a][b]->update();
			}
		}
	}

	players[0].updatePosition();
	players[1].updatePosition();

	// Check for projectile collisions

	for (unsigned int i = 0; i < projectiles[1].size(); ++i)
	{
		if (getDistance(projectiles[1][i]->getPosition(), players[0].getPosition()) < RADIUS + projectiles[1][i]->getRadius())
		{
			projectiles[1][i]->onEnemy(&players[0]);
			delete projectiles[1][i];
			projectiles[1].erase(projectiles[1].begin() + i);
			i--;
		}
	}

	for (unsigned int i = 0; i < projectiles[0].size(); ++i)
	{
		if (getDistance(projectiles[0][i]->getPosition(), players[1].getPosition()) < RADIUS + projectiles[0][i]->getRadius())
		{
			projectiles[0][i]->onEnemy(&players[1]);
			delete projectiles[0][i];
			projectiles[0].erase(projectiles[0].begin() + i);
			i--;
		}
	}

	//check for player collisions

	sf::Vector2f pos1 = players[0].getPosition();
	sf::Vector2f pos2 = players[1].getPosition();

	float distance = getDistance(pos1, pos2);

	if (distance < RADIUS*2) //collision
	{
		//velocities
		sf::Vector2f v1 = players[0].getVelocity();
		sf::Vector2f v2 = players[1].getVelocity();
		double m1 = players[0].getMass();
		double m2 = players[1].getMass();

		float deltax = players[1].getPosition().x - players[0].getPosition().x;
		float deltay = players[1].getPosition().y - players[0].getPosition().y;

		sf::Vector2f p1_vel, p2_vel, collision(deltax, deltay);
		collision.x /= distance;
		collision.y /= distance;
		float ai = dot(v1, collision);
		float bi = dot(v2, collision);
		float af = collisionSpeed(ai, bi, m1, m2);
		float bf = collisionSpeed(bi, ai, m1, m2);

		v1.x += collision.x * (af - ai);
		v1.y += collision.y * (af - ai);
		v2.x += collision.x * (bf - bi);
		v2.y += collision.y  *(bf - bi);
		players[0].setVelocity(v1);
		players[1].setVelocity(v2);

		// Make sure the players are not overlapping
		distance = -(distance - RADIUS*2)/2;
		int dirx = (pos1.x > pos2.x) ? 1 : -1;
		int diry = (pos1.y > pos2.y) ? 1 : -1;

		pos1 = sf::Vector2f(pos1.x + distance*dirx, pos1.y + distance*diry);
		pos2 = sf::Vector2f(pos2.x - distance*dirx, pos2.y - distance*diry);

		players[0].setPosition(pos1);
		players[1].setPosition(pos2);

		players[0].updateDirection();
		players[1].updateDirection();
	}

	return 0;
}
