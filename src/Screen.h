#ifndef _SCREEN_H // Guarding
#define _SCREEN_H

namespace JumpGame {

  /* A screen is a wrapper around any object that
     allows us to draw on the screen. It will take
     care of scaling and drawing each individual
     object. */
  class Screen {
  public: 
    void drawAll(); // Draw all visible objects on the screen
  };

}

#endif
