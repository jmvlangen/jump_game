#ifndef _MOVABLE_OBJECT_H // Guarding
#define _MOVABLE_OBJECT_H

#include Object.h

namespace JumpGame {

  /* A movable object is an object that can move, i.e.
     it can have a horizontal and/or a vertical speed. */
  class MovableObject : public Object {
  public: 
    double get_x_speed(); // The horizontal speed of the object
    double get_y_speed(); // The vertical speed of the object

    void move(); // Move the object according to its speed.
  };

}

#endif
