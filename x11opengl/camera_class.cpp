#include "camera_class.h"

CameraClass::CameraClass()
{
  m_positionX = 0.0f;
  m_positionY = 0.0f;
  m_positionZ = 0.0f;

  m_rotationX = 0.0f;
  m_rotationY = 0.0f;
  m_rotationZ = 0.0f;
}

CameraClass::~CameraClass(const CameraClass& other)
{
}

CameraClass::~CameraClass()
{
}
