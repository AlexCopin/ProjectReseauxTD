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
	returnTower.cost = specialTower.cost;
	returnTower.fireRate = specialTower.fireRate;
  return returnTower;
}

Tower TowerSimpleToTower()
{
  Tower returnTower;
  TowerSimple specialTower;
  returnTower.typeTower = specialTower.typeTower;
  returnTower.name = specialTower.name;
  returnTower.radius = specialTower.radius;
  returnTower.range = specialTower.range;
  returnTower.cost = specialTower.cost;
  returnTower.fireRate = specialTower.fireRate;
  return returnTower;
}