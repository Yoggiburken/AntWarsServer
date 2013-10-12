#include"../include/MinionSquad.hpp"

using namespace sf;

MinionSquad::MinionSquad(Vector2f position)
{
	this->position 	= position;
}

void MinionSquad::setPosition(Vector2f position)
{
	this->position	= position;
}

void MinionSquad::addMinion(Minion& minion)
{
	this->squad.push_back(minion);
}

void MinionSquad::removeMinion(int index)
{
	this->squad.erase(this->squad.begin()+index);
}

Vector2f MinionSquad::getPosition()
{
	return this->position;
}

bool MinionSquad::isEmpty()
{
	if(this->squad.size() == 0) {
		return true; 
	}
	return false;
}
