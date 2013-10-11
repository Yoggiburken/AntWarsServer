#include"../include/Minion.hpp"

using namespace sf;

Minion::Minion(Vector2f position, int health, int damage)
{
	this->position 	= position;
	this->health	= health;
	this->damage	= damage;
}

void Minion::attacked(int damage)
{
	this->health -= damage;
}

void Minion::setPosition(Vector2f position)
{
	this->position = position;
}

int Minion::getHealth()
{
	return this->health;
}

Vector2f Minion::getPosition()
{
	return this->position;
}

int Minion::getDamage()
{
	return this->damage;
}
