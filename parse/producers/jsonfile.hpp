#ifndef JSON_FILE_HPP
#define JSON_FILE_HPP

#include <filesystem>

#include <point_producer.hpp>

namespace parse::producers {

class JsonFile: public PointProducer {
public:
  JsonFile(std::filesystem::path source);
  PointProducer::Generator get_points() final;
  std::string get_source() final;
private:
  std::filesystem::path source_;
};

}

#endif //JSON_FILE_HPP
