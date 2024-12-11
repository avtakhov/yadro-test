#ifndef BINFILE_HPP
#define BINFILE_HPP

#include <filesystem>

#include <point_producer.hpp>

namespace parse::producers {

class BinFile: public PointProducer {
public:
  BinFile(std::filesystem::path source);
  PointProducer::Generator get_points() final;
  std::string get_source() final;
private:
  std::filesystem::path source_;
};

}

#endif //BINFILE_HPP
