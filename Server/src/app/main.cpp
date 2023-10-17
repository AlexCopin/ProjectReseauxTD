#include <app/constants.hpp>
#include <utils/exit_capsule.hpp>
#include <enet6/enet.h>
#include <math/quaternion.hpp>
#include <iostream>
#include <string>
#include <vector>
#include "Protocol.hpp"
#include "Characters.h"
#include <map>

struct ServerData
{
	ENetHost* host;
};

struct GameData
{
	std::uint32_t currentEnemyIndex = 0;
	std::uint32_t goldAttacker = 0;
	std::uint32_t goldDefender = 0;
	std::map<std::uint32_t, Player> players;
	std::map<std::uint32_t, Enemy> enemies;
	std::map<std::uint32_t, Tower> towers;

	float goldTimer = TimerGold;

	bool gameStarted = false;
public:
	bool CanGameStart() 
	{
		if (AttackerConnected() && DefenderConnected())
			return true;
		else
			return false;
	}
	bool AttackerConnected()
	{
		for (auto& player : players)
		{
			if (player.second.type == PlayerType::Attacker)
				return true;
		}
		return false;
	}
	bool DefenderConnected()
	{
		for (auto& player : players)
		{
			if (player.second.type == PlayerType::Defender)
				return true;
		}
		return false;
	}
};

void tick(ServerData& serverData, GameData& gameData);
void handle_message(const std::vector<std::uint8_t>& message, GameData& gameData);
void build_packet_gold(GameData& gameData);
void build_packet_spawnableData(GameData& gameData);
void send_packet(const Player& player, ENetPacket* packet);

int main()
{
	GameData gameData;
	// Initialisation d'enet
	if (enet_initialize() != 0)
	{
		std::cerr << "failed to initialize enet" << std::endl;
		return EXIT_FAILURE;
	}

	// On planifie la libération d'enet à la fin de la fonction
	ExitCapsule uninitEnet([&] { enet_deinitialize(); });

	ENetAddress address;
	enet_address_build_any(&address, ENET_ADDRESS_TYPE_IPV6);

	address.port = AppPort;

	ServerData serverData;

	serverData.host = enet_host_create(ENET_ADDRESS_TYPE_ANY, &address, 10, 10, 0, 0);
	if (!serverData.host)
	{
		std::cout << "Failed to create ENet host (is port free?)" << std::endl;
		return EXIT_FAILURE;
	}

	// On planifie la destruction du host ENet à la fin de la fonction
	ExitCapsule freeHost([&] { enet_host_destroy(serverData.host); });

	std::uint32_t nextTick = enet_time_get();

	// Boucle principale
	for (;;)
	{
		// On récupère le temps actuel
		std::uint32_t now = enet_time_get();

		// On gère les événements ENet
		ENetEvent event;
		while (enet_host_service(serverData.host, &event, 1) > 0)
		{
			do 
			{
				switch (event.type)
				{
					// Un nouveau joueur s'est connecté
					case ENetEventType::ENET_EVENT_TYPE_CONNECT:
					{
						//Send info 
						Player newPlayer;
						std::uint32_t ind = enet_peer_get_id(event.peer);
						newPlayer.index = ind;
						newPlayer.peer = event.peer;

						if (!gameData.AttackerConnected())
						{
							GoldServerPacket packet;
							newPlayer.type = PlayerType::Attacker;
							gameData.goldAttacker = AttackerStartingGold;
							packet.value = AttackerStartingGold;
							send_packet(newPlayer, build_packet(packet, 0));
							//build_packet_spawnableData(gameData);
							std::cout << "Attacker connect" << std::endl;

						}
						else if (gameData.AttackerConnected() && !gameData.DefenderConnected())
						{
							GoldServerPacket packet;
							newPlayer.type = PlayerType::Defender;
							gameData.goldDefender = DefenderStartingGold;
							packet.value = DefenderStartingGold;
							send_packet(newPlayer, build_packet(packet, 0));
							build_packet_spawnableData(gameData);

							std::cout << "Defender connect" << std::endl;
							gameData.gameStarted = true;
						}
						else if(gameData.CanGameStart())
						{
							std::cout << "Spectator connect" << std::endl;
							newPlayer.type = PlayerType::Spectator;
						}

						gameData.players.emplace(ind, std::move(newPlayer));
						PlayerInitServerPacket packet;
						packet.player = newPlayer;
						enet_peer_send(event.peer, 0, build_packet(packet, 0));
						std::cout << "Peer #" << enet_peer_get_id(event.peer) << " connected!" << std::endl;
						break;
					}

					// Un joueur s'est déconnecté
					case ENetEventType::ENET_EVENT_TYPE_DISCONNECT:
					{
						auto player = gameData.players.find(enet_peer_get_id(event.peer));
						std::cout << "Peer #" << player->second.index << " disconnected!" << std::endl;
						gameData.players.erase(enet_peer_get_id(event.peer));
						break;
					}
					// On a reçu des données d'un joueur
					case ENetEventType::ENET_EVENT_TYPE_RECEIVE:
					{
						// On a reçu un message ! Traitons-le
						std::vector<std::uint8_t> content(event.packet->dataLength); //< On copie son contenu dans un std::vector pour plus de facilité de gestion
						std::memcpy(content.data(), event.packet->data, event.packet->dataLength);

						// On gère le message qu'on a reçu
						handle_message(content, gameData);

						// On n'oublie pas de libérer le packet
						enet_packet_destroy(event.packet);
						break;
					}
				}
			}
			while (enet_host_check_events(serverData.host, &event) > 0);
		}
		// On déclenche un tick si suffisamment de temps s'est écoulé
		if (now >= nextTick)
		{
			tick(serverData, gameData);
			nextTick += TickDelay;
		}
	}

	return EXIT_SUCCESS;
}


void send_packet(const Player& player, ENetPacket* packet)
{
	enet_peer_send(player.peer, 0, packet);
}

void build_packet_gold(GameData& gameData)
{
	GoldServerPacket packet;
	for(auto& player : gameData.players)
	{
		switch(player.second.type)
		{
		case PlayerType::Attacker:
			gameData.goldAttacker += AttackerGoldPerSecond;
			packet.value = gameData.goldAttacker;
			send_packet(player.second, build_packet(packet, 0));
			break;
		case PlayerType::Defender:
			gameData.goldDefender += DefenderGoldPerSecond;
			packet.value = gameData.goldDefender;
			send_packet(player.second, build_packet(packet, 0));
			break;
		default:
			break;
		}
	}
}

void build_packet_spawnableData(GameData& gameData)
{
	TowerDataServerPacket packet01;
	TowerSimple simpleTower;
	packet01.towerData.typeTower = simpleTower.typeTower;
	packet01.towerData.name = simpleTower.name;
	packet01.towerData.radius = simpleTower.radius;
	packet01.towerData.range = simpleTower.range;
	packet01.towerData.cost = simpleTower.cost;
	packet01.towerData.fireRate = simpleTower.fireRate;
	std::cout << packet01.towerData.name << std::endl;
	TowerDataServerPacket packet02;
	TowerFrost towerFrost;
	packet02.towerData.typeTower = towerFrost.typeTower;
	packet02.towerData.name = towerFrost.name;
	packet02.towerData.radius = towerFrost.radius;
	packet02.towerData.range = towerFrost.range;
	packet02.towerData.cost = towerFrost.cost;
	packet02.towerData.fireRate = towerFrost.fireRate;
	std::cout << packet02.towerData.name << std::endl;
	TowerDataServerPacket packet03;
	TowerFast towerFast;
	packet03.towerData.typeTower = towerFast.typeTower;
	packet03.towerData.name = towerFast.name;
	packet03.towerData.radius = towerFast.radius;
	packet03.towerData.range = towerFast.range;
	packet03.towerData.cost = towerFast.cost;
	packet03.towerData.fireRate = towerFast.fireRate;
	std::cout << packet03.towerData.name << std::endl;

	for (auto& player : gameData.players)
	{
		switch (player.second.type)
		{
		case PlayerType::Attacker:
			//Do enemies
			break;
		case PlayerType::Defender:
			send_packet(player.second, build_packet(packet01, 0));
			send_packet(player.second, build_packet(packet02, 0));
			send_packet(player.second, build_packet(packet03, 0));
			break;
		default:
			break;
		}
	}
}

void handle_message(const std::vector<std::uint8_t>& message, GameData& gameData)
{
	// On traite les messages reçus par un joueur, différenciés par l'opcode
	std::size_t offset = 0;

	Opcode opcode = static_cast<Opcode>(Unserialize_u8(message, offset));
	switch (opcode)
	{
		case Opcode::C_EnemySpawn:
		{
			EnemySpawnClientPacket enemyPacket = EnemySpawnClientPacket::Unserialize(message, offset);

			std::cout << "enemyPacket received" << std::endl;
			break;
		}
		case Opcode::C_TowerSpawn:
		{
			TowerSpawnClientPacket towerPacket = TowerSpawnClientPacket::Unserialize(message, offset);

			std::cout << "towerSpawnPacket received" << std::endl;
			break;
		}
		case Opcode::C_CastlePosition:
		{
			CastlePositionClientPacket castlePacket = CastlePositionClientPacket::Unserialize(message, offset);

			std::cout << "castlePacket recived : " << castlePacket.posX << " " << castlePacket.posY << " " << castlePacket.posZ << std::endl;
			break;
		}
		case Opcode::C_EnemyPath:
		{
			EnemyPathClientPacket enemyPathPacket = EnemyPathClientPacket::Unserialize(message, offset);

			for (auto i = 0; i < enemyPathPacket.pathPoints.size(); i++)
			{
				std::cout << "enemyPathPacket received : " << enemyPathPacket.pathPoints[i] << std::endl;
			}

			EnemyPositionServerPacket enemyPositionPacket;
			enemyPositionPacket.nextPos = enemyPathPacket.pathPoints[0];

			send_packet(gameData.players.begin()->second, build_packet(enemyPositionPacket, 0));
		}
	}
}

void tick(ServerData& serverData, GameData& gameData)
{
	if (gameData.gameStarted)
	{
		if (gameData.goldTimer <= 0)
		{
			build_packet_gold(gameData);
			gameData.goldTimer = TimerGold;
		}
		gameData.goldTimer -= TickDelay;
	}
}
