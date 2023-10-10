#pragma once
#include "CoreMinimal.h"

#include "Containers/Array.h"
#include <string>
//#include "Protocol.generated.h"

enum class Opcode : uint8
{
	C_PlayerName,
	C_PlayerInput,
	S_Gold
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

float Unserialize_f32(const TArray<uint8>& byteArray, int32& offset);
int8 Unserialize_i8(const TArray<uint8>& byteArray, int32& offset);
int16 Unserialize_i16(const TArray<uint8>& byteArray, int32& offset);
int32 Unserialize_i32(const TArray<uint8>& byteArray, int32& offset);
uint8 Unserialize_u8(const TArray<uint8>& byteArray, int32& offset);
uint16 Unserialize_u16(const TArray<uint8>& byteArray, int32& offset);
uint32 Unserialize_u32(const TArray<uint8>& byteArray, int32& offset);
FString Unserialize_str(const TArray<uint8>& byteArray, int32& offset);