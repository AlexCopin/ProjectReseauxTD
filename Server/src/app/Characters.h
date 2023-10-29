#pragma once

#include <enet6/enet.h>
#include "math/vector3.hpp"
#include <vector>



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

//See for inheritance
struct Tower 
{
	TowerType typeTower;
	std::string name;
	std::uint32_t radius;
	std::uint32_t range;
	std::uint32_t damage;
	std::uint32_t cost;
	float fireRate;
};
struct TowerSimple
{
	const TowerType typeTower = TowerType::Normal;
	const std::string name = "Simple Tower";
	const std::uint32_t radius = 150;
	const std::uint32_t range = 600;
	const std::uint32_t damage = 50;
	const std::uint32_t cost = 100;
	const float fireRate = 0.7f;
};
struct TowerFrost
{
	const TowerType typeTower = TowerType::Slow;
	const std::string name = "Frost Tower";
	const std::uint32_t radius = 250;
	const std::uint32_t range = 800;
	const std::uint32_t damage = 80;
	const std::uint32_t cost = 150;
	const float fireRate = 1.2f;
};
struct TowerFast
{
	const TowerType typeTower = TowerType::Fast;
	const std::string name = "Fast Tower";
	const std::uint32_t radius = 200;
	const std::uint32_t range = 600;
	const std::uint32_t damage = 30;
	const std::uint32_t cost = 150;
	const float fireRate = 0.4f;
};

struct Enemy
{
	std::uint32_t index;
	std::vector<Vector3> pathPoints;
	std::uint8_t actualPointIndex;
	Vector3 nextPoint;
	Vector3 actualPosition;

	EnemyType typeEnemy;
	std::string name;
	std::uint32_t health;
	std::uint32_t damage;
	std::uint32_t range;
	std::uint32_t cost;
	std::uint32_t gain;
	float fireRate;
};
struct EnemyFast
{
	const EnemyType typeEnemy = EnemyType::Fast;
	const std::string name = "Fast Enemy";
	const std::uint32_t health = 170;
	const std::uint32_t damage = 50;
	const std::uint32_t range = 20;
	const std::uint32_t cost = 70;
	const std::uint32_t gain = 20;
	const float fireRate = 0.4f;
};
struct EnemyGold
{
	const EnemyType typeEnemy = EnemyType::Gold;
	const std::string name = "Gold Enemy";
	const std::uint32_t health = 150;
	const std::uint32_t damage = 0;
	const std::uint32_t range = 15;
	const std::uint32_t cost = 120;
	const std::uint32_t gain = 60;
	const float fireRate = 0.0f;
};
struct EnemyTank
{
	const EnemyType typeEnemy = EnemyType::Tank;
	const std::string name = "Tank Enemy";
	const std::uint32_t health = 300;
	const std::uint32_t damage = 30;
	const std::uint32_t range = 10;
	const std::uint32_t cost = 150;
	const std::uint32_t gain = 20;
	const float fireRate = 1.0f;
};


Tower TowerFastToTower();
Tower TowerSlowToTower();
Tower TowerSimpleToTower();

Enemy EnemyFastToEnemy();
Enemy EnemyGoldToEnemy();
Enemy EnemyTankToEnemy();