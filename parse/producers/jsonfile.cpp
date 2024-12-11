#include "jsonfile.hpp"

#include <fstream>
#include <ranges>

#include <boost/json/conversion.hpp>
#include <boost/json/value.hpp>
#include <boost/json/parse.hpp>
#include <boost/json/error.hpp>

#include <point.hpp>

namespace parse::producers {
namespace {

Point point_from_json(const boost::json::value& jv) {
    const auto& group = jv.at("group").as_string();
    return Point{
      std::string(group.begin(), group.end()),
      jv.at("x").as_int64(),
      jv.at("y").as_int64()
    };
}

}

JsonFile::JsonFile(std::filesystem::path source): source_(std::move(source)) {}

PointProducer::Generator JsonFile::get_points() {
  std::ifstream file(source_);
  boost::system::error_code ec;
  const boost::json::value parsed_json = boost::json::parse(file, ec);
  if (ec) {
    throw ProducerError(ec.message());
  }

  const auto points_view = std::views::all(parsed_json.at("points").as_array());
  for (auto point : points_view | std::views::transform(point_from_json)) {
    co_yield std::move(point);
  }
}

std::string JsonFile::get_source() {
  return source_.string();
}

}
