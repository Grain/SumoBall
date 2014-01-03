#include "projectile.hpp"

Projectile::Projectile()
{

}

Projectile::Projectile(Player * shooter, short tempType)
{
	type = (Types)tempType;
	body.setPosition(shooter->getPosition());
    direction = shooter->getAngle();
    speed = shooter->getSpeed() * 2 + 1;
    body.setFillColor(sf::Color::Black);
    body.setRadius(5);
	body.setOrigin(sf::Vector2f(body.getRadius(), body.getRadius()));
}

Projectile::~Projectile()
{

}

sf::Vector2f Projectile::getPosition()
{
	return body.getPosition();
}

int Projectile::getRadius()
{
	return (int)body.getRadius();
}

void Projectile::onEnemy(Player * enemy)
{
    float power;

    if (type == PUSH)
        power = 0.5;
    else
        power = -0.5;
    enemy->changeVelocity(sf::Vector2f(speed * power * cos(direction * (PI / 180)), speed * power * sin(direction * (PI / 180))));
}

void Projectile::draw(sf::RenderTarget * window)
{
	window->draw(body);
}

void Projectile::update()
{
    body.move(speed*cos(direction*(PI/180)), speed*sin(direction*(PI/180)));
}
