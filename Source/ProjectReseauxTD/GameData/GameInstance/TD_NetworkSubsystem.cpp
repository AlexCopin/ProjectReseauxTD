// Fill out your copyright notice in the Description page of Project Settings.


#include "TD_NetworkSubsystem.h"
#include "ProjectReseauxTD/GameData/PlayerController/TD_PlayerController.h"

DEFINE_LOG_CATEGORY_STATIC(ENet6, Log, All);


void UTD_NetworkSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	if (enet_initialize() != 0)
	{
		UE_LOG(ENet6, Error, TEXT("failed to initialize ENet"));
		return;
	}
	/*if (!Connect("localhost"))
	{
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 10.0f, FColor::Red, TEXT("Not connected!"));
		return;
	}*/
}

void UTD_NetworkSubsystem::Deinitialize()
{
	enet_deinitialize();
}

void UTD_NetworkSubsystem::SendEnemySpawnClientPacket(const FEnemySpawnClientPacket& packet)
{
	send_packet(build_packet(packet, 0));
}

void UTD_NetworkSubsystem::SendEnemyPathClientPacket(const FEnemyPathClientPacket& packet)
{
	send_packet(build_packet(packet, 0));
}

void UTD_NetworkSubsystem::SendEnemyPosClientPacket(const FEnemyPosClientPacket& packet)
{
	send_packet(build_packet(packet, 0));
}

void UTD_NetworkSubsystem::SendSpawnTowerClientPacket(const FTowerSpawnClientPacket& packet)
{
	send_packet(build_packet(packet, 0));
}

void UTD_NetworkSubsystem::SendCastlePositionPacket(const FCastlePositionClientPacket& packet)
{
	send_packet(build_packet(packet, 0));
}

ETickableTickType UTD_NetworkSubsystem::GetTickableTickType() const
{
	return ETickableTickType::Always;
}

bool UTD_NetworkSubsystem::IsAllowedToTick() const
{
	return true;
}

void UTD_NetworkSubsystem::Tick(float /*DeltaTime*/)
{
	if (!Host)
	{
		return;
	}

	ENetEvent Event;
	if (enet_host_service (Host, &Event, 1) > 0)
	{
		do
		{
			switch (Event.type)
			{
				// On est connecté au serveur (ne devrait pas arriver si géré dans la méthode Connect)
			case ENetEventType::ENET_EVENT_TYPE_CONNECT:
				UE_LOG(ENet6, Log, TEXT("Peer %u connected!"), enet_peer_get_id(Event.peer));
				break;

				// On a perdu la connexion au serveur
			case ENetEventType::ENET_EVENT_TYPE_DISCONNECT:
        //Add event disconnected used in BP i don't know
				UE_LOG(ENet6, Log, TEXT("Peer %u disconnected!"), enet_peer_get_id(Event.peer));
				break;

				// On a reçu des données de la part du serveur (plus qu'à désérialiser ^_^)
			case ENetEventType::ENET_EVENT_TYPE_RECEIVE:
				UE_LOG(ENet6, Log, TEXT("Peer %u sent data (%u bytes)"), enet_peer_get_id(Event.peer), enet_packet_get_length(Event.packet));

				// On a reçu un message ! Traitons-le
				std::vector<std::uint8_t> content(Event.packet->dataLength); //< On copie son contenu dans un std::vector pour plus de facilité de gestion
				std::memcpy(content.data(), Event.packet->data, Event.packet->dataLength);

				// On gère le message qu'on a reçu
				handle_message(content);

				enet_packet_dispose(Event.packet);
				break;
			}
		} while (enet_host_check_events(Host, &Event) > 0);
	}
}

TStatId UTD_NetworkSubsystem::GetStatId() const
{
	return TStatId();
}

bool UTD_NetworkSubsystem::Connect(FString AddressString)
{
	// Petit défaut de cette méthode : elle bloque le temps de la connexion (cinq secondes de timeout)
	// Pour éviter ça, vous pouvez le gérer en asynchrone via l'événement ENET_EVENT_TYPE_CONNECT dans le tick
	Disconnect();

	ENetAddress Address;
	if (enet_address_set_host(&Address, ENET_ADDRESS_TYPE_ANY, TCHAR_TO_UTF8(*AddressString)) != 0)
	{
		UE_LOG(ENet6, Error, TEXT("Failed to resolve %s"), *AddressString);
		return false;
	}

	Address.port = 14769;
	UE_LOG(ENet6, Log, TEXT("Connecting..."));

	// On ne créé l'host qu'une fois qu'on connait le type d'adresse (IPv4/IPv6)
	Host = enet_host_create(Address.type, nullptr, 1, 0, 0, 0);
	if (!Host)
	{
		UE_LOG(ENet6, Error, TEXT("Failed to initialize host"));
		return false;
	}

	// On tente une connexion...
	ServerPeer = enet_host_connect(Host, &Address, 0, 0);
	check(ServerPeer);

	// On utilise la fonction enet_host_service avant d'entrer dans la boucle pour valider la connexion
	// Note : il est important d'appeler plusieurs fois la fonction plutôt qu'une seule fois dans ce cas, pour autoriser plusieurs tentatives de connexion
	// (l'API ENet ayant été conçue autour du fait qu'enet_host_service était appelé en boucle)

	// Nous simulons ça avec une boucle, où chaque tour de boucle va attendre 100ms (pour un total de 5s)
	for (std::size_t i = 0; i < 50; ++i)
	{
		ENetEvent Event;
		if (enet_host_service(Host, &Event, 100) > 0)
		{
			// Nous avons un événement, la connexion a soit pu s'effectuer (ENET_EVENT_TYPE_CONNECT) soit échoué (ENET_EVENT_TYPE_DISCONNECT)
			break; //< On sort de la boucle
		}
	}

	// On vérifie après la boucle l'état du peer, sommes-nous connectés ?
	if (ServerPeer->state != ENET_PEER_STATE_CONNECTED)
	{
		// On force la réinitialisation du serveur auprès d'enet pour pouvoir allouer un nouveau peer
		enet_peer_reset(ServerPeer);
		UE_LOG(ENet6, Error, TEXT("Failed to connect"));
		return false;
	}

	// Si on arrive ici, on est connecté !
	UE_LOG(ENet6, Log, TEXT("Connected to %s"), *AddressString);

	return true;
}

void UTD_NetworkSubsystem::Disconnect()
{
	if (ServerPeer)
	{
		check(Host);

		enet_peer_disconnect(ServerPeer, 0);
		enet_host_flush(Host);
		ServerPeer = nullptr;
	}

	if (Host)
	{
		enet_host_destroy(Host);
		Host = nullptr;
	}
}


void UTD_NetworkSubsystem::send_packet(ENetPacket* packet)
{
	if(ServerPeer)
		enet_peer_send(ServerPeer, 0, packet);
}

void UTD_NetworkSubsystem::handle_message(const std::vector<std::uint8_t>& message)
{
	TArray<uint8> messageArray;
	for(std::uint8_t i : message)
	{
		messageArray.Add(i);
	}
	// On décode l'opcode pour savoir à quel type de message on a affaire
	int32 offset = 0;
	EOpcode opcode = static_cast<EOpcode>(Unserialize_u8(messageArray, offset));
	switch (opcode)
	{
		case EOpcode::S_PlayerInit:
		{
			FPlayerInitServerPacket packet = FPlayerInitServerPacket::Unserialize(messageArray, offset);
			//PUT CODE TO SPAWN THE RIGHT PAWN
			FString playerTypeStr = packet.type == EPlayerType::Attacker ? "Attacker" : "Defender";
			FString debugString = FString::Printf(TEXT("Player Id = %f - PlayerType = %s"), (float)packet.index, *playerTypeStr);
			GEngine->AddOnScreenDebugMessage(INDEX_NONE, 10.0f, FColor::Red, *debugString);
			GetWorld()->GetFirstPlayerController<ATD_PlayerController>()->SpawnRightPawn(packet.type);
			//OnPlayerInitEvent.Broadcast(packet.type);
			break;
		}
		case EOpcode::S_EnemySpawn:
		{
			FEnemySpawnServerPacket packet = FEnemySpawnServerPacket::Unserialize(messageArray, offset);
			OnEnemySpawnEvent.Broadcast(packet);
			break;
		}
		case EOpcode::S_TowerSpawn:
		{
			FTowerSpawnServerPacket packet = FTowerSpawnServerPacket::Unserialize(messageArray, offset);
			OnTowerSpawnEvent.Broadcast(packet);
			break;
		}
		case EOpcode::S_EnemyPos:
		{
			FEnemyPositionServerPacket packet = FEnemyPositionServerPacket::Unserialize(messageArray, offset);
			OnEnemyPositionEvent.Broadcast(packet);
			break;
		}
		case EOpcode::S_Gold:
		{
			FGoldServerPacket packet = FGoldServerPacket::Unserialize(messageArray, offset);
			OnGoldChangeEvent.Broadcast(packet.value); 
			break;
		}
		case EOpcode::S_TowerData:
		{
			FTowerDataServerPacket packet = FTowerDataServerPacket::Unserialize(messageArray, offset);
			FString debugString = FString::Printf(TEXT("Name tower = : %s - Radius tower : %f"), *packet.towerData.Name, (float)packet.towerData.Radius);
			GEngine->AddOnScreenDebugMessage(INDEX_NONE, 10.0f, FColor::Red, *debugString);
			auto PC = GetWorld()->GetFirstPlayerController<ATD_PlayerController>();
			if (PC->PlayerWidget)
				PC->ReceiveTowerData(packet.towerData, true);
			else
				PC->ReceiveTowerData(packet.towerData, false);
			break;
		}
	}
}