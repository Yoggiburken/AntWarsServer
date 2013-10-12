#include<vector>
#include"Minion.hpp"

#ifndef MINIONSQUAD_HPP
#define MINIONSQUAD_HPP
class MinionSquad {
public:
	std::vector<Minion> 	squad;
	sf::Vector2f			position;
							MinionSquad(sf::Vector2f position);
	void					setPosition(sf::Vector2f position);
	void					addMinion(Minion& minion);
	void					removeMinion(int index);	
	sf::Vector2f				getPosition();
	bool						isEmpty();
};
#endif
