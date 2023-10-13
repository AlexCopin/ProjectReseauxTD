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
	std::uint32_t currentPlayerIndex = 0;
	std::uint32_t currentEnemyIndex = 0;
	std::map<std::uint32_t, Player> players;
	std::map<std::uint32_t, Enemy> enemies;
	std::map<std::uint32_t, Tower> towers;

	float goldTimer = TimerGold;

	bool gameStarted = false;
};

void tick(ServerData& serverData);
void handle_message(const std::vector<std::uint8_t>& message, GameData& gameData);
void build_packet_gold(GameData& gameData);


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
						//Send info 
						Player newPlayer;
						newPlayer.index = gameData.currentPlayerIndex;
						newPlayer.peer = event.peer;

						switch(gameData.currentPlayerIndex)
						{
						case 0:
							newPlayer.type = PlayerType::Attacker;
							break;
						case 1:
							newPlayer.type = PlayerType::Defender;
							break;
						default:
							newPlayer.type = PlayerType::Spectator;
							break;
						}
						gameData.currentPlayerIndex++;
						gameData.players.emplace(newPlayer.index ,std::move(newPlayer));
						if(gameData.players.size() >= 2)
						{
							gameData.gameStarted = true;
						}
						PlayerInitServerPacket packet;
						packet.player = newPlayer;
						enet_peer_send(event.peer, 0, build_packet(packet, 0));

						std::cout << "Peer #" << enet_peer_get_id(event.peer) << " connected!" << std::endl;
						break;

					// Un joueur s'est déconnecté
					case ENetEventType::ENET_EVENT_TYPE_DISCONNECT:
						std::cout << "Peer #" << enet_peer_get_id(event.peer) << " disconnected!" << std::endl;
						break;

					// On a reçu des données d'un joueur
					case ENetEventType::ENET_EVENT_TYPE_RECEIVE:

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
			while (enet_host_check_events(serverData.host, &event) > 0);
		}
		if (gameData.gameStarted)
		{
			uint32_t elapsedTime = now - nextTick;
			if (gameData.goldTimer <= 0)
			{
				build_packet_gold(gameData);
				gameData.goldTimer = TimerGold;
			}
			gameData.goldTimer -= elapsedTime;
		}
		// On déclenche un tick si suffisamment de temps s'est écoulé
		if (now >= nextTick)
		{
			tick(serverData);
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
			player.second.golds += AttackerGoldPerSecond;
			packet.value = player.second.golds;
			send_packet(player.second, build_packet(packet, 0));
			break;
		case PlayerType::Defender:
			player.second.golds += DefenderGoldPerSecond;
			packet.value = player.second.golds;
			send_packet(player.second, build_packet(packet, 0));
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
	}
}

void tick(ServerData& serverData)
{
}
