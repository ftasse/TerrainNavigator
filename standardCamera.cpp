#include "standardCamera.h"

#if QT_VERSION >= 0x040000
# include <QWheelEvent>
#endif

using namespace qglviewer;

StandardCamera::StandardCamera()
{
  standard = true;
  setType(Camera::PERSPECTIVE);
  orthoSize = 1.0;
}

float StandardCamera::zNear() const
{
  if (standard)
    return 10.0f;
  else
    return Camera::zNear();
}

float StandardCamera::zFar() const
{
  if (standard)
    return 10000000.0;
  else
    return Camera::zFar();
}

void StandardCamera::changeOrthoFrustumSize(int delta)
{
  if (delta > 0)
    orthoSize *= 1.1f;
  else
    orthoSize /= 1.1f;
}

void StandardCamera::getOrthoWidthHeight(GLdouble &halfWidth, GLdouble &halfHeight) const
{
  if (standard)
  {
    halfHeight = orthoSize;
    halfWidth = aspectRatio() * orthoSize;
  }
  else
    Camera::getOrthoWidthHeight(halfWidth, halfHeight);
}
