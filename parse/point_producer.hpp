#ifndef POINT_PRODUCER_HPP
#define POINT_PRODUCER_HPP

#include <vector>

#include <external/generator.hpp>
#include <point.hpp>

namespace parse {


class PointProducer {
public:
  using Generator = external::generator<Point>;
  virtual Generator get_points() = 0;
  virtual std::string get_source() = 0;
  virtual ~PointProducer() = default;
};

class ProducerError : public std::runtime_error {
    using std::runtime_error::runtime_error;
};

}

#endif //POINT_PRODUCER_HPP
