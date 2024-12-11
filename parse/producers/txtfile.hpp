#ifndef TXTFILE_HPP
#define TXTFILE_HPP

#include <filesystem>

#include <point_producer.hpp>

namespace parse::producers {

class TxtFile : public PointProducer {
public:
  TxtFile(std::filesystem::path source);
  PointProducer::Generator get_points() final;
  std::string get_source() final;
private:
  std::filesystem::path source_;
};

}

#endif //TXTFILE_HPP
