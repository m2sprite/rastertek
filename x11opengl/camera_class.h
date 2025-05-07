#ifndef _CAMERACLASS_H_
#define _CAMERACLASS_H_
#include <math.h>
class CameraClass
{
private:
  strcut VectorType;
  {
    float x, y, z;
  }
public:
  CameraClass();
  CameraClass(const CameraClass&);
  ~CameraClass();

  void SetPosition(float, float, float);
  void SetRotation(float, float, float);

  void GetPosition(float*);
  void GetRotation(float*);

  void Render();
  void GetViewMatrix(float*);
private:
  void MatrixRotationYawPitchRoll(float*,float,float,float);
  void TransfromCoord(VectorType&,float*);
  void BuildViewMatrix(float*, VectorType, VectorType, VectorType);

private:
  float m_positionX, m_positionY, m_positionZ;
  float m_rotationX,, m_rotationY, m_rotationZ;
  float m_viewMatrix[16];
};
#endif
