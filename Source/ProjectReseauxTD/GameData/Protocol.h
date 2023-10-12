#pragma once
#include "CoreMinimal.h"

#include "Containers/Array.h"
#include <string>
#include "Math/Vector.h"
#include <enet6/enet.h>
#include "Protocol.generated.h"


UENUM(BlueprintType)
enum class EOpcode : uint8
{
	C_PlayerName,
	C_PlayerInput,
	C_EnemySpawn,
	C_TowerSpawn,
	C_EnemyPath,
	S_PlayerList,
	S_EnemySpawn,
	S_EnemyPos,
	S_TowerSpawn,
	S_Gold
};

UENUM(BlueprintType)
enum class EEnemyType : uint8
{
	Fast,
	Gold,
	Tank
};
UENUM(BlueprintType)
enum class ETowerType : uint8
{
	Normal,
	Slow,
	Fast
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
void Serialize_str(TArray<uint8>& byteArray, const  std::string& value);
void Serialize_str(TArray<uint8>& byteArray, int32 offset, const  FString& value);
void Serialize_str(TArray<uint8>& byteArray, int32 offset, const  std::string& value);
void Serialize_v3(TArray<uint8>& byteArray, FVector value);
void Serialize_v3(TArray<uint8>& byteArray, int32 offset, FVector value);

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


// Le serveur envoie � un client la liste de tous les joueurs connect�s
//0

USTRUCT(BlueprintType)
struct FEnemySpawnClientPacket
{
	GENERATED_BODY()
public:

	uint8 line;
	EEnemyType enemyType;
	static constexpr EOpcode opcode = EOpcode::C_EnemySpawn;
	void Serialize(TArray<uint8>& byteArray) const;
	//static FEnemySpawnClientPacket Unserialize(const std::vector<std::uint8_t>& byteArray, std::size_t& offset);
};

USTRUCT(BlueprintType)
struct FEnemySpawnServerPacket
{
	GENERATED_BODY()
public:
	uint8 line;
	uint8 enemyType;
	uint32 index;

	static constexpr EOpcode opcode = EOpcode::S_EnemySpawn;
	static FEnemySpawnServerPacket Unserialize(const TArray<uint8>& byteArray, int32& offset);
};

USTRUCT(BlueprintType)
struct FTowerSpawnClientPacket
{
	GENERATED_BODY()
public:

	ETowerType towerType;
	float posX;
	float posY;
	float posZ;
	int32 radius;

	static constexpr EOpcode opcode = EOpcode::C_TowerSpawn;
	void Serialize(TArray<uint8>& byteArray) const;
};

USTRUCT(BlueprintType)
struct FTowerSpawnServerPacket
{
	GENERATED_BODY()
public:

	uint8 towerType;
	float posX;
	float posY;
	float posZ;
	int32 range;
	int32 radius;

	static constexpr EOpcode opcode = EOpcode::S_TowerSpawn;
	static FTowerSpawnServerPacket Unserialize(const TArray<uint8>& byteArray, int32& offset);
};

USTRUCT(BlueprintType)
struct FEnemyPathClientPacket
{
	GENERATED_BODY()
public:

	TArray<FVector> pathPoints;
	uint8 elementsNumber;

	static constexpr EOpcode opcode = EOpcode::C_EnemyPath;
	void Serialize(TArray<uint8>& byteArray) const;
	//static FEnemySpawnClientPacket Unserialize(const std::vector<std::uint8_t>& byteArray, std::size_t& offset);
};

USTRUCT(BlueprintType)
struct FEnemyPositionServerPacket
{
	GENERATED_BODY()
public:

	TArray<FVector> pathPoints;
	uint8 elementsNumber;

	static constexpr EOpcode opcode = EOpcode::S_EnemyPos;
	static FEnemyPositionServerPacket Unserialize(const TArray<uint8>& byteArray, int32& offset);
};


// Petite fonction d'aide pour construire un packet ENet � partir d'une de nos structures de packet, ins�re automatiquement l'opcode au d�but des donn�es
template<typename T> ENetPacket* build_packet(const T& packet, enet_uint32 flags)
{
	// On s�rialise l'opcode puis le contenu du packet dans un std::vector<std::uint8_t>
	TArray<uint8> byteArray;

	Serialize_u8(byteArray, static_cast<uint8>(T::opcode));
	packet.Serialize(byteArray);
		
	// On copie le contenu de ce vector dans un packet enet, et on l'envoie au peer
	return enet_packet_create(byteArray.GetData(), byteArray.Num(), flags);
}
