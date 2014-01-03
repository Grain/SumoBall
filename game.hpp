#ifndef GAME_H
#define GAME_H

#include <cmath>
#include "player.hpp"
#include "projectile.hpp"
#include <iostream>

class Game
{
public:
	Game();
	virtual ~Game();

	void draw(sf::RenderTarget*);
	int update();
	float collisionSpeed(float, float, double, double);
private:
	float getDistance(sf::Vector2f, sf::Vector2f);
	bool isOutside(sf::Vector2f);
	float getAngle(sf::Vector2f);

    sf::Keyboard::Key key[2][8];

	Player players[2];
	sf::CircleShape ring;
	std::vector<Projectile*> projectiles[2];
	int cooldown[2][4];
};

#endif
