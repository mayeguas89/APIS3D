#pragma once
#include "object3d.h"

#include <string>
class ObjectLoader
{
public:
  static int LoadObjectFromFile(Object* object, const std::string& filename, int context);
};