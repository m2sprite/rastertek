#ifndef _APPLICATION_CLASS_H_
#define _APPLICATION_CLASS_H_
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_NEAR = 0.3f;
const float SCREEN_DEPTH = 1000.0f;

#include "input_class.h"
#include "opengl_class.h"
class ApplicationClass
{
public:
  ApplicationClass();
  ApplicationClass(const ApplicationClass&);
  ~ApplicationClass();
  bool Initialize(Display*, Window, int, int);
  void Shutdown();
  bool Frame(InputClass*);
private:
  bool Render();
private:
  OpenGLClass *m_OpenGL;
};
#endif
