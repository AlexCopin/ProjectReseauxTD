#pragma once

#include <enet6/enet.h>
#include "math/vector3.hpp"



//Gold
constexpr std::int32_t TimerGold = 1000;
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
};



enum class EnemyType : std::uint8_t
{
	Fast,
	Gold,
	Tank
};

enum class TowerType : std::uint8_t
{
	Normal,
	Slow,
	Fast
};

struct Enemy
{
	std::uint32_t index;
	std::vector<Vector3> pathPoints;
};

//See for inheritance
struct Tower 
{
	TowerType typeTower;
	std::string name;
	std::uint32_t radius;
	std::uint32_t range;
	std::uint32_t cost;
	float fireRate;
};
struct TowerSimple
{
	const TowerType typeTower = TowerType::Normal;
	const std::string name = "Simple Tower";
	const std::uint32_t radius = 100;
	const std::uint32_t range = 300.0f;
	const std::uint32_t cost = 100;
	const float fireRate = 0.7f;
};
struct TowerFrost
{
	const TowerType typeTower = TowerType::Slow;
	const std::string name = "Frost Tower";
	const std::uint32_t radius = 150;
	const std::uint32_t range = 400;
	const std::uint32_t cost = 150;
	const float fireRate = 1.2f;
};
struct TowerFast
{
	const TowerType typeTower = TowerType::Fast;
	const std::string name = "Fast Tower";
	const std::uint32_t radius = 100;
	const std::uint32_t range = 50;
	const std::uint32_t cost = 150;
	const float fireRate = 0.4f;
};