#ifndef _MASK_H // Guarding
#define _MASK_H

#include Screen.h

namespace JumpGame {

  /* A mask is a picture that should be drawn on the screen */
  class Mask {
  public: 
    int get_screen_x(); // The horizontal position of the mask on the screen
    int get_screen_y(); // The vertical position of the mask on the screen

    void draw(Screen * screen); // Draws this mask on the given screen.
  };

}

#endif
