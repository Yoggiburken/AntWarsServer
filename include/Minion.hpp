#include<SFML/System.hpp>

#ifndef MINION_HPP
#define MINION_HPP
class Minion {
private:
	int 			health;
	sf::Vector2f 	position;
	int 			damage;
public:
					Minion(sf::Vector2f position, int health, int damage);
	void			attacked(int damage);
	void			setPosition(sf::Vector2f position);

	int				getHealth();
	sf::Vector2f	getPosition();
	int				getDamage();
};
#endif
