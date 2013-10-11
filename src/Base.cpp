#include"../include/Base.hpp"

using namespace sf;

Base::Base(Vector2f position)
{
	this->position	=	position;
	this->health	= 	1000;
}

void Base::setHealth(int health)
{
	this->health	=	health;
}

void Base::setPosition(Vector2f position)
{
	this->position = position;
}

void Base::attacked(int damage)
{
	this->health -= damage;
}

int Base::getHealth()
{
	return this->health;
}

Vector2f Base::getPosition()
{
	return this->position;
}
