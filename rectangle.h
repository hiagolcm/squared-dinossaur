#ifndef RECTANGLE_H
#define RECTANGLE_H

#include <QtOpenGL>

class Rectangle
{
  QVector2D position;
  float width;
  float height;
  QVector2D velocity;

public:
  Rectangle(QVector2D position,
            float width,
            float height
            );
  QVector2D getPosition();
  float getWidth();
  float getHeight();
  QVector2D getVelocity();
  void setVelocity(QVector2D velocity);
  void simulateJump(float deltaTime);
  void jump();
  void setPosition(QVector2D position);
};

#endif // RECTANGLE_H
