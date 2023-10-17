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
	std::uint32_t currentTowerIndex = 0;
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
void build_packet_spawnableData(GameData& gameData, const Player& player);
void send_packet(const Player& player, ENetPacket* packet);
void check_enemies_position(GameData& gameData);

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
							build_packet_spawnableData(gameData, newPlayer);

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

void check_enemies_position(GameData& gameData)
{
	for (const auto& enemy : gameData.enemies)
	{
		if (Vector3::Distance(enemy.second.actualPosition, enemy.second.nextPoint) <= 0.3f)
		{
			//enemy.second.nextPoint++;
			//Send nextPoint to client
		}
	}
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

void build_packet_spawnableData(GameData& gameData, const Player& player)
{
	TowerDataServerPacket packet01;
	packet01.towerData = TowerSimpleToTower();
	send_packet(player, build_packet(packet01, 0));
	TowerDataServerPacket packet02;
	packet02.towerData = TowerSlowToTower();
	send_packet(player, build_packet(packet02, 0));
	TowerDataServerPacket packet03;
	packet03.towerData = TowerFastToTower();
	send_packet(player, build_packet(packet03, 0));
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
			//if(Bla bla bla test Radius avec autres tourelles) pas le temps
			//Oh c'est bon on peut y aller!! on envoie packet retour
			TowerSpawnServerPacket serverTowerPacket;
			serverTowerPacket.towerType = (std::uint8_t)towerPacket.towerType;
			serverTowerPacket.index = gameData.currentTowerIndex;
			serverTowerPacket.position = towerPacket.position;
      
			//Add Pos in Tower later
      Tower towerData;
			switch(towerPacket.towerType)
			{
			case TowerType::Normal:
        towerData = TowerSimpleToTower();
				break;
			case TowerType::Fast:
        towerData = TowerFastToTower();
				break;
			case TowerType::Slow:
        towerData = TowerSlowToTower();
				break;
			}
      if (gameData.goldDefender < towerData.cost)
        return;
      gameData.towers.emplace(gameData.currentTowerIndex, towerData);

      GoldServerPacket goldPacket;
      gameData.goldDefender -= towerData.cost;
      goldPacket.value = gameData.goldDefender;

			for(const auto& player : gameData.players)
			{
        std::cout << "Send ServerTowerPacket" << std::endl;
        if(player.second.type == PlayerType::Defender)
        {
				  send_packet(player.second, build_packet(goldPacket, 0));
        }

				send_packet(player.second, build_packet(serverTowerPacket, 0));
			}
			gameData.currentTowerIndex++;
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

			if (gameData.enemies.size() != 0)
			{
				gameData.enemies.find(enemyPathPacket.enemyIndex)->second.nextPoint = enemyPathPacket.pathPoints[0];

				EnemyPositionServerPacket enemyPositionPacket;
				enemyPositionPacket.nextPos = enemyPathPacket.pathPoints[0];

				send_packet(gameData.players.begin()->second, build_packet(enemyPositionPacket, 0));
			}
			break;
		}
		case Opcode::C_EnemyPos:
		{
			EnemyPosClientPacket enemyPosPacket = EnemyPosClientPacket::Unserialize(message, offset);

			if (gameData.enemies.size() != 0)
				gameData.enemies.find(enemyPosPacket.enemyIndex)->second.actualPosition = enemyPosPacket.actualPos;

			check_enemies_position(gameData);
			break;
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
