#ifndef PLAYER_H
#define PLAYER_H

#include <cmath>
#include <SFML/Graphics.hpp>
#define PI 3.14159265359
#define RADIUS 20

class Player
{
public:
	Player(bool);
	Player();
	virtual ~Player();

	void setPosition(sf::Vector2f);
	sf::Vector2f getPosition();
	void setMass(double);
	double getMass();
	void setVelocity(sf::Vector2f);
	sf::Vector2f getVelocity();
	float getAngle();
	double getSpeed();

	void changeVelocity(sf::Vector2f);

	void updateDirection();
	void draw(sf::RenderTarget*);
	void accelerate(int);
	void updatePosition();
private:
	sf::CircleShape body;
	double mass;
	sf::Vector2f velocity;
	sf::RectangleShape direction;
};

#endif
