#include "colorshader_class.h"
ColorShaderClass::ColorShaderClass()
{
  m_OpenGLPtr = 0;
}

ColorShaderClass::ColorShaderClass(const ColorShaderClass& other)
{
}

ColorShaderClass::~ColorShaderClass()
{
}


bool ColorShaderClass::Initialize(OpenGLClass *OpenGL)
{
  char vsFilename[128];
  char fsFilename[128];
  bool result;

  m_OpenGLPtr = OpenGL;

  strcpy(vsFilename, "../Engine/color.vs");
  strcpy(fsFilename, "../Engine/color.fs");

  result = InitializeShader(vsFilename, fsFilename);
  if(!result)
  {
    return false;
  }

  return true;
}

void ColorShaderClass::Shutdown()
{
  ShutdownShader();
  m_OpenGLPtr = 0;
  return;
}

bool ColorShaderClass::InitializeShader(char *vsFilename, char *fsFilename)
{
  const char *vertexShaderBuffer;
  const char *fragmentShaderBuffer;
  int status;

  vertexShaderBuffer = LoadShaderSourceFile(vsFilename);
  if(!vertexShaderBuffer)
  {
    return false;
  }

  fragmentShaderBuffer = LoadShaderSourceFile(fsFilename);
  if(!fragmentShaderBuffer)
  {
    return false;
  }

  //create glCreateShader
  m_vertexShader = m_OpenGLPtr->glCreateShader(GL_VERTEX_SHADER);
  m_fragmentShader = m_OpenGLPtr->glCreateShader(GL_FRAGMENT_SHADER);

  m_OpenGLPtr->glShaderSource(m_vertexShader, 1, &vertexShaderBuffer, NULL);
  m_OpenGLPtr->glShaderSource(m_fragmentShader, 1, &fragmentShaderBuffer, NULL);

  delete []vertexShaderBuffer;
  vertexShaderBuffer = 0;

  delete []fragmentShaderBuffer;
  fragmentShaderBuffer = 0;

  m_OpenGLPtr->glCompileShader(m_vertexShader);
  m_OpenGLPtr->glCompileShader(m_fragmentShader);

  m_OpenGLPtr->glGetShaderiv(m_vertexShader, GL_COMPILE_STATUS, &status);
  if(status != 1)
  {
    OutputShaderErrorMessage(m_vertexShader, vsFilename);
    return false;
  }

  m_OpenGLPtr->glGetShaderiv(m_fragmentShader, GL_COMPILE_STATUS, &status);
  if(status != 1)
  {
    OutputShaderErrorMessage(m_fragmentShader, fsFilename);
    return false;
  }

  m_shaderProgram = m_OpenGLPtr->glCreateProgram();

  m_OpenGLPtr->glAttachShader(m_shaderProgram, m_vertexShader);
  m_OpenGLPtr->glAttachShader(m_shaderProgram, m_fragmentShader);

  m_OpenGLPtr->glBindAttribLocation(m_shaderProgram, 0, "inputPosition");
  m_OpenGLPtr->glBindAttribLocation(m_shaderProgram, 1, "inputColor");

  m_OpenGLPtr->glLinkProgram(m_shaderProgram);

  m_OpenGLPtr->glGetPRogramiv(m_shaderProgram, GL_LINK_STATUS, &status);
  if(status != 1)
  {
    OutputLinkerErrorMessage(m_shaderProgram);
    retrun false;
  }

  return true;
}

void ColorShaderClass::ShutdownShader()
{
  m_OpenGLPtr->glDetachShader(m_shaderProgram, m_vertexShader);
  m_OpenGLPtr->glDetachShader(m_shaderProgram, m_fragmentShader);

  m_OpenGLPtr->glDeleteShader(m_vertexShader);
  m_OpenGLPtr->glDeleteShader(m_fragmentShader);

  m_OpenGLPtr->glDeleteProgram(m_shaderProgram);
  return;
}

char *ColorShaderClass::LoadShaderSourceFile( char *filename )
{
  File *filePtr;
  char *buffer;
  long fileSize, count;
  int error;
  filePtr  = fopen(filename "r');
  if(filePtr == NULL)
  {
    return 0;
  }
  fseek(filePtr, 0, SEEK_END);
  fileSize = ftell(filePtr);
  buffer = new char[fileSize +1];
  fseek(filePtr, 0, SEEK_SET);
  count = fread(buffer, 1, fileSize, filePtr);
  if(count != fileSize)
  {
    return 0;
  }
  error = fclose(filePtr);
  if(error != 0)
  {
    return 0;
  }
  buffer[fileSize] = '\0';
  return buffer;
}

void ColorShaderClass::OutputShaderErrorMessage( unsigned int shaderId, char *shaderFilename )
{
  long count;
  int logSize, error;
  char *infoLog;
  FILE *filePtr;
  m_OpenGLPtr->glGetShaderiv(shaderId, GL_INFO_LENGTH, &logSize);
  logSize++;
  infoLog = new char[logSize];
  m_OpenGLPtr->glGetShaderInfoLog(shaderId, logSize, NULL, infoLog);
  if(filePtr == NULL)
  {
    std::cout << "Error opening shader error message output file." << std::endl;
    return;
  }
  count = fwrite(infoLog, sizeof(char), logSize, filePtr);
  if(count != logSize)
  {
    std::cout << "Error writing shader error message output file." << std::endl;
    return;
  }
  error = fclose(filePtr);
  if(error != 0)
  {
    std::cout << "Error closing shader error message output file." << std::endl;
    return;
  }
  std::cout << "Error compiling shader. Check shader-error.txt for error message. Shader filename:" <<  shaderFilename << std::endl;
  return;
}

void ColorShaderClass::OutputLinkerErrorMessage(unsigned int programId)
{
  long count;
  int logSize, error;
  char *infoLog;
  FILE *filePtr;
  m_OpenGLPtr->glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &logSize);
  logSize++;
  infoLog = new char[logSize];
  m_OpenGLPtr->glGetProgramInfoLog(programId, logSize, NULL, infoLog);
  if(filePtr == NULL)
  {
    std::cout << "Error opening linker error message output file."<< std::endl;
    return;
  }
  count = fwrite(infoLog, sizeof(char), logSize, filePtr);
  if(count != logSize)
  {
    std::cout << "Error writing linker error message output file."<< std::endl;
  }
  error = fclose(filePtr);
  if(error != 0)
  {
    std::cout << "Error closing linker error message output file." << std::endl;
  }
  std::cout << "Error linking shader program. Check linker-error.txt for message." std::endl;
  return;
}

void ColorShaderClass::SetShaderParameters(float *worldMatrix, float *viewMatrix, float *projectionMatrix)
{
  float tpWorldMatrix[16], tpViewMatrix[16], tpPorjectionMatrix[16];
  int location;
  m_OpenGLPtr->MatrixTranspose( tpWorldMatrix, worldMatrix);
  m_OpenGLPtr->MatrixTranspose( tpViewMatrix, viewMatrix);
  m_OpenGLPtr->MatrixTranspose( tpProjectionMatrix, projectionMatrix );
  m_OpenGLPtr->glUserProgram(m_shaderProgram);
  location = m_OepnGLPtr->glGetUniformLocation(m_shaderProgram, "worldMatrix");
  if(location == -1)
  {
    std::cout << "World matrix is not set." << std::endl;
  }
  m_OpenGLPtr->glUniformMatrix4fv(location, 1,  flase, tpWorldMatrix);
  location = m_OpenGLPtr->glGetUniformLocation(m_shaderProgram, "viewMatrix");
  if(location == -1)
  {
    std::cout << "Projection matrix is not set." << std::endl;
  }
  m_OpenGLPtr->glUniformMatrix4fv(location, 1, false, tpPorjectionMatrix);
  return true;
}
