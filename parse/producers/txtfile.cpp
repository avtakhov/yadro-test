#include "txtfile.hpp"

#include <tuple>

#include <boost/fusion/container/deque.hpp>
#include <boost/spirit/include/classic_file_iterator.hpp>
#include <boost/spirit/home/x3.hpp>

namespace parse::producers {

namespace {

Point parse_point(const boost::fusion::deque<std::string, int, int>& attr) {
  return Point{
    boost::fusion::at_c<0>(attr),
    boost::fusion::at_c<1>(attr),
    boost::fusion::at_c<2>(attr)
  };
}

namespace x3 = boost::spirit::x3;
const x3::rule<class SinglePoint, Point> SinglePoint = "SinglePoint";
const auto SinglePoint_def = (
    x3::lexeme [ +(x3::char_ - ':' - x3::eol) ]
    >> ':' 
    >> x3::int_ 
    >> ',' 
    >> x3::int_ 
    >> -x3::eol
) [([](auto& ctx) { x3::_val(ctx) = parse_point(x3::_attr(ctx)); })];

BOOST_SPIRIT_DEFINE(SinglePoint);

}

TxtFile::TxtFile(std::filesystem::path source) : source_(std::move(source)) {}

PointProducer::Generator TxtFile::get_points() {
  using Iterator = boost::spirit::classic::file_iterator<char>;
  Iterator current = Iterator(source_);
  Iterator end = current.make_end();
  
  Point point;
  while (x3::parse(current, end, SinglePoint, point)) {
    co_yield std::move(point);
  }

  if (!x3::parse(current, end, x3::eoi)) {
    throw ProducerError("Parse txt error: parser can't reach the end of file '" + source_.string() + "'");
  }
}

std::string TxtFile::get_source() {
  return source_.string();
}

}
