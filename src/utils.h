#pragma once

#include <sstream>
#include <string>
#include <vector>

namespace utils
{

template<typename T>
std::vector<T> SplitString(const std::string& str, char delim)
{
  std::vector<T> elems;
  if (str != "")
  {
    std::stringstream ss(str);
    std::string item;
    while (std::getline(ss, item, delim))
    {
      T value;
      std::stringstream ss2(item);
      ss2 >> value;
      elems.push_back(value);
    }
  }
  return elems;
}

}
