#include "binfile.hpp"

#include <cstdint>
#include <fstream>
#include <string>

#include <boost/spirit/home/support/iterators/istream_iterator.hpp>
#include <boost/spirit/include/classic_file_iterator.hpp>
#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/binary/binary.hpp>
#include <boost/phoenix/object/construct.hpp>


namespace parse::producers {
namespace {
/**
* bits 8 12 12
*/
Point parse_point(uint32_t data) {
  return Point(std::to_string(data & 0xFF), data >> 8 & 0xFFF, data >> 20 & 0xFFF);
}

namespace x3 = boost::spirit::x3;
const x3::rule<class SinglePoint, Point> SinglePoint = "SinglePoint";

const auto SinglePoint_def =
    x3::little_dword
    [([](auto& ctx) { x3::_val(ctx) = parse_point(x3::_attr(ctx)); })];

BOOST_SPIRIT_DEFINE(SinglePoint);

}

BinFile::BinFile(std::filesystem::path source) : source_(std::move(source)) {}

PointProducer::Generator BinFile::get_points() {
  using Iterator = boost::spirit::istream_iterator;

  std::ifstream file(source_, std::ios::binary);
  Iterator current = Iterator(file);
  Point point;
  while (x3::parse(current, Iterator(), SinglePoint, point)) {
    co_yield std::move(point);
  }
  if (!x3::parse(current, Iterator(), x3::eoi)) {
    throw ProducerError("Parser can't reach end of file");
  }
}

std::string BinFile::get_source() {
  return source_.string();
}

}
