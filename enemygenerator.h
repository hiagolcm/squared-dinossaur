#ifndef ENEMYGENERATOR_H
#define ENEMYGENERATOR_H

#include "rectangle.h"

class EnemyGenerator
{

Rectangle *enemies = nullptr;
unsigned int numEnemies;

public:
  EnemyGenerator(unsigned int numEnemies);
  void walkAll(float deltaTime);
  Rectangle *getEnemies();
  unsigned int getNumEnemies();
};

#endif // ENEMYGENERATOR_H
