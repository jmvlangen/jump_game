#ifndef _MASKED_OBJECT_H // Guarding
#define _MASKED_OBJECT_H

#include Object
#include Mask

namespace JumpGame {

  /* A masked object is an object that has a mask. */
  class MaskedObject : public Object {
  public: 
    Mask * getMask(); // Returns the mask of this object.
  };

}

#endif
