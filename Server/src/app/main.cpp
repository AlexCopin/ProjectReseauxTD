#include <app/constants.hpp>
#include <utils/exit_capsule.hpp>
#include <enet6/enet.h>
#include <math/quaternion.hpp>
#include <iostream>
#include <string>
#include <vector>
#include "Protocol.hpp"

struct ServerData
{
	ENetHost* host;
};

struct Enemy
{
	std::uint32_t index; //< La position du joueur dans le tableau (sert d'id numérique lors de l'affichage)
	std::uint8_t name; //< Nom du joueur

};

struct GameData
{
	std::uint32_t currentEnemyIndex = 0;
	std::vector<Enemy> enemies;
};

void tick(ServerData& serverData);
void handle_message(const std::vector<std::uint8_t>& message);

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
						handle_message(content);

						// On n'oublie pas de libérer le packet
						enet_packet_destroy(event.packet);
						break;
				}
			}
			while (enet_host_check_events(serverData.host, &event) > 0);
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


void handle_message(const std::vector<std::uint8_t>& message, GameData& gameData)
{
	// On traite les messages reçus par un joueur, différenciés par l'opcode
	std::size_t offset = 0;

	Opcode opcode = static_cast<Opcode>(Unserialize_u8(message, offset));
	switch (opcode)
	{
		case Opcode::C_EnemySpawn:
			EnemySpawnClientPacket enemyPacket = EnemySpawnClientPacket::Unserialize(message, offset);

			std::cout << "enemyPacket received" << std::endl;
			break;

	}
}

void tick(ServerData& serverData)
{
}
