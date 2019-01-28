#include "CollisionTable.h"

namespace JumpGame {

  CollisionTable::CollisionTable(int width, int height) {
    objectSets.resize(width*height);
    objectSets.shrink_to_fit();
    
    columnPointers.reserve(width);
    for(int i = 0; i < width; ++i) {
      columnPointers.push_back( &objectSets[height*i] );
    }
    columnPointers.shrink_to_fit();
  }

  int CollisionTable::registerObject(Object * object, int x, int y) {
    auto& set = lookup(x,y);
    set.insert(object);
    return set.size();
  }

  int CollisionTable::deregisterObject(Object * object, int x, int y) {
    auto& set = lookup(x,y);
    auto iterator = set.find(object);
    if(iterator != set.end() ) {
      set.erase(iterator);
    }

    return set.size();
  }

  std::set<Object *>& CollisionTable::lookup(int x, int y) {
    return columnPointers[x][y];
  }

}
