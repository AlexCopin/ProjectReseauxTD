#pragma once 

#include <iostream>
#include <vector>
#include <string>

enum class Opcode : std::uint8_t
{
	C_PlayerName,
	C_PlayerInput,
	C_EnemySpawn,
	S_PlayerList,
	S_WorldInit,
	S_EnemySpawn,
	S_Gold
};

void Serialize_f32(std::vector<std::uint8_t>& byteArray, float value);
void Serialize_f32(std::vector<std::uint8_t>& byteArray, std::size_t offset, float value);
void Serialize_i8(std::vector<std::uint8_t>& byteArray, std::int8_t value);
void Serialize_i8(std::vector<std::uint8_t>& byteArray, std::size_t offset, std::int8_t value);
void Serialize_i16(std::vector<std::uint8_t>& byteArray, std::int16_t value);
void Serialize_i16(std::vector<std::uint8_t>& byteArray, std::size_t offset, std::int16_t value);
void Serialize_i32(std::vector<std::uint8_t>& byteArray, std::int32_t value);
void Serialize_i32(std::vector<std::uint8_t>& byteArray, std::size_t offset, std::int32_t value);
void Serialize_u8(std::vector<std::uint8_t>& byteArray, std::uint8_t value);
void Serialize_u8(std::vector<std::uint8_t>& byteArray, std::size_t offset, std::uint8_t value);
void Serialize_u16(std::vector<std::uint8_t>& byteArray, std::uint16_t value);
void Serialize_u16(std::vector<std::uint8_t>& byteArray, std::size_t offset, std::uint16_t value);
void Serialize_u32(std::vector<std::uint8_t>& byteArray, std::uint32_t value);
void Serialize_u32(std::vector<std::uint8_t>& byteArray, std::size_t offset, std::uint32_t value);
void Serialize_str(std::vector<std::uint8_t>& byteArray, const std::string& value);
void Serialize_str(std::vector<std::uint8_t>& byteArray, std::size_t offset, const std::string& value);

float Unserialize_f32(const std::vector<std::uint8_t>& byteArray, std::size_t& offset);
std::int8_t Unserialize_i8(const std::vector<std::uint8_t>& byteArray, std::size_t& offset);
std::int16_t Unserialize_i16(const std::vector<std::uint8_t>& byteArray, std::size_t& offset);
std::int32_t Unserialize_i32(const std::vector<std::uint8_t>& byteArray, std::size_t& offset);
std::uint8_t Unserialize_u8(const std::vector<std::uint8_t>& byteArray, std::size_t& offset);
std::uint16_t Unserialize_u16(const std::vector<std::uint8_t>& byteArray, std::size_t& offset);
std::uint32_t Unserialize_u32(const std::vector<std::uint8_t>& byteArray, std::size_t& offset);
std::string Unserialize_str(const std::vector<std::uint8_t>& byteArray, std::size_t& offset);



struct EnemySpawnClientPacket
{
	std::uint8_t line;
	std::uint8_t enemyType;



	static constexpr Opcode opcode = Opcode::C_EnemySpawn;
	static EnemySpawnClientPacket Unserialize(const std::vector<std::uint8_t>& byteArray, std::size_t& offset);
};