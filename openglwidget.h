#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include <memory>
#include <QtOpenGL>
#include <QOpenGLWidget>
#include <QOpenGLExtraFunctions>

#include "rectangle.h"
#include "enemygenerator.h"

class OpenGLWidget : public QOpenGLWidget, protected QOpenGLExtraFunctions
{
  Q_OBJECT

  GLsizei viewportWidth;
  GLsizei viewportHeight;

  GLuint vboVertices = 0;
  GLuint vboColors = 0;
  GLuint vboIndices = 0;

  GLuint vao = 0;

  std::unique_ptr<QVector4D[]> vertices = nullptr;
  std::unique_ptr<QVector4D []> colors = nullptr;
  std::unique_ptr<unsigned int []> indices = nullptr;

  GLuint shaderProgram;

  Rectangle *player = nullptr;
  Rectangle *ground = nullptr;
  EnemyGenerator *enemyGenerator = nullptr;

  QTimer timer;
  QTime time;

public:
  explicit OpenGLWidget(QWidget *parent = nullptr);
  ~OpenGLWidget() override;

  void createVBOs();
  void createShaders();
  void destroyVBOs();
  void destroyShaders();
  void createGameObjects();

protected:
  void initializeGL() override;
  void resizeGL(int width, int height) override;
  void paintGL() override;

  void keyPressEvent(QKeyEvent *event) override;
signals:

public slots:
  void animate();
};

#endif // OPENGLWIDGET_H
