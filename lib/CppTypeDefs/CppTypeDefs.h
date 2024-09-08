#ifndef CppTypeDefs_H
#define CppTypeDefs_H

#include <vector>

using byte = unsigned char;
using sbyte = char;
using string = std::string;

using LocalTime = struct tm;
using DateTime = tm;

template <typename T>
using List = std::vector<T>;

template <typename T>
using Action = std::function<T>;

#endif