#ifndef SEND_POINTS_HPP
#define SEND_POINTS_HPP

#include <string>
#include <vector>

#include <http_session.hpp>
#include <point.hpp>

namespace parse {

void send_points(
  const std::string& source, 
  const std::vector<Point>& points, 
  HttpSession& session
);

}

#endif //SEND_POINTS_HPP
