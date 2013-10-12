#include<vector>
#include<SFML/Network.hpp>

#include"MinionSquad.hpp"
#include"Base.hpp"

#ifndef	PACKET_TYPE 
#define PACKET_TYPE
enum PacketType {UPDATE_SQUAD = 0, ADD_SQUAD, UPDATE_MINION, ADD_MINION, REMOVE_MINION, UPDATE_BASE, ADD_PLAYER, REMOVE_PLAYER, ATTACK_MINION, ATTACK_BASE};
#endif

#ifndef USERS
#define USERS
struct User {
	sf::Packet		packet;
	sf::IpAddress 	address;
	unsigned short 	port;
	sf::Time		ping;
};
#endif
//packet<<playerID<<type<<update<<type<<update
//minion_update<<position
//attack<<playerID<<minionID<<damage
//remove_player<<playerID
//connect<<playerID
//add_minion<<position<<health<<damage
#ifndef SERVER_HPP
#define SERVER_HPP
class Server {
private:
	std::vector<Base>						bases;
	std::vector<std::vector<MinionSquad> > 	squads;
	std::vector<User>						users;
	int										new_players;
	
	sf::UdpSocket							sendingSocket;
	sf::UdpSocket							receivingSocket;
	sf::Thread								sendingThread;
	sf::Thread								receivingThread;
	unsigned int							type;

	bool 									running;
	
	void									sending();
	void									receiving();
	
	bool									userExists(sf::IpAddress& address);
	int										getUserID(sf::IpAddress& address);
public:
											Server() /*: receivingThread(&Server::receiving, this), sendingThread(&Server::sending, this)*/;
	void									main() {while(this->running) {}}
};
#endif
