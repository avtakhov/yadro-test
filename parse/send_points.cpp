#include "send_points.hpp"

#include <boost/json.hpp>

namespace parse {

namespace {

struct GroupToJsonVisitor {
  boost::json::value operator()(const auto& group) {
    return boost::json::value(group);
  }
};

boost::json::value to_json(const std::string& source, const std::vector<Point>& points) {
  boost::json::array json_array;
  for (const auto& point : points) {
    boost::json::object obj = {
        {"x", point.x},
        {"y", point.y},
        {"source", source},
        {"group", std::visit(GroupToJsonVisitor{}, point.group)}
    };
    json_array.push_back(std::move(obj));
  }

  return boost::json::object{
      {"points", std::move(json_array)}
  };
}

}

void send_points(
  const std::string& source, 
  const std::vector<Point>& points, 
  HttpSession& session
) {
    const auto json = to_json(source, points);
    session.post_data("/points/add", boost::json::serialize(json));
}

}
