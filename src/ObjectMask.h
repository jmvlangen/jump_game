#ifndef _OBJECT_MASK_H // Guarding
#define _OBJECT_MASK_H

#include Mask.h
#include Object.h

namespace JumpGame {

  /* An object mask is a mask that is aligned with a
     certain object. */
  class ObjectMask : public Mask {
  public: 
    Object * get_object() // The object this mask is linked to.
  };

}

#endif
