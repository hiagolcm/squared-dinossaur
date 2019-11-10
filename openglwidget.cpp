#include "openglwidget.h"
#include "rectangle.h"
#include <stdlib.h>

OpenGLWidget::OpenGLWidget(QWidget *parent) : QOpenGLWidget(parent)
{
}

OpenGLWidget::~OpenGLWidget()
{
  destroyVBOs();
  destroyShaders();
}

void OpenGLWidget::initializeGL()
{
  initializeOpenGLFunctions () ;
  qDebug (" OpenGL version : %s", glGetString ( GL_VERSION ) ) ;
  qDebug (" GLSL %s", glGetString ( GL_SHADING_LANGUAGE_VERSION ) ) ;
  createShaders();
  createGameObjects();
  createVBOs ();

  connect(&timer, SIGNAL(timeout()), this, SLOT(animate()));
  timer.start(0);
  time.start();
}

void OpenGLWidget::createGameObjects() {
  player = (Rectangle*) malloc(sizeof(Rectangle));
  *player = Rectangle(QVector2D(-0.5, -0.40), 0.1f, 0.1f);

  ground = (Rectangle*) malloc(sizeof(Rectangle));
  *ground = Rectangle(QVector2D(0, -0.75), 1, 0.25);

  enemyGenerator = (EnemyGenerator*) malloc(sizeof(EnemyGenerator));
  *enemyGenerator = EnemyGenerator(5000);
}

void OpenGLWidget::resizeGL(int width ,int height)
{
  viewportWidth = width;
  viewportHeight = height;
}

void OpenGLWidget::paintGL()
{
  glViewport (0 , 0 , viewportWidth , viewportHeight );
  glClear ( GL_COLOR_BUFFER_BIT );
  glClearColor (0 , 0 , 0 , 1);
  glUseProgram ( shaderProgram );
  glBindVertexArray ( vao );

  GLint locWidth = glGetUniformLocation(shaderProgram, "width");
  GLint locHeight = glGetUniformLocation(shaderProgram, "height");
  GLint locTranslation = glGetUniformLocation(shaderProgram, "translation");

  // Player
  glUniform4f(locTranslation, player->getPosition().x(), player->getPosition().y(), 0, 0);
  glUniform1f(locHeight, player->getHeight());
  glUniform1f(locWidth, player->getWidth());
  glDrawElements (GL_TRIANGLES , 2 * 3 , GL_UNSIGNED_INT ,0);

  // Ground
  glUniform4f(locTranslation, ground->getPosition().x(), ground->getPosition().y(), 0, 0);
  glUniform1f(locHeight, ground->getHeight());
  glUniform1f(locWidth, ground->getWidth());
  glDrawElements (GL_TRIANGLES , 2 * 3 , GL_UNSIGNED_INT ,0);

  // Enemies
  Rectangle *enemies = enemyGenerator->getEnemies();
  unsigned int numEnemies = enemyGenerator->getNumEnemies();

  for (unsigned int i = 0; i < numEnemies; i++)
  {
      glUniform4f(locTranslation, enemies[i].getPosition().x(), enemies[i].getPosition().y(), 0, 0);
      glUniform1f(locHeight, enemies[i].getHeight());
      glUniform1f(locWidth, enemies[i].getWidth());
      glDrawElements (GL_TRIANGLES , 2 * 3 , GL_UNSIGNED_INT ,0);
  }
}

void OpenGLWidget::createShaders ()
{
  // makeCurrent ();
  destroyShaders();
  QString vertexShaderFile(":/shaders/vshader1.glsl");
  QString fragmentShaderFile(":/shaders/fshader1.glsl");
  QFile vs(vertexShaderFile);
  QFile fs(fragmentShaderFile) ;
  vs.open(QFile::ReadOnly | QFile::Text);
  fs.open(QFile::ReadOnly | QFile::Text);
  QTextStream streamVs(&vs), streamFs(&fs);
  QString qtStringVs = streamVs.readAll();
  QString qtStringFs = streamFs.readAll();
  std::string stdStringVs = qtStringVs.toStdString();
  std::string stdStringFs = qtStringFs.toStdString();

  // Create an empty vertex shader handle
  GLuint vertexShader = 0;
  vertexShader = glCreateShader( GL_VERTEX_SHADER ) ;
  // Send the vertex shader source code to GL
  const GLchar *source = stdStringVs.c_str () ;
  glShaderSource( vertexShader , 1 , &source , 0) ;
  // Compile the vertex shader
  glCompileShader( vertexShader ) ;
  GLint isCompiled = 0;
  glGetShaderiv( vertexShader , GL_COMPILE_STATUS , & isCompiled ) ;
  if ( isCompiled == GL_FALSE )
  {
    GLint maxLength = 0;
    glGetShaderiv ( vertexShader , GL_INFO_LOG_LENGTH , & maxLength ) ;
    // The maxLength includes the NULL character
    std::vector<GLchar> infoLog ( maxLength ) ;
    glGetShaderInfoLog( vertexShader , maxLength , &maxLength , &infoLog[0]) ;
    qDebug ("%s", &infoLog[0]) ;
    glDeleteShader( vertexShader ) ;
    return;
   }


    // Create an empty fragment shader handle
    GLuint fragmentShader = 0;
    fragmentShader = glCreateShader ( GL_FRAGMENT_SHADER ) ;
    // Send the fragment shader source code to GL
    // Note that std :: string ’s . c_str is NULL character terminated .
    source = stdStringFs . c_str () ;
    glShaderSource ( fragmentShader , 1 , & source , 0) ;
    // Compile the fragment shader
    glCompileShader ( fragmentShader ) ;
    glGetShaderiv ( fragmentShader , GL_COMPILE_STATUS , & isCompiled ) ;
    if ( isCompiled == GL_FALSE )
    {
      GLint maxLength = 0;
      glGetShaderiv ( fragmentShader , GL_INFO_LOG_LENGTH , & maxLength ) ;
      std :: vector < GLchar > infoLog ( maxLength ) ;
      glGetShaderInfoLog ( fragmentShader , maxLength , & maxLength ,
      & infoLog [0]) ;
      qDebug ("%s", & infoLog [0]) ;
      glDeleteShader ( fragmentShader ) ;
      glDeleteShader ( vertexShader ) ;
      return ;
    }

    shaderProgram = glCreateProgram() ;
    // Attach our shaders to our program
    glAttachShader( shaderProgram , vertexShader ) ;
    glAttachShader ( shaderProgram , fragmentShader ) ;
    // Link our program
    glLinkProgram( shaderProgram ) ;
    // Note the different functions here : glGetProgram * instead of glGetShader *.
    GLint isLinked = 0;
    glGetProgramiv( shaderProgram , GL_LINK_STATUS , & isLinked ) ;
    if ( isLinked == GL_FALSE )
    {
      GLint maxLength = 0;
      glGetProgramiv ( shaderProgram , GL_INFO_LOG_LENGTH , & maxLength ) ;
      // The maxLength includes the NULL character
      std :: vector < GLchar > infoLog ( maxLength ) ;
      glGetProgramInfoLog ( shaderProgram , maxLength , & maxLength ,
      & infoLog [0]) ;
      qDebug ("%s", & infoLog [0]) ;
      glDeleteProgram ( shaderProgram ) ;
      glDeleteShader ( vertexShader ) ;
      glDeleteShader ( fragmentShader ) ;
      return ;
    }

    glDetachShader( shaderProgram , vertexShader ) ;
    glDetachShader ( shaderProgram , fragmentShader ) ;
    glDeleteShader( vertexShader ) ;
    glDeleteShader ( fragmentShader ) ;
    vs . close () ;
    fs . close () ;
}

void OpenGLWidget :: destroyShaders ()
{
  makeCurrent();
  glDeleteProgram ( shaderProgram ) ;
}

void OpenGLWidget :: createVBOs ()
{
  makeCurrent();
      destroyVBOs();

      vertices = std::make_unique<QVector4D[]>(4);
      colors = std::make_unique<QVector4D[]>(4);
      indices = std::make_unique<unsigned int[]>(2 * 3);

      vertices[0] = QVector4D(-1, -1, 0, 1);
      vertices[1] = QVector4D( 1, -1, 0, 1);
      vertices[2] = QVector4D( 1,  1, 0, 1);
      vertices[3] = QVector4D(-1,  1, 0, 1);

      colors[0] = QVector4D(1, 0, 0, 1); // red
      colors[1] = QVector4D(0, 1, 0, 1); // green
      colors[2] = QVector4D(0, 0, 1, 1); // blue
      colors[3] = QVector4D(1, 1, 0, 1); // yellow

      indices[0] = 0;
      indices[1] = 1;
      indices[2] = 2;
      indices[3] = 2;
      indices[4] = 3;
      indices[5] = 0;

      glGenVertexArrays(1, &vao);
      glBindVertexArray(vao);

      glGenBuffers(1, &vboVertices);
      glBindBuffer(GL_ARRAY_BUFFER, vboVertices);
      glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(QVector4D), vertices.get(), GL_STATIC_DRAW);
      glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, nullptr);
      glEnableVertexAttribArray(0);

      glGenBuffers(1, &vboColors);
      glBindBuffer(GL_ARRAY_BUFFER, vboColors);
      glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(QVector4D), colors.get(), GL_STATIC_DRAW);
      glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, nullptr);
      glEnableVertexAttribArray(1);

      glGenBuffers(1, &vboIndices);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIndices);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, 2 * 3 * sizeof(unsigned int), indices.get(), GL_DYNAMIC_DRAW);
}
void OpenGLWidget :: destroyVBOs ()
{
  // makeCurrent ();
  glDeleteBuffers(1 , &vboVertices ) ;
  glDeleteBuffers (1 , &vboColors ) ;
  glDeleteBuffers (1 , &vboIndices ) ;
  glDeleteVertexArrays (1 , &vao ) ;
  vboVertices = 0;
  vboIndices = 0;
  vboColors = 0;
  vao = 0;
}

void OpenGLWidget::animate() {
  float deltaTime = time.restart() / 1000.0f;

  player->simulateJump(deltaTime);
  enemyGenerator->walkAll(deltaTime);

  update();
}

void OpenGLWidget::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Space)
        player->jump();
}
