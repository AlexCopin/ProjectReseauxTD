#include "Protocol.h"
#include "Containers/Array.h"

void Serialize_f32(TArray<uint8>& byteArray, float value)
{
	int32 offset = byteArray.Num();
	byteArray.SetNum(offset + sizeof(value));

	return Serialize_f32(byteArray, offset, value);
}

void Serialize_f32(TArray<uint8>& byteArray, int32 offset, float value)
{
	float v = NETWORK_ORDERF(value);

	check((offset + sizeof(v)) <= byteArray.Num());
	std::memcpy(&byteArray[offset], &v, sizeof(v));
}

void Serialize_i8(TArray<uint8>& byteArray, int8 value)
{
	return Serialize_u8(byteArray, StaticCast<uint8>(value));
}

void Serialize_i8(TArray<uint8>& byteArray, int32 offset, int8 value)
{
	return Serialize_u8(byteArray, offset, StaticCast<uint8>(value));
}

void Serialize_i16(TArray<uint8>& byteArray, int16 value)
{
	return Serialize_u16(byteArray, StaticCast<uint16>(value));
}

void Serialize_i16(TArray<uint8>& byteArray, int32 offset, int16 value)
{
	return Serialize_u16(byteArray, offset, StaticCast<uint16>(value));
}

void Serialize_i32(TArray<uint8>& byteArray, int32 value)
{
	Serialize_u32(byteArray, StaticCast<uint32>(value));
}

void Serialize_i32(TArray<uint8>& byteArray, int32 offset, int32 value)
{
	Serialize_u32(byteArray, offset, StaticCast<uint32>(value));
}

void Serialize_u8(TArray<uint8>& byteArray, uint8 value)
{
	int32 offset = byteArray.Num();
	byteArray.SetNum(offset + sizeof(value));

	return Serialize_u8(byteArray, offset, value);
}

void Serialize_u8(TArray<uint8>& byteArray, int32 offset, uint8 value)
{
	check(offset < byteArray.Num());
	byteArray[offset] = value;
}

void Serialize_u16(TArray<uint8>& byteArray, uint16 value)
{
	int32 offset = byteArray.Num();
	byteArray.SetNum(offset + sizeof(value));

	return Serialize_u16(byteArray, offset, value);
}

void Serialize_u16(TArray<uint8>& byteArray, int32 offset, uint16 value)
{
	value = NETWORK_ORDER16(value);

	check(offset + sizeof(value) <= byteArray.Num());
	std::memcpy(&byteArray[offset], &value, sizeof(value));
}

void Serialize_u32(TArray<uint8>& byteArray, uint32 value)
{
	int32 offset = byteArray.Num();
	byteArray.SetNum(offset + sizeof(value));

	return Serialize_u32(byteArray, offset, value);
}

void Serialize_u32(TArray<uint8>& byteArray, int32 offset, uint32 value)
{
	value = NETWORK_ORDER32(value);

	check(offset + sizeof(value) <= byteArray.Num());
	std::memcpy(&byteArray[offset], &value, sizeof(value));
}

void Serialize_str(TArray<uint8>& byteArray, const FString& value)
{
	return Serialize_str(byteArray, std::string(TCHAR_TO_UTF8(*value)));
}

void Serialize_str(TArray<uint8>& byteArray, const std::string& value)
{
	int32 offset = byteArray.Num();

	byteArray.SetNum(offset + sizeof(int32) + value.size());
	return Serialize_str(byteArray, offset, value);
}

void Serialize_str(TArray<uint8>& byteArray, int32 offset, const FString& value)
{
	return Serialize_str(byteArray, offset, std::string(TCHAR_TO_UTF8(*value)));
}

void Serialize_str(TArray<uint8>& byteArray, int32 offset, const std::string& value)
{
	Serialize_u32(byteArray, offset, StaticCast<uint32>(value.size()));
	offset += sizeof(uint32);

	if (!value.empty())
		std::memcpy(&byteArray[offset], value.data(), value.size());
}

void Serialize_v3(TArray<uint8>& byteArray, FVector value)
{
	Serialize_f32(byteArray, value.X);
	Serialize_f32(byteArray, value.Y);
	Serialize_f32(byteArray, value.Z);
}

void Serialize_v3(TArray<uint8>& byteArray, int32 offset, FVector value)
{
	Serialize_f32(byteArray, offset, value.X);
	Serialize_f32(byteArray, offset, value.Y);
	Serialize_f32(byteArray, offset, value.Z);
}

FVector Unserialize_v3(const TArray<uint8>& byteArray, int32& offset)
{
	FVector returnValue;
	returnValue.X = Unserialize_f32(byteArray, offset);
	returnValue.Y = Unserialize_f32(byteArray, offset);
	returnValue.Z = Unserialize_f32(byteArray, offset);

	return returnValue;
}

float Unserialize_f32(const TArray<uint8>& byteArray, int32& offset)
{
	float value;
	std::memcpy(&value, &byteArray[offset], sizeof(value));

	offset += sizeof(value);

	return NETWORK_ORDERF(value);
}

int8 Unserialize_i8(const TArray<uint8>& byteArray, int32& offset)
{
	return StaticCast<int8>(Unserialize_u8(byteArray, offset));
}

int16 Unserialize_i16(const TArray<uint8>& byteArray, int32& offset)
{
	return StaticCast<int16>(Unserialize_u16(byteArray, offset));
}

int32 Unserialize_i32(const TArray<uint8>& byteArray, int32& offset)
{
	return StaticCast<int32>(Unserialize_u32(byteArray, offset));
}

uint8 Unserialize_u8(const TArray<uint8>& byteArray, int32& offset)
{
	uint8 value = byteArray[offset];
	offset += sizeof(value);

	return value;
}

uint16 Unserialize_u16(const TArray<uint8>& byteArray, int32& offset)
{
	std::uint16_t value;
	std::memcpy(&value, &byteArray[offset], sizeof(value));
	value = NETWORK_ORDER16(value);

	offset += sizeof(value);

	return value;
}

uint32 Unserialize_u32(const TArray<uint8>& byteArray, int32& offset)
{
	uint32 value;
	std::memcpy(&value, &byteArray[offset], sizeof(value));
	value = NETWORK_ORDER32(value);

	offset += sizeof(value);

	return value;
}

FString Unserialize_str(const TArray<uint8>& byteArray, int32& offset)
{
	uint32 length = Unserialize_u32(byteArray, offset);
	std::string str(length, ' ');
	std::memcpy(&str[0], &byteArray[offset], length);

	offset += length;

	return UTF8_TO_TCHAR(str.data());
}

void FEnemySpawnClientPacket::Serialize(TArray<uint8>& byteArray) const
{
	Serialize_u8(byteArray, line);
	Serialize_u8(byteArray, (uint8)enemyType);
}

void FEnemyPathClientPacket::Serialize(TArray<uint8>& byteArray) const
{
	Serialize_u8(byteArray, pathPoints.Num());

	for (const auto& pos : pathPoints)
		Serialize_v3(byteArray, pos);
}

void FTowerSpawnClientPacket::Serialize(TArray<uint8>& byteArray) const
{
	Serialize_u8(byteArray, (uint8)towerType);
	Serialize_f32(byteArray, posX);
	Serialize_f32(byteArray, posY);
	Serialize_f32(byteArray, posZ);
	Serialize_u32(byteArray, radius);
}


FEnemySpawnServerPacket FEnemySpawnServerPacket::Unserialize(const TArray<uint8>& byteArray, int32& offset)
{
	FEnemySpawnServerPacket packet;
	packet.line = Unserialize_u8(byteArray, offset);
	packet.enemyType = Unserialize_u8(byteArray, offset);
	packet.index = Unserialize_u32(byteArray, offset);
	return packet;
}

FEnemyPositionServerPacket FEnemyPositionServerPacket::Unserialize(const TArray<uint8>& byteArray, int32& offset)
{
	FEnemyPositionServerPacket packet;
	
	packet.enemyIndex = Unserialize_u8(byteArray, offset);
	packet.nextPos = Unserialize_v3(byteArray, offset);

	return packet;
}

FTowerSpawnServerPacket FTowerSpawnServerPacket::Unserialize(const TArray<uint8>& byteArray, int32& offset)
{
	FTowerSpawnServerPacket packet;
	packet.towerType = Unserialize_u8(byteArray, offset);
	packet.posX = Unserialize_f32(byteArray, offset);
	packet.posY = Unserialize_f32(byteArray, offset);
	packet.posZ = Unserialize_f32(byteArray, offset);
	packet.range = Unserialize_u32(byteArray, offset);
	packet.radius = Unserialize_u32(byteArray, offset);
	return packet;
}

void FCastlePositionClientPacket::Serialize(TArray<uint8>& byteArray) const
{
	Serialize_f32(byteArray, posX);
	Serialize_f32(byteArray, posY);
	Serialize_f32(byteArray, posZ);
}

FPlayerInitServerPacket FPlayerInitServerPacket::Unserialize(const TArray<uint8>& byteArray, int32& offset)
{
	FPlayerInitServerPacket packet;
	packet.type = EPlayerType(Unserialize_u8(byteArray, offset));
	packet.index = Unserialize_u32(byteArray, offset);

	return packet;
}

FGoldServerPacket FGoldServerPacket::Unserialize(const TArray<uint8>& byteArray, int32& offset)
{
	FGoldServerPacket packet;
	packet.value = Unserialize_u32(byteArray, offset);
	return packet;
}

FTowerDataServerPacket FTowerDataServerPacket::Unserialize(const TArray<uint8>& byteArray, int32& offset)
{
	FTowerDataServerPacket packet;
	packet.towerData.Name = Unserialize_str(byteArray, offset);
	packet.towerData.Radius = Unserialize_u32(byteArray, offset);
	packet.towerData.Range = Unserialize_u32(byteArray, offset);
	packet.towerData.Cost = Unserialize_u32(byteArray, offset);
	return packet;
}
