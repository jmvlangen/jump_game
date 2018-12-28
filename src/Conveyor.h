#ifndef _CONVEYOR_H // Guarding
#define _CONVEYOR_H

#include MaskedObject

namespace JumpGame {

  /* A conveyor is a stationary object that moves
     movable objects on top of it in a predetermined
     direction. */
  class Conveyor : public MaskedObject {
  public:
    /*
      Constructor of a conveyor.
      
      INPUT:
      - ``x`` -- The horizontal position of the
        conveyor
      - ``y`` -- The vertical position of the
        conveyor
      - ``speed`` -- The speed with which the
        conveyor moves movable objects on top.
	The speed is horizontal, meaning a
	positive speed moves to the right and
	a negative speed moves to the left.
     */
    Conveyor(int x, int y, double speed)
  };

}

#endif
