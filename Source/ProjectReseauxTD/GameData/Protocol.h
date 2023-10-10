#pragma once
#include "CoreMinimal.h"

#include "Containers/Array.h"
//#include "Protocol.generated.h"

#include <enet6/enet.h>

UENUM(BlueprintType)
enum class EOpcode : uint8
{
	C_PlayerName,
	C_PlayerInput,
	C_EnemySpawn,
	S_PlayerList,
	S_WorldInit,
	S_EnemySpawn,
	S_Gold
};

UENUM(BlueprintType)
enum class EEnemyType
{
	Fast,
	Gold,
	Tank
};

void Serialize_f32(TArray<uint8>& byteArray, float value);
void Serialize_f32(TArray<uint8>& byteArray, int32 offset, float value);
void Serialize_i8(TArray<uint8>& byteArray, int8 value);
void Serialize_i8(TArray<uint8>& byteArray, int32 offset, int8 value);
void Serialize_i16(TArray<uint8>& byteArray, int16 value);
void Serialize_i16(TArray<uint8>& byteArray, int32 offset, int16 value);
void Serialize_i32(TArray<uint8>& byteArray, int32 value);
void Serialize_i32(TArray<uint8>& byteArray, int32 offset, int32 value);
void Serialize_u8(TArray<uint8>& byteArray, uint8 value);
void Serialize_u8(TArray<uint8>& byteArray, int32 offset, uint8 value);
void Serialize_u16(TArray<uint8>& byteArray, uint16 value);
void Serialize_u16(TArray<uint8>& byteArray, int32 offset, uint16 value);
void Serialize_u32(TArray<uint8>& byteArray, uint32 value);
void Serialize_u32(TArray<uint8>& byteArray, int32 offset, uint32 value);
void Serialize_str(TArray<uint8>& byteArray, const  FString& value);
void Serialize_str(TArray<uint8>& byteArray, int32 offset, const  FString& value);

float Unserialize_f32(const TArray<uint8>& byteArray, int32& offset);
int8 Unserialize_i8(const TArray<uint8>& byteArray, int32& offset);
int16 Unserialize_i16(const TArray<uint8>& byteArray, int32& offset);
int32 Unserialize_i32(const TArray<uint8>& byteArray, int32& offset);
uint8 Unserialize_u8(const TArray<uint8>& byteArray, int32& offset);
uint16 Unserialize_u16(const TArray<uint8>& byteArray, int32& offset);
uint32 Unserialize_u32(const TArray<uint8>& byteArray, int32& offset);
FString Unserialize_str(const TArray<uint8>& byteArray, int32& offset);


inline uint32 htonf(float value);


// Un joueur souhaite renseigner son nom
struct PlayerNamePacket
{
	static constexpr EOpcode opcode = EOpcode::C_PlayerName;

	std::string name;

	void Serialize(std::vector<std::uint8_t>& byteArray) const;
	static PlayerNamePacket Unserialize(const std::vector<std::uint8_t>& byteArray, std::size_t& offset);
};


// Le serveur envoie à un client la liste de tous les joueurs connectés
//0

// Le serveur indique au client comment générer le monde
USTRUCT(BlueprintType)
struct FWorldInitPacket
{
	GENERATED_BODY()
public:

	std::uint16_t width;
	std::uint16_t height;
	std::uint32_t seed;
private:

	static constexpr EOpcode opcode = EOpcode::S_WorldInit;
	void Serialize(std::vector<std::uint8_t>& byteArray) const;
	static FWorldInitPacket Unserialize(const std::vector<std::uint8_t>& byteArray, std::size_t& offset);
};

USTRUCT(BlueprintType)
struct FEnemySpawnClientPacket
{
	GENERATED_BODY()
public:

	uint8 line;
	EEnemyType enemyType;
private:

	static constexpr EOpcode opcode = EOpcode::C_EnemySpawn;
	void Serialize(std::vector<std::uint8_t>& byteArray) const;
	static FEnemySpawnClientPacket Unserialize(const std::vector<std::uint8_t>& byteArray, std::size_t& offset);
};



// Petite fonction d'aide pour construire un packet ENet à partir d'une de nos structures de packet, insère automatiquement l'opcode au début des données
template<typename T> ENetPacket* build_packet(const T& packet, enet_uint32 flags)
{
	// On sérialise l'opcode puis le contenu du packet dans un std::vector<std::uint8_t>
	TArray<uint8> byteArray;

	Serialize_u8(byteArray, static_cast<uint8>(T::opcode));
	packet.Serialize(byteArray);

	// On copie le contenu de ce vector dans un packet enet, et on l'envoie au peer
	return enet_packet_create(byteArray.data(), byteArray.size(), flags);
}
