#include <iostream>
#include <iterator>
#include <filesystem>
#include <memory>
#include <ranges>
#include <string>

#include <boost/json.hpp>
#include <boost/asio.hpp>

#include <producers/jsonfile.hpp>
#include <producers/txtfile.hpp>
#include <producers/binfile.hpp>
#include <http_session.hpp>
#include <send_points.hpp>


std::unique_ptr<parse::PointProducer> make_producer(const std::filesystem::path& file) {
  auto ext = file.extension();
  if (ext == ".txt") {
    return std::make_unique<parse::producers::TxtFile>(file);
  } else if (ext == ".json") {
    return std::make_unique<parse::producers::JsonFile>(file);
  } else if (ext == ".bin") {
    return std::make_unique<parse::producers::BinFile>(file);
  } else {
    throw std::runtime_error("Unsupported file extension: '" + ext.string() + "'");
  }
}

std::string getenv_or_throw(const char* name) {
  const auto env = std::getenv(name);
  if (env == nullptr) {
    throw std::runtime_error("No env variable '" + std::string(name) + "'");
  }

  return env;
}

external::generator<std::vector<parse::Point>> chunked(parse::PointProducer& producer, size_t chunk_size) {
  std::vector<parse::Point> result;
  for (auto point : producer.get_points()) {
    result.push_back(std::move(point));
    if (result.size() == chunk_size) {
      co_yield std::move(result);
    }
  }
  if (!result.empty()) {
    co_yield std::move(result);
  }
}

int main(int argc, char* argv[]) {
  const auto host = getenv_or_throw("PARSE_API_HOST");
  const auto port = getenv_or_throw("PARSE_API_PORT");

  boost::asio::thread_pool queue(2);
  std::vector<std::future<void>> futures;
  
  for (int i = 1; i < argc; ++i) {
    std::filesystem::path file(std::string(argv[i]));    
    
    std::promise<void> promise;
    futures.emplace_back(promise.get_future());

    boost::asio::post(
      queue,
      [&host, &port, file=std::move(file), promise=std::move(promise)]() mutable {
        try {
          auto producer = make_producer(file);
          parse::HttpSession session(host, port);
          for (const auto& chunk : chunked(*producer, 100)) {      
            send_points(producer->get_source(), chunk, session);
          }
          promise.set_value();
        } catch (...) {
          promise.set_exception(std::current_exception());
        }
      }
    );
  }
  queue.join();

  for (auto& future : futures) {
    // throw
    future.get();
  }
}
