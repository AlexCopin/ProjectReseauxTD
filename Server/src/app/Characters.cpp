#include "Characters.h"

Tower
TowerFastToTower()
{
  Tower returnTower;
  TowerFast specialTower;
  returnTower.typeTower = specialTower.typeTower;
  returnTower.name = specialTower.name;
  returnTower.radius = specialTower.radius;
  returnTower.range = specialTower.range;
  returnTower.damage = specialTower.damage;
  returnTower.cost = specialTower.cost;
  returnTower.fireRate = specialTower.fireRate;
  return returnTower;
}

Tower
TowerSlowToTower()
{
	Tower returnTower;
	TowerFrost specialTower;
	returnTower.typeTower = specialTower.typeTower;
	returnTower.name = specialTower.name;
	returnTower.radius = specialTower.radius;
	returnTower.range = specialTower.range;
	returnTower.damage = specialTower.damage;
	returnTower.cost = specialTower.cost;
	returnTower.fireRate = specialTower.fireRate;
  return returnTower;
}

Tower 
TowerSimpleToTower()
{
  Tower returnTower;
  TowerSimple specialTower;
  returnTower.typeTower = specialTower.typeTower;
  returnTower.name = specialTower.name;
  returnTower.radius = specialTower.radius;
  returnTower.range = specialTower.range;
  returnTower.damage = specialTower.damage;
  returnTower.cost = specialTower.cost;
  returnTower.fireRate = specialTower.fireRate;
  return returnTower;
}

Enemy 
EnemyFastToEnemy()
{
	Enemy returnEnemy;
	EnemyFast specialEnemy;
	returnEnemy.typeEnemy = specialEnemy.typeEnemy;
	returnEnemy.name = specialEnemy.name;
	returnEnemy.health = specialEnemy.health;
	returnEnemy.damage = specialEnemy.damage;
	returnEnemy.range = specialEnemy.range;
	returnEnemy.cost = specialEnemy.cost;
	returnEnemy.gain = specialEnemy.gain;
	returnEnemy.fireRate = specialEnemy.fireRate;
	return returnEnemy;
}

Enemy 
EnemyGoldToEnemy()
{
	Enemy returnEnemy;
	EnemyGold specialEnemy;
	returnEnemy.typeEnemy = specialEnemy.typeEnemy;
	returnEnemy.name = specialEnemy.name;
	returnEnemy.health = specialEnemy.health;
	returnEnemy.damage = specialEnemy.damage;
	returnEnemy.range = specialEnemy.range;
	returnEnemy.cost = specialEnemy.cost;
	returnEnemy.gain = specialEnemy.gain;
	returnEnemy.fireRate = specialEnemy.fireRate;
	return returnEnemy;
}

Enemy 
EnemyTankToEnemy()
{
	Enemy returnEnemy;
	EnemyTank specialEnemy;
	returnEnemy.typeEnemy = specialEnemy.typeEnemy;
	returnEnemy.name = specialEnemy.name;
	returnEnemy.health = specialEnemy.health;
	returnEnemy.damage = specialEnemy.damage;
	returnEnemy.range = specialEnemy.range;
	returnEnemy.cost = specialEnemy.cost;
	returnEnemy.gain = specialEnemy.gain;
	returnEnemy.fireRate = specialEnemy.fireRate;
	return returnEnemy;
}

