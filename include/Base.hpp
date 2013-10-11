#include<SFML/System.hpp>

#ifndef BASE_HPP
#define BASE_HPP
class Base {
private:
	sf::Vector2f 	position;
	int 			health;
public:
					Base(sf::Vector2f position);
	void			setHealth(int health);
	void			setPosition(sf::Vector2f position);
	void			attacked(int damage);

	int				getHealth();
	sf::Vector2f	getPosition();
};
#endif
