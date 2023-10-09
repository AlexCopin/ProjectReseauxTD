#include "Protocol.h"
#include "terse/utils/Endianness.h"

void Serialize_f32(TArray<uint8>& byteArray, float value)
{
	int32 offset = TArray::CountBytes(byteArray);
	byteArray.Reserve(offset + sizeof(value));

	return Serialize_f32(byteArray, offset, value);
}

void Serialize_f32(TArray<uint8>& byteArray, int32 offset, float value)
{
	uint32 v = hton(value);

	check(offset + sizeof(v) <= byteArray.CountBytes());
	std::memcpy(&byteArray[offset], &v, sizeof(v));
}

void Serialize_i8(TArray<uint8>& byteArray, int8 value)
{
}

void Serialize_i8(TArray<uint8>& byteArray, int32 offset, int8 value)
{
}

void Serialize_i16(TArray<uint8>& byteArray, int16 value)
{
}

void Serialize_i16(TArray<uint8>& byteArray, int32 offset, int16 value)
{
}

void Serialize_i32(TArray<uint8>& byteArray, int32 value)
{
}

void Serialize_i32(TArray<uint8>& byteArray, int32 offset, int32 value)
{
}

void Serialize_u8(TArray<uint8>& byteArray, uint8 value)
{
}

void Serialize_u8(TArray<uint8>& byteArray, int32 offset, uint8 value)
{
}

void Serialize_u16(TArray<uint8>& byteArray, uint16 value)
{
}

void Serialize_u16(TArray<uint8>& byteArray, int32 offset, uint16 value)
{
}

void Serialize_u32(TArray<uint8>& byteArray, uint32 value)
{
}

void Serialize_u32(TArray<uint8>& byteArray, int32 offset, uint32 value)
{
}

void Serialize_str(TArray<uint8>& byteArray, const FString& value)
{
}

void Serialize_str(TArray<uint8>& byteArray, int32 offset, const FString& value)
{
}

float Unserialize_f32(const TArray<uint8>& byteArray, int32& offset)
{
	return 0.0f;
}

int8 Unserialize_i8(const TArray<uint8>& byteArray, int32& offset)
{
	return int8();
}

int16 Unserialize_i16(const TArray<uint8>& byteArray, int32& offset)
{
	return int16();
}

int32 Unserialize_i32(const TArray<uint8>& byteArray, int32& offset)
{
	return int32();
}

uint8 Unserialize_u8(const TArray<uint8>& byteArray, int32& offset)
{
	return uint8();
}

uint16 Unserialize_u16(const TArray<uint8>& byteArray, int32& offset)
{
	return uint16();
}

uint32 Unserialize_u32(const TArray<uint8>& byteArray, int32& offset)
{
	return uint32();
}

FString Unserialize_str(const TArray<uint8>& byteArray, int32& offset)
{
	return FString();
}
