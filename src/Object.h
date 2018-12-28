#ifndef _OBJECT_H // Guarding
#define _OBJECT_H

namespace JumpGame {

  /* An object is the base object for anything that
     interacts with the gameplay. Examples of objects
     would be walls, the player character, enemies and
     platforms. Each object has an x and a y position. */
  class Object {
  public: 
    double get_x(); // The horizontal position of the object
    double get_y(); // The vertical position of the object
  };

}

#endif
