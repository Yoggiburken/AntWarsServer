#include"../include/Server.hpp"

using namespace sf;

Server::Server() : receivingThread(&Server::receiving, this), sendingThread(&Server::sending, this)
{
	sendingSocket.bind(9654);
	receivingSocket.bind(9655);

	sendingThread.launch();
	receivingThread.launch();
}

void Server::sending()
{
	while(this->running)
	{
		if(this->new_players > 0) {
			Packet packet;
			for(int i=0; i<this->bases.size(); i++) {
				Vector2f	base_position 	= this->bases[i].getPosition();
				int 		health			= this->bases[i].getHealth();
				packet<<ADD_PLAYER<<base_position.x<<base_position.y<<health;
				for(int j=0; j<minions[i].size(); j++) {
					Vector2f 	minion_position = this->minions[i][j].getPosition();
					int			health			= this->minions[i][j].getHealth();
					packet<<ADD_MINION<<minion_position.x<<minion_position.y<<health;
				}
			}
		}
		for(int i=0; i<this->users.size(); i++) {
			sendingSocket.send(this->users[i].packet, this->users[i].address, this->users[i].port);
		}
	}
}

void Server::receiving()
{
	IpAddress 		address;
	unsigned short 	port;
	std::vector<Vector2f>	minion_positions;
	Packet packet;
	
	while(this->running)
	{
		receivingSocket.receive(packet, address, port);
		if(!this->userExists(address)) {
			Vector2f base_position;

			User user;
			user.packet 	= packet;
			user.address	= address;
			user.port		= port;
			user.ping		= seconds(0);
			this->users.push_back(user);
			packet>>base_position.x>>base_position.y;
			this->bases.push_back(Base(base_position));
			this->new_players +=1;
		} else {
			int userID = this->getUserID(address);
			this->users[userID].packet = packet;
			while(!packet.endOfPacket()) {
				packet>>this->type;
				if(this->type == UPDATE_MINION) {
					Vector2f	minion_position;
					packet>>minion_position.x>>minion_position.y;
					minion_positions.push_back(minion_position);	
				} else if(this->type == ADD_MINION) {
					Vector2f 	position;
					int			health;
					int			damage;

					packet>>position.x>>position.y>>health>>damage;

					this->minions[userID].push_back(Minion(position, health, damage));
				} else if(this->type == REMOVE_MINION) {
					int minionID;
					packet>>minionID;
					this->minions[userID].erase(minions[userID].begin()+minionID);
				} else if(this->type == UPDATE_BASE) {
					int health;
					packet>>health;
					this->bases[userID].setHealth(health);
				} else if(this->type == REMOVE_PLAYER) {
					this->bases.erase(this->bases.begin()+userID);
					this->minions.erase(this->minions.begin()+userID);
				} else if(this->type == ATTACK_MINION) {
					int minionID;
					int damage;
					packet>>minionID>>damage;
					this->minions[userID][minionID].attacked(damage);
				} else if(this->type == ATTACK_BASE) {
					int damage;
					packet>>damage;
					this->bases[userID].attacked(damage);
				}
			}
			packet.clear();
		}
	}
}

bool Server::userExists(IpAddress& address)
{
	for(int i=0; i<this->users.size(); i++) {
		if(address == this->users[i].address) {
			return true;
		}
	}
	return false;
}

int Server::getUserID(IpAddress& address)
{
	for(int i=0; i<this->users.size(); i++) {
		if(address == this->users[i].address) {
			return i;
		}
	}
}
