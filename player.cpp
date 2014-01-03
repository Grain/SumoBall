#include "player.hpp"

Player::Player()
{

}

Player::Player(bool playerOne)
{
	body.setRadius(RADIUS);
	body.setOrigin(sf::Vector2f(RADIUS, RADIUS));
	if (playerOne == true)
	{
		body.setFillColor(sf::Color::Red);
		body.setPosition(sf::Vector2f(300, 300));	//left side
	}
	else
	{
		body.setFillColor(sf::Color::Blue);
		body.setPosition(sf::Vector2f(500, 300));	   //right side
	}

	direction.setSize(sf::Vector2f(RADIUS, 5));
	direction.setOrigin(sf::Vector2f(0, 3));
	direction.setPosition(body.getPosition());
	direction.setFillColor(sf::Color::Black);
	mass = 100;
	velocity.x = 0;
	velocity.y = 0;

	if (playerOne == false)
    {
        direction.setRotation(180);
    }
}

Player::~Player()
{

}

void Player::setPosition(sf::Vector2f pos)
{
	body.setPosition(pos);
}

sf::Vector2f Player::getPosition()
{
	return body.getPosition();
}

void Player::setMass(double i)
{
    mass = i;
}

double Player::getMass()
{
	return mass;
}

void Player::setVelocity(sf::Vector2f i)
{
	velocity = i;
}

sf::Vector2f Player::getVelocity()
{
	return velocity;
}

float Player::getAngle()	//Degrees
{
	return direction.getRotation();
}

double Player::getSpeed()
{
	return sqrt(pow(velocity.x, 2) + pow(velocity.y, 2));
}

void Player::changeVelocity(sf::Vector2f change)
{
	velocity.x += change.x;
	velocity.y += change.y;
}

void Player::updateDirection()
{
	direction.setPosition(body.getPosition());

	if (velocity.x == 0)
	{
		if (velocity.y == 0)
			;
		else if (velocity.y > 0)
			direction.setRotation(90);
		else
			direction.setRotation(270);
	}
	else
	{
		if (velocity.x < 0)
			direction.setRotation(atan(velocity.y / velocity.x) * (180/PI) + 180);
		else
			direction.setRotation(atan(velocity.y / velocity.x) * (180/PI));
	}
}

void Player::draw(sf::RenderTarget * window)
{
	window->draw(body);
	window->draw(direction);
}

void Player::accelerate(int dir)
{
	float acceleration = 0.25; //temp?

	if (dir == 0)
		velocity.y -= acceleration;
	else if (dir == 1)
		velocity.y += acceleration;
	else if (dir == 2)
		velocity.x -= acceleration;
	else if (dir == 3)
		velocity.x += acceleration;
}

void Player::updatePosition()
{
	double friction = 0.1;
	double speed = sqrt(pow(velocity.x, 2) + pow(velocity.y, 2));

	double frictionX = 0;
	double frictionY = 0;

	if (speed != 0)
	{
		frictionX = friction * ((double)velocity.x / speed);
		frictionY = friction * ((double)velocity.y / speed);
	}

	if (std::abs(velocity.x) <= std::abs((float)frictionX))
		velocity.x = 0;
	else
		velocity.x -= frictionX;

	if (std::abs(velocity.y) <= std::abs((float)frictionY))
		velocity.y = 0;
	else
		velocity.y -= frictionY;

	body.move(velocity);
	updateDirection();
}
