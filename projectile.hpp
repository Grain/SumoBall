#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <SFML/Graphics.hpp>
#include "player.hpp"

class Projectile
{
public:
	Projectile();
	Projectile(Player*, short);
	virtual ~Projectile();

	sf::Vector2f getPosition();
	int getRadius();

	void onEnemy(Player*);
	void draw(sf::RenderTarget*);
	void update();

	enum Types {PUSH, PULL};
private:
	sf::CircleShape body;
	Types type;
	double direction;
	double speed;
};

#endif
