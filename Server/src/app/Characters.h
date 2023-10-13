#pragma once

#include <enet6/enet.h>



//Gold
constexpr std::int32_t TimerGold = 500;
//Attacker
constexpr std::uint32_t AttackerStartingGold = 100;
constexpr std::uint32_t AttackerGoldPerSecond = 20;
constexpr float Multiplicator = 1.2f;

//Defender
constexpr std::uint32_t DefenderStartingGold = 200;
constexpr std::uint32_t DefenderGoldPerSecond = 5;

enum class PlayerType : std::uint8_t
{
	Attacker,
	Defender,
	Spectator
};

struct Player
{
	ENetPeer* peer;
	PlayerType type;
	std::uint32_t index;
	std::uint32_t golds;
};



enum class EnemyType
{
	Fast,
	Gold,
	Tank
};

enum class TowerType
{
	Normal,
	Slow,
	Fast
};

struct Enemy
{
	std::uint32_t index;
};

//See for inheritance
struct Tower 
{
	const float radius = 100.0f;
};
struct TowerSimple : Tower
{
	const float radius = 100.0f;
	const float range = 300.0f;
	const float fireRate = 0.7f;
};
struct TowerFrost : Tower
{
	const float radius = 150.0f;
	const float range = 400.0f;
	const float fireRate = 1.2f;
};
struct TowerFast : Tower
{
	const float radius = 100.0f;
	const float range = 500.0f;
	const float fireRate = 0.4f;
};