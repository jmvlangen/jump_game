#pragma once // Compile only once

#include <set>
#include Object.h

namespace JumpGame {

  /*
    A collision table is a table indexed by the grid
    positions on the screen that contains for each
    such index the objects that overlap part of that
    grid square with top left coordinate the index.
  */
  class CollisionTable {
  public:
    /*
      Constructor of a collision table

      INPUT:
      - ``width`` -- The number of horizontal grid
        points to be in this table.
      - ``height`` -- The number of vertical grid
        points to be in this table.
     */
    CollisionTable(int width, int height);

    /*
      Registers an object for possible collision
      at a given position.

      INPUT:
      - ``object`` -- A pointer to the object to
        be registered.
      - ``x`` -- The horizontal position at which
        the object should be registered.
      - ``y`` -- The vertical position at which
        the object should be registered.
      
      OUTPUT:
      The number of objects currently registered
      at the given position after this object was
      added.
     */
    int register(Object* object, int x, int y);

    /*
      Looks up all objects at a given position.

      INPUT:
      - ``x`` -- The horizontal position at which
        to perform the lookup.
      - ``y`` -- The vertical position at which
        to perform the lookup.

      OUTPUT:
      The set of objects registered at the given
      position.
     */
    std::set<Object> lookup(int x, int y);
  };

}
