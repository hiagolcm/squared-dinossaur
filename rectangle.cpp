#include "rectangle.h"

Rectangle::Rectangle(
  QVector2D position,
  float width,
  float height
)
{
  this->position = position;
  this->width = width;
  this->height = height;

  this->velocity = QVector2D(0, 2.5f);
}

QVector2D Rectangle::getPosition() {
  return position;
}

float Rectangle::getWidth() {
  return width;
}

float Rectangle::getHeight() {
  return height;
}

QVector2D Rectangle::getVelocity() {
  return velocity;
}

void Rectangle::setVelocity(QVector2D velocity) {
  this->velocity = velocity;
}

void Rectangle::simulateJump(float deltaTime) {
  float positionY = position.y() + velocity.y()*deltaTime;

  if (positionY <= -0.4f) {
       position.setY(-0.4f);
       velocity.setY(0.0f);
       return;
   }

  velocity.setY(velocity.y() + deltaTime*(-6));
  position.setY(positionY);
}

void Rectangle::jump() {
  if(velocity.y() == 0)
  {
    velocity.setY(2.5f);
  }
}

void Rectangle::setPosition(QVector2D position) {
  this->position = position;
}
