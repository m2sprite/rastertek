#ifndef _COLORSHADERCLASS_H_
#define _COLORSHADERCLASS_H_
#include <iostream>
#include "openglclass.h"
class ColorShaderClass
{
public:
  ColorShaderClass();
  ColorShaderClass(const ColorShaderClass&);
  ~ColorShaderClass();
  bool Initialize(OpenGLClass*);
  void Shutdown();
  bool SetShaderParameters(float*,float*,float*);

private:
  bool InitializeShader(char*, char*);
  void ShutdownSahder();
  char *LoadShaderSourceFile(char*);
  void OutputShaderErrorMessage(unsigned int, char*);
  void OutputLikerErrorMessage(unsigned int);

private:
  OpenGLClass *m_OpenGLPtr;
  unsigned int m_vertexShader;
  unsigned int m_fragmentShader;
  unsigned int m_shaderProgram;
};
#endif
