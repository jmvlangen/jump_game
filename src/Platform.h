#ifndef _PLATFORM_H // Guarding
#define _PLATFORM_H

#include MaskedObject
#include MovableObject

namespace JumpGame {

  /* A moving platform */
  class Platform : public MaskedObject, public MovableObject {
  public:
    /*
      The constructor of a platform.
      
      INPUT:
      - ``x`` -- The horizontal position of the platform
      - ``y`` -- The vertical position of the platform
      - ``speed`` -- The speed of the platform, i.e. the
        horizontal speed. The platform moves to the right
	if this is positive and to the left if it is
	negative.
     */
    Platform(double x, double y, double speed);
  };

}

#endif
