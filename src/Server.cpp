#include<iostream>
#include"../include/Server.hpp"

using namespace sf;
using namespace std;

Server::Server() : receivingThread(&Server::receiving, this), sendingThread(&Server::sending, this)
{
	sendingSocket.bind(9654);
	receivingSocket.bind(9655);

	sendingThread.launch();
	receivingThread.launch();

	cout<<"Server running at port: 9655";
	cout.flush();
}

void Server::sending()
{
	while(this->running)
	{
		//send gamedata to newly connected players
		if(this->new_players > 0) {
			Packet packet;
			for(int i=0; i<this->bases.size(); i++) {
				Vector2f	base_position 	= this->bases[i].getPosition();
				int 		health			= this->bases[i].getHealth();
				packet<<ADD_PLAYER<<base_position.x<<base_position.y<<health;
				for(int j=0; j<squads[i].size(); j++) {
					Vector2f squad_position = this->squads[i][j].getPosition();
					packet<<ADD_SQUAD<<squad_position.x<<squad_position.y;
					for(int k=0; k<squads[i][j].squad.size(); k++) {
						Vector2f 	minion_position = this->squads[i][j].squad[k].getPosition();
						int			health			= this->squads[i][j].squad[k].getHealth();
						packet<<ADD_MINION<<minion_position.x<<minion_position.y<<health;
					}
				}
				while(this->new_players > 0)
				{
					User user = this->users[this->users.size()-this->new_players];
					cout<<endl<<"Sending gamedata to User: "<<user.address<<" to Port: "<<user.port;
					cout.flush();
					sendingSocket.send(packet, user.address, user.port);
					this->new_players--;
				}
			}
		}

		//relay data to all other users
		for(int i=0; i<this->users.size(); i++) {
			for(int j=0; j<this->users.size(); j++) {
				if(j == i) {
						continue;
				}
				sendingSocket.send(this->users[i].packet, this->users[j].address, this->users[j].port);
			}
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
			cout<<endl<<"User connected from: "<<address;
			cout.flush();
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
					int 		squadID;

					packet>>squadID>>position.x>>position.y>>health>>damage;

					this->squads[userID][squadID].squad.push_back(Minion(position, health, damage));
				} else if(this->type == REMOVE_MINION) {
					int squadID;
					int minionID;
					
					packet>>squadID>>minionID;
					
					//erase minion from squad
					this->squads[userID][squadID].removeMinion(minionID);
					//erase the squad if it is empty
					if(this->squads[userID][squadID].isEmpty()) {
						this->squads[userID].erase(this->squads[userID].begin()+squadID);
					}
				} else if(this->type == UPDATE_BASE) {
					int health;
					
					packet>>health;
					
					this->bases[userID].setHealth(health);
				} else if(this->type == REMOVE_PLAYER) {
					//erase playerdata
					this->bases.erase(this->bases.begin()+userID);
					this->squads.erase(this->squads.begin()+userID);
					this->users.erase(this->users.begin()+userID);
				} else if(this->type == ATTACK_MINION) {
					int squadID;
					int minionID;
					int damage;
					
					packet>>squadID>>minionID>>damage;
					
					this->squads[userID][squadID].squad[minionID].attacked(damage);
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
