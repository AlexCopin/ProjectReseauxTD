#pragma once


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