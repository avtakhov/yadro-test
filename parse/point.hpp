#ifndef POINT_HPP
#define POINT_HPP

#include <cstdint>
#include <string>
#include <variant>

namespace parse {

using GroupVariant = std::variant<int8_t, std::string>;

struct Point {
  GroupVariant group;
  int64_t x;
  int64_t y;
};

}

#endif //POINT_HPP
