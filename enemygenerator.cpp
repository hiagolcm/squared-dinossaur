#include "enemygenerator.h"

#include<stdlib.h>
#include <algorithm>
EnemyGenerator::EnemyGenerator(unsigned int numEnemies)
{
  this->numEnemies = numEnemies;
  this->enemies = (Rectangle*) malloc(numEnemies * sizeof(Rectangle));

  float position = 0;

  for (unsigned int i = 0; i < numEnemies; i++)
  {
    position = position + std::max(((rand() % 100 + 1) / 50.0f),0.9f);
    this->enemies[i] = Rectangle(QVector2D(position, -0.40), 0.05f, 0.1f);
  }
}

void EnemyGenerator::walkAll(float deltaTime) {
  float currentX;
  float velocity = 1.0f;
  for  (unsigned int i = 0; i < numEnemies; i++)
  {
    currentX = this->enemies[i].getPosition().x();
    this->enemies[i].setPosition(QVector2D(currentX - velocity * deltaTime, -0.4));
  }
}

Rectangle *EnemyGenerator::getEnemies() {
  return enemies;
}

unsigned int EnemyGenerator::getNumEnemies() {
  return numEnemies;
}
