#include "object_loader.h"

#include "system.h"

#include <chrono>
int ObjectLoader::LoadObjectFromFile(Object* object, const std::string& filename, int context)
{
  // System::SetContext(context);
  object->LoadDataFromFile(filename);
  // std::this_thread::sleep_for(std::chrono::milliseconds(5000));
  return context;
}